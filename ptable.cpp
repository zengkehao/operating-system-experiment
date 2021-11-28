#include "ptable.h"
#include "pcb.h"
#include <cstring>
#include<string>
#include<sstream>
#include <iostream>
#include <QMessageBox>
using namespace std;

//初始化分区表
void InitTable (Table *tlink)
{
    (*tlink) = (Table)malloc(sizeof(Node2));
    (*tlink)->PID = 0;
    (*tlink)->address = -1;
    (*tlink)->memsize = -1;

    (*tlink)->next = NULL;
}

//新增分区表行
void AddRowToTable(int PID, int address, int memsize,Table tlink)
{
    Table t;
    Table head;
    head = tlink;

    t = (Table)malloc(sizeof(Node2));

    t->PID = PID;
    t->address = address;
    t->memsize = memsize;

    t->next = NULL;

    while(head->next != NULL)
    {
        head = head->next;
    }

    head->next = t;
}

//控制台打印分区表
void PrintTable(Table tlink)
{
    cout << "Table" << endl;

    Table t = tlink->next;
    Table tN;

    cout << "PID" << "	" << "address" << "	" << "memsize" << endl;

    while(t != NULL)
    {
        cout << "P" << t->PID << "	";
        cout << t->address << "		";
        cout << t->memsize << "		";

        //tN = t->next;
        t = t->next;
        cout << endl;

     }
}
