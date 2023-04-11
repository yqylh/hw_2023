rm ../judge-1/replay/*
if g++ main.cpp -o main -std=c++17 -O3 -pthread; then
    for mapNum in {1..2} 
    do
        echo "map$mapNum :"
        echo "\t [蓝方=我, 红方=demo]"
        ../judge-1/Robot -m ../judge-1/maps/$mapNum.txt -r ../judge-1/replay/map$mapNum%Y-%m-%d.%H.%M.%S.rep -f ./main ../judge-1/Demo/SimpleDemo
        echo "\t [蓝方=demo, 红方=我]"
        ../judge-1/Robot -m ../judge-1/maps/$mapNum.txt -r ../judge-1/replay/map$mapNum%Y-%m-%d.%H.%M.%S.rep -f ../judge-1/Demo/SimpleDemo ./main
    done
    if [ -f "main" ]; then
        rm main
    fi
    if [ -d "main.dSYM" ]; then
        rm -r ./main.dSYM
    fi
    if [ -d "replay" ]; then
        rm -r ./replay
    fi
else 
    echo "some error occured"
fi
