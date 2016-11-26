#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include <QDateTime>
#include "subject.h"
#include "observer.h"
#include "modulenetwork.h"
#include "statenetwork.h"
#include "savecustomsettings.h"
#include "xmlparsing.h"
#include "algorithmescan.h"

namespace Ui {
class MainWindow;
}

class MainWindow :
        public QMainWindow,
        public Observer,
        public Subject
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onNotify(Event *event);

private slots:
    void startServer();
    void stopServer();
    void outputLog(const QString &text);
    void outputTimeStamp();
    void outputLogRobotData(RobotData data);

    void enableMotionButton(bool isEnable);

    //ui
    void buttonConnect();

    //motion
    //need to be refactored this code!!!!
    void moveXn();
    void moveXp();
    void moveYn();
    void moveYp();
    void moveZn();
    void moveZp();
    void moveAn();
    void moveAp();
    void moveBn();
    void moveBp();
    void moveCn();
    void moveCp();

    void setPosRobot();

    //scanning
    void gotoScan();
    void stopScan();

protected:
    void closeEvent(QCloseEvent *e);

private:
    void notify2(Event *event, bool isEnable);
    //config class
    void configGUI();
    void connection();
    void releaseResource();
    //config architecture
    void configArchitecture();
    //set output data
    void outputRobotData(RobotData robotData);

    void setPortNumber(quint16 port);
    void updateStatusConnection();

    //attributes
    Ui::MainWindow *ui;
    ModuleNetwork *moduleNetwork_;
    StateNetwork *stateNetwork_;
    XMLparsing *xmlParsing_;
    SaveCustomSettings *saveSettings_;
    AlgorithmeScan *algo_;

    //status bar
    QLabel *statusConnection_;
    QLabel *statusClinetConnection_;
    QLabel *statusErrorClient_;
};

#endif // MAINWINDOW_H
