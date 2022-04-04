#include "qrest.h"

QRest::QRest(QNetworkAccessManager& m)
    :manager(m)
{
}

void QRest::DoRequest(HTTP_CMD method, QString st, QString cmd, QJsonObject data)
{
    QString url = "http://localhost:9080/" + st;
    QNetworkRequest request(url+cmd);
    if(!data.empty())
        request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
    QJsonDocument doc;
    if(!data.empty())
        doc.setObject(data);
    QString s = doc.toJson();
    switch (method)
    {
    case HTTP_GET:
        manager.get(request);
        break;
    case HTTP_PUT:
        manager.put(request, doc.toJson());
        break;
    case HTTP_POST:
        manager.post(request, doc.toJson());
        break;
    case HTTP_DELETE:
        manager.deleteResource(request);
        break;
    }
}
