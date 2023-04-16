if g++ main.cpp -o main -std=c++17 -DEBUG -DCREATE -O3 -pthread; then
    for seed in {30..10000} 
    do
        echo "seed :$seed"
        echo "\t [蓝方=我, 红方=demo]"
        ../judge-1/Robot -s $seed -m ../judge-1/maps/1.txt -r ../judge-1/replay/map1%Y-%m-%d.%H.%M.%S.rep -f ./main ../judge-1/Demo/SimpleDemo
        sleep 10
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
