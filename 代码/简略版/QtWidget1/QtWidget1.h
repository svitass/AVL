#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidget1.h"
#include "AVLTree.hpp"

class QtWidget1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidget1(QWidget *parent = Q_NULLPTR);
    QString print();   
public slots:
    void on_insertButton_clicked();
    void on_deleteButton_clicked();
    void on_searchButton_clicked();

private:
    Ui::QtWidget1Class ui;
    AVL<int> avl;
    QGraphicsScene* scene;
};
