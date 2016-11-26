#include "savecustomsettings.h"
#include <QApplication>
#include <QDebug>

SaveCustomSettings::SaveCustomSettings(QObject *parent)
    : QObject(parent),
      settings_(NULL)
{
    initialize();
}

void SaveCustomSettings::saveServerIpAddressPort(const QHostAddress &hostAddress,
                                                 quint16 port)
{
    if(isInitilized())
    {
        settings_->setValue("address/server/ip",hostAddress.toString());
        settings_->setValue("address/server/port", port);
    }
}

void SaveCustomSettings::getServerIpAddressPort(QHostAddress &hostAddress,
                                                quint16 &port)
{
    if(isInitilized())
    {
        QString ipAddress = settings_->value("address/server/ip",
                                             QHostAddress(QHostAddress::LocalHost)
                                             .toString()).toString();
        hostAddress = QHostAddress(ipAddress);
        port = settings_->value("address/server/port",1234).toInt();
    }
}

void SaveCustomSettings::initialize()
{
    if(qApp->organizationName().isEmpty() ||
       qApp->applicationName().isEmpty())
    {
        qDebug() << "ERORR : AUTO SAVING SETTING IS NOT INITIALIZED : qApp does not have orgnization and application names";
        return;
    }

    settings_ = new QSettings(qApp->organizationName(),
                              qApp->applicationName(),
                              this);
    qDebug() << "AUTO SAVING SETTING IS INITIALIZED";
}

bool SaveCustomSettings::isInitilized() const
{
    if(!settings_)
    {
        qDebug() << "ERORR : AUTO SAVING SETTING IS NOT INITIALIZED : qApp does not have orgnization and application names";
        return false;
    }
    return true;
}
