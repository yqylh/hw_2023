#ifndef __REBOT_H__
#define __REBOT_H__

#include "config.hpp"
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

struct Robot{
    int id; // 机器人的 id
    double x; // 机器人的 x 坐标
    double y; // 机器人的 y 坐标
    int worktableId; // 机器人正在处理的工作台的 id, -1 表示没有
    int bringId; // 携带的物品的类型, 0 表示没有
    double timeCoef; // 机器人的时间系数
    double crashCoef; // 机器人的碰撞系数
    double angularSeppd; // 机器人的角速度
    double linearSpeedX; // 机器人的线速度X
    double linearSpeedY; // 机器人的线速度Y
    double direction; // 机器人的方向
    Robot() {
        this->id = -1;
        this->x = -1;
        this->y = -1;
        worktableId = -1;
        bringId = 0;
        timeCoef = 0;
        crashCoef = 0;
        angularSeppd = 0;
        linearSpeedX = 0;
        linearSpeedY = 0;
        direction = 0;

    }
    Robot(int id, double x, double y) {
        this->id = id;
        this->x = x;
        this->y = y;
        worktableId = -1;
        bringId = 0;
        timeCoef = 0;
        crashCoef = 0;
        angularSeppd = 0;
        linearSpeedX = 0;
        linearSpeedY = 0;
        direction = 0;
    }
    void outputTest() {
        TESTOUTPUT(fout << "Robot id: " << id << std::endl;)
        TESTOUTPUT(fout << "x: " << x << std::endl;)
        TESTOUTPUT(fout << "y: " << y << std::endl;)
        TESTOUTPUT(fout << "worktableId: " << worktableId << std::endl;)
        TESTOUTPUT(fout << "bringId: " << bringId << std::endl;)
        TESTOUTPUT(fout << "timeCoef: " << timeCoef << std::endl;)
        TESTOUTPUT(fout << "crashCoef: " << crashCoef << std::endl;)
        TESTOUTPUT(fout << "angularSeppd: " << angularSeppd << std::endl;)
        TESTOUTPUT(fout << "linearSpeedX: " << linearSpeedX << std::endl;)
        TESTOUTPUT(fout << "linearSpeedY: " << linearSpeedY << std::endl;)
        TESTOUTPUT(fout << "direction: " << direction << std::endl;)
    }
    // 卖材料的函数
    void Sell() {
        if (bringId == 0) return;// 机器人没有携带原材料
        // 机器人正在携带原材料
        if (worktableId == -1) return; // 机器人不在工作台附近
        // 机器人正在工作台附近
        if (worktables[worktableId].inputId[bringId] == 0 
            && sellSet.find(std::make_pair(bringId, worktables[worktableId].type)) != sellSet.end()) {
            // 该工作台的此原料不满,且支持卖出
            TESTOUTPUT(fout << "sell " << id << std::endl;)
            printf("sell %d\n", id);
            bringId = 0;
        }
    }
    // 买商品的函数
    void Buy() {
        if (bringId != 0) return;// 机器人已经携带原材料
        // 机器人没有携带原材料
        if (worktableId == -1) return; // 机器人不在工作台附近
        // 机器人正在工作台附近
        if (worktables[worktableId].output == true && money >= buyMoneyMap[createMap[worktables[worktableId].type]]) {
            TESTOUTPUT(fout << "buy " << id << std::endl;)
            printf("buy %d\n", id);
            bringId = worktables[worktableId].type;
            money -= buyMoneyMap[createMap[worktables[worktableId].type]];
        }
    }
    int FindBuy() {
        std::vector<std::pair<int, double>> distance;
        // 找到所有可以买的工作台
        for (int i = 0; i <= worktableNum; i++) {
            if (worktables[i].output == true && money >= buyMoneyMap[createMap[worktables[i].type]]) {
                distance.push_back(std::make_pair(i, (x - worktables[i].x) * (x - worktables[i].x) + (y - worktables[i].y) * (y - worktables[i].y)));
            }
        }
        std::sort(distance.begin(), distance.end(), [](std::pair<int, double> a, std::pair<int, double> b) {
            return a.second < b.second;
        });
        // 如果没有可以买的工作台
        // 那么就找一个生产剩余时间最少的工作台
        if (distance.size() == 0) {
            std::vector<std::pair<int, int>> timeLess;
            for (int i = 0; i <= worktableNum; i++) {
                timeLess.push_back(std::make_pair(i, worktables[i].remainTime));
            }
            std::sort(timeLess.begin(), timeLess.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
                return a.second < b.second;
            });
            // 如果这个工作台这回合没人去买
            for (auto & i : timeLess) {
                if (worktables[i.first].anyOneChooseBuy != nowTime) {
                    worktables[i.first].anyOneChooseBuy = nowTime;
                    return i.first;
                }
            }
            return timeLess[0].first;
        }
        // 如果这个工作台这回合没人去买
        for (auto & i : distance) {
            if (worktables[i.first].anyOneChooseBuy != nowTime) {
                worktables[i.first].anyOneChooseBuy = nowTime;
                return i.first;
            }
        }
        return distance[0].first;
    }
    int FindSell() {
        // 找到所有可以卖的工作台
        std::vector<std::pair<int, double>> distance;
        for (int i = 0; i <= worktableNum; i++) {
            if (worktables[i].inputId[bringId] == 0 && sellSet.find(std::make_pair(bringId, worktables[i].type)) != sellSet.end()) {
                distance.push_back(std::make_pair(i, (x - worktables[i].x) * (x - worktables[i].x) + (y - worktables[i].y) * (y - worktables[i].y)));
            }
        }
        // 选一个距离最近的
        std::sort(distance.begin(), distance.end(), [](std::pair<int, double> a, std::pair<int, double> b) {
            return a.second < b.second;
        });
        // 如果没有可以卖的工作台
        if (distance.size() == 0) {
            std::vector<std::pair<int, int>> timeLess;
            // 那么就找一个可以卖企鹅生产剩余时间最少的工作台
            for (int i = 0; i <= worktableNum; i++) {
                if (sellSet.find(std::make_pair(bringId, worktables[i].type)) != sellSet.end()) {
                    timeLess.push_back(std::make_pair(i, worktables[i].remainTime));
                }
            }
            std::sort(timeLess.begin(), timeLess.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
                return a.second < b.second;
            });
            // 如果这个工作台这回合没人去
            for (auto & i : timeLess) {
                if (worktables[i.first].anyOneChooseSell[bringId] != nowTime) {
                    worktables[i.first].anyOneChooseSell[bringId] = nowTime;
                    return i.first;
                }
            }
            return timeLess[0].first;
        }
        // 如果这个工作台这回合没人去
        for (auto & i : distance) {
            if (worktables[i.first].anyOneChooseSell[bringId] != nowTime) {
                worktables[i.first].anyOneChooseSell[bringId] = nowTime;
                return i.first;
            }
        }
        return distance[0].first;
    }
    bool DetecteCollision() {
        return false;
    }
    void Move(int worktableTogo) {
        std::vector<double> vec1 = {1, 0};
        std::vector<double> vec2 = {worktables[worktableTogo].x - x, worktables[worktableTogo].y - y};
        double cos = (vec1[0] * vec2[0] + vec1[1] * vec2[1]) / (sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1]) * sqrt(vec2[0] * vec2[0] + vec2[1] * vec2[1]));
        double angle = acos(cos);
        // 通过叉乘判断方向
        // > 0 逆时针 在上面
        // < 0 顺时针 在下面
        if (vec1[0] * vec2[1] - vec1[1] * vec2[0] < 0) {
            angle = -angle;
        }
        /*
            决定一下转向的大小
        */
        double rotate = 0;
        if (angle > direction) {
            if (std::abs(angle - direction) < M_PI) { // 如果角度差小于180°
                rotate = std::abs(angle - direction); // 就直接逆时针转过去
            } else { // 如果角度差大于180°
                rotate = -(M_PI*2-std::abs(angle - direction)); // 就顺时针转过去
            }
        } else { 
            if (std::abs(angle - direction) < M_PI) { // 如果角度差小于180°
                rotate = -(std::abs(angle - direction)); // 就直接顺时针转过去
            } else { // 如果角度差大于180°
                rotate = M_PI*2-std::abs(angle - direction); // 就逆时针转过去
            }
        }
        double absRotate = std::abs(rotate);
        double length = sqrt(vec2[0] * vec2[0] + vec2[1] * vec2[1]);
        /*
            决定一下速度
        */
        double speed = 0;
        // 由于判定范围是 0.4m,所以如果度数满足这个条件, 就直接冲过去
        if (absRotate < asin(0.4 / length)) {
            speed = 6;
        // } else if (absRotate < M_PI / 18) {
        //     // 如果度数小于10°, 就直接加速冲过去
        //     speed = 6;
        // } else if (absRotate < M_PI / 4) {
        //     // 如果度数大于10°小于 45° 距离大于10 就加速冲过去, 不然先转过去
        //     if (length > 10) {
        //         speed = 6;
        //     } else {
        //         speed = 0.5;
        //     }
        } else if (absRotate < M_PI / 4) {
            // 如果度数大于x小于45° 
            // 如果转向时间的路程不会更长,就可以走
            if (absRotate > 0.0001)
                speed = length * 50 / (absRotate / 3.6 + 1);
            if (speed > 6) {
                speed = 6;
            }
        } else if (absRotate < M_PI / 2) {
            // 如果度数大于45°小于 90°
            speed = 0;
        } else {
            // 如果度数大于90°, 就先倒退转过去
            speed = -2;
        }
        TESTOUTPUT(fout << "forward " << id << " " << speed << std::endl;)
        printf("forward %d %lf\n", id, speed);
        /*
            由于转向决定的是速度,所以 1s 最多转3.6°
            如果转向大于3.6°, 就直接最大速度转向
            如果转向小于3.6°, 就按照比例转向
        */
        if (absRotate < M_PI / 50) {
            double Ratio = absRotate / (M_PI / 50); 
            rotate = rotate / absRotate * M_PI * Ratio;
        } else {
            rotate = rotate / absRotate * M_PI;
        }
        TESTOUTPUT(fout << "rotate " << id << " " << rotate << std::endl;)
        printf("rotate %d %lf\n", id, rotate);
    }
    // 查找下一个目的地
    void FindMove() {
        int worktableTogo = -1;
        if (bringId == 0) { // 找地方去买
            worktableTogo = FindBuy();
        } else { // 找地方去卖
            worktableTogo = FindSell();
        }
        TESTOUTPUT(fout << "robot" << id << " want-go " << worktableTogo << " type=" << worktables[worktableTogo].type << std::endl;)
        Move(worktableTogo);
    }
    // 机器人具体的行为
    void action(){
        Sell();
        Buy();
        FindMove();
    }
};

Robot robots[MAX_Robot_Num];

#endif