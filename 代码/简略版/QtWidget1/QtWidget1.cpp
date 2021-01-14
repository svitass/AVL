#include "QtWidget1.h"
#include <qtextcodec.h>   //防止中文乱码
#include <regex>
#include <cstring>

QtWidget1::QtWidget1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QMetaObject::connectSlotsByName(this);
    scene = new QGraphicsScene;
    //ui.graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui.graphicsView->setScene(scene);
    QString str = print();
    scene->addText(str);
}


void QtWidget1::on_insertButton_clicked() {
    //ui.insertButton->setText(tr("insert"));
    QString str = ui.input->toPlainText();
    scene = new QGraphicsScene;
    bool ok;
    int element = str.toInt(&ok);
    if (ok) {
        avl.insert(element);
    }
    str = print();
    ui.graphicsView->setScene(scene);
    scene->addText(str);
    ui.input->setText(tr(""));
    /*if (!ok) {
        str = "Invalid input!";
        ui.graphicsView->setScene(scene);
        scene->addText(str);
        ui.input->setText(tr(""));

    }else{
        avl.insert(element);
        str = print();
        ui.graphicsView->setScene(scene);
        scene->addText(str);
        ui.input->setText(tr(""));
    }*/
    //显示树？？？
    //ui.graphicsView->setBackgroundBrush(QBrush(Qt::black));
    
}

void QtWidget1::on_deleteButton_clicked() {
    //ui.deleteButton->setText(tr("delete"));
    QString str = ui.input->toPlainText();
    bool ok;
    int element = str.toInt(&ok);
    if (ok) {
        avl.deleteNode(element);
    }
    //显示树？？？
    scene = new QGraphicsScene;
    //ui.graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui.graphicsView->setScene(scene);
    str = print();
    scene->addText(str);
    ui.input->setText(tr(""));
}

void QtWidget1::on_searchButton_clicked() {
    //ui.searchButton->setText(tr("search"));
    QString str = ui.input->toPlainText();
   /* string s = str.toStdString();
    int elem = atoi(s.c_str());*/
    bool ok;
    int element = str.toInt(&ok);
    bool isExist = false;
    if (ok) {
        isExist = avl.search(element);
    }
    if (isExist) {
        str = "Exsit!!!";
    }
    else {
        str = "Not Exsit!!!";
    }
    scene = new QGraphicsScene;
    //ui.graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui.graphicsView->setScene(scene);
    scene->addText(str);
    ui.input->setText(tr(""));
}

QString QtWidget1::print() {
    Node<int>* root = avl.getRoot();
    int height = avl.getNodeHeight(root);
    QString qStr = "";
    if (height == 0) {
        qStr = "empty tree!";
        return qStr;
    }
    int h = height * 2 - 1;
    int w = pow(2, height - 1) * 3 + 1;
 /*   QString s1 = "h:" + QString::number(h);
    QString s2 = "w:" + QString::number(w);
    ui.input->setText(s1+s2);*/
    vector<vector<string>> res(h, vector<string>(w));
    //string** res = new string[h][w];
    for (int i = 0;i < h;i++) {   //初始化数组
        for (int j = 0;j < w;j++) {
            res[i][j] = " ";
        }
    }
    avl.writeArray(root, 0, w / 2, res, height);
    for (int i = 0;i < h;i++) {
        string str = "";
        for (int j = 0;j < w;j++) {
            string con = res[i][j];
            str = str + con;
        }
        str = str + "\n";
        QString tempStr = QString::fromStdString(str);
        qStr = qStr + tempStr;
    }
    return qStr;
}