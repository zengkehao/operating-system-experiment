#include "widget.h"
#include "ui_widget.h"
#include<iostream>
#include <cstring>
#include<string>
#include<sstream>
#include<QMessageBox>
#include<QStandardItemModel>
#include<QHeaderView>
#include<QTableWidgetItem>
#include "pcb.h"
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    for (int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            partiti[i][j] = 0;
        }

    }

    ui->setupUi(this);
    InitPCB(&plink1);
    InitPCB(&plink2);
    InitPCB(&plink3);
    InitPCB(&plink4);

    InitTable(&tlink);

    NumOfP1 = 0;
    NumOfP2 = 0;
    NumOfP3 = 0;
    NumOfP4 = 0;

    ShowMemoryTable();
}

Widget::~Widget()
{
    delete ui;
}

//点击AddButton按钮添加新PCB
void Widget::on_AddButton_clicked()
{
    QString PID = ui->PID->text();
    int RunTime = ui->RunTime->text().toInt();
    int priority = ui->priority->text().toInt();
    int memsize = ui->memsize->text().toInt();

    //判断输入是否正确
    if(PID == NULL || RunTime == 0)
    {
        QMessageBox::information(NULL, "错误", "请输入对应数据");
    }

    else {
        string s1 = PID.toStdString();
        int n = getNum(s1);

        //cout << n << endl;

        if(AddPCB1(plink1,n,RunTime,priority,1,memsize,partiti) == true)
        {
            NumOfP1++;
        //PrintPCB(plink1);

        //排序plink1？

        ShowPool();

        //暂时先不用，方便测试，最后修改过来
        Widget::on_ClearButton_clicked();

        on_JobpushButton_clicked();

        }

        else{
            QMessageBox::information(NULL,"错误", "内存空间不足或出现同名进程");
        }
        PrintPCB(plink1);

        ShowMemoryTable();

        ShowPartitionTable();
    }

    PrintPartitionArray();
}

//打印plink1中PCB
void Widget::ShowPool()
{
    ui->PoolplainTextEdit->clear();
    PCB p = plink1->next;
    string s;
    while(p != NULL)
    {
        s = "P" + toString(p->PID) + " " + toString(p->runTime) + " " + toString(p->priority) + " ";
        switch(p->state){
            case 1:
                s = s + "Reserve";
            break;
            case 2:
                s = s + "Ready";
            break;
            case 3:
                s = s + "Run";
            break;
            case 4:
                s = s + "Suspended";
        }

        //cout << s << endl;

        QString s2 = QString::fromStdString(s);
        ui->PoolplainTextEdit->appendPlainText(s2);

        p = p->next;
     }
}

void Widget::ShowMemoryTable()
{
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    //设置表头
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("PID"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("address"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("length"));

    ui->MemoryTableView->setModel(model);

    //表头信息显示居左

    ui->MemoryTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置列宽不可变
    ui->MemoryTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);

    ui->MemoryTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);

    ui->MemoryTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);

    ui->MemoryTableView->verticalHeader()->setDefaultSectionSize(15);

    ui->MemoryTableView->setColumnWidth(0,90);

    ui->MemoryTableView->setColumnWidth(1,90);

    ui->MemoryTableView->setColumnWidth(2,90);

    //将行头隐藏
    ui->MemoryTableView->verticalHeader()->hide();

    //设置选中时为整行选中
    ui->MemoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->MemoryTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    string s;

    //显示分区表
    for(int i = 0; i < 25; i++)
    {
        if(partiti[0][i] == 2)
        {
            s = "P" + toString(partiti[1][i]);
            QString s1 = QString::fromStdString(s);
            model->setItem(i,0,new QStandardItem(s1));

            s = toString(i + 1);
            QString s2 = QString::fromStdString(s);
            model->setItem(i,1,new QStandardItem(s2));

            model->setItem(i,2,new QStandardItem("1"));

            int j = i + 1;
            int count = 1;
            while(j < 25 && partiti[0][j] == 1)
            {
                count++;
                s = toString(count);
                QString s3 = QString::fromStdString(s);
                model->setItem(j,2,new QStandardItem(s3));

                j++;
            }
        }

        if(partiti[0][i] == 0)
        {
            model->setItem(i,2,new QStandardItem(" "));
        }
    }

/*
 * 之前使用名字为PartitionTableplainTextEdit的控件
    ui->PartitionTableplainTextEdit->clear();

    string s;
    for(int i = 0; i < 25; i++)
    {
        if(partiti[0][i] == 2)
        {
            s = "P" + toString(partiti[1][i]) + "       " + toString(i + 1) + "        " + toString(1);
            QString s1 = QString::fromStdString(s);
            ui->PartitionTableplainTextEdit->appendPlainText(s1);

            int j = i + 1;
            int count = 1;
            while(j < 25 && partiti[0][j] == 1)
            {
                count++;
                s = "                  " + toString(count);
                QString s2 = QString::fromStdString(s);
                ui->PartitionTableplainTextEdit->appendPlainText(s2);
                j++;
            }
        }

        if(partiti[0][i] == 0)
        {
            s = " ";
            QString s3 = QString::fromStdString(s);
            ui->PartitionTableplainTextEdit->appendPlainText(s3);
        }
    }
    */
}

void Widget::ShowPartitionTable()
{
    QStandardItemModel  *model = new QStandardItemModel();

    model->setColumnCount(3);

    //设置表头
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("PID"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("address"));

    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("length"));

    ui->PartitionTableView->setModel(model);

    //表头信息显示居左

    ui->PartitionTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    //设置列宽不可变
    ui->PartitionTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);

    ui->PartitionTableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);

    ui->PartitionTableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);

    ui->PartitionTableView->verticalHeader()->setDefaultSectionSize(15);

    ui->PartitionTableView->setColumnWidth(0,90);

    ui->PartitionTableView->setColumnWidth(1,90);

    ui->PartitionTableView->setColumnWidth(2,90);

    //将行头隐藏
    ui->PartitionTableView->verticalHeader()->hide();

    //设置选中时为整行选中
    ui->PartitionTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->PartitionTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    string s;
    int i = 0;

    //显示分区表
    Table t = tlink->next;
    while(t != NULL)
    {
        s = "P" + toString(t->PID);
        QString s1 = QString::fromStdString(s);
        model->setItem(i,0,new QStandardItem(s1));

        s = toString(t->address);
        QString s2 = QString::fromStdString(s);
        model->setItem(i,1,new QStandardItem(s2));

        s = toString(t->memsize);
        QString s3 = QString::fromStdString(s);
        model->setItem(i,2,new QStandardItem(s3));

        i++;

        t = t->next;
    }

}

void Widget::PrintPartitionArray()
{
    //添加后查看二维数组
    for (int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            cout << partiti[i][j] << " ";
        }
           cout << endl;
    }
}

//清空添加新进程中各个输入栏的数据
void Widget::on_ClearButton_clicked()
{
    ui->PID->clear();
    ui->RunTime->clear();
    ui->priority->setValue(1);
    ui->memsize->clear();
}

//点击JobpushButton进行作业调度
void Widget::on_JobpushButton_clicked()
{
    ui->PoolplainTextEdit->clear();
    ui->ReadyplainTextEdit->clear();

    PCB p = plink1;
    PCB pF;
    PCB pT;
    string s;
    int numOfTrack = ui->NumOfTrack->text().toInt();//设置道数
    int n = 0;          //判断有多少PCB进程进入plink2

    //判断后备队列中是否有进程
    if(NumOfP1 == 0)
    {
        QMessageBox::information(NULL,"错误","后备队列中无进程");
    }

    //判断就绪队列是否已满
    else if(numOfTrack - NumOfP2 == 0)
    {
        //QMessageBox::information(NULL,"错误","就绪队列已满");
        ShowPool();
        ShowReady();
    }

    else{
        int address = -1;

        //将plink1前6位(若不足6则全部)移动至plink2
        PCB head2 = plink2;
        while(head2->next != NULL)
        {
            head2 = head2->next;
        }

        //将进程添加到内存中
        while(p->next != NULL && n < numOfTrack - NumOfP2)
        {
            pF = p;
            p = p->next;

            address = getAddress(p->size,partiti);
            if(address >= 0)
            {
                p->startAddress = address + 1;

                partiti[0][address] = 2;
                partiti[1][address] = p->PID;

                for(int i = address + 1; i < address + p->size; i++)
                {
                    partiti[0][i] = 1;
                    partiti[1][i] = -1; //容易观察
                }

                p->state = 2;

                pT = p;
                //p = p->next;

                head2->next = pT;
                pF->next = pT->next;
                pT->next = NULL;

                //添加进程到tlink中
                AddRowToTable(pT->PID,pT->startAddress,pT->size,tlink);

                n++;
            }

            else{
                if(p->next == NULL)
                {
                    break;
                }

                else{
                    pF = p;
                    p = p->next;
                }
            }
         }
        cout << "n:" << n << endl;
        NumOfP2 = NumOfP2 + n;
        cout << "NumOfP2:" << NumOfP2 << endl;

//        head2->next = plink1->next;
//        plink1->next = p->next;
//        p->next = NULL;

        ShowPartitionTable();
        ShowMemoryTable();
        ShowPool();
        ShowReady();

        PrintPartitionArray();

        PrintPCB(plink1);
        PrintPCB(plink2);
        PrintTable(tlink);
    }
}

//显示就绪队列中的PCB
void Widget::ShowReady()
{
    ui->ReadyplainTextEdit->clear();
    PCB p = plink2->next;
    PCB pN;
    string s;

    while(p != NULL)
    {
        s = "P" + toString(p->PID) + " " + toString(p->runTime) + " " + toString(p->priority) + " ";
        switch(p->state){
            case 1:
                s = s + "Reserve";
            break;
            case 2:
                s = s + "Ready";
            break;
            case 3:
                s = s + "Run";
            break;
            case 4:
                s = s + "Suspended";
        }


        pN = p->next;
        if(pN != NULL)
        {
            s = s + " P" + toString(pN->PID);
        }
        else {
            s = s + " NULL";
        }

        //cout << s << endl;

        QString s2 = QString::fromStdString(s);
        ui->ReadyplainTextEdit->appendPlainText(s2);

        p = p->next;
    }
}

//点击SuspendedpushButton挂起指定进程
void Widget::on_SuspendedpushButton_clicked()
{
    QString PID = ui->SuspendedlineEdit->text();
    //全完成后再删除注释
    ui->SuspendedlineEdit->clear();

    if(PID == NULL)
    {
        QMessageBox::information(NULL, "错误", "请输入对应数据");
    }

    else
    {
        string s = PID.toStdString();
        int id = getNum(s);

        cout << "id: " << id << endl;
        //设置flag判断是否找到要求的进程
        bool flag = false;

        //遍历plink2链表，找出对应PID，放入plink4中
        PCB p = plink2->next;
        PCB pF = plink2;

        while(p != NULL)
        {

            if(p->PID == id)
            {
                flag = true;
                break;
            }

            p = p->next;
            pF = pF->next;
        }

//        cout << "flag " << flag << endl;
//        cout << "p:P " << p->PID << endl;
//        cout << "pF:P " << pF->PID << endl;

        if(flag == true)
        {
            PCB head4 = plink4;
            while(head4->next != NULL)
            {
                head4 = head4->next;
            }

            head4->next = p;
            pF->next = p->next;
            p->next = NULL;

            ShowReady();
            ShowSuspended();

            cout << "plink2" << endl;
            PrintPCB(plink2);
            cout << "plink4" << endl;
            PrintPCB(plink4);
        }

        else {
            QMessageBox::information(NULL, "提示","就绪队列中无此进程");
        }
    }
}

//显示挂起队列中的PCB
void Widget::ShowSuspended()
{
    ui->SuspendedplainTextEdit->clear();
    PCB p = plink4->next;
    PCB pN;
    string s;

    while(p != NULL)
    {
        s = "P" + toString(p->PID) + " " + toString(p->runTime) + " " + toString(p->priority) + " ";
        switch(p->state){
            case 1:
                s = s + "Reserve";
            break;
            case 2:
                s = s + "Ready";
            break;
            case 3:
                s = s + "Run";
            break;
            case 4:
                s = s + "Suspended";
        }


        pN = p->next;
        if(pN != NULL)
        {
            s = s + " P" + toString(pN->PID);
        }
        else {
            s = s + " NULL";
        }

        //cout << s << endl;

        QString s2 = QString::fromStdString(s);
        ui->SuspendedplainTextEdit->appendPlainText(s2);

        p = p->next;
    }
}

//点击LosspushButton解挂某一个指定的PCB
void Widget::on_LosspushButton_clicked()
{
    QString PID = ui->LosslineEdit->text();
    ui->LosslineEdit->clear();

    if(PID == NULL)
    {
        QMessageBox::information(NULL, "错误", "请输入对应数据");
    }

    else
    {
        string s = PID.toStdString();
        int id = getNum(s);

        cout << "id: " << id << endl;
        bool flag = false;

        //遍历plink4链表，找出对应PID，放入plink2中
        PCB p = plink4->next;
        PCB pF = plink4;

        while(p != NULL)
        {
            if(p->PID == id)
            {
                flag = true;
                break;
            }

            p = p->next;
            pF = pF->next;
        }

        if(flag == true)
        {
            PCB head2 = plink2;
            while(head2->next != NULL)
            {
                head2 = head2->next;
            }

            head2->next = p;
            pF->next = p->next;
            p->next = NULL;

            ShowReady();
            //ShowPool();
            ShowSuspended();

            cout << "plink2" << endl;
            PrintPCB(plink2);
            cout << "plink4" << endl;
            PrintPCB(plink4);
        }

        else {
            QMessageBox::information(NULL, "提示","就绪队列中无此进程");
        }

    }
}

//点击CPUpushButton根据CPUSchedulingMethod中数据选择算法调度
void Widget::on_CPUpushButton_clicked()
{
    int method = ui->CPUSchedulingMethod->text().toInt();

    PCB p = plink2->next;
    PCB pF = plink2;

    switch (method) {
        case 1:
        //优先级调度
        PCB pMin,pT;

           if(p != NULL)
           {
               int minPriority = 8;
               do
               {
                   int pri = p->priority;
                   if(pri < minPriority)
                   {
                       minPriority = pri;
                       pMin = p;
                   }

                   p = p->next;
               }while(p != NULL);

               while(pF->next != NULL)
               {

                   if(pF->next == pMin)
                   {
                       break;
                   }

                   pF = pF->next;
               }

               cout << "pMin: P" << pMin->PID << endl;
               cout << "pF: P" << pF->PID << endl;

               //处理PCB的运行时间和优先级
               DealPCB(pMin,pF);

               ShowReady();
           }

           else {
               QMessageBox::information(NULL,"提示","就绪队列中暂无进程");
           }

            break;


        case 2:
        //轮换调度
        //从TimeSlice处取时间片的长度
        int timeSlice = ui->TimeSlice->text().toInt();

        PCB pTemp;
        InitPCB(&pTemp);

        ui->RunTimelineEdit->clear();
        ui->PIDlineEdit->clear();
        ui->PrioritylineEdit->clear();

        if(p == NULL)
        {
            QMessageBox::information(NULL, "提示", "就绪队列中暂无进程");
        }
        else {
            while (p != NULL)
            {
                if(p->runTime <= timeSlice)
                {
                    p->runTime = 0;
                    pF->next = p->next;
                    pTemp = p;

                    string s = toString(pTemp->PID);
                    s = "P" + s;
                    QString s2 = QString::fromStdString(s);
                    ui->PIDlineEdit->setText(s2);

                    QString s3 = QString::fromStdString(toString(pTemp->runTime));
                    ui->RunTimelineEdit->setText(s3);

                    QString s4 = QString::fromStdString(toString(pTemp->priority));
                    ui->PrioritylineEdit->setText(s4);

                    p = p->next;
                    if(p == NULL)
                    {
                        ui->RunTimelineEdit->clear();
                        ui->PIDlineEdit->clear();
                        ui->PrioritylineEdit->clear();
                    }

                    delete pTemp;
                    NumOfP2--;
                }

                else {
                    p->runTime = p->runTime - timeSlice;

                    string s = toString(p->PID);
                    s = "P" + s;
                    QString s2 = QString::fromStdString(s);
                    ui->PIDlineEdit->setText(s2);

                    QString s3 = QString::fromStdString(toString(p->runTime));
                    ui->RunTimelineEdit->setText(s3);

                    QString s4 = QString::fromStdString(toString(p->priority));
                    ui->PrioritylineEdit->setText(s4);

                    p = p->next;
                    pF = pF->next;
                }
            }

            ShowReady();
            ShowPartitionTable();
        }

        break;
    }
}

//处理PCB，用于优先级调度
void Widget::DealPCB(PCB pMin,PCB pF)
{
    pMin->runTime--;

    if(pMin->runTime != 0)
    {
        if(pMin->priority != 1)
        {
            pMin->priority--;
        }

        string s = toString(pMin->PID);
        s = "P" + s;
        QString s2 = QString::fromStdString(s);
        ui->PIDlineEdit->setText(s2);

        QString s3 = QString::fromStdString(toString(pMin->runTime));
        ui->RunTimelineEdit->setText(s3);

        QString s4 = QString::fromStdString(toString(pMin->priority));
        ui->PrioritylineEdit->setText(s4);
    }

    else {
        pF->next = pMin->next;
        pMin->next = NULL;

        int id = pMin->PID;
        cout << id << endl;
        int index = pMin->startAddress - 1;

        //此时再遍历tlink，找到对应的id，从链表中删除
        Table t,tF,tT;
        t = tlink;
        while(t->next != NULL)
        {
            tF = t;
            t = t->next;
            if(t->PID == id)
            {
                tT = t;
                tF->next = t->next;
                delete tT;
                break;
            }
        }

        cout << index << endl;
        for(int i = index; i < 25; i++)
        {
            partiti[0][index] = 0;
            partiti[1][index] = 0;
            int j = i + 1;
            while(j < 25 && partiti[0][j] != 0 && partiti[0][j] != 2)
            {
                if(partiti[0][j] == 1 && partiti[1][j] == -1)
                {
                    partiti[0][j] = 0;
                    partiti[1][j] = 0;
                }
                j++;
            }
            break;
        }
        delete pMin;
        NumOfP2--;

        PrintPartitionArray();
        ShowPartitionTable();
        ShowMemoryTable();

        ui->PIDlineEdit->clear();
        ui->RunTimelineEdit->clear();
        ui->PrioritylineEdit->clear();
    }
}

//排序链表
void Widget::on_SortpushButton_clicked()
{
    SortPCB(plink1);
    ShowPool();
}

//排序链表
void Widget::on_SortReadypushButton_clicked()
{
    SortPCB(plink2);
    ShowReady();
}
