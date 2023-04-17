import os
import datetime
import subprocess
import random
import re
from multiprocessing import Process, Manager


def runMapBlue(mapName, randomSeed):
    com = f'../judge-1/Robot -s {randomSeed} -m ../judge-1/maps/{mapName}.txt -r ../judge-1/replay/{mapName}-Blue-{randomSeed}.rep -f ./main ../judge-1/Demo/SimpleDemo'
    output = subprocess.run(com, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    match = re.search(r'"score":\[(\d+),(\d+)\]', output.stdout.decode('utf-8'))
    return match.group(1)
def runMapRed(mapName, randomSeed):
    com = f'../judge-1/Robot -s {randomSeed} -m ../judge-1/maps/{mapName}.txt -r ../judge-1/replay/{mapName}-Red-{randomSeed}.rep -f ../judge-1/Demo/SimpleDemo ./main'
    output = subprocess.run(com, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    match = re.search(r'"score":\[(\d+),(\d+)\]', output.stdout.decode('utf-8'))
    return match.group(2)

if os.system('g++ main.cpp -o main -std=c++17 -O3 -pthread') != 0:
    print("compiling failed")
    exit(0)
print("compiling successfully")

def run():
    randomSeed = random.randint(0, 65535)
    mapList = ["1", "2"]
    for i, mapName in enumerate(mapList):
        scoreBlue = runMapBlue(mapName, randomSeed)
        scoreRed = runMapRed(mapName, randomSeed)
        print("map=", mapName, "seed=", randomSeed, "scoreBlue=", scoreBlue, "scoreRed=", scoreRed)
processes = []
for step in range(16):
    print("step = ", step)
    for threadNum in range(32):
        p = Process(target=run)
        processes.append(p)
        p.start()
    for p in processes:
        p.join()
