#include <iostream>
#include "config.hpp"
#include "robot.hpp"
#include "worktable.hpp"
#include "input.hpp"

int main() {
    inputMap();
    while (inputFrame()) {
        solveFrame();
    }
    TESTOUTPUT(quit();)
    return 0;
}
