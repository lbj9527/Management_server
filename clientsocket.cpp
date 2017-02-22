#include <QtNetwork>
#include <QSqlRecord>
#include "clientsocket.h"
#include <QDebug>

ClientSocket::ClientSocket(QObject *parent)
    : QTcpSocket(parent)
{

    model = new QSqlTableModel(this);

    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    nextBlockSize = 0;
}

void ClientSocket::readClient()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_3);

    if (nextBlockSize == 0) {
        if (bytesAvailable() < sizeof(quint16))
            return;
        in >> nextBlockSize;
    }

    if (bytesAvailable() < nextBlockSize)
        return;

    quint8 requestType;
    QString from;
    QString to;
    QDate date;
    QTime time;
    quint8 flag;

    in >> requestType;
    if (requestType == 'S') {
        in >> from >> to >> date >> time >> flag;
        /*
       //通过参数提供的种子，产生随机数，一般使用时间作为种子
        std::srand(from.length() * 3600 + to.length() * 60
                   + time.hour());
        int numTrips = std::rand() % 8;
        for (int i = 0; i < numTrips; ++i)
        */
            generateRandomTrip(from, to, date, time);

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    close();
}

void ClientSocket::generateRandomTrip(const QString & /* from */,
        const QString & /* to */, const QDate &date, const QTime &time)
{

    model->setTable("scooter");   //与数据库中的表关联

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);
    quint16 duration = std::rand() % 200;
    QSqlRecord record = model->record(1);
    QString new_add_string = record.value("description").toString();
    qDebug() << new_add_string;

    out << quint16(0) << date << time << duration << quint8(1)
        << new_add_string;
    //    << QString("InterCity");
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}
