#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QSqlTableModel>

//class QSqlTableModel;

class QDate;
class QTime;

class ClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket(QObject *parent = 0);

private slots:
    void readClient();

private:
    QSqlTableModel *model;
    void generateRandomTrip(const QString &from, const QString &to,
                            const QDate &date, const QTime &time);

    quint16 nextBlockSize;
};

#endif
