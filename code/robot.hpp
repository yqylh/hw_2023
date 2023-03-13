#ifndef __REBOT_H__
#define __REBOT_H__

#include "config.hpp"
#include "worktable.hpp"
#include "vector.hpp"
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
#include <climits>

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
    int worktableTogo; // 机器人要去的工作台的 id
    double collisionRotate; // 机器人为防止碰撞的旋转角度
    double collisionSpeed; // 机器人为防止碰撞的线速度
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
        TESTOUTPUT(fout << "worktableTogo: " << worktableTogo << std::endl;)
        TESTOUTPUT(fout << "collisionRotate: " << collisionRotate << std::endl;)
        TESTOUTPUT(fout << "collisionSpeed: " << collisionSpeed << std::endl;)
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
    // 检查卖出去最少需要多少时间
    int minSellTime(int worktableId) {
        // 找到所有可以卖的工作台
        double minDistance = INT_MAX;
        int minId = -1;
        int sellID = createMap[worktables[worktableId].type];
        for (int i = 0; i <= worktableNum; i++) {
            // 对于每一个记录一下距离
            if (worktables[i].inputId[sellID] == 0 && sellSet.find(std::make_pair(sellID, worktables[i].type)) != sellSet.end()) {
                double distance = sqrt((x - worktables[i].x) * (x - worktables[i].x) + (y - worktables[i].y) * (y - worktables[i].y));
                if (distance < minDistance) {
                    minDistance = distance;
                    minId = i;
                }
            }
        }
        if (minId == -1) return MAX_TIME;
        // 0.12 是机器人的一帧的速度
        int minTime = minDistance / 0.12;
        return minTime;
    }
    double getDistance(int worktableId) {
        return sqrt((x - worktables[worktableId].x) * (x - worktables[worktableId].x) + (y - worktables[worktableId].y) * (y - worktables[worktableId].y));
    }
    int getMinGoToTime(int worktableId) {
        return getDistance(worktableId) / 0.12 + 25;
    }
    // 买商品的函数
    void Buy() {
        if (bringId != 0) return;// 机器人已经携带原材料
        // 机器人没有携带原材料
        if (worktableId == -1) return; // 机器人不在工作台附近
        // 机器人正在工作台附近
        if (canBuy[createMap[worktables[worktableId].type]] <= 0) return; // 这种物品已经足够多了
        if (worktables[worktableId].output == true && money >= buyMoneyMap[createMap[worktables[worktableId].type]]) {
            // 如果买了卖不出去
            if (nowTime + 25 + minSellTime(worktableId) > MAX_TIME) {
                worktables[worktableId].dontBuy = true;
                return;
            }
            // 未启用功能
            // canBuy[createMap[worktables[worktableId].type]]--;
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
            // 有生产的物品 && 买得起 && 还有可以买的
            if (worktables[i].output == true && money >= buyMoneyMap[createMap[worktables[i].type]] 
                && canBuy[createMap[worktables[i].type]] > 0 && worktables[i].dontBuy == false) {
                distance.push_back(std::make_pair(i, (x - worktables[i].x) * (x - worktables[i].x) + (y - worktables[i].y) * (y - worktables[i].y)));
            }
        }
        std::sort(distance.begin(), distance.end(), [](std::pair<int, double> a, std::pair<int, double> b) {
            return a.second < b.second;
        });
        // 如果没有可以买的工作台
        // 那么就找一个生产剩余时间最少的工作台(>0)
        if (distance.size() == 0) {
            std::vector<std::pair<int, int>> timeLess;
            for (int i = 0; i <= worktableNum; i++) /*if (worktables[i].remainTime != -1)*/ { // 真的在生产 !未启用功能
                timeLess.push_back(std::make_pair(i, worktables[i].remainTime));
                // timeLess.push_back(std::make_pair(i, getDistance(i)));
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
            // 如果一个都没找到可以去的,就去最中间
            // if (timeLess.size() == 0) return worktableNum / 2;
            // return timeLess[0].first;
            return worktableNum / 2;
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
            return -1;
        }
        // 如果这个工作台这回合没人去 且 按照优先级高到低进行
        for (int priority = 5; priority >= 2; priority--) {
            int index;
            for (auto & i : distance) {
                index++;
                if (index > distance.size() / 3) break;
                if (worktables[i.first].waitPriority == priority) {
                    if (worktables[i.first].anyOneChooseSell[bringId] != nowTime) {
                        worktables[i.first].anyOneChooseSell[bringId] = nowTime;
                        return i.first;
                    }
                }
            }
        }
        // 按照优先级前1/3没找到的话就随便找一个
        for (auto & i : distance) {
            if (worktables[i.first].anyOneChooseSell[bringId] != nowTime) {
                worktables[i.first].anyOneChooseSell[bringId] = nowTime;
                return i.first;
            }
        }
        return distance[0].first;
    }

    void Move() {
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
        if (collisionSpeed != -1) {
            speed = collisionSpeed;
        }
        TESTOUTPUT(fout << "forward " << id << " " << speed << std::endl;)
        printf("forward %d %lf\n", id, speed);
        /*
            由于转向决定的是速度,所以 1s 最多转3.6°
            如果转向大于3.6°, 就直接最大速度转向
            如果转向小于3.6°, 就按照比例转向
        */
        if (absRotate > 0.0001){
            if (absRotate < M_PI / 50) {
                double Ratio = absRotate / (M_PI / 50); 
                rotate = rotate / absRotate * M_PI * Ratio;
            } else {
                rotate = rotate / absRotate * M_PI;
            }
        }
        if (collisionRotate != -1) {
            rotate = collisionRotate;
        }
        TESTOUTPUT(fout << "rotate " << id << " " << rotate << std::endl;)
        printf("rotate %d %lf\n", id, rotate);
    }
    // 如果没地方卖,就销毁
    int Destroy() {
        /**
         * 特殊情况，有个工作台快做完了，而且材料全了（没啥用，暂时弃用）
        */
        for (auto i : worktables) {
            if (i.remainTime > 0 && sellSet.find(std::make_pair(bringId, i.type)) != sellSet.end()) {
                int all = 0;
                int have = 0;
                for (int item = 1; item <= MAX_Item_Type_Num; item++) {
                    if (sellSet.find(std::make_pair(item, i.type)) != sellSet.end()) {
                        all++;
                        if (i.inputId[item] > 0) {
                            have++;
                        }
                    }
                }
                if (have == all) {
                    return i.id;
                }
            }
        }
        TESTOUTPUT(fout << "destroy " << id << std::endl;)
        printf("destroy %d\n", id);
        return -1;
    }
    // 查找下一个目的地
    int FindMove() {
        int worktableTogo = -1;
        if (bringId == 0) { // 找地方去买
            worktableTogo = FindBuy();
        } else { // 找地方去卖
            worktableTogo = FindSell();
            if (worktableTogo == -1) { // 如果没有可以卖的地方
                int haveDestory = Destroy();
                if (haveDestory == -1) {
                    worktableTogo = FindBuy(); // 找地方去买
                } else {
                    worktableTogo = haveDestory;
                }
            }
        }
        TESTOUTPUT(fout << "robot" << id << " want-go " << worktableTogo << " type=" << worktables[worktableTogo].type << std::endl;)
        return worktableTogo;
    }
    // 机器人具体的行为
    void action(){
        Sell();
        Buy();
        worktableTogo = FindMove();
    }
    void checkCanBuy() {
        if (bringId != 0) {
            canBuy[bringId]--;
        }
    }
};

Robot robots[MAX_Robot_Num];

void DetecteCollision(int robot1, int robot2) {
    Vector2D robot1Pos = Vector2D(robots[robot1].x, robots[robot1].y);
    Vector2D robot2Pos = Vector2D(robots[robot2].x, robots[robot2].y);
    double robot1Radii = robots[robot1].bringId == 0 ? 0.45 : 0.53;
    double robot2Radii = robots[robot2].bringId == 0 ? 0.45 : 0.53;

    int futureTime = 13;
    // 距离太远 即 13hz*0.12m/s * 2=3.12m
    if ((robot1Pos-robot2Pos).length() > futureTime * 0.12 * 2) {
        return;
    }
    bool isCollision = false;
    for (int i = 0; i <= futureTime; i++) {
        auto robot1PosTemp = robot1Pos + Vector2D(robots[robot1].linearSpeedX * 0.02 * i, robots[robot1].linearSpeedY * 0.02 * i);
        auto robot2PosTemp = robot2Pos + Vector2D(robots[robot2].linearSpeedX * 0.02 * i, robots[robot2].linearSpeedY * 0.02 * i);
        if ((robot1PosTemp-robot2PosTemp).length() <= robot1Radii + robot2Radii + 0.3) {
            isCollision = true;
            break;
        }
    }
    if (!isCollision) {
        return;
    }
    TESTOUTPUT(fout << "robot" << robot1 << " and robot" << robot2 << " 检测碰撞" << std::endl;)

    double speed1 = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY).length();
    double speed2 = Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY).length();

    for (double speed = 6; speed >= -2; speed -= 0.1) {
        bool isCollision = false;
        Vector2D robot1PosTemp;
        Vector2D robot2PosTemp;
        int Realless1 = (6 - speed1) * 0.02 / 2;
        int Realless2 = (speed - speed2) * 0.02 / 2;
        for (int i = 0; i <= futureTime; i++) {
            robot1PosTemp = robot1Pos + Vector2D(6 / speed1 * robots[robot1].linearSpeedX * 0.02 * i - Realless1, 6 / speed1 * robots[robot1].linearSpeedY * 0.02 * i - Realless1);
            robot2PosTemp = robot2Pos + Vector2D(speed / speed2 * robots[robot2].linearSpeedX * 0.02 * i - Realless2 , speed / speed2 * robots[robot2].linearSpeedY * 0.02 * i - Realless2);
            if ((robot1PosTemp-robot2PosTemp).length() <= robot1Radii + robot2Radii + 1) {
                isCollision = true;
                break;
            }
        }
        if (!isCollision) {
            robots[robot2].collisionSpeed = speed;
            robots[robot2].collisionRotate = 0;
            robots[robot1].collisionSpeed = 6;
            robots[robot1].collisionRotate = 0;
            return;
        }
    }
    // double angle = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY).angle(Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY));
    // if (angle < M_PI / 4) {// 0~45
    //     int status = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY)^Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY);
    //     status = status > 0 ? 1 : -1;
    //     // 叉积 > 0 逆时针到达对方. < 0 顺时针到达对方
    //     robots[robot1].collisionRotate = -status * M_PI;
    //     robots[robot2].collisionRotate = status * M_PI;
    // } else if (angle > M_PI * 3 / 4) { // 135~180  ! 或许都是一个方向会比较好用
    //     robots[robot1].collisionRotate = M_PI;
    //     robots[robot2].collisionRotate = M_PI;
    // } else {
        // 旋转 > 135 同向 < 45 反向远离
        robots[robot1].collisionSpeed = 6;
        robots[robot1].collisionRotate = M_PI;
        robots[robot2].collisionSpeed = -2;
        robots[robot2].collisionRotate = -M_PI;
    // }
}
#endif