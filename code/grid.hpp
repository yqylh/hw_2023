#ifndef __GRID_H__
#define __GRID_H__

#include "vector.hpp"
#include <vector>
#include <map>

struct Grid {
    Vector2D index;
    int type; // 0->空地 1->障碍物 2->工作台
    std::pair<int, int> *visited; // robotId, time
    std::vector<Vector2D> obstacles; // 障碍物的四个坐标
    Grid(){}
    Grid(Vector2D index, int type) : index(index), type(type){
        visited = nullptr;
        obstacles = std::vector<Vector2D>();
    }
};
std::map<Vector2D, Grid *> grids;

void detectionObstacle() {
    // 增加两行两列的边界
    for (double x = 0.25; x < 49.75; x += 0.5) {
        grids[Vector2D(x, -0.25)] = new Grid(Vector2D(x, -0.25), 1);
        grids[Vector2D(x, 50.25)] = new Grid(Vector2D(x, 50.25), 1);
        grids[Vector2D(-0.25, x)] = new Grid(Vector2D(-0.25, x), 1);
        grids[Vector2D(50.25, x)] = new Grid(Vector2D(50.25, x), 1);
    }
    grids[Vector2D(-0.25, -0.25)] = new Grid(Vector2D(-0.25, -0.25), 1);
    grids[Vector2D(-0.25, 50.25)] = new Grid(Vector2D(-0.25, 50.25), 1);
    grids[Vector2D(50.25, -0.25)] = new Grid(Vector2D(50.25, -0.25), 1);
    grids[Vector2D(50.25, 50.25)] = new Grid(Vector2D(50.25, 50.25), 1);
    // 对所有的网格计算 3*3 内的所有障碍物的四个坐标
    for (auto & i : grids) {
        double addx[] = {0, 0.5, -0.5};
        double addy[] = {0, 0.5, -0.5};
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                Vector2D index = i.second->index + Vector2D(addx[j], addy[k]);
                if (grids.find(index) != grids.end()) {
                    if (grids[index]->type == 1) {
                        i.second->obstacles.emplace_back(index.x+0.25, index.y+0.25);
                        i.second->obstacles.emplace_back(index.x+0.25, index.y-0.25);
                        i.second->obstacles.emplace_back(index.x-0.25, index.y+0.25);
                        i.second->obstacles.emplace_back(index.x-0.25, index.y-0.25);
                    }
                }
            }
        }
    }
}

#endif