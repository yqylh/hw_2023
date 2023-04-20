g++ main.cpp -o main -std=c++17 -O3 -pthread -DEBUG -DCREATE -DDEBUG_RADAR;
g++ main.cpp -o mainHack -std=c++17 -O3 -pthread -DHACK;

echo "\t [蓝方=我, 红方=hack]"
../judge-1/Robot -m ../judge-1/maps/1.txt -r ../judge-1/replay/map1%Y-%m-%d.%H.%M.%S.rep -f ./main ./mainHack
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
