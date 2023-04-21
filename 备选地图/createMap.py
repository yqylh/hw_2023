import argparse
import random

class Point:
    def __init__(self,p) -> None:
        self.type = "#"
        self.p = p
        self.RoundWallCount = 0
    def create_wall(self):
        tmp_p = random.random()
        if tmp_p<self.p:
            self.type = "#"
        else:
            self.type = "."
    # def create_worker(self):

def check(num_1,num_a,num_2,num_b):
    for i in range(9):
        if num_1[i]<num_2[i]:
            return True
        if num_a[i]<num_b[i]:
            return True
    return False

def tran(q):
    if q == 0: return 'a'
    if q == 1: return 'b'
    if q == 2: return 'c'
    if q == 3: return 'd'
    if q == 4: return 'e'
    if q == 5: return 'f'
    if q == 6: return 'g'
    if q == 7: return 'h'
    if q == 8: return 'i'


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    
    parser.add_argument('--p', type=float, default=0.5, help='')
    parser.add_argument('--_1', type=int, default=4, help='')
    parser.add_argument('--_2', type=int, default=3, help='')
    parser.add_argument('--_3',type=int,default=3,help="")
    parser.add_argument('--_4', type=int, default=2, help='')
    parser.add_argument('--_5', type=int, default=2, help='')
    parser.add_argument('--_6',type=int,default=2,help="")
    parser.add_argument('--_7', type=int, default=2, help='')
    parser.add_argument('--_8', type=int, default=1, help='')
    parser.add_argument('--_9',type=int,default=2,help="")

    parser.add_argument('--a', type=int, default=4, help='')
    parser.add_argument('--b', type=int, default=3, help='')
    parser.add_argument('--c',type=int,default=3,help="")
    parser.add_argument('--d', type=int, default=2, help='')
    parser.add_argument('--e', type=int, default=2, help='')
    parser.add_argument('--f',type=int,default=2,help="")
    parser.add_argument('--g', type=int, default=2, help='')
    parser.add_argument('--h', type=int, default=1, help='')
    parser.add_argument('--i',type=int,default=2,help="")
    args = parser.parse_args()
    num_1 = [0 for i in range(100)]
    num_a = [0 for i in range(100)]
    num_2 = []
    num_b = []
    num_2.append(args._1)
    num_2.append(args._2)
    num_2.append(args._3)
    num_2.append(args._4)
    num_2.append(args._5)
    num_2.append(args._6)
    num_2.append(args._7)
    num_2.append(args._8)
    num_2.append(args._9)

    num_b.append(args.a)
    num_b.append(args.b)
    num_b.append(args.c)
    num_b.append(args.d)
    num_b.append(args.e)
    num_b.append(args.f)
    num_b.append(args.g)
    num_b.append(args.h)
    num_b.append(args.i)


    Map = [[Point(args.p) for i in range(100)] for j in range(100)]
    for i in range(100):
        for j in range(100):
            Map[i][j].create_wall()
    
    # 求格子周围
    for i in range(100):
        for j in range(100):
            Map[i][j].RoundWallCount = 0
            for q in range(i-2,i+3):
                for w in range(j-2,j+3):
                    if i-2>=0 and j-2>=0 and i+3<=99 and j+3<=99:
                        if Map[q][w].type == '#':
                            Map[i][j].RoundWallCount += 1
    # 如果当前格子是墙，那么当他周围的墙<5个或者大于7个时，将其转变为路面
    # 如果当前格子是路，那么当他周围的墙>=4个或者小于2个时，把他变成墙
    # 修改地形
    for i in range(100):
        for j in range(100):
            if Map[i][j].type == '#':
                if Map[i][j].RoundWallCount < 5 or Map[i][j].RoundWallCount > 9:
                    Map[i][j].type= '.'
            # else:
            #     if Map[i][j].RoundWallCount >= 4 or Map[i][j].RoundWallCount < 2:
            #         Map[i][j].type= '#'

    # 生成工作台
    while check(num_1,num_a,num_2,num_b):
        for i in range(100):
            for j in range(100):
                if Map[i][j].type == '.':
                    for q in range(9):
                        if random.random()<0.0001 and num_1[q]<num_2[q]:
                            Map[i][j].type = str(q)
                            num_1[q] += 1
                            break
                        if random.random()<0.00005 and num_a[q]<num_b[q]:
                            Map[i][j].type = tran(q)
                            num_a[q] += 1
                            break
                    

    # 生成机器人
    num_robot_A = 0
    num_robot_B = 0
    while num_robot_A <= 4 or num_robot_B<=4:
        for i in range(100):
            for j in range(100):
                if Map[i][j].type == '.':
                    if random.random()<0.001 and num_robot_A<=4:
                        Map[i][j].type = 'A'
                        num_robot_A += 1
                    if random.random()<0.001 and num_robot_B<=4:
                        Map[i][j].type = 'B'
                        num_robot_B += 1

    open('map.txt','w').close()

    with open('map.txt','a') as map_file:
        for i in range(100):
            for j in range(100):
                print(Map[i][j].type,end="",file=map_file)
                print(Map[i][j].type,end="")
            print()
            print(file=map_file)
    




