#ifndef XMLPARSING_H
#define XMLPARSING_H
#include <QObject>
#include "subject.h"
#include "observer.h"
#include <QDomDocument>
#include <QByteArray>
#include "robotdata.h"

class XMLparsing :
        public QObject,
        public Observer,
        public Subject
{
    Q_OBJECT
public:
    XMLparsing(QObject *parent = 0);
    ~XMLparsing();

    void onNotify(Event *event);

private slots:

private:
    void config();
    void connection();
    void releaseResource();
    void clearBuffer();
    void clearXMLdoc();
    void extractData();
    void createXMLContant(RobotData robotData);
    bool validateXMLContent();

    //attributes
    QByteArray incommingBytes_;
    QByteArray outcommingBytes_;
    QDomDocument xmlDoc_;
    RobotData robotData_;
};

#endif // XMLPARSING_H
