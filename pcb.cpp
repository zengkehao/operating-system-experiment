#include "pcb.h"
#include <cstring>
#include<string>
#include<sstream>
#include <iostream>
#include <QMessageBox>
using namespace std;

//初始化
void InitPCB (PCB *plink)
{
    (*plink) = (PCB)malloc(sizeof(Node));
    (*plink)->PID = 0;
    (*plink)->runTime = 0;
    (*plink)->priority = 8;
    (*plink)->state = 1;
    (*plink)->size = -1;
    (*plink)->startAddress = -1;
    (*plink)->next = NULL;
}

//添加新结点（PCB）至链表
bool AddPCB(PCB plink,int PID, int runTime, int priority,int state)
{
    PCB p;
    PCB head;
    head = plink;
    //flag1用于检测是否有同名进程
    bool flag = true;

        p = (PCB)malloc(sizeof(Node));

        p->PID = PID;
        p->runTime = runTime;
        p->priority = priority;
        p->state = state;

        p->next = NULL;

        while(head->next != NULL)
        {
            head = head->next;
            if(head->PID == PID)
            {
                flag = false;
                break;
            }
        }

        if(flag)
        {
            head->next = p;
        }
        else {
            QMessageBox::information(NULL,"错误", "待加入的队列中有同名进程");
        }

    return flag;
}


//添加新结点（PCB）至链表
bool AddPCB1(PCB plink,int PID, int runTime, int priority,int state,int size, int partitionTable[2][25])
{
    PCB p;
    PCB head;
    head = plink;
    //flag1用于检测是否有同名进程
    bool flag1 = true;
    //判读能否加入进程
    bool flag = false;

    while(head->next != NULL)
    {
        head = head->next;
        if(head->PID == PID)
        {
            flag1 = false;
            break;
        }
    }

    if(flag1)
    {
//        int address = getAddress(size,partitionTable);

//        if(address >= 0)
//        {
            flag = true;

            p = (PCB)malloc(sizeof(Node));
            head->next = p;

            p->PID = PID;
            p->runTime = runTime;
            p->priority = priority;
            p->state = state;

            p->size = size;
//            p->startAddress = address + 1;

//            partitionTable[0][address] = 2;
//            partitionTable[1][address] = p->PID;
//            for(int i = address + 1; i < address + size; i++)
//            {
//                partitionTable[0][i] = 1;
//                partitionTable[1][i] = -1; //容易观察
//            }

            p->next = NULL;

//        }

//        else {
//            flag = false;
//        }

    }
    else {
        QMessageBox::information(NULL,"错误", "待加入的队列中有同名进程");
    }
    return flag;
}

int getAddress(int size,int partitionTable[2][25])
{
    int s = 0;
    int result = -1;
    int address;

    if(size > 25)
    {
        return result;
    }

    else {
        for(int i = 0; i < 25; i++)
        {
            if(partitionTable[0][i] == 0)
            {
                s = 0;
                address = i;
                for (int j = i; j < 25; j++)
                {
                    if(partitionTable[0][j] == 0)
                    {
                        s++;
                    }

                    else{
                        break;
                    }
                }

                if(s >= size)
                {
                    result = address;
                    break;
                }
            }
        }

        return result;
    }
}


int getMaxSize(int partitionTable[])
{
    int size = 0;
    int address = -1;
    int maxSize = 0;
    int maxAddress = -1;
    for(int i = 0; i < 25; i++)
    {
        if(partitionTable[i] == 0)
        {
            size = 0;
            address = i;
            for (int j = i; j < 25; j++)
            {
                if(partitionTable[j] == 0)
                {
                    size++;
                }

                else{
                    break;
                }
            }

            if(size > maxSize)
            {
                maxSize = size;
                maxAddress = address;
            }
        }
    }

    return maxSize;
}

int getMaxSizeAddress(int partitionTable[])
{
    int size = 0;
    int address = -1;
    int maxSize = 0;
    int maxAddress = -1;
    for(int i = 0; i < 25; i++)
    {
        if(partitionTable[i] == 0)
        {
            size = 0;
            address = i;
            for (int j = i; j < 25; j++)
            {
                if(partitionTable[j] == 0)
                {
                    size++;
                }

                else{
                    break;
                }
            }

            if(size > maxSize)
            {
                maxSize = size;
                maxAddress = address;
            }
        }
    }

    return maxAddress;
}

//打印链表，显示PCB的各个属性 （控制台处）
void PrintPCB(PCB plink)
{
    cout << "PCB:" << endl;
    PCB p = plink->next;
    PCB pN;
    cout << "PID" << "  " << "RunTime" << "  " << "priority" << "  " << "state" << endl;
    while(p != NULL)
    {
        cout << "P" << p->PID << " ";
        cout << p->runTime << " ";
        cout << p->priority << " ";
        cout << p->state << " ";
        cout << p->size << " ";
        cout << p->startAddress << " ";

        //判断一下pN是否为0
        pN = p->next;
        if(pN != NULL)
        {
            cout << "P" << pN->PID << " ";
        }

        else {
            cout << "NULL";
        }


        p = p->next;

        cout << endl;
    }
}

//提取PCB名字中的数字
int getNum(string s)
{
    int n;
    for(int i = 1; i <= s.length() - 1; i++)
    {
        n = n * 10;
        n = n + s.at(i) - 48;
    }
    return n;
}

//转换数字到字符串
string toString(int n)
{
        stringstream s;
        s << n;
        return s.str();
}

//将两个PCB的除next指针的属性进行交换
void Swap(PCB pIndex,PCB pMin)
{
    PCB pTemp;
    InitPCB(&pTemp);

    pTemp->PID = pIndex->PID;
    pTemp->runTime = pIndex->runTime;
    pTemp->priority = pIndex->priority;
    pTemp->state = pIndex->state;

    pIndex->PID = pMin->PID;
    pIndex->runTime = pMin->runTime;
    pIndex->priority = pMin->priority;
    pIndex->state = pMin->state;

    pMin->PID = pTemp->PID;
    pMin->runTime = pTemp->runTime;
    pMin->priority = pTemp->priority;
    pMin->state = pTemp->state;
}

//对链表按照优先级的次序进行排序
void SortPCB(PCB plink)
{
    PCB pStart = plink->next;
    PCB p,pMin,pIndex;
    int n = 0;
    while(pStart != NULL)
    {
        p = pStart;
        pMin = pStart;
        pIndex = pStart;
        while(p->next != NULL)
        {
            p = p->next;
            if(p->priority < pMin->priority)
            {
                pMin = p;
            }
        }

//        cout << "pIndex->PID:" << "P" << pIndex->PID << endl;
//        cout << "pMin->PID:" << "P" << pMin->PID << endl;

         Swap(pIndex,pMin);

        pStart = pStart->next;
    }

//    cout << "n:" << n << endl;
}
