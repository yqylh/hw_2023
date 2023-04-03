#ifndef __REBOT_H__
#define __REBOT_H__

#include "config.hpp"
#include "worktable.hpp"
#include "vector.hpp"
#include "path.hpp"
#include "grid.hpp"
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
#include <queue>

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
    int collisionSpeedTime; // 机器人为防止碰撞的调整速度的时间
    int collisionRotateTime; // 机器人为防止碰撞的时间
    Path *path; // 机器人的路径
    std::vector<Vector2D> pathPoints; // 机器人的路径点
    int zeroTime = 0; // 机器人的零速度时间
    bool isWait = false; // 机器人是否在等待
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
        collisionSpeedTime = 0;
        collisionRotateTime = 0;
        path = nullptr;
        zeroTime = 0;
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
        collisionSpeedTime = 0;
        collisionRotateTime = 0;
        path = nullptr;
        zeroTime = 0;
    }
    void checkCanBuy() {
        if (bringId != 0) {
            canBuy[bringId]--;
        }
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
        if (path == nullptr) return;
        // 机器人正在携带原材料
        if (worktableId != path->sellWorktableId) return; // 机器人不在预想工作台附近
        // 机器人正在工作台附近
        if (worktables[worktableId].inputId[bringId] == 0 
            && sellSet.find(std::make_pair(bringId, worktables[worktableId].type)) != sellSet.end()) {
            // 该工作台的此原料不满,且支持卖出
            TESTOUTPUT(fout << "sell " << id << std::endl;)
            printf("sell %d\n", id);
            // 统计损失
            lossCollMoney += sellMoneyMap[worktables[worktableId].type] * (1 - crashCoef);
            if (crashCoef < 1) {
                TESTOUTPUT(fout << "有碰撞损失: " << crashCoef << std::endl;)
            }
            lossTimeMoney += sellMoneyMap[worktables[worktableId].type] * (1 - timeCoef);
            // 工作台这一位设置成有东西了
            worktables[worktableId].inputId[bringId] = 1;
            {
                /**
                 * 如果没有生产的情况下, 该工作台的所有原料都满了, 则清空该工作台的原料
                */
                bool full = true;
                for (int item = 1; item <= MAX_Item_Type_Num; item++) {
                    if (sellSet.find(std::make_pair(item, worktables[worktableId].type)) != sellSet.end()) {
                        if (worktables[worktableId].inputId[item] == 0) {
                            full = false;
                            break;
                        }
                    }
                }
                if (full && worktables[worktableId].remainTime == -1) {
                    for (int item = 1; item <= MAX_Item_Type_Num; item++) {
                        worktables[worktableId].inputId[item] = 0;
                    }
                }
            }
            worktables[worktableId].someWillSell[bringId]--;
            bringId = 0;
            path = nullptr;
            worktableTogo = -1;
        }
    }
    // 买商品的函数
    void Buy() {
        if (bringId != 0) return;// 机器人已经携带原材料
        if (path == nullptr) return;
        // 机器人没有携带原材料
        if (worktableId != path->buyWorktableId) return; // 机器人不在预想工作台附近
        // 机器人正在工作台附近
        if (worktables[worktableId].output == true && money >= buyMoneyMap[createMap[worktables[worktableId].type]]) {
            // 如果买了卖不出去
            TESTOUTPUT(fout << "buy " << id << std::endl;)
            printf("buy %d\n", id);
            bringId = createMap[worktables[worktableId].type];
            money -= buyMoneyMap[createMap[worktables[worktableId].type]];
            worktables[worktableId].someWillBuy--;
            worktables[worktableId].output = false;
            if (worktables[worktableId].remainTime == 0) {
                worktables[worktableId].output = true;
            }
            worktableTogo = path->sellWorktableId;
            pathPoints = movePath();
        }
    }


    void Move() {
        Vector2D go;
        if (worktableTogo == -1) {
            pathPoints.push_back(Vector2D(worktables[1].x, worktables[1].y));
        }
        while (pathPoints.size() > 0 && (Vector2D(x, y) - pathPoints[0]).length() < 0.4) {
            pathPoints.erase(pathPoints.begin());
        }
        if (isWait == true && pathPoints.size() == 0) {
            TESTOUTPUT(fout << "forward " << id << " " << 0 << std::endl;)
            printf("forward %d %d\n", id, 0);
            TESTOUTPUT(fout << "rotate " << id << " " << 0 << std::endl;)
            printf("rotate %d %d\n", id, 0);
            return;
        }
        TESTOUTPUT(fout << "from" << "(" << x << ", " << y << ")" << "to" << "(" << pathPoints[0].x << ", " << pathPoints[0].y << ")" << std::endl;)
        std::vector<double> vec1 = {1, 0};
        std::vector<double> vec2 = {pathPoints[0].x - x, pathPoints[0].y - y};
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
        // TESTOUTPUT(fout << "absRotate: " << absRotate << " length =" << length << " asin=" << asin(0.4 / length) << std::endl;)
        // 由于判定范围是 0.4m,所以如果度数满足这个条件, 就直接冲过去
        if (0.4 / length > 1) {
            speed = 6;
        } else
        if (absRotate < asin(0.4 / length)) {
            speed = 6;
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
            speed = -1;
        }
        if (collisionSpeedTime > 0) {
            collisionSpeedTime--;
            speed = collisionSpeed;
        }
        if (Vector2D(vec2[0], vec2[1]).length() < 1.2 && speed > 0) {
            // TESTOUTPUT(fout << "changed from" << speed << " route=" << absRotate << std::endl;)
            speed = Vector2D(vec2[0], vec2[1]).length() / 1.2 * 6;
            if (speed < 1) speed = 1;
        }
        TESTOUTPUT(fout << "forward " << id << " " << speed << std::endl;)
        printf("forward %d %lf\n", id, speed);
        /*
            由于转向决定的是速度,所以 1s 最多转3.6°
            如果转向大于3.6°, 就直接最大速度转向
            如果转向小于3.6°, 就按照比例转向
        */
        if (absRotate > 0.0001){
            if (absRotate < M_PI / 25) {
                double Ratio = absRotate / (M_PI / 25); 
                rotate = rotate / absRotate * M_PI * Ratio;
            } else {
                rotate = rotate / absRotate * M_PI;
            }
        }
        if (collisionRotateTime > 0) {
            collisionRotateTime--;
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
    double getMinGoToTime(double x1, double y1, double x2, double y2) {
        double length = (Vector2D(x1, y1) - Vector2D(x2, y2)).length();
        return length / 0.12 + 25;
    }
    void FindAPath() {
        std::vector<Path *> paths;
        std::vector<Path *> paths4567;
        for (auto & buy : worktables) {
            /**
             * 有产物, 没人预约
             * 有产物, 有人预约买,但是第二个在做或者做完阻塞了
             * 没产物, 没人预约买,在做了
            */
            if (buy.id == -1) break;
            if ((buy.output == true && buy.someWillBuy == 0)
                || (buy.output == true && buy.someWillBuy == 1 && buy.remainTime != -1)
                || (buy.output == false && buy.someWillBuy == 0 && buy.remainTime != -1)
                || (buy.type < 4)
            ) {} else continue;
            // 等待生产的时间
            double waitBuyTime = 0; 
            if ( (buy.someWillBuy == 1) || (buy.output == false && buy.someWillBuy == 0) ) waitBuyTime = buy.remainTime;
            // 路程时间消耗
            double goBuyTime;
            if (worktableId == -1) {
                goBuyTime = RobotToWT[id][buy.id];
            } else {
                goBuyTime = WTtoWT[worktableId][buy.id];
            }
            goBuyTime = goBuyTime * 0.6 / 0.11;
            // double goBuyTime = getMinGoToTime(x, y, buy.x, buy.y);
            // 如果等待时间比路程时间长,就不用买了
            if (goBuyTime < waitBuyTime) continue;
            // 购买的产品
            int productId = createMap[buy.type];
            for (auto & sell : worktables) {
                if (sell.id == -1) break;
                // 确保这个工作台支持买,而且输入口是空的
                if (sellSet.find(std::make_pair(productId, sell.type)) == sellSet.end() || sell.inputId[productId] == 1) continue;
                // 确保不是墙角
                if (sell.isNearCorner) continue;
                /**
                 * 确保没人预约卖
                 * 或者类型是8 || 9
                */
                if (sell.someWillSell[productId] == 0 || sell.type == 8 || sell.type == 9) {} else continue;
                // 时间消耗
                double goSellTime = WTtoWTwithItem[buy.id][sell.id];
                goSellTime = goSellTime * 0.6 / 0.11;
                // double goSellTime = getMinGoToTime(buy.x, buy.y, sell.x, sell.y);
                double sumTime = std::max(goBuyTime, waitBuyTime) + goSellTime;
                if (sumTime + 30 + nowTime > MAX_TIME) continue;
                // 时间损失
                double timeLoss;
                if (sumTime >= 9000) {
                    timeLoss = 0.8;
                } else {
                    timeLoss = 0.8 + (1-0.8) * (1 - sqrt(1 - (1 - sumTime / 9000) * (1 - sumTime / 9000)));
                }
                // 卖出产品赚取的钱
                double earnMoney = sellMoneyMap[productId] * timeLoss - buyMoneyMap[productId];
                // 尽量不卖给 9
                if (sell.type == 9) earnMoney = earnMoney * 0.6;
                earnMoney *= buy.near7;
                earnMoney *= sell.near7;
                if (sell.waitPriority == 5) {
                    earnMoney *= 1.2;
                }
                if (buy.isNearCorner) {
                    earnMoney *= 0.1;
                }
                // 有资源缺口 即卖工作台的类型对应的产品(type 相同)有缺口 就促进生产
                if (canBuy[sell.type] > 0 && (sell.type == 4 || sell.type == 5 || sell.type == 6) && sell.near7 != 1) {
                    // TESTOUTPUT(fout << "canBuy " << sell.type << std::endl;)
                    earnMoney *= 1.2;
                }
                Path * path = new Path(buy.id, sell.id, id, earnMoney, sumTime);
                if ((productId == 4 || productId == 5 || productId == 6 || productId == 7) && ((buy.remainTime == 0 && buy.someWillBuy == 0) || (buy.remainTime < goBuyTime && buy.output == true && buy.someWillBuy == 0))) {
                    paths4567.push_back(path);
                }else {
                    paths.push_back(path);
                }
            }
        }
        // 理论上只有快结束才出现
        if (paths.size() == 0 && paths4567.size() == 0) {
            TESTOUTPUT(fout << "error" << std::endl;)
            worktableTogo = -1;
            return;
        }
        std::sort(paths.begin(), paths.end(), [](Path * a, Path * b) {
            return a->parameters > b->parameters;
        });
        std::sort(paths4567.begin(), paths4567.end(), [](Path * a, Path * b) {
            return a->parameters > b->parameters;
        });
        if (paths4567.size() > 0 && (paths.size() == 0 || paths4567[0]->parameters > paths[0]->parameters * 0.95)) {
            path = paths4567[0];
        } else {
            path = paths[0];
        }
        TESTOUTPUT(fout << "robot" << id << " find path " << path->buyWorktableId << " " << path->sellWorktableId << std::endl;)
        worktableTogo = path->buyWorktableId;
        pathPoints = movePath();
        worktables[path->buyWorktableId].someWillBuy++;
        worktables[path->sellWorktableId].someWillSell[createMap[worktables[path->buyWorktableId].type]]++;
    }
    double point_to_segment_distance(Vector2D begin, Vector2D end, Vector2D obstacle) {
        Vector2D begin_to_end = end-begin;
        Vector2D begin_to_obstacle = obstacle - begin;
        Vector2D end_to_obstacle = obstacle - end;
        if (begin_to_end * begin_to_obstacle <= 0) return (begin-obstacle).length();
        if (begin_to_end * end_to_obstacle >= 0) return (end-obstacle).length();
        return fabs(begin_to_end ^ begin_to_obstacle) / (begin-end).length();
    }
    std::vector<std::pair<Vector2D, Vector2D>> fixpath(std::vector<std::pair<Vector2D, Vector2D>> path, std::set<Vector2D> *blocked = nullptr) {
        // TESTOUTPUT(fout << path.size() << std::endl;)
        std::vector<std::pair<Vector2D, Vector2D>> ret;
        auto begin = path.begin();
        // 不断延迟线段的终点
        // 如果碰撞了,就不加这个点, 以最后一个点开始继续这个过程
        // 如果没有碰撞,就加上这个点的碰撞点
        while (begin != path.end()) {
            // TESTOUTPUT(fout << "线段从 " << begin->x << "," << begin->y << "开始" << std::endl;)
            auto end = begin;
            end++;
            while (end != path.end()) {
                end++;
                if (end == path.end()) break;
                bool flag = false;
                // TESTOUTPUT(fout << "测试到 " << end->x << "," << end->y << "是否碰撞" << std::endl;)
                for (double x = begin->first.x; flag == false ; x += (end->first.x - begin->first.x) / std::abs(end->first.x - begin->first.x) * 0.5) {
                    // TESTOUTPUT(fout << "x = " << x << std::endl;)
                    for (double y = begin->first.y; flag == false ; y += (end->first.y - begin->first.y) / std::abs(end->first.y - begin->first.y) * 0.5) {
                        for (auto & obstacle : grids[Vector2D(x, y)]->obstacles) {
                            // 计算obstacle到 begin->end这条线段的距离
                            double distance = point_to_segment_distance(begin->second, end->second, obstacle);
                            if (distance < 0.53) {// 碰撞了
                                // TESTOUTPUT(fout << "碰撞点" << obstacle.x << "," << obstacle.y << std::endl;)
                                flag = true;
                                break;
                            }
                        }
                        if (blocked != nullptr)for (auto & obstacle : *blocked) {
                            // TESTOUTPUT(fout << "blocked" << obstacle.x << "," << obstacle.y << std::endl;)
                            double distance = point_to_segment_distance(begin->second, end->second, obstacle);
                            if (distance < 0.53 + 0.4) {// 碰撞了 0.53是机器人半径, 0.4是方格斜边的一半
                                flag = true;
                                break;
                            }
                        }
                        if (y == end->first.y) break;
                    }
                    if (x == end->first.x) break;
                }
                if (flag) {
                    // TESTOUTPUT(fout << "碰撞了" << std::endl;)
                    break;
                }
            }
            ret.push_back(*begin);
            end--; 
            if (end == begin) end++;
            begin = end;
        }
        ret.push_back(path.back());
        return ret;
    }
    std::vector<Vector2D> DodgingCorners(std::vector<Vector2D> path, std::set<Vector2D> *blocked = nullptr) {
        std::vector<std::pair<Vector2D, Vector2D>> solved;
        auto lastPoint = path.front();
        for (auto & point : path) {
            // 如果是起点,就不用处理, 精度更准
            if (point == path.front()) {
                Vector2D p1 = point;
                Vector2D p2 = Vector2D(x,y);
                solved.push_back(std::make_pair(p1, p2));
                continue;
            }
            Vector2D p1 = point;
            Vector2D p2 = point;
            if (bringId == 0) {
                int num = 0;
                for (auto & item : grids[p1]->obstacles) {
                    if ((item - p1).length() < 0.45) {
                        auto delta = p1 - item;
                        double ratio = 0.49 / 0.25;
                        // double ratio = 0.5 / delta.length();
                        // if (num == 0) {
                            p2 = item + delta * ratio;
                        //     num++;
                        // } else {
                            // p2 = p2 + delta * ratio;
                            break;
                        // }
                    }
                }
            } else {
                for (auto & item : grids[p1]->obstacles) {
                    if ((item - p1).length() < 0.53) {
                        auto delta = item - p1;
                        auto deltaLastToNow = p1 - lastPoint;
                        double ratio;
                        if ( !(lastPoint == p1) && deltaLastToNow.angle(delta) < M_PI / 2) {
                            ratio = 0.7788;
                        } else {
                            ratio = 0.6;
                        }
                        p2 = item - delta / delta.length() * ratio;
                        // auto delta = p1 - item;
                        // double ratio = 0.55 / 0.25;
                        // p2 = item + delta * ratio;
                        break;
                    }
                }
            }
            solved.push_back(std::make_pair(p1, p2));
            lastPoint = point;
        }
        solved = fixpath(solved, blocked);
        path.clear();
        for (auto & item : solved) {
            path.push_back(item.second);
        }
        CREATEMAP(mapOut << "time=" << nowTime << " robotId=" << id << " optimized carry=" << (bringId == 0 ? false : true) << std::endl;)
        for (auto & item : path) {
            CREATEMAP(mapOut << "(" << item.x << "," << item.y << ")" << "->";)
        }
        CREATEMAP(mapOut << std::endl;)
        return path;
    }
    /**
     * 计算路径
     * 计算从一个坐标移动到另一个坐标的路径
     * 通过 BFS 实现
     * 返回值应该是一个n个点的坐标的数组
    */
    std::vector<Vector2D> movePath(std::set<Vector2D> *blocked = nullptr){
        Vector2D to(worktables[worktableTogo].x, worktables[worktableTogo].y);
        std::vector<Vector2D> path;
        std::map<Vector2D, Vector2D> fromWhere;
        std::queue<Vector2D> q;
        double x = this->x;
        double y = this->y;
        // 计算当前位置的格子
        x = int(x / 0.5) * 0.5 + 0.25;
        y = int(y / 0.5) * 0.5 + 0.25;
        q.push(Vector2D(x, y));
        fromWhere.insert(std::make_pair(Vector2D(x, y), Vector2D(x, y)));
        // int findItme = 0;
        bool find = false;
        while (!q.empty() && find == false) {
            // findItme++;
            // 当前的位置
            Vector2D now = q.front();
            q.pop();
            // 八个方向的移动
            std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
            for (auto &add : adds) {
                // 移动后的位置
                Vector2D next = now + Vector2D(add.first, add.second);
                if (next.x <= 0.25 || next.x >= 49.75 || next.y <= 0.25 || next.y >= 49.75) continue;
                // 没访问过
                if (fromWhere.find(next) != fromWhere.end()) continue;
                // 是墙
                if (grids[next]->type == 1) continue;
                // 是其他机器人的位置
                if (blocked != nullptr && blocked->find(next) != blocked->end()) continue;
                if (bringId == 0) {
                    // 不携带物品
                    // 可以碰两个角
                    int num = 0;
                    if (!(next == to))for (auto & item : grids[next]->obstacles) {
                        if ((next - item).length() < 0.45) {
                            num++;
                        }
                    }
                    int nowNum = 0;
                    for (auto & item : grids[now]->obstacles) {
                        if ((now - item).length() < 0.45) {
                            nowNum++;
                        }
                    }
                    if (nowNum >= 3) {
                        if (num > 1) continue; 
                    } else 
                    if (num > 2) continue;
                } else {
                    // 携带物品
                    int num = 0;
                    Vector2D obstacles;
                    if (!(next == to))for (auto & item : grids[next]->obstacles) {
                        if ((next - item).length() < 0.53) {
                            num++;
                            obstacles = item;
                        }
                    }
                    // 碰到了至少两个角,一定不能去
                    if (num > 1) continue;
                    // 只碰到了一个角,可能可以去
                    if (num == 1) {
                        Vector2D deltaNextToObstacles = obstacles - next;
                        Vector2D deltaNowToNext = next - now;
                        // 左上或者右下
                        if (deltaNextToObstacles.x == deltaNextToObstacles.y) {
                            if ((deltaNowToNext ^ deltaNextToObstacles) < 0) {
                                // 顺时针转
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 2 * deltaNextToObstacles.y)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 4 * deltaNextToObstacles.y)]->type == 1) continue;
                            } else if ((deltaNowToNext ^ deltaNextToObstacles) > 0)  {
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(2 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(4 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                            } else {
                                continue;
                            }
                        } else {
                            // 右下或者左上
                            if ((deltaNowToNext ^ deltaNextToObstacles) > 0) {
                                // 顺时针转
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 2 * deltaNextToObstacles.y)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 4 * deltaNextToObstacles.y)]->type == 1) continue;
                            } else if ((deltaNowToNext ^ deltaNextToObstacles) < 0)  {
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(2 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(4 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                            } else {
                                continue;
                            }
                        }
                    }   
                }
                fromWhere.insert(std::make_pair(next, now));
                q.push(next);
                if (next == to){
                    find = true;
                }
            }
        }
        // TESTOUTPUT(fout << "find " << findItme << std::endl;)
        while ( 1 ) {
            path.push_back(to);
            if (to == fromWhere[to]) break;
            to = fromWhere[to];
        }
        std::reverse(path.begin(), path.end());
        CREATEMAP(mapOut << "time=" << nowTime << " robotId=" << id << " origin carry=" << (bringId == 0 ? false : true) << std::endl;)
        for (auto & item : path) {
            CREATEMAP(mapOut << "(" << item.x << "," << item.y << ")" << "->";)
        }
        CREATEMAP(mapOut << std::endl;)
        if (path.size() > 2) path = DodgingCorners(path, blocked);
        return path;
    }
    void checkDead();
    // 机器人具体的行为
    void action(){
        Sell();
        if (path == nullptr) FindAPath();
        Buy();
        checkDead();
        TESTOUTPUT(fout << "robot" << id << " want-go " << worktableTogo << " type=" << worktables[worktableTogo].type << std::endl;)
    }
    void checkWall() {
        if (worktableTogo == -1) return;
        bool wallnear = false;
        double toWallX = std::min(worktables[worktableTogo].y - 0.53 - 0.12, 50 - 0.53 - 0.12 -worktables[worktableTogo].y);
        double toWallY = std::min(worktables[worktableTogo].x - 0.53 - 0.12, 50 - 0.53 - 0.12 -worktables[worktableTogo].x);
        // 刹车最常的距离 1.86
        if (toWallX < 0.02 * 3 * 31 || toWallY < 0.02 * 3 * 31) {
            wallnear = true;
        }
        if (wallnear == false) return;
        if (worktables[worktableTogo].type == 9 || worktables[worktableTogo].type == 8) {
            return;
        }
        if (bringId > 0 && worktables[worktableTogo].output == false) {
            return;
        }
        double speed = Vector2D(linearSpeedX, linearSpeedY).length();
        double length = 0;
        int costTime = 0;
        while (speed > 0) {
            length += speed * 0.02;
            costTime++;
            speed -= 0.3;
        }
        // double radii = bringId == 0 ? 0.45 : 0.53;
        // double acceleration = 250 / (20 * M_PI * radii * radii * 50);
        // double length = speed * speed / (2 * acceleration);
        toWallX = std::min(y - 0.53 - 0.12, 50 - 0.53 - 0.12 -y);
        toWallY = std::min(x - 0.53 - 0.12, 50 - 0.53 - 0.12 -x);
        TESTOUTPUT(fout << "robot" << id << " length=" << length << " toWallX=" << toWallX << " toWallY=" << toWallY << std::endl;)
        if (toWallX < std::abs(length * sin(direction)) || toWallY < std::abs(length * cos(direction))) {
            TESTOUTPUT(fout << "robot" << id << " 有撞墙风险 " << std::endl;)
            // Vector2D normal(0, 0);
            // if (toWallX < length * sin(direction) && toWallX == y-0.53-0.1) {
            //     normal = Vector2D(0, -1);
            // } else if (toWallX < length * sin(direction) && toWallX == 50-0.53-0.1-y) {
            //     normal = Vector2D(0, 1);
            // } else if (toWallY < length * cos(direction) && toWallY == x-0.53-0.1) {
            //     normal = Vector2D(-1, 0);
            // } else if (toWallY < length * cos(direction) && toWallY == 50-0.53-0.1-x) {
            //     normal = Vector2D(1, 0);
            // }
            // collisionSpeed = -2;
            // collisionRotate = normal^Vector2D(cos(direction), sin(direction)) * M_PI;
            collisionSpeed = -2;
            collisionSpeedTime = costTime - 1;
            // if (bringId != 0)
            // collisionRotate = M_PI;
        }
    }
    void findNullPath(std::set<Vector2D> *cantGo, std::set<Vector2D> *blocked) {
        isWait = true;
        std::vector<Vector2D> path;
        std::map<Vector2D, Vector2D> fromWhere;
        std::queue<Vector2D> q;
        // 计算当前位置的格子
        double nowx = int(x / 0.5) * 0.5 + 0.25;
        double nowy = int(y / 0.5) * 0.5 + 0.25;
        q.push(Vector2D(nowx, nowy));
        fromWhere.insert(std::make_pair(Vector2D(nowx, nowy), Vector2D(nowx, nowy)));
        bool find = false;
        Vector2D target;
        while (!q.empty() && find == false) {
            // 当前的位置
            Vector2D now = q.front();
            q.pop();
            // 八个方向的移动
            std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
            for (auto &add : adds) {
                // 移动后的位置
                Vector2D next = now + Vector2D(add.first, add.second);
                if (next.x <= 0.25 || next.x >= 49.75 || next.y <= 0.25 || next.y >= 49.75) continue;
                // 没访问过
                if (fromWhere.find(next) != fromWhere.end()) continue;
                // 是墙
                if (grids[next]->type == 1) continue;
                // 是其他机器人的位置
                if (cantGo->find(next) != cantGo->end()) continue;
                if (bringId == 0) {
                    // 不携带物品
                    // 可以碰两个角
                    int num = 0;
                    for (auto & item : grids[next]->obstacles) {
                        if ((next - item).length() < 0.45) {
                            num++;
                        }
                    }
                    int nowNum = 0;
                    for (auto & item : grids[now]->obstacles) {
                        if ((now - item).length() < 0.45) {
                            nowNum++;
                        }
                    }
                    if (nowNum >= 3) {
                        if (num > 1) continue; 
                    } else 
                    if (num > 2) continue;
                } else {
                    // 携带物品
                    int num = 0;
                    Vector2D obstacles;
                    for (auto & item : grids[next]->obstacles) {
                        if ((next - item).length() < 0.53) {
                            num++;
                            obstacles = item;
                        }
                    }
                    // 碰到了至少两个角,一定不能去
                    if (num > 1) continue;
                    // 只碰到了一个角,可能可以去
                    if (num == 1) {
                        Vector2D deltaNextToObstacles = obstacles - next;
                        Vector2D deltaNowToNext = next - now;
                        // 左上或者右下
                        if (deltaNextToObstacles.x == deltaNextToObstacles.y) {
                            if ((deltaNowToNext ^ deltaNextToObstacles) < 0) {
                                // 顺时针转
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 2 * deltaNextToObstacles.y)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 4 * deltaNextToObstacles.y)]->type == 1) continue;
                            } else if ((deltaNowToNext ^ deltaNextToObstacles) > 0)  {
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(2 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(4 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                            } else {
                                continue;
                            }
                        } else {
                            // 右下或者左上
                            if ((deltaNowToNext ^ deltaNextToObstacles) > 0) {
                                // 顺时针转
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 2 * deltaNextToObstacles.y)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 4 * deltaNextToObstacles.y)]->type == 1) continue;
                            } else if ((deltaNowToNext ^ deltaNextToObstacles) < 0)  {
                                if (grids[next + Vector2D(0, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(-deltaNextToObstacles.x * 4, 0)]->type == 1) continue;
                                if (grids[next + Vector2D(2 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                                if (grids[next + Vector2D(4 * deltaNextToObstacles.x, -deltaNextToObstacles.y * 4)]->type == 1) continue;
                            } else {
                                continue;
                            }
                        }
                    }   
                }
                fromWhere.insert(std::make_pair(next, now));
                q.push(next);
                bool isNull = true;
                for (auto &add : adds) {
                    if (grids[next + Vector2D(add.first, add.second)]->type == 0 
                        && blocked->find(next + Vector2D(add.first, add.second)) == blocked->end()) {
                        continue;
                    } else {
                        isNull = false;
                        break;
                    }
                }
                if (isNull) {
                    find = true;
                    target = next;
                }
            }
        }
        while ( 1 ) {
            path.push_back(target);
            if (target == fromWhere[target]) break;
            target = fromWhere[target];
        }
        std::reverse(path.begin(), path.end());
        CREATEMAP(mapOut << "time=" << nowTime << " robotId=" << id << " origin carry=" << (bringId == 0 ? false : true) << std::endl;)
        for (auto & item : path) {
            CREATEMAP(mapOut << "(" << item.x << "," << item.y << ")" << "->";)
        }
        CREATEMAP(mapOut << std::endl;)
        if (path.size() > 2) path = DodgingCorners(path);
        pathPoints = path;
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

std::set<Vector2D> *getPathLabel(std::vector<Vector2D> path, int id) {
    std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
    path.insert(path.begin(), Vector2D(robots[id].x, robots[id].y));
    std::set<Vector2D> *pathPoints = new std::set<Vector2D>();
    for (auto from = path.begin(); from != path.end(); from++) {
        auto next = from + 1;
        if (next == path.end()) break;
        auto vec = *next - *from; // 方向向量
        auto aStep = vec / vec.length() * 0.25; // 一次移动0.25 的距离
        for (auto nextVec = *from; (nextVec - *next).length() > 0.25; nextVec = nextVec + aStep) {
            double nowx = int(nextVec.x / 0.5) * 0.5 + 0.25;
            double nowy = int(nextVec.y / 0.5) * 0.5 + 0.25;
            for (auto & add : adds) {
                pathPoints->insert(Vector2D(nowx + add.first, nowy + add.second));
            }
        }
    }
    double nowx = int(path.rbegin()->x / 0.5) * 0.5 + 0.25;
    double nowy = int(path.rbegin()->y / 0.5) * 0.5 + 0.25;
    for (auto & add : adds) {
        pathPoints->insert(Vector2D(nowx + add.first, nowy + add.second));
    }
    return pathPoints;
}

void DetecteCollision(int robot1, int robot2){
    if (robots[robot1].worktableTogo == -1 || robots[robot2].worktableTogo == -1) return;
    if ((Vector2D(robots[robot1].x, robots[robot1].y) - Vector2D(robots[robot2].x, robots[robot2].y)).length() > 5) return;
    // 给 robot1/robot2 的路径打标签
    std::set<Vector2D> *robot1PathPoints = getPathLabel(robots[robot1].pathPoints, robot1);
    std::set<Vector2D> *robot2PathPoints = getPathLabel(robots[robot2].pathPoints, robot2);
    bool isCollision = false;
    // 检测 robot1 的路径上是否有 robot2 的位置
    for (auto & item : *robot2PathPoints) {
        if (robot1PathPoints->find(item) != robot1PathPoints->end()) {
            // 发生碰撞
            isCollision = true;
            break;
        }
    }
    if (isCollision == false) return;
    TESTOUTPUT(fout << "robot" << robot1 << " and robot" << robot2 << " 检测碰撞" << std::endl;)
    // 在robot1的路径上删除 robot2 开始的位置
    {
        double nowx = int(robots[robot2].x / 0.5) * 0.5 + 0.25;
        double nowy = int(robots[robot2].y / 0.5) * 0.5 + 0.25;
        std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
        for (auto & add : adds) {
            if (robot1PathPoints->find(Vector2D(nowx + add.first, nowy + add.second)) != robot1PathPoints->end()) {
                robot1PathPoints->erase(Vector2D(nowx + add.first, nowy + add.second));
            }
        }
    }
    // 在robot2的路径上删除 robot1 开始的位置
    {
        double nowx = int(robots[robot1].x / 0.5) * 0.5 + 0.25;
        double nowy = int(robots[robot1].y / 0.5) * 0.5 + 0.25;
        std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
        for (auto & add : adds) {
            if (robot2PathPoints->find(Vector2D(nowx + add.first, nowy + add.second)) != robot2PathPoints->end()) {
                robot2PathPoints->erase(Vector2D(nowx + add.first, nowy + add.second));
            }
        }
    }
    std::set<Vector2D> *robot1Points = new std::set<Vector2D>();
    std::set<Vector2D> *robot2Points = new std::set<Vector2D>();
    // 记录 robot1 现在的位置
    {
        double nowx = int(robots[robot1].x / 0.5) * 0.5 + 0.25;
        double nowy = int(robots[robot1].y / 0.5) * 0.5 + 0.25;
        std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
        for (auto & add : adds) {
            robot1Points->insert(Vector2D(nowx + add.first, nowy + add.second));
        }
    }
    // 记录 robot2 现在的位置
    {
        double nowx = int(robots[robot2].x / 0.5) * 0.5 + 0.25;
        double nowy = int(robots[robot2].y / 0.5) * 0.5 + 0.25;
        std::vector<std::pair<double, double>> adds = {{0, 0.5}, {0.5, 0}, {0, -0.5}, {-0.5, 0}, {0.5, 0.5}, {-0.5, 0.5}, {0.5, -0.5}, {-0.5, -0.5}, {0, 0}};
        for (auto & add : adds) {
            robot2Points->insert(Vector2D(nowx + add.first, nowy + add.second));
        }
    }
    // 重新规划路径
    robots[robot2].pathPoints = robots[robot2].movePath(robot1PathPoints); 
    if (robots[robot2].pathPoints[0] == Vector2D(0,0)) {
        TESTOUTPUT(fout << "robot" << robot2 << " 无法规划路径" << std::endl;)
        robots[robot2].findNullPath(robot1Points, robot1PathPoints);
    } else {
        TESTOUTPUT(fout << "robot" << robot2 << " 已重新规划路径" << std::endl;)
    }
}


void Robot::checkDead(){
    if (isWait == false && pathPoints.size() == 0) return;
    if (pathPoints[0] == Vector2D(0,0)) return;
    if (std::abs(Vector2D(linearSpeedX,linearSpeedY).length()) < 0.0001) {
        zeroTime++;
    } else {
        zeroTime = 0;
    }
    for (int i = 0; i <= robotNum; i++) {
        if (i == id) continue;
        // 碰撞
        if (Vector2D(x - robots[i].x, y - robots[i].y).length() < 0.53 * 2 + 0.3) {
            return;
        }
    }
    if (zeroTime > 30) {
        TESTOUTPUT(fout << "robotDead " << id << " " << nowTime << std::endl;)
        pathPoints = movePath();
        isWait = false;
    }
}

#endif