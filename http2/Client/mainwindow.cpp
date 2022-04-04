#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , rest(manager)
{
    ui->setupUi(this);
    connect(&manager, &QNetworkAccessManager::finished, this, &MainWindow::onManagerFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onManagerFinished(QNetworkReply *reply)
{
    ui->listWidget->clear();
    QString s = reply->readAll();
    QJsonDocument json_doc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject obj = json_doc.object();
    for(auto st : obj){
        ui->listWidget->addItem(
                    st.toObject()["id"].toString() + " " +
                    st.toObject()["name"].toString() + " " +
                    st.toObject()["surname"].toString() + " " +
                    st.toObject()["date"].toString()
                );
    }

    reply->deleteLater();
}

void MainWindow::on_pushButton_clicked()
{
    rest.DoRequest(HTTP_GET, "ready");
}

