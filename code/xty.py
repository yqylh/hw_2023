import os
import datetime
import subprocess
import random
from tqdm import trange

def runMap(mapName, randomSeed):
    com = f'../judge-1/Robot -s {randomSeed} -m ../judge-1/maps/{mapName}.txt -r ../judge-1/replay/{mapName}-{datetime.datetime.now().strftime("%Y-%m-%d.%H.%M.%S")}.rep -f ./main'
    # print(com)
    output = subprocess.run(com, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # print(output)
    # input()
    # statu = output.stdout.decode('utf-8').split('"status":')[1].split(',')[0]
    score = output.stdout.decode('utf-8').split('"score":')[1].split('}')[0]
    return score

os.system('rm /q "../judge-1/replay/*"')
if os.system('g++ main.cpp -o main -std=c++17 -DEBUG -O3 -pthread') != 0:
    print("compiling failed")
    exit(0)
    '''
    for mapNum in range(1, 5):
        print("running on:", mapNum)
        replay_file_name = f"../replay/map{mapNum}-{datetime.datetime.now().strftime('%Y-%m-%d.%H.%M.%S')}.rep"
        # print("replay file name:", replay_file_name)
        # print("running:", f'..\\Robot.exe -m ..\\maps\\{mapNum}.txt -r {replay_file_name} -f .\\main.exe')
        com = f'..\\Robot.exe -m ..\\maps\\{mapNum}.txt -r {replay_file_name} -f .\\main.exe'
        output = subprocess.run(com, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        #sample = 'CompletedProcess(args='..\\Robot.exe -m ..\\maps\\1.txt -r ../replay/map1-2023-04-09.13.06.08.rep -f .\\main.exe', returncode=0, stdout=b'{"status":"Successful","score":793469}\r\n', stderr=b'[INFO]D:\\Project\\CodeCraft2023\\Robot\\Game.cpp:53|Game::Init|frame[1]|Loaded workbench : 37\r\n[INFO]D:\\Project\\CodeCraft2023\\Robot\\Game.cpp:528|Game::GameOver|frame[15001]|player skipped frames:15\r\n')'
        score = output.stdout.decode('utf-8').split('"score":')[1].split('}')[0]
        print(int(score))
    if os.path.isfile("main.exe"):
        os.remove("main.exe")
    if os.path.isdir("replay"):
        os.system('rd /s /q "replay"')
    '''
print("compiling successfully")

def writeConfg(cfgList):
    with open("tc.txt", "w") as f:
        for cfg in cfgList:
            f.write(str(cfg) + "\n")

def run(stepNumber=16, randomNumber=8):
    mapList = ["1", "2"]
    beginValue = 0.2
    endValue = 2.0
    interval = (endValue - beginValue) / stepNumber
    nowValue = beginValue
    for step in range(stepNumber):
        mapScores = [[] for i in range(len(mapList))]
        cfgList = [nowValue]
        nowValue += interval
        writeConfg(cfgList)
        for i, mapName in enumerate(mapList):
            mapScore = []
            for j in trange(randomNumber):
                randomSeed = random.randint(0, 65535)
                score = runMap(mapName, randomSeed)
                mapScore.append(int(score))
            print("nowMap:", mapName, "nowValue:", cfgList[0])
            print("\tmapScoresAcg:", sum(mapScore) / len(mapScore))
            print("\tmapScoresMin:", min(mapScore))
            print("\tmapScoresMax:", max(mapScore))

run()