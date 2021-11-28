#include "widget.h"
#include <cstring>
#include <stdlib.h>
#include<iostream>
#include <QApplication>
#include "pcb.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

//    PCB plink1;

//        InitPCB(&plink1);

//        AddPCB(plink1, 1,4,2,1);
//        AddPCB(plink1, 2,3,1,2);
//        AddPCB(plink1, 3,2,3,3);
//        SortPCB(plink1);
//        PrintPCB(plink1);

    return a.exec();
}
