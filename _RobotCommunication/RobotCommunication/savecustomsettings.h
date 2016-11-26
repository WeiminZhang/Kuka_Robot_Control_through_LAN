#ifndef SAVECUSTOM_H
#define SAVECUSTOM_H
#include <QObject>
#include <QSettings>
#include <QHostAddress>

class SaveCustomSettings:
        public QObject
{
    Q_OBJECT
public:
    SaveCustomSettings(QObject *parent = 0);

    void saveServerIpAddressPort(const QHostAddress &hostAddress,
                                 quint16 port);
    void getServerIpAddressPort(QHostAddress &hostAddress,
                                quint16 &port);

private:
    void initialize();
    bool isInitilized() const;
    QSettings *settings_;
};

#endif // SAVECUSTOM_H
