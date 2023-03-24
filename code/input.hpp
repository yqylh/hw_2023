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

void solveGraph() {
    TESTOUTPUT(fout << "special worktable" << std::endl;)   
    std::vector<std::pair<int, double> > near7;
    for (auto & i : worktables) {
        if (i.type == 7) {
            double dis = 1e8;
            for (auto & j : worktables) {
                if (j.type == 8 || j.type == 9) {
                    dis = std::min(dis, Vector2D(i.x - j.x, i.y - j.y).length());
                }
            }
            near7.push_back(std::make_pair(i.id, dis));
        }
    }
    if (near7.size() == 0) {
        // 没有7号工作台
        for (auto & i : worktables) {
            if (i.type == 4 || i.type == 5 || i.type == 6) {
                double dis = 1e8;
                for (auto & j : worktables) {
                    if (j.type == 9) {
                        dis = std::min(dis, Vector2D(i.x - j.x, i.y - j.y).length());
                    }
                }
                near7.push_back(std::make_pair(i.id, dis));
            }
        }
        std::sort(near7.begin(), near7.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
            return a.second < b.second;
        }); // 按照距离排序
        int index = 0;
        for (auto & num7 : near7) {
            int id7 = num7.first;
            index++;
            if (index <= near7.size() * 0.5) {
                worktables[id7].near7 = 2;
            } else {
                break;
            }
            std::vector<std::pair<int, double> > near1, near2;
            for (auto & i : worktables) {
                if (i.type == 1 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near1.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 2 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near2.push_back(std::make_pair(i.id, dis));
                }
            }
            std::sort(near1.begin(), near1.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near2.begin(), near2.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
                TESTOUTPUT(fout << "near with id=" << id7 << " is ";)
            if (near1.size() >= 1) {
                worktables[near1[0].first].near7 = 1.2;
                TESTOUTPUT(fout << near1[0].first << " ";)
            }
            if (near2.size() >= 1) {
                worktables[near2[0].first].near7 = 1.2;
                TESTOUTPUT(fout << near2[0].first << " ";)
            }
            TESTOUTPUT(fout << std::endl;)
        }
        return;
    }
    // 有7号工作台  
    std::sort(near7.begin(), near7.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
        return a.second < b.second;
    }); // 按照距离排序
    if (near7.size() == 8) {
        int index = 0;
        for (auto & num7 : near7) {
            int id7 = num7.first;
            worktables[id7].near7 = 5;
            if (++index == 2) break;
            std::vector<std::pair<int, double> > near4, near5, near6;
            for (auto & i : worktables) {
                if (i.type == 4 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near4.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 5 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near5.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 6 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near6.push_back(std::make_pair(i.id, dis));
                }
            }
            std::sort(near4.begin(), near4.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near5.begin(), near5.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near6.begin(), near6.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            TESTOUTPUT(fout << "near with id=" << id7 << " is ";)
            if (near4.size() >= 1) {
                worktables[near4[0].first].near7 = 3;
                TESTOUTPUT(fout << near4[0].first << " ";)
            }
            if (near5.size() >= 1) {
                worktables[near5[0].first].near7 = 3;
                TESTOUTPUT(fout << near5[0].first << " ";)
            }
            if (near6.size() >= 1) {
                worktables[near6[0].first].near7 = 3;
                TESTOUTPUT(fout << near6[0].first << " ";)
            }
            TESTOUTPUT(fout << std::endl;)
        }
        return;
    }
    if (near7.size() == 2) {
        for (auto & num7 : near7) {
            int id7 = num7.first;
            worktables[id7].near7 = 5;
            std::vector<std::pair<int, double> > near4, near5, near6;
            for (auto & i : worktables) {
                if (i.type == 4 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near4.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 5 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near5.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 6 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near6.push_back(std::make_pair(i.id, dis));
                }
            }
            std::sort(near4.begin(), near4.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near5.begin(), near5.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near6.begin(), near6.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            TESTOUTPUT(fout << "near with id=" << id7 << " is ";)
            if (near4.size() >= 1) {
                worktables[near4[0].first].near7 = 1.2;
                TESTOUTPUT(fout << near4[0].first << " ";)
            }
            if (near5.size() >= 1) {
                worktables[near5[0].first].near7 = 1.2;
                TESTOUTPUT(fout << near5[0].first << " ";)
            }
            if (near6.size() >= 1) {
                worktables[near6[0].first].near7 = 1.4;
                TESTOUTPUT(fout << near6[0].first << " ";)
            }
            TESTOUTPUT(fout << std::endl;)
        }
        return;
    }
    if (near7.size() == 1) {
        for (auto & num7 : near7) {
            int id7 = num7.first;
            worktables[id7].near7 = 5;
            std::vector<std::pair<int, double> > near4, near5, near6;
            for (auto & i : worktables) {
                if (i.type == 4 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near4.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 5 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near5.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 6 && i.near7 == 1) {
                    double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                    near6.push_back(std::make_pair(i.id, dis));
                }
            }
            std::sort(near4.begin(), near4.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near5.begin(), near5.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            std::sort(near6.begin(), near6.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
                return a.second < b.second;
            }); // 按照距离排序
            TESTOUTPUT(fout << "near with id=" << id7 << " is ";)
            if (near4.size() >= 1) {
                worktables[near4[0].first].near7 = 3;
                TESTOUTPUT(fout << near4[0].first << " ";)
            }
            if (near5.size() >= 1) {
                worktables[near5[0].first].near7 = 2;
                TESTOUTPUT(fout << near5[0].first << " ";)
            }
            if (near6.size() >= 1) {
                worktables[near6[0].first].near7 = 2;
                TESTOUTPUT(fout << near6[0].first << " ";)
            }
            TESTOUTPUT(fout << std::endl;)
        }
        return;
    }
    for (auto & num7 : near7) {
        int id7 = num7.first;
        std::vector<std::pair<int, double> > near4, near5, near6;
        for (auto & i : worktables) {
            if (i.type == 4 && i.near7 == 1) {
                double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                near4.push_back(std::make_pair(i.id, dis));
            }
            if (i.type == 5 && i.near7 == 1) {
                double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                near5.push_back(std::make_pair(i.id, dis));
            }
            if (i.type == 6 && i.near7 == 1) {
                double dis = Vector2D(i.x - worktables[id7].x, i.y - worktables[id7].y).length();
                near6.push_back(std::make_pair(i.id, dis));
            }
        }
        std::sort(near4.begin(), near4.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
            return a.second < b.second;
        }); // 按照距离排序
        std::sort(near5.begin(), near5.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
            return a.second < b.second;
        }); // 按照距离排序
        std::sort(near6.begin(), near6.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
            return a.second < b.second;
        }); // 按照距离排序
        TESTOUTPUT(fout << "near with id=" << id7 << " is ";)
        if (near4.size() >= 1) {
            worktables[near4[0].first].near7 = 1.2;
            TESTOUTPUT(fout << near4[0].first << " ";)
        }
        if (near5.size() >= 1) {
            worktables[near5[0].first].near7 = 1.2;
            TESTOUTPUT(fout << near5[0].first << " ";)
        }
        if (near6.size() >= 1) {
            worktables[near6[0].first].near7 = 1.2;
            TESTOUTPUT(fout << near6[0].first << " ";)
        }
        TESTOUTPUT(fout << std::endl;)
    }
}

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
    solveGraph();
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

    for (int i = 0; i <= worktableNum; i++) {
        worktables[i].checkWait();
        // TESTOUTPUT(worktables[i].outputTest();)
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].action();
        // TESTOUTPUT(robots[i].outputTest();) 
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