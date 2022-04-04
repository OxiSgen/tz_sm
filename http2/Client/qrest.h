#ifndef QREST_H
#define QREST_H

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>

enum HTTP_CMD
{
    HTTP_GET,
    HTTP_PUT,
    HTTP_POST,
    HTTP_DELETE
};

class QRest
{
    QNetworkAccessManager& manager;
public:
    QRest(QNetworkAccessManager& pManager);
    void DoRequest(HTTP_CMD method, QString st = "", QString cmd = "", QJsonObject data = QJsonObject());
};

#endif // QREST_H
