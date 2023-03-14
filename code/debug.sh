rm ../judge-1/replay/*
if g++ main.cpp -o main -std=c++17 -O3 -pthread -DEBUG; then
    ../judge-1/Robot -m ../judge-1/maps/1.txt -r ../judge-1/replay/map1%Y-%m-%d.%H.%M.%S.rep -f "./main"
    # ../judge-1/Robot -m ../judge-1/maps/2.txt -r ../judge-1/replay/map2%Y-%m-%d.%H.%M.%S.rep -f "./main"
    # ../judge-1/Robot -m ../judge-1/maps/3.txt -r ../judge-1/replay/map3%Y-%m-%d.%H.%M.%S.rep -f "./main"
    # ../judge-1/Robot -m ../judge-1/maps/4.txt -r ../judge-1/replay/map4%Y-%m-%d.%H.%M.%S.rep -f "./main"
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
