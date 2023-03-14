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
    int collisionTime; // 机器人为防止碰撞的时间
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
        collisionTime = 0;
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
        collisionTime = 0;
    }
    void outputTest() {
        TESTOUTPUT(fout << "Robot id: " << id << std::endl;)
        // TESTOUTPUT(fout << "x: " << x << std::endl;)
        // TESTOUTPUT(fout << "y: " << y << std::endl;)
        // TESTOUTPUT(fout << "worktableId: " << worktableId << std::endl;)
        // TESTOUTPUT(fout << "bringId: " << bringId << std::endl;)
        // TESTOUTPUT(fout << "timeCoef: " << timeCoef << std::endl;)
        // TESTOUTPUT(fout << "crashCoef: " << crashCoef << std::endl;)
        // TESTOUTPUT(fout << "angularSeppd: " << angularSeppd << std::endl;)
        // TESTOUTPUT(fout << "linearSpeedX: " << linearSpeedX << std::endl;)
        // TESTOUTPUT(fout << "linearSpeedY: " << linearSpeedY << std::endl;)
        // TESTOUTPUT(fout << "direction: " << direction << std::endl;)
        // TESTOUTPUT(fout << "worktableTogo: " << worktableTogo << std::endl;)
        // TESTOUTPUT(fout << "collisionRotate: " << collisionRotate << std::endl;)
        // TESTOUTPUT(fout << "collisionSpeed: " << collisionSpeed << std::endl;)
        TESTOUTPUT(fout << "speed" << Vector2D(linearSpeedX, linearSpeedY).length() << std::endl;)
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
                // timeLess.push_back(std::make_pair(i, std::max(worktables[i].remainTime, getMinGoToTime(i))));
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
        // // 如果这个工作台这回合没人去 且 按照优先级高到低进行
        // for (int priority = 3; priority >= 1; priority--) {
        //     int index;
        //     for (auto & i : distance) {
        //         if (i.second > distance[0].second * 1.5) break;
        //         if (buyPrioriryMap[worktables[i.first].type] == priority) {
        //             if (worktables[i.first].anyOneChooseBuy != nowTime) {
        //                 worktables[i.first].anyOneChooseBuy = nowTime;
        //                 return i.first;
        //             }
        //         }
        //     }
        // }
        // 按照优先级前1/3没找到的话就随便找一个 如果这个工作台这回合没人去买
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
        double cosAns = (vec1[0] * vec2[0] + vec1[1] * vec2[1]) / (sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1]) * sqrt(vec2[0] * vec2[0] + vec2[1] * vec2[1]));
        double angle = acos(cosAns);
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
        if (collisionSpeed != -3) {
            // collisionTime = futureTime;
            speed = collisionSpeed;
        } else {
            if (collisionTime > 0) {
                collisionTime--;
                speed = Vector2D(linearSpeedX, linearSpeedY).length();
                if (sin(direction) * linearSpeedY < 0 || cos(direction) * linearSpeedX < 0) {
                    speed = -speed;
                }
            }
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
            // collisionTime = futureTime;
            rotate = collisionRotate;
        } else {
            if (collisionTime > 0) {
                collisionTime--;
                rotate = 0;
            }
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

double solveChangeSpeed(double speed, double diff) {
    speed += diff;
    if (speed > 6) {
        speed = 6;
    }
    if (speed < -2) {
        speed = -2;
    }
    return speed;
}

void DetecteCollision(int robot1, int robot2) {
    // robot1 robot2 的坐标
    Vector2D robot1Pos = Vector2D(robots[robot1].x, robots[robot1].y);
    Vector2D robot2Pos = Vector2D(robots[robot2].x, robots[robot2].y);
    // robot1 robot2 的半径
    double robot1Radii = robots[robot1].bringId == 0 ? 0.451 : 0.531;
    double robot2Radii = robots[robot2].bringId == 0 ? 0.451 : 0.531;
    // 距离太远
    if ((robot1Pos-robot2Pos).length() > futureTime * 0.12 * 2 + robot1Radii + robot2Radii) {
        return;
    }
    bool isCollision = false;
    int collisionTime = 0;
    for (int i = 0; i <= futureTime; i++) {
        auto robot1PosTemp = robot1Pos + Vector2D(robots[robot1].linearSpeedX * 0.02 * i, robots[robot1].linearSpeedY * 0.02 * i);
        auto robot2PosTemp = robot2Pos + Vector2D(robots[robot2].linearSpeedX * 0.02 * i, robots[robot2].linearSpeedY * 0.02 * i);
        if ((robot1PosTemp-robot2PosTemp).length() <= robot1Radii + robot2Radii + 0.24) {
            isCollision = true;
            collisionTime = i;
            break;
        }
    }
    if (!isCollision) {
        return;
    }
    TESTOUTPUT(fout << "robot" << robot1 << " and robot" << robot2 << " 检测碰撞" << std::endl;)

    double angle = robots[robot1].direction - robots[robot2].direction;
    angle = std::abs(angle);
    if (angle > M_PI) {
        angle = 2 * M_PI - angle;
    }
    TESTOUTPUT(fout << "碰撞角度" << angle << std::endl;)

    if (angle > M_PI * 135 / 180) { // 135~180  ! 或许都是一个方向会比较好用
        TESTOUTPUT(fout << "collision need rotate" << std::endl;)
        int status1 = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY)^Vector2D(robots[2].x - robots[1].x, robots[2].y - robots[1].y);
        status1 = status1 > 0 ? 1 : -1;
        int status2 = Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY)^Vector2D(robots[1].x - robots[2].x, robots[1].y - robots[2].y);
        status2 = status2 > 0 ? 1 : -1;
        // 叉积 > 0 逆时针到达对方. < 0 顺时针到达对方
        robots[robot1].collisionRotate = -status1 * M_PI;
        robots[robot2].collisionRotate = -status2 * M_PI;
        robots[robot1].collisionSpeed = 6;
        robots[robot2].collisionSpeed = 6;
        // robots[robot1].collisionTime = 10;
        // robots[robot2].collisionTime = 10;
        if ((robot1Pos-robot2Pos).length() - robot1Radii - robot2Radii - 0.25 < 0) {
            // 距离比较近的情况
            // 不管
            // robots[robot1].collisionSpeed = -3;
            // robots[robot2].collisionSpeed = -3;
            // robots[robot1].collisionRotate = -1;
            // robots[robot2].collisionRotate = -1;
            // 随缘
            // robots[robot1].collisionRotate = -status1 * M_PI;
            // robots[robot2].collisionRotate = -status2 * M_PI;
            // robots[robot1].collisionSpeed = 6;
            // robots[robot2].collisionSpeed = -2;
        }
        return;
    }

    double speed1 = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY).length();
    if (sin(robots[robot1].direction) * robots[robot1].linearSpeedY < 0 || cos(robots[robot1].direction) * robots[robot1].linearSpeedX < 0) {
        speed1 = -speed1;
        TESTOUTPUT(fout << "反方向 robot" << robot1 << " speed1 = " << speed1 << std::endl;)
    }
    double speed2 = Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY).length();
    if (sin(robots[robot2].direction) * robots[robot2].linearSpeedY < 0 || cos(robots[robot2].direction) * robots[robot2].linearSpeedX < 0) {
        speed2 = -speed2;
        TESTOUTPUT(fout << "反方向 robot" << robot2 << " speed2 = " << speed2 << std::endl;)
    }
    // 按照0.3的速度改变为一个单位, 时间跨度是futureTime次, 改变单位的次数是 - future ~ future
    // 根据牵引力 大小 密度 算出来的加速度
    double acceleration1 = 250 / (20 * M_PI * robot1Radii * robot1Radii * 50);
    double acceleration2 = 250 / (20 * M_PI * robot2Radii * robot2Radii * 50);
    for (int accelerationTime1 = futureTime; accelerationTime1 >= -futureTime; accelerationTime1--) {
        for (int accelerationTime2 = futureTime; accelerationTime2 >= -futureTime; accelerationTime2--) {
            // 枚举每个机器人速度改变的帧率数量
            Vector2D robot1PosTemp = robot1Pos;
            Vector2D robot2PosTemp = robot2Pos;
            // 下一帧位置 i = 1 第一帧不论速度怎么设, 仍然维持之前的速度
            // 下一帧移动,然后才改变速度
            robot1PosTemp = robot1PosTemp + Vector2D(robots[robot1].linearSpeedX * 0.02, robots[robot1].linearSpeedY * 0.02);
            robot2PosTemp = robot2PosTemp + Vector2D(robots[robot2].linearSpeedX * 0.02, robots[robot2].linearSpeedY * 0.02);
            // 从第二帧开始检测
            bool isCollision = false;
            // 根据加速度和次数计算出每次的该变量
            double changeSpeed1 = 0;
            double changeSpeed2 = 0;
            if (accelerationTime1 != 0) changeSpeed1 = accelerationTime1 / std::abs(accelerationTime1) * acceleration1;
            if (accelerationTime2 != 0) changeSpeed2 = accelerationTime2 / std::abs(accelerationTime2) * acceleration2;
            // 计算出每个机器人需要改变多少次
            int changeTime1 = std::abs(accelerationTime1);
            int changeTime2 = std::abs(accelerationTime2);
            // 目前使用的速度
            double testSpeed1 = speed1;
            double testSpeed2 = speed2;
            if (changeTime1-- > 0) testSpeed1 = solveChangeSpeed(testSpeed1, changeSpeed1);
            if (changeTime2-- > 0) testSpeed2 = solveChangeSpeed(testSpeed2, changeSpeed2);
            for (int i = 2; i <= futureTime; i++) {
                robot1PosTemp = robot1PosTemp + Vector2D(testSpeed1 * cos(robots[robot1].direction) * 0.02, testSpeed1 * sin (robots[robot1].direction) * 0.02);
                robot2PosTemp = robot2PosTemp + Vector2D(testSpeed2 * cos(robots[robot2].direction) * 0.02 , testSpeed2 * sin (robots[robot2].direction) * 0.02);
                if ((robot1PosTemp-robot2PosTemp).length() <= robot1Radii + robot2Radii + 0.24) {
                    isCollision = true;
                    break;
                }
                if (changeTime1-- > 0) testSpeed1 = solveChangeSpeed(testSpeed1, changeSpeed1);
                if (changeTime2-- > 0) testSpeed2 = solveChangeSpeed(testSpeed2, changeSpeed2);
            }
            if (std::abs(testSpeed1) < 2 && std::abs(testSpeed2) < 2) {
                continue;
            }
            if (!isCollision) {
                TESTOUTPUT(
                    fout << "robot" << robot1 << " 速度改变" << accelerationTime1 << " 帧" << std::endl;
                    fout << "robot" << robot2 << " 速度改变" << accelerationTime2 << " 帧" << std::endl;
                    fout << "robot" << robot1 << " 速度从" << speed1 << "->" << testSpeed1 << std::endl;
                    fout << "robot" << robot2 << " 速度从" << speed2 << "->" << testSpeed2 << std::endl;
                )
                if (accelerationTime1 != 0) {
                    robots[robot1].collisionSpeed = 6 * accelerationTime1 / std::abs(accelerationTime1);
                    // robots[robot1].collisionTime = collisionTime - 2;
                } else {
                    robots[robot1].collisionSpeed = speed1;
                }
                if (accelerationTime2 != 0) {
                    robots[robot2].collisionSpeed =  6 * accelerationTime2 / std::abs(accelerationTime2);
                    // robots[robot2].collisionTime = collisionTime - 2;
                } else {
                    robots[robot2].collisionSpeed = speed2;
                }
                robots[robot1].collisionRotate = 0;
                robots[robot2].collisionRotate = 0;
                return;
            }
        }
    }
    int status1 = Vector2D(robots[robot1].linearSpeedX, robots[robot1].linearSpeedY)^Vector2D(robots[2].x - robots[1].x, robots[2].y - robots[1].y);
    status1 = status1 > 0 ? 1 : -1;
    int status2 = Vector2D(robots[robot2].linearSpeedX, robots[robot2].linearSpeedY)^Vector2D(robots[1].x - robots[2].x, robots[1].y - robots[2].y);
    status2 = status2 > 0 ? 1 : -1;
    // 叉积 > 0 逆时针到达对方. < 0 顺时针到达对方
    robots[robot1].collisionRotate = -status1 * M_PI;
    robots[robot2].collisionRotate = -status2 * M_PI;
    robots[robot1].collisionSpeed = 6;
    robots[robot2].collisionSpeed = -2;
    return;
}
#endif