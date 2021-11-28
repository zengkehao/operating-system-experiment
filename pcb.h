#ifndef PCB_H
#define PCB_H
#include <cstring>
#include<string>
#include<sstream>
#include <iostream>
#include<QMessageBox>
using namespace std;

typedef struct Node{
    int PID;            //进程名
    int runTime;        //运行时间
    int priority;       //优先权   小数字表示高优先级
    int state;          //状态   1：后备队列   2：就绪队列  3：CPU内  4：挂起队列
    int size;           //内存大小
    int startAddress;   //起址

    Node* next;         //指针
}Node,*PCB;

void InitPCB (PCB *plink);
bool AddPCB(PCB plink,int PID, int runTime, int priority,int state);
bool AddPCB1(PCB plink,int PID, int runTime, int priority,int state,int size, int partitionTable[2][25]);
int getAddress(int size,int partitionTable[2][25]);
int getMaxSize(int partitionTable[]);
int getMaxSizeAddress(int partitionTable[]);
void PrintPCB(PCB plink);
int getNum(string s);
string toString(int n);
void SortPCB(PCB plink);
void Swap(PCB pIndex,PCB pMin);

#endif // PCB_H
