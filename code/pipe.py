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

def run(scoresRedMap1, scoresBlueMap1, scoresRedMap2, scoresBlueMap2):
    randomSeed = random.randint(0, 65535)
    mapList = ["1", "2"]
    for i, mapName in enumerate(mapList):
        scoreBlue = runMapBlue(mapName, randomSeed)
        scoreRed = runMapRed(mapName, randomSeed)
        print("map=", mapName, "seed=", randomSeed, "scoreBlue=", scoreBlue, "scoreRed=", scoreRed)
        if i == 0:
            scoresBlueMap1.append(scoreBlue)
            scoresRedMap1.append(scoreRed)
        else:
            scoresBlueMap2.append(scoreBlue)
            scoresRedMap2.append(scoreRed)
random.seed(19260817)
processes = []
for step in range(1):
    print("step = ", step)
    scoresRedMap1 = Manager().list()
    scoresBlueMap1 = Manager().list()
    scoresRedMap2 = Manager().list()
    scoresBlueMap2 = Manager().list()
    for threadNum in range(32):
        p = Process(target=run, args=(scoresRedMap1, scoresBlueMap1, scoresRedMap2, scoresBlueMap2))
        processes.append(p)
        p.start()
    for p in processes:
        p.join()
    scoresBlueMap1 = list(scoresBlueMap1)
    scoresRedMap1 = list(scoresRedMap1)
    scoresBlueMap2 = list(scoresBlueMap2)
    scoresRedMap2 = list(scoresRedMap2)
    print("scoresBlueMap1Avg=", sum(map(int, scoresBlueMap1)) / len(scoresBlueMap1))
    print("scoresRedMap1Avg=", sum(map(int, scoresRedMap1)) / len(scoresRedMap1))
    print("scoresBlueMap2Avg=", sum(map(int, scoresBlueMap2)) / len(scoresBlueMap2))
    print("scoresRedMap2Avg=", sum(map(int, scoresRedMap2)) / len(scoresRedMap2))
