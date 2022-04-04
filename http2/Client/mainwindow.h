#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include "qrest.h"
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onManagerFinished(QNetworkReply *reply);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QRest rest;
    QNetworkAccessManager manager;
};
#endif // MAINWINDOW_H
