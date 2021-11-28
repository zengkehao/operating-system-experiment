#ifndef WIDGET_H
#define WIDGET_H
#include <cstring>
#include<string>
#include<sstream>
#include <QWidget>
#include<QMessageBox>
#include<QStandardItemModel>
#include "pcb.h"
#include "ptable.h"
using namespace std;

//extern PCB plink1;
//extern PCB plink2;
//extern PCB plink3;
//extern PCB plink4;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    PCB plink1;
    PCB plink2;
    PCB plink3;
    PCB plink4;

    Table tlink;

    int NumOfP1;
    int NumOfP2;
    int NumOfP3;
    int NumOfP4;

    int partiti[2][25];


private slots:
    void on_AddButton_clicked();

    void ShowPool();

    void on_ClearButton_clicked();

    void on_JobpushButton_clicked();

    void ShowReady();

    void on_SuspendedpushButton_clicked();

    void ShowSuspended();

    void on_LosspushButton_clicked();

    void on_CPUpushButton_clicked();

    void DealPCB(PCB pMin,PCB pF);

    void on_SortpushButton_clicked();

    void on_SortReadypushButton_clicked();

    void ShowPartitionTable();

    void ShowMemoryTable();

    void PrintPartitionArray();

private:
    Ui::Widget *ui;
};



#endif // WIDGET_H
