#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Table.h"
#include <QDebug>
#include "iostream"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setHorizontalHeaderLabels(QString("Código;Nombre;C;HT;HP;HL").split(";"));
    ui->tableWidget->width();
    ui->tableWidget->setColumnWidth(2,30);
    ui->tableWidget->setColumnWidth(3,30);
    ui->tableWidget->setColumnWidth(4,30);
    ui->tableWidget->setColumnWidth(5,30);
    ui->tableWidget->setColumnWidth(1,300);
    ui->tableWidget->setColumnWidth(0,50);
    //ui->tableWidget->setItem(1,1,new QTableWidgetItem("ñacaaaaaaaaaaaaaaa"));
    //ui->tableWidget->setItem(0,0,new QTableWidgetItem("1001223"));
    //ui->tableWidget->setItem(0,1,new QTableWidgetItem("ITEM 2"));
    //ui->tableWidget->setItem(0,2,new QTableWidgetItem("2"));
    //ui->tableWidget->setItem(0,3,new QTableWidgetItem("3"));
    //ui->tableWidget->setItem(0,4,new QTableWidgetItem("1"));
    //ui->tableWidget->setItem(0,5,new QTableWidgetItem("0"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->tableWidget->clearContents();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}

void MainWindow::on_pushButton_7_clicked()
{
    //string tmp = ui->lineEdit->text().toStdString();
    Table tt("D:/cursos.dat");
    Campo campo1;
    if(tt.find("PROYECTO I",2,campo1)){
        qDebug("hola");
    }
    else{
        qDebug("asdaklsk");
    }
    cout<<campo1.size()<<endl;
    for(auto iter = campo1.begin(); iter != campo1.end(); ++iter){
        cout<<*iter<<endl;
    }
    cout<<campo1.back()<<endl;
    ui->lineEdit_6->setText(QString::fromStdString(campo1.back()));
    campo1.pop_back();
    ui->lineEdit_5->setText(QString::fromStdString(campo1.back()));
    campo1.pop_back();    
    ui->lineEdit_4->setText(QString::fromStdString(campo1.back()));
    campo1.pop_back();
    ui->lineEdit_3->setText(QString::fromStdString(campo1.back()));
    campo1.pop_back();
    ui->lineEdit_2->setText(QString::fromStdString(campo1.back()));
    campo1.clear();

}
