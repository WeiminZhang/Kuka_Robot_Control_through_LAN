#include "xmlparsing.h"
#include <QDomNode>
#include <QDomNodeList>
#include <QXmlStreamWriter>

XMLparsing::XMLparsing(QObject *parent)
    : QObject(parent)
{

}

XMLparsing::~XMLparsing()
{

}

void XMLparsing::onNotify(Event *event)
{
    if(event)
    {
        EventNetworkReceivedData *eventReceivedData
                = dynamic_cast<EventNetworkReceivedData*>(event);

        if(eventReceivedData)
        {
            incommingBytes_ += eventReceivedData->byte().data();
            if(validateXMLContent())
            {
                extractData();
                clearBuffer();
                clearXMLdoc();
            }
        }
        else
        {
            qDebug() << "TRY";
            EventNetworkStateClient *eventStateClient
                    = dynamic_cast<EventNetworkStateClient*>(event);

            if(eventStateClient)
            {
                if(eventStateClient->stateNetwork()->isStarted())
                {
                    clearBuffer();
                    clearXMLdoc();
                }
            }
            else
            {
                EventRobotDataEmbedData *eventRobotDataEmbedData
                        = dynamic_cast<EventRobotDataEmbedData*>(event);
                if(eventRobotDataEmbedData)
                {
                    createXMLContant(eventRobotDataEmbedData->robotData());
                    notify(new EventNetworkSendData(outcommingBytes_));
                }
            }
        }
    }
}

void XMLparsing::config()
{

}

void XMLparsing::connection()
{

}

void XMLparsing::releaseResource()
{

}

bool XMLparsing::validateXMLContent()
{
    return xmlDoc_.setContent(incommingBytes_);
}

void XMLparsing::clearBuffer()
{
    incommingBytes_.clear();
}

void XMLparsing::clearXMLdoc()
{
    xmlDoc_.clear();
}

void XMLparsing::extractData()
{
    robotData_ = RobotData{};

    //this code should refactored by tag!!!
    QDomElement docElem = xmlDoc_
            .documentElement()//convert doc element
            .firstChild()//Robot
            .toElement() //Data
            .firstChild()//Pos
            .toElement();//Values

    robotData_.X = docElem.attribute("X").toDouble();
    robotData_.Y = docElem.attribute("Y").toDouble();
    robotData_.Z = docElem.attribute("Z").toDouble();
    robotData_.A = docElem.attribute("A").toDouble();
    robotData_.B = docElem.attribute("B").toDouble();
    robotData_.C = docElem.attribute("C").toDouble();
    robotData_.isValid = true;

    notify(new EventRobotDataExtractData(robotData_));
}

void XMLparsing::createXMLContant(RobotData robotData)
{

    QByteArray xmlByte;
    QXmlStreamWriter xmlWrite(&xmlByte);
    xmlWrite.setAutoFormatting(false);
    xmlWrite.writeStartDocument();
    xmlWrite.writeStartElement("Pc");
    xmlWrite.writeStartElement("Data");
    xmlWrite.writeStartElement("SetPos");
    xmlWrite.writeAttribute("X",QString("%1").arg(robotData.X));
    xmlWrite.writeAttribute("Y",QString("%1").arg(robotData.Y));
    xmlWrite.writeAttribute("Z",QString("%1").arg(robotData.Z));
    xmlWrite.writeAttribute("A",QString("%1").arg(robotData.A));
    xmlWrite.writeAttribute("B",QString("%1").arg(robotData.B));
    xmlWrite.writeAttribute("C",QString("%1").arg(robotData.C));
    xmlWrite.writeCharacters("");
    xmlWrite.writeEndElement();
    xmlWrite.writeEndElement();
    xmlWrite.writeEndElement();
    xmlWrite.writeEndDocument();
    outcommingBytes_ = xmlByte;

//    QString xml;
//    xml+="<Pc>";
//    xml+="<Data>";
//    xml+=QString("<SetPos X=\"%1\" Y=\"%2\" Z=\"%3\" A=\"%4\" B=\"%5\" C=\"%6\" >")
//            .arg(robotData.X)
//            .arg(robotData.Y)
//            .arg(robotData.Z)
//            .arg(robotData.A)
//            .arg(robotData.B)
//            .arg(robotData.C);
//    xml+="</SetPos>";
//    xml+="</Data>";
//    xml+="</Pc>";

//    outcommingBytes_ = QByteArray(xml.toUtf8());

//    qDebug() << "XML" << xmlByte;
}
