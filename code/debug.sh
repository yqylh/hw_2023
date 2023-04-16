rm ../judge-1/replay/*
if g++ main.cpp -o main -std=c++17 -O3 -pthread -DEBUG -DCREATE; then
    # 如果$2为1
    if [ $2 -eq 0 ]; then
        echo "map$mapNum :"
        echo "\t [蓝方=我, 红方=demo]"
        ../judge-1/Robot -s 3333 -m ../judge-1/maps/$1.txt -r ../judge-1/replay/map$1%Y-%m-%d.%H.%M.%S.rep -f ./main ../judge-1/Demo/SimpleDemo
    fi
    if [ $2 -eq 1 ]; then
        echo "map$mapNum :"
        echo "\t [蓝方=demo, 红方=我]"
        ../judge-1/Robot -s 3333 -m ../judge-1/maps/$1.txt -r ../judge-1/replay/map$1%Y-%m-%d.%H.%M.%S.rep -f ../judge-1/Demo/SimpleDemo ./main
    fi
    if [ -f "main" ]; then
        rm main
    fi
    if [ -d "main.dSYM" ]; then
        rm -r ./main.dSYM
    fi
    if [ -d "replay" ]; then
        rm -r ./replay
    fi
    mv ../genMap/path.txt ../genMap/path$1.txt
else 
    echo "some error occured"
fi
