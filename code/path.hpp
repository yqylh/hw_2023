#ifndef _PATH_H_
#define _PATH_H_

struct Path{
    int buyWorktableId;
    int sellWorktableId;
    int robotId;
    int earnedMoney;
    int time;
    double Parameters;
    Path(int buyWorktableId, int sellWorktableId, int robotId, int earnedMoney, int time, double Parameters) {
        this->buyWorktableId = buyWorktableId;
        this->sellWorktableId = sellWorktableId;
        this->robotId = robotId;
        this->earnedMoney = earnedMoney;
        this->time = time;
        this->Parameters = Parameters;
    }
    inline bool operator < (const Path &b) const {
        return Parameters < b.Parameters;
    }
};

#endif