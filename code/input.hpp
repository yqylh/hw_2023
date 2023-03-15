#ifndef __INPUT_H__
#define __INPUT_H__
#include "config.hpp"
#include "robot.hpp"
#include "worktable.hpp"
#include <fstream>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

void inputMap(){
    for (int i = 0; i < MAP_Line_Length; i++) {
        for (int j = 0; j < MAP_Col_Length; j++) {
            char c;
            scanf("%c", &c);
            while(c != '.' && c != 'A' && (c < '0' || c > '9')) scanf("%c", &c);
            if (c == '.') continue;
            if (c == 'A') {
                robotNum++;
                robots[robotNum] = Robot(robotNum, 0.5*j+0.25, 50 - 0.25 - 0.5*i);
            } else {
                worktableNum++;
                worktables[worktableNum] = Worktable(worktableNum, 0.5*j+0.25, 50 - 0.25 - 0.5*i, c - '0');
            }
        }
    }
    std::string line;
    while(getline(std::cin, line) && line != "OK");
    puts("OK");
    fflush(stdout);
}

bool inputFrame() {
    if (scanf("%d%d",&nowTime, &money ) == EOF) {
        return false;
    }
    int tableNum;
    scanf("%d", &tableNum);
    // 处理工作台的信息
    for (int i = 0; i < tableNum; i++) {
        int type; // 工作台类型
        double x, y; // 工作台坐标
        int inputStatus; // 原材料状态
        int outputStatus; // 输出状态
        scanf("%d%lf%lf%d%d%d", &type, &x, &y, &worktables[i].remainTime, &inputStatus, &outputStatus);

        worktables[i].output = outputStatus;
        // 处理原材料对应的信息
        for (int bitnum = 1; bitnum <= MAX_Item_Type_Num; bitnum++) {
            if (inputStatus & (1 << bitnum)) {
                worktables[i].inputId[bitnum] = 1;
            } else {
                worktables[i].inputId[bitnum] = 0;
            }
        }
    }
    // 处理机器人的信息
    for (int i = 0; i <= robotNum; i++) {
        scanf("%d%d%lf%lf%lf%lf%lf%lf%lf%lf", &robots[i].worktableId, &robots[i].bringId, &robots[i].timeCoef, &robots[i].crashCoef, &robots[i].angularSeppd, &robots[i].linearSpeedX, &robots[i].linearSpeedY, &robots[i].direction, &robots[i].x, &robots[i].y);
    }
    std::string line;
    while(getline(std::cin, line) && line != "OK");
    return true;
}

void solveFrame() {
    printf("%d\n", nowTime);
    TESTOUTPUT(fout << nowTime << std::endl;)

    std::fill(canBuy, canBuy + MAX_Item_Type_Num + 1, 0);
    for (int i = 0; i <= worktableNum; i++) {
        worktables[i].checkCanBuy();
        worktables[i].checkWait();
        // TESTOUTPUT(worktables[i].outputTest();)
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].checkCanBuy();
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].action();
        TESTOUTPUT(robots[i].outputTest();) 
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].collisionRotate = -1;
        robots[i].collisionSpeed = -3;
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].checkWall();
    }
    for (int i = 0; i <= robotNum; i++) {
        for (int j = i + 1; j <= robotNum; j++) {
            DetecteCollision(i, j);
        }
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].Move();
    }

    puts("OK");
    TESTOUTPUT(fout << "OK" << std::endl;)
    fflush(stdout);
}
#endif