#ifndef PTABLE_H
#define PTABLE_H
#include <cstring>
#include<string>
#include<sstream>
#include <iostream>
#include<QMessageBox>
using namespace std;

typedef struct Node2{
    int PID;
    int address;
    int memsize;

    Node2* next;
}Node2,*Table;

void InitTable (Table *tlink);

void AddRowToTable(int PID, int address, int memsize,Table tlink);

void PrintTable(Table tlink);

#endif // PTABLE_H
