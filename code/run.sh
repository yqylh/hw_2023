rm ../judge-1/replay/*
if g++ main.cpp -o main -std=c++17 -O3 -pthread; then
    for mapNum in {1..6} 
    do
        ../judge-1/Robot -m ../judge-1/maps/$mapNum.txt -r ../judge-1/replay/map$mapNum%Y-%m-%d.%H.%M.%S.rep -f "./main"
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
