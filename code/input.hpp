#ifndef __INPUT_H__
#define __INPUT_H__
#include "config.hpp"
#include "robot.hpp"
#include "worktable.hpp"
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

void solveGraph() {
    std::vector<std::pair<int, double> > near7;
    for (auto & i : worktables) {
        if (i.type == 7) {
            double dis = 1e8;
            for (auto & j : worktables) {
                if (j.type == 8 || j.type == 9) {
                    dis = std::min(dis, WTtoWTwithItem[i.id][j.id]);
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
                        dis = std::min(dis, WTtoWTwithItem[i.id][j.id]);
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
            // if (index <= near7.size() * 0.5) {
                worktables[id7].near7 = (cos(num7.second / near7.rbegin()->second * M_PI / 2) + 1.5) * 2;
                // worktables[id7].near7 = (cos(num7.second / near7.rbegin()->second * M_PI) + 2) * 2;
                // TESTOUTPUT(fout << "id " << id7 << " " << worktables[id7].near7 << std::endl;)
                if (worktables[id7].near7 < 1) worktables[id7].near7 = 1;
                // worktables[id7].near7 = 2;
            // } else {
            //    break;
            // }
            std::vector<std::pair<int, double> > near1, near2;
            for (auto & i : worktables) {
                if (i.type == 1 && i.near7 == 1) {
                    double dis = WTtoWTwithItem[i.id][id7];
                    near1.push_back(std::make_pair(i.id, dis));
                }
                if (i.type == 2 && i.near7 == 1) {
                    double dis = WTtoWTwithItem[i.id][id7];
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
        // for (int id = 0; id < 8; id++) worktables[id].near7 = 0.01;
        // for (int id = worktableNum - 15 + 1; id <= worktableNum; id++) worktables[id].near7 = 0.01;
        return;
    }
    // 有7号工作台  
    std::sort(near7.begin(), near7.end(), [](const std::pair<int, double> & a, const std::pair<int, double> & b) {
        return a.second < b.second;
    }); // 按照距离排序
    // int index = 0;
    for (auto & num7 : near7) {
        int id7 = num7.first;
        // if (index < near7.size() * 0.5) {
        //     worktables[id7].near7 = 4;
        // } else {
        //     break;
        // }
        worktables[id7].near7 = 4;
        std::vector<std::pair<int, double> > near4, near5, near6;
        for (auto & i : worktables) {
            if (i.type == 4 && i.near7 == 1) {
                double dis = WTtoWTwithItem[i.id][id7];
                near4.push_back(std::make_pair(i.id, dis));
            }
            if (i.type == 5 && i.near7 == 1) {
                double dis = WTtoWTwithItem[i.id][id7];
                near5.push_back(std::make_pair(i.id, dis));
            }
            if (i.type == 6 && i.near7 == 1) {
                double dis = WTtoWTwithItem[i.id][id7];
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
        if (near4.size() >= 1 ) {
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

double bfs(Vector2D from, Vector2D to, int bring) {
        std::vector<Vector2D> path;
        std::map<Vector2D, Vector2D> fromWhere;
        std::queue<Vector2D> q;
        q.push(from);
        fromWhere.insert(std::make_pair(from, from));
        bool find = false;
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
                if (bring == 0) {
                    // 不携带物品
                    // 可以碰两个角
                    int num = 0;
                    std::vector<Vector2D> obstacles;
                    if (!(next == to))for (auto & item : grids[next]->obstacles) {
                        if ((next - item).length() < 0.45) {
                            num++;
                            obstacles.push_back(item);
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
                    // 被两边的卡死
                    if (num == 2 && (obstacles[0]-obstacles[1]).length() > 0.51) continue;
                    // 在墙角斜着走
                    if (num == 2 && nowNum == 2 && add.first != 0 && add.second != 0) continue;
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
        while ( 1 ) {
            path.push_back(to);
            if (to == fromWhere[to]) break;
            to = fromWhere[to];
        }
        if (find == false) return 1e8;
        else 
        return path.size();
}

void solveWorktableToWorktable() {
    for (int i = 0; i <= worktableNum; i++) {
        for (int j = 0; j <= worktableNum; j++) {
            WTtoWT[i][j] = -1;
            WTtoWTwithItem[i][j] = -1;
        }
    }
    for (int i = 0; i <= worktableNum; i++) {
        if (worktables[i].near7 == 0) continue;
        for (int j = 0; j <= worktableNum; j++) {
            if (worktables[j].near7 == 0) continue;
            if (i == j) {
                WTtoWT[i][j] = 0;
                WTtoWTwithItem[i][j] = 0;
                WTtoWT[j][i] = 0;
                WTtoWTwithItem[j][i] = 0;
                continue;
            }
            if (WTtoWT[i][j] != -1) continue;
            double distance = bfs(Vector2D(worktables[i].x, worktables[i].y), Vector2D(worktables[j].x, worktables[j].y), 0);
            WTtoWT[i][j] = distance;
            WTtoWT[j][i] = distance;
            distance = bfs(Vector2D(worktables[i].x, worktables[i].y), Vector2D(worktables[j].x, worktables[j].y), 1);
            WTtoWTwithItem[i][j] = distance;
            WTtoWTwithItem[j][i] = distance;
        }
    }
    TESTOUTPUT(fout << "worktable to worktable 不带物品" << std::endl);
    for (int i = 0; i <= worktableNum; i++) {
        TESTOUTPUT(fout << i << "-->");
        for (int j = 0; j <= worktableNum; j++) {
            if (WTtoWT[i][j] == -1) WTtoWT[i][j] = 1e8;
            TESTOUTPUT(fout << WTtoWT[i][j] << " ");
        }
        TESTOUTPUT(fout << std::endl);
    }
    TESTOUTPUT(fout << "worktable to worktable 带物品" << std::endl);
    for (int i = 0; i <= worktableNum; i++) {
        TESTOUTPUT(fout << i << "-->");
        for (int j = 0; j <= worktableNum; j++) {
            if (WTtoWTwithItem[i][j] == -1) WTtoWTwithItem[i][j] = 1e8;
            TESTOUTPUT(fout << WTtoWTwithItem[i][j] << " ");
        }
        TESTOUTPUT(fout << std::endl);
    }
}
void solveRobotToWorktable(){
    for (int i = 0; i <= robotNum; i++) {
        for (int j = 0; j <= worktableNum; j++) {
            RobotToWT[i][j] = -1;
        }
    }
    for (int i = 0; i <= robotNum; i++) {
        for (int j = 0; j <= worktableNum; j++) {
            if (worktables[j].near7 == 0) continue;
            double distance = bfs(Vector2D(robots[i].x, robots[i].y), Vector2D(worktables[j].x, worktables[j].y), 0);
            if (distance < 1e7) {
                robots[i].couldReach.push_back(j);
            }
            RobotToWT[i][j] = distance;
        }
    }
    TESTOUTPUT(fout << "robot to worktable" << std::endl);
    for (int i = 0; i <= robotNum; i++) {
        TESTOUTPUT(fout << i << "-->");
        for (int j = 0; j <= worktableNum; j++) {
            TESTOUTPUT(fout << RobotToWT[i][j] << " ");
        }
        TESTOUTPUT(fout << std::endl);
    }
}
void solveMapNum() {
    int numWT[MAX_Worktable_Type_Num] = {0};
    int numWTFoe[MAX_Worktable_Type_Num] = {0};
    for (int i = 0; i <= worktableNum; i++) {
        numWT[worktables[i].type]++;
    }
    for (int i = 0; i <= worktableNumFoe; i++) {
        numWTFoe[worktablesFoe[i].type]++;
    }
    if (worktableNumFoe == 13) {
        // 地图 1
        if (numWT[4] == 2 && numWT[5] == 2 && numWT[6] == 2) {
            if (RoB == RED) {
                gankType = 1;
            } else {
                gankType = 2;
            }
        }
        // 地图 2
        if (numWT[6] == 1) {
            if (RoB == RED) {
                gankType = 3;
            } else {
                gankType = 4;
            }
        }
    }

}
void inputMap(){
    std::string line;
    getline(std::cin, line);
    if (line == std::string("BLUE")) {
        RoB = BLUE;
        MAX_SPEED = 6;
        TOL_Collision = 4;
    } else {
        RoB = RED;
        MAX_SPEED = 7;
        TOL_Collision = 6;
    }
    for (int i = 0; i < MAP_Line_Length; i++) {
        std::string line;
        getline(std::cin, line);
        for (int j = 0; j < MAP_Col_Length; j++) {
            double x = 0.5*j+0.25;
            double y = 50 - 0.25 - 0.5*i;
            if (line[j] == '.') {
                grids[Vector2D(x,y)] = new Grid(Vector2D(x,y), 0);
                continue;
            }
            if (line[j] == '#') {
                grids[Vector2D(x,y)] = new Grid(Vector2D(x,y), 1);
                // 墙壁
                continue;
            }
            grids[Vector2D(x,y)] = new Grid(Vector2D(x,y), 0);
            if (RoB == BLUE) {
                // 蓝方
                if (line[j] == 'A') {
                    // A 表示自己的机器人 蓝色
                    robotNum++;
                    robots[robotNum] = Robot(robotNum, x,y);
                } else if (line[j] == 'B') {
                    // B 表示对方的机器人 红色
                    // todo
                    robotNumFoe++;
                    robotsFoe[robotNumFoe] = Robot(robotNumFoe, x,y);
                } else if (line[j] >= 'a' && line[j] <= 'i') {
                    // 对方工作台 红色
                    worktableNumFoe++;
                    worktablesFoe[worktableNumFoe] = Worktable(worktableNumFoe, x,y, char(line[j]) - 'a' + 1);
                } else if (line[j] >= '0' && line[j] <= '9') {
                    // 自己的工作台 蓝色
                    worktableNum++;
                    worktables[worktableNum] = Worktable(worktableNum, x,y, char(line[j]) - '0');
                } else throw;
            } else {
                // 红方
                if (line[j] == 'A') {
                    // A 表示对方的机器人 蓝色
                    // todo
                    robotNumFoe++;
                    robotsFoe[robotNumFoe] = Robot(robotNumFoe, x,y);
                } else if (line[j] == 'B') {
                    // B 表示自己的机器人 红色
                    robotNum++;
                    robots[robotNum] = Robot(robotNum, x,y);
                } else if (line[j] >= 'a' && line[j] <= 'i') {
                    // 自己的工作台 蓝色
                    worktableNum++;
                    worktables[worktableNum] = Worktable(worktableNum, x,y, char(line[j]) - 'a' + 1);
                } else if (line[j] >= '0' && line[j] <= '9') {
                    // 对方工作台 红色
                    worktableNumFoe++;
                    worktablesFoe[worktableNumFoe] = Worktable(worktableNumFoe, x,y, char(line[j]) - '0');
                } else throw;
            }
        }
    }
    detectionObstacle();
    while(getline(std::cin, line) && line != "OK");
    solveWorktableToWorktable();
    solveGraph();
    solveRobotToWorktable();
    solveMapNum();
    for (auto & robot : robotsFoe) {
        TESTOUTPUT(fout << "robot " << robot.id << " (" << robot.x << "," << robot.y << ") " << std::endl);
    }
    for (auto & wt : worktablesFoe) {
        TESTOUTPUT(fout << "worktable " << wt.id << " (" << wt.x << "," << wt.y << ") " << " type=" << wt.type << std::endl);
    }
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
    for (int i = 0; i <= robotNum; i++) {
        for (int j = 0; j < 360; j++) {
            scanf("%lf", &robots[i].lasers[j]);
        }
    }
    std::string line;
    while(getline(std::cin, line) && line != "OK");
    return true;
}
std::vector<std::pair<int, int>> getRobotPriority() {
    std::vector<std::pair<int, int>> robotPriority;
    for (int i = 0; i <= robotNum; i++) {
        auto gotoTable = robots[i].worktableTogo;
        if (gotoTable <= 0) {
            robotPriority.push_back(std::make_pair(i, 0));
            continue;
        }
        if (robots[i].path == nullptr) {
            robotPriority.push_back(std::make_pair(i, 0));
            continue;
        }
        auto type = worktables[gotoTable].type;
        if (robots[i].bringId == 0) {
            if (type == 7) robotPriority.push_back(std::make_pair(i, 3));
            if (type == 4 || type == 5 || type == 6) {
                if (worktables[robots[i].path->sellWorktableId].type == 7 && worktables[robots[i].path->sellWorktableId].waitPriority == 5) {
                    robotPriority.push_back(std::make_pair(i, 1));
                } else robotPriority.push_back(std::make_pair(i, 5));
            }
            if (type == 1 || type == 2 || type == 3) {
                if (worktables[robots[i].path->sellWorktableId].waitPriority == 4) {
                    robotPriority.push_back(std::make_pair(i, 3));
                } else robotPriority.push_back(std::make_pair(i, 6));
            }
        } else {
            if (robots[i].bringId == 7) robotPriority.push_back(std::make_pair(i, 1));
            if (robots[i].bringId == 4 || robots[i].bringId == 5 || robots[i].bringId == 6) {
                if (worktables[robots[i].path->sellWorktableId].type == 7 && worktables[robots[i].path->sellWorktableId].waitPriority == 5) {
                    robotPriority.push_back(std::make_pair(i, 1));
                } else robotPriority.push_back(std::make_pair(i, 2));  
            } 
            if (robots[i].bringId == 1 || robots[i].bringId == 2 || robots[i].bringId == 3) {
                if (worktables[robots[i].path->sellWorktableId].waitPriority == 4) {
                    robotPriority.push_back(std::make_pair(i, 2));
                } else robotPriority.push_back(std::make_pair(i, 4));
            }
        }
        // if (robots[i].bringId == 0) {
        //     if (type == 7) {
        //         robotPriority.push_back(std::make_pair(i, INT_MAX));
        //     } else {
        //         robotPriority.push_back(std::make_pair(i, nowTime - robots[i].runTime));
        //         if (type == 4 || type == 5 || type == 6) {
        //             robotPriority.back().second *= 1.2;
        //         }
        //     }
        // } else {
        //     if (robots[i].bringId == 7) {
        //         robotPriority.push_back(std::make_pair(i, INT_MAX));
        //     } else {
        //         robotPriority.push_back(std::make_pair(i, nowTime - robots[i].runTime));
        //         if (robots[i].bringId == 4 || robots[i].bringId == 5 || robots[i].bringId == 6) {
        //             robotPriority.back().second *= 1.4;
        //         }
        //     }
        // }
    }
    std::sort(robotPriority.begin(), robotPriority.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        if (a.second == b.second) {
            return a.first < b.first;
        } else {
            return a.second < b.second;
        }
    });
    return robotPriority;
}
void solveFrame() {
    printf("%d\n", nowTime);
    TESTOUTPUT(fout << nowTime << std::endl;)
    
    std::fill(canBuy, canBuy + MAX_Item_Type_Num + 1, 0);
    for (int i = 0; i <= worktableNum; i++) {
        worktables[i].checkWait();
        worktables[i].checkCanBuy();
        // TESTOUTPUT(worktables[i].outputTest();)
    }
    for (int i = 0; i <= robotNum; i++) {
        robots[i].checkCanBuy();
    }
    for (int i = 0; i <= robotNum; i++) {
        if (gankType == 1 || gankType == 2) {
            if (i == 2) {
                if (robots[i].bringId == 0) robots[i].buyOne(5);
                else robots[i].moveToFoeWT(8);
                robots[i].isGanking = true;
                continue;
            }
            if (i == 3) {
                if (robots[i].bringId == 0) robots[i].buyOne(5);
                else robots[i].moveToFoeWT(12);
                robots[i].isGanking = true;
                continue;
            }
        }
        if (gankType == 3) {
            if (i == 3) {
                robots[i].moveToPoint(Vector2D(9.75, 25.75));
                robots[i].isGanking = true;
                continue;
            }
        }
        if (gankType == 4) {
            if (i == 2) {
                robots[i].moveToPoint(Vector2D(37.25, 10.75));
                robots[i].isGanking = true;
                continue;
            }
            if (i == 3) {
                robots[i].moveToPoint(Vector2D(38.25, 8.75));
                robots[i].isGanking = true;
                continue;
            }
        }
        robots[i].action();
        // TESTOUTPUT(robots[i].outputTest();) 
    }
    auto robotPriority = getRobotPriority();
    for (int robot2 = 1; robot2 <= robotNum; robot2++) {
        std::set<Vector2D> *robot1PathPoints = new std::set<Vector2D>();
        std::set<Vector2D> *robot1Points = new std::set<Vector2D>();        
        for (int robot1 = 0; robot1 < robot2; robot1++) {
            if (robots[robotPriority[robot1].first].isGanking) continue;
            if (robots[robotPriority[robot2].first].isGanking) continue;
            DetecteCollision(robotPriority[robot1].first, robotPriority[robot2].first, robot1PathPoints, robot1Points);
            // DetecteCollision(robot1, robot2, robot1PathPoints, robot1Points);
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