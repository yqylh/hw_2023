g++ main.cpp -o main -std=c++17 -O3 -pthread -DEBUG -DDEBUG_RADAR;
g++ main.cpp -o mainHack -std=c++17 -O3 -pthread -DHACK -DEBUG;

# echo "\t [蓝方=我, 红方=hack]"
# ../judge-1/Robot -m ../judge-1/maps/6.txt -r ../judge-1/replay/map6%Y-%m-%d.%H.%M.%S.rep -f ./main ./mainHack
# echo "\t [蓝方=hack, 红方=我]"
../judge-1/Robot -m ../judge-1/maps/5.txt -r ../judge-1/replay/map5%Y-%m-%d.%H.%M.%S.rep -f ./mainHack ./main

if [ -f "main" ]; then
    rm main
fi
if [ -f "mainHack" ]; then
    rm mainHack
fi
if [ -d "main.dSYM" ]; then
    rm -r ./main.dSYM
fi
if [ -d "mainHack.dSYM" ]; then
    rm -r ./mainHack.dSYM
fi
if [ -d "replay" ]; then
    rm -r ./replay
fi
