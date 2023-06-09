# 华为软件精英挑战赛2023 年年旅游嘎嘎开心团队 代码开源
华为软件精英挑战赛 2023  年年旅游嘎嘎开心团队 代码

比赛成绩: 江山赛区第一, 总决赛战绩1-2(现场赛刚开始有 bug，名次不好，双败赛制匹配的对手太强了)



## 文件结构
### 备选地图
该文件夹内存放自己构造的几张地图,都是针对一些已有问题的针对性 gank 思路.

createMap.py 是一个自动随机地图生成器,适合随机一些地图然后针对想要攻击地图模块再进行二次串座

### code
存放了所有的代码,脚本,测试 log 文件,详情参加 代码结构一节
### genMap
一个由 python 编写的地图与路径可视化工具

## 主要思路

大部分的思路和小 tracks 都记录在 [NotionLink](https://yqylh.notion.site/2023-14ed9cecfea04a179d557eb3e2da4bcd)  中, 所有commitLog里也写的比较详细，下面只是做一个简单的介绍

### main

```c++
    inputMap();
    while (1) {
        std::thread tInput(inputFrame);
        tInput.join();
        if (inputFlag == false) break;
        std::thread t1(solveFrame);
        t1.join();
    }
    TESTOUTPUT(quit();)
```

首先在inputMap中读入地图信息

输入地图后,每次用一个线程执行 `inputFrame`, 读入帧信息

如果没结束,用一个线程执行 `solveFrame`, 处理帧信息

### inputMap

在读入 100*100 的地图信息后,将障碍物存放到grids 中

detectionObstacle();

处理 100\*100 地图的 3\*3内的所有障碍点坐标

solveWorktableToWorktable();

记录任意两个工作台之间的路径距离, 带物品不带物品各计算一遍

solveGraph();

对于一半的 7 号工作台,提高价值到 4 倍, 7 号工作台最近的 456 工作台,价值提高到 2 倍

solveRobotToWorktable();

计算所有机器人初始位置到达其他工作台的距离,计算 couldReach, 避免找到去不了的

solveMapNum();

根据地图类型设定攻击机器人的数量

### inputFrame

读入所有的输入信息

处理激光雷达`solveFoeRobotPosition`

如果识别到了敌方占领到了一个位置,则设定这个位置最多 30hz 内都被占领了

如果机器人的估算速度是<1,则认定其可能占领工作台,用来屏蔽工作台`solveWTblocked`

如果工作台被占领了,那么就去一次判断

带物品了:换一个去卖/销毁

没带物品:换一个去买卖

### solveFrame

预处理:每个工作台的优先级,现在的资源缺口

对于每个机器人:如果是攻击机器人,执行攻击算法

如果是普通机器人,执行买卖调度

设定避障的优先级

任意两个机器人单循环跑一遍避障检测

执行移动算法

### 攻击算法

攻击时不考虑所有地方机器人占领的位置,会将设置成墙的虚拟墙改回来,最后再改回去

对于所有探测到的敌方机器人的位置(如果是1:3的分区图,只考虑自己探测到的)

设定路径去跟踪他,如果找不到了就跟踪到这条路径的结束前 2m,希望能遇到

如果没有机器人可以跟踪,就在地方的 4567 工作台进行巡逻

### 调度算法

依次执行以下函数

```c++
Sell(); 尝试去卖
if (path == nullptr) FindAPath(); 找一条可以买-卖的路径
Buy(); 尝试去买
checkDead(); 检测是否卡死
```

### 找一个买-卖的路径

遍历所有的可以买的工作台, 查找所有可以卖的工作台, 用收益/距离进行评估收益

一些需要考虑的:

一个工作台最多被一个人预约去买, 除非其正在生产或者阻塞了一个,或者是 123

如果到之前能生产完也算

去卖的工作台只能由一个人去这个格子卖

如果估算超时就不买卖了

之前评估的价值会×到最后的收益系数上

平衡 456 的生产

### 避障检测

按照持有物品的类型判断优先级

按照优先级高->低的顺序去枚举,每个人只避让高优先级的部分

对于两个机器人的5m 内路径进行打标签

如果标签有重复,则认为有碰撞风险, 除非有一方的第一次路径重复的位置>1m,我们认为这种属于追逐情况, 不需要避障

检测到碰撞后,低优先级的去找一条不经过这个路线的新路径,如果找不到,就找一个最近的位置去避让 停住

### 查找移动的路径

BFS->避免碰撞->拉直

BFS我是按照 100\*100 的格子进行的,由于历史原因,没有改成 201\*201,导致后期堆了很多屎山

BFS会去寻找一条可以行动的路径

每次都会枚举一个点 3\*3 内的碰撞点数量（一个格子只统计四个角）

不带物品允许两个障碍点的碰撞，或者两个点构成了一个卡死的情况

带物品原则上不允许 3\*3 内有障碍点，但是如果只有一个障碍点，且满足一些复杂的情况（通过叉积和运动方向进行判断）则也可以，详情可以看 Notion 里一张手画的图

执行完 BFS 后执行避免碰撞墙角DodgingCorners

具体方案就是每个点都是一个格子的中点，如果 3\*3 内有障碍点，那么就让他像障碍物点偏移一段距离

拉直路径就是对于路径上的点，依次判断是否可以和前面合并起来。合并的条件是，计算起点到终点的包围盒内的所有障碍物的点到合并后的线段的长度，如果不会碰撞就可以合并

### 移动算法

移动首先计算需要向哪个方向转向快，转多少度，根据叉积很容易判断

速度：

如果直接冲到终点不会偏移，就速度拉满

如果45°内，根据一个公式计算不会产生绕圈的最大速度

45~90°，速度为 0

90°以上，速度为 -0.65

如果距离终点比较远，或者碰撞系数已经没了，那么速度最小设置为 3.如果快到这个路径的终点（也就是要转弯了）那么速度最小设置为 1

转向：只要转向大于3.6°（1hz 的最大转向速度），角速度就设置为M_PI拉满，不然就设置为相应的比例，保证 1hz 转过去

### 测活机制

如果一个点停住不动 10hz，就认为死掉了，那么就重新规划路径（碰撞检测里去空白区域等待就用的这个策略）

每隔 100hz（`nowTime % 100 == id * 10`）对每个节点重新规划路径

## 代码结构
### Cmakelist
官方提供的 Cmakelist
### code_unused*
随着比赛的进行弃用的代码
### CodeCraft_zip.sh upload.sh
CodeCraft_zip.sh:

修改后的压缩脚本, 压缩命名从默认值改为:

```shell
yesterday=`date +%m-%d--%H:%M:%S`
zip -9 -r $yesterday.zip *.cpp *.cc *.c *.hpp *.h CMakeLists.txt
```
这样比较好做版本管理,最后累积了近二百个提交版本

upload.sh:

删除upload文件夹下的所有文件,将最新的代码拷贝进去, 执行CodeCraft_zip.sh

### codeLog output.txt outputHACK.txt radarOutput.txt
codeLog 用于记录一些参数的测试结果

output.txt : 在-DEBUG 模式下会输出中间结果用于调试

outputHACK.txt : 在决赛时,区分红蓝方两个程序的输出结果

radarOutput.txt : 用于测试激光雷达的信息

### run.sh debug.sh test.sh hack.sh tc.py pipe.py
用于测试的一系列脚本

run.sh:

以`g++ main.cpp -o main -std=c++17 -O3 -pthread`执行编译命令, 然后再红蓝双方分别执行一遍
根据命令行参数分别传入 地图(1234) 蓝红双方(0 1) 随机种子(int)

删除main main.dSYM replay/

debug.sh:

相较于run.sh,增加编译选项`-DEBUG -DCREATE -DDEBUG_RADAR`

同时将 path.txt 改名为对应的地图 id `mv ../genMap/path.txt ../genMap/path$1.txt`

test.sh:

批量化执行不同种子,在本地以 debug 模式运行,方便复现 bug

hack.sh:

编译两个文件,其中一个增加编译选项`-DHACK`, 其 log 文件命名不会冲突

tc.py:

自动调参器,枚举一个参数的取值范围和枚举细粒度,然后执行并记录成绩, 每个参数的取值都会多线程运行多次

pipe.py:

决赛期间主要使用的批量化测试脚本, 每个 step 内 32 个进程执行四张地图的红蓝双方结果,记录对应的成绩



### config.hpp
一些全局 setting 信息, 由于不能读入其他文件, 故写了一个全局 setting 信息
### grid.hpp
存放地图(100\*100的细粒度)的相信信息,包括 3\*3 内的碰撞点信息, 是否被敌方占领
### input.hpp
存放了输入输出的所有代码,包括初始化时所有预处理的代码
### path.hpp
记录了一次买卖规划的全部信息
### radar.hpp
激光雷达的具体实现
### robot.hpp
对于 robot 类的具体实现,以及所有的成员函数的实现
### vector.hpp
二维向量类
### worktable.hpp
工作台类的具体实现

## genMap

路径信息由-DCREATE编译选项控制,会输出相应的path.txt到 genMap 文件夹下

主要用于可视化的观察 BFS 寻找的路径(也就是具有锯齿状的路径)以及避免撞墙和最后拉直后的路径

方便查看自己优化后的路径是否够值,撞墙是因为路径规划中避免撞墙的问题还是拉直的 bug

