rm ../judge-1/replay/*
if g++ main.cpp -o main -std=c++17 -O3 -pthread -DEBUG; then
    ../judge-1/Robot -m ../judge-1/maps/$1.txt -r ../judge-1/replay/map$1%Y-%m-%d.%H.%M.%S.rep -f "./main"
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
