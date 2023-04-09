#include <iostream>
#include <fstream>
#include "config.hpp"
#include "robot.hpp"
#include "worktable.hpp"
#include "input.hpp"

int main() {
    // TESTOUTPUT(
    //     std::ifstream input("tc.txt");
    //     input >> TC;
    //     input.close();
    // )
    inputMap();
    while (inputFrame()) {
        solveFrame();
    }
    TESTOUTPUT(quit();)
    return 0;
}
