#ifndef __WORKTABLE_H__
#define __WORKTABLE_H__
#include "config.hpp"

struct Worktable{
    int id; // 工作台的 id
    double x; // 工作台的 x 坐标
    double y; // 工作台的 y 坐标
    int remainTime; // 工作台的剩余时间, -1 表示没有
    int type; // 工作台的类型
    bool output; // 工作台的输出状态
    int inputId[MAX_Item_Type_Num + 1]; // 工作台的输入物品的 id, 0 表示没有
    int anyOneChooseBuy; // 是否有人选择了这个工作台
    int anyOneChooseSell[MAX_Item_Type_Num + 1]; // 是否有人选择了这个工作台卖出

    Worktable() {
        this->id = -1;
        this->x = -1;
        this->y = -1;
        this->remainTime = -1;
        this->type = -1;
        this->output = false;
        for (int i = 0; i <= MAX_Item_Type_Num; i++) {
            this->inputId[i] = 0;
            this->anyOneChooseSell[i] = 0;
        }
        this->anyOneChooseBuy = -1;
    }
    Worktable(int id, double x, double y, int type) {
        this->id = id;
        this->x = x;
        this->y = y;
        this->remainTime = -1;
        this->type = type;
        this->output = false;
        for (int i = 0; i <= MAX_Item_Type_Num; i++) {
            this->inputId[i] = 0;
            this->anyOneChooseSell[i] = 0;
        }
        this->anyOneChooseBuy = -1;
    }
    void outputTest() {
        TESTOUTPUT(fout << "Worktable id: " << id << std::endl;)
        TESTOUTPUT(fout << "x: " << x << std::endl;)
        TESTOUTPUT(fout << "y: " << y << std::endl;)
        TESTOUTPUT(fout << "remainTime: " << remainTime << std::endl;)
        TESTOUTPUT(fout << "type: " << type << std::endl;)
        TESTOUTPUT(fout << "output: " << output << std::endl;)
        TESTOUTPUT(fout << "inputId: ";)
        for (int i = 1; i <= MAX_Item_Type_Num; i++) {
            TESTOUTPUT(fout << "item" << i << inputId[i] << " ";)
        }
        TESTOUTPUT(fout << std::endl;)
    }
};
Worktable worktables[MAX_Worktable_Num];

#endif