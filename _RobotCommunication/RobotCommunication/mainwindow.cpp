#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messagebox.h"
#include <QDoubleValidator>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    moduleNetwork_(NULL),
    stateNetwork_(NULL),
    xmlParsing_(NULL),
    saveSettings_(NULL),
    algo_(NULL)
{
    ui->setupUi(this);

    configGUI();
    connection();
    configArchitecture();
}

MainWindow::~MainWindow()
{
    qDebug() << "DESTRUCTOR MainWindow";
    releaseResource();
    delete ui;
}

void MainWindow::onNotify(Event *event)
{
    qDebug() << "EVENT";
    enableMotionButton(false);
    if(event)
    {
        EventNetworkReceivedData *eventReceivedData
                = dynamic_cast<EventNetworkReceivedData*>(event);

        if(eventReceivedData)
        {
            if(ui->checkBoxShowRxData->isChecked())
            {
                outputTimeStamp();
                outputLog(eventReceivedData->byte().data());
            }
        }
        else
        {
            EventAlgorithm1Status *eventStatus
                    = dynamic_cast<EventAlgorithm1Status*>(event);
            if(eventStatus)
            {
                ui->pushButtonGoScan->setEnabled(!eventStatus->isRun());
                ui->pushButtonStopScan->setEnabled(eventStatus->isRun());
                outputTimeStamp();
                outputLog(QString("ALG : X %1 | Y %2 | Z %3 = exec %4 isRun = %5")
                          .arg(eventStatus->x())
                          .arg(eventStatus->y())
                          .arg(eventStatus->z())
                          .arg(eventStatus->executed())
                          .arg(eventStatus->isRun()?"TRUE":"FALSE"));
            }
            else
            {
                EventRobotDataExtractData *eventRobotDataExtractData
                        = dynamic_cast<EventRobotDataExtractData*>(event);
                if(eventRobotDataExtractData)
                {
                    outputRobotData(eventRobotDataExtractData->robotData());
                    if(ui->checkBoxShowRxData)
                    {
                        outputTimeStamp();
                        outputLogRobotData(eventRobotDataExtractData->robotData());
                        enableMotionButton(eventRobotDataExtractData->robotData().isValid);
                    }
                }
                else
                {
                    EventNetworkStateServer *eventStateServer
                            = dynamic_cast<EventNetworkStateServer*>(event);
                    if(eventStateServer)
                    {
                        //                    qDebug() <<"EVENT SERVER";
                        //                    qDebug() << eventStateServer->stateNetwork()
                        //                                ->isStarted();
                        //                    qDebug() << eventStateServer->stateNetwork()
                        //                                ->hostaddressServer();
                        //                    qDebug() << eventStateServer->stateNetwork()
                        //                                ->hostAddressClient();
                        stateNetwork_ = eventStateServer->stateNetwork();

                        if(eventStateServer->stateNetwork()->isStarted())
                        {
                            outputTimeStamp();
                            outputLog("SERVER STARTED");
                            outputLog(QString("IP %1 : PORT %2")
                                      .arg(eventStateServer->stateNetwork()
                                           ->hostaddressServer().toString())
                                      .arg(eventStateServer->stateNetwork()
                                           ->portServer()));
                        }
                        else
                        {
                            outputTimeStamp();
                            outputLog("SERVER STOPED");
                            //stopscan
                            stopScan();
                        }
                    }
                    else
                    {
                        //                    qDebug() << "TRY";
                        EventNetworkStateClient *eventStateClient
                                = dynamic_cast<EventNetworkStateClient*>(event);
                        if(eventStateClient)
                        {
                            //                        qDebug() <<"EVENT CLIENT";
                            //                        qDebug() << eventStateClient->stateNetwork()
                            //                                    ->isStarted();
                            //                        qDebug() << eventStateClient->stateNetwork()
                            //                                    ->hostaddressServer();
                            //                        qDebug() << eventStateClient->stateNetwork()
                            //                                    ->hostAddressClient();

                            stateNetwork_ = eventStateClient->stateNetwork();

                            if(eventStateClient->stateNetwork()->isStarted())
                            {
                                outputTimeStamp();
                                outputLog("*** NEW CONNECTION ***");
                                outputLog(QString("IP %1 : PORT %2")
                                          .arg(eventStateClient->stateNetwork()
                                               ->hostAddressClient().toString())
                                          .arg(eventStateClient->stateNetwork()
                                               ->portClient()));
                                //stopscan
                                stopScan();
                            }
                            else
                            {
                                outputTimeStamp();
                                outputLog("SERVER STOPED");
                                //stopscan
                                stopScan();
                            }

                        }
                        else
                        {
                            EventNetworkStateError *eventStateError
                                    = dynamic_cast<EventNetworkStateError*>(event);
                            if(eventStateError)
                            {
                                //                            qDebug() <<"EVENT ERROR";
                                //                            qDebug() << eventStateError->stateNetwork()
                                //                                        ->isStarted();
                                //                            qDebug() << eventStateError->stateNetwork()
                                //                                        ->hostaddressServer();
                                //                            qDebug() << eventStateError->stateNetwork()
                                //                                        ->hostAddressClient();

                                stateNetwork_ = eventStateError->stateNetwork();

                                if(eventStateError->stateNetwork()->isStarted())
                                {
                                    outputTimeStamp();
                                    if(!eventStateError
                                            ->stateNetwork()->statusError()
                                            == QAbstractSocket::RemoteHostClosedError)
                                        outputLog("!!! ERROR !!!");
                                    outputLog(eventStateError
                                              ->stateNetwork()->errorString());
                                    //stopscan
                                    stopScan();
                                }
                                else
                                {
                                    outputTimeStamp();
                                    outputLog("SERVER STOPED");
                                    //stopscan
                                    stopScan();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    updateStatusConnection();
}

void MainWindow::startServer()
{
    try
    {
        notify(new EventNetworkStartServer(HostAddressConfig
                                           (QHostAddress(ui->lineEditAddres->text()),
                                            ui->lineEditPort->text().toInt())));
    }
    catch(QString &error)
    {
        MessageBox("WARNING", error, this);
    }

}

void MainWindow::stopServer()
{
    notify(new EventNetworkStopServer());
}

void MainWindow::outputLog(const QString &text)
{
    ui->plainTextEditLog->appendPlainText(text);
}

void MainWindow::outputTimeStamp()
{
    ui->plainTextEditLog->appendPlainText("----------------");
    ui->plainTextEditLog
            ->appendPlainText(
                "TIME : "
                + QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
                + " | DATA : "
                + QDateTime::currentDateTime().toString("dd.MM.yyyy"));
}

void MainWindow::outputLogRobotData(RobotData data)
{
    ui->plainTextEditLog
            ->appendPlainText(QString("Robot pos : X %1 | Y %2 | Z %3| "
                                      "A %4 | B %5 | C %6")
                              .arg(data.X)
                              .arg(data.Y)
                              .arg(data.Z)
                              .arg(data.A)
                              .arg(data.B)
                              .arg(data.C));
}

void MainWindow::enableMotionButton(bool isEnable)
{
    ui->pushButtonXn->setEnabled(isEnable);
    ui->pushButtonXp->setEnabled(isEnable);
    ui->pushButtonYn->setEnabled(isEnable);
    ui->pushButtonYp->setEnabled(isEnable);
    ui->pushButtonZn->setEnabled(isEnable);
    ui->pushButtonZp->setEnabled(isEnable);
    ui->pushButtonAn->setEnabled(isEnable);
    ui->pushButtonAp->setEnabled(isEnable);
    ui->pushButtonBn->setEnabled(isEnable);
    ui->pushButtonBp->setEnabled(isEnable);
    ui->pushButtonCn->setEnabled(isEnable);
    ui->pushButtonCp->setEnabled(isEnable);
    ui->pushButtonSetPos->setEnabled(isEnable);
}

void MainWindow::buttonConnect()
{
    if(stateNetwork_ && stateNetwork_->isStarted())
    {
        stopServer();
    }
    else
    {
        startServer();
    }
}

void MainWindow::moveXn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelX->text().toDouble()*(-1);
    robotData.X = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveXp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelX->text().toDouble();
    robotData.X = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveYn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelY->text().toDouble()*(-1);
    robotData.Y = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveYp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelY->text().toDouble()*(1);
    robotData.Y = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveZn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelZ->text().toDouble()*(-1);
    robotData.Z = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveZp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelZ->text().toDouble()*(1);
    robotData.Z = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveAn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelA->text().toDouble()*(-1);
    robotData.A = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveAp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelA->text().toDouble()*(1);
    robotData.A = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveBn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelB->text().toDouble()*(-1);
    robotData.B = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveBp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelB->text().toDouble()*(1);
    robotData.B = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveCn()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelC->text().toDouble()*(-1);
    robotData.C = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::moveCp()
{
    RobotData robotData{};
    robotData.isValid = true;
    double del = ui->lineEditDelC->text().toDouble()*(1);
    robotData.C = del;
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::setPosRobot()
{
    RobotData robotData{};
    robotData.isValid = true;
    robotData.X = ui->lineEditSetX->text().toDouble();
    robotData.Y = ui->lineEditSetY->text().toDouble();
    robotData.Z = ui->lineEditSetZ->text().toDouble();
    robotData.A = ui->lineEditSetA->text().toDouble();
    robotData.B = ui->lineEditSetB->text().toDouble();
    robotData.C = ui->lineEditSetC->text().toDouble();
    notify2(new EventRobotDataEmbedData(robotData),false);
}

void MainWindow::gotoScan()
{
    qDebug() << "START SCAN";
    notify(new EventAlgorithm1Start(
               ui->lineEditLenX->text().toDouble(),
               ui->lineEditLenY->text().toDouble(),
               ui->lineEditLenZ->text().toDouble(),
               ui->lineEditLenStep->text().toDouble()));
}

void MainWindow::stopScan()
{
    qDebug() << "STOP SCAN";
    notify(new EventAlgorithm1Stop());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    qDebug() << "to close app";
    saveSettings_->saveServerIpAddressPort(QHostAddress(ui->lineEditAddres->text()),
                                           ui->lineEditPort->text().toInt());
    e->accept();
}

void MainWindow::notify2(Event *event, bool isEnable)
{
    enableMotionButton(isEnable);
    notify(event);
}

void MainWindow::configGUI()
{
    setWindowTitle("Test Communication App for the"
                   " KUKA's robot application");

    //instance custom settings
    saveSettings_ = new SaveCustomSettings(this);

    //config log output
    ui->plainTextEditLog->setMaximumBlockCount(0xFFFF);

    //prepare info status
    statusConnection_ = new QLabel("NOT CONNECTED");
    ui->statusBar->addWidget(statusConnection_);

    statusClinetConnection_ = new QLabel("NO CLIENT");
    ui->statusBar->addWidget(statusClinetConnection_);

    statusErrorClient_ = new QLabel("---");
    ui->statusBar->addWidget(statusErrorClient_);

    //organize tha validator
    //for the input ip number
    ui->lineEditAddres->setInputMask("000.000.000.000");
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator =
            new QRegExpValidator(ipRegex, this);
    //apply validator
    ui->lineEditAddres->setValidator(ipValidator);
    ui->lineEditPort->setValidator(new QIntValidator(1, 65535, this));

    //getting saved settings
    QHostAddress hostServerAddress;
    quint16 port = 0;
    saveSettings_->getServerIpAddressPort(hostServerAddress,port);

    //set by default local address
    ui->lineEditAddres->setText(hostServerAddress.toString());
    //set by default port 1234
    setPortNumber(port);

    updateStatusConnection();

    outputRobotData(RobotData{});
    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    doubleValidator->setDecimals(2);

    ui->lineEditDelX->setValidator(doubleValidator);
    ui->lineEditDelX->setText("1");
    ui->lineEditDelY->setValidator(doubleValidator);
    ui->lineEditDelY->setText("1");
    ui->lineEditDelZ->setValidator(doubleValidator);
    ui->lineEditDelZ->setText("1");
    ui->lineEditDelA->setValidator(doubleValidator);
    ui->lineEditDelA->setText("1");
    ui->lineEditDelB->setValidator(doubleValidator);
    ui->lineEditDelB->setText("1");
    ui->lineEditDelC->setValidator(doubleValidator);
    ui->lineEditDelC->setText("1");

    ui->lineEditSetX->setValidator(doubleValidator);
    ui->lineEditSetX->setText("1");
    ui->lineEditSetY->setValidator(doubleValidator);
    ui->lineEditSetY->setText("1");
    ui->lineEditSetZ->setValidator(doubleValidator);
    ui->lineEditSetZ->setText("1");
    ui->lineEditSetA->setValidator(doubleValidator);
    ui->lineEditSetA->setText("1");
    ui->lineEditSetB->setValidator(doubleValidator);
    ui->lineEditSetB->setText("1");
    ui->lineEditSetC->setValidator(doubleValidator);
    ui->lineEditSetC->setText("1");

    QDoubleValidator *doubleValidator2 = new QDoubleValidator(this);
    doubleValidator2->setDecimals(2);
    doubleValidator2->setBottom(1);

    ui->lineEditLenX->setValidator(doubleValidator2);
    ui->lineEditLenX->setText("1");
    ui->lineEditLenY->setValidator(doubleValidator2);
    ui->lineEditLenY->setText("1");
    ui->lineEditLenZ->setValidator(doubleValidator2);
    ui->lineEditLenZ->setText("1");
    ui->lineEditLenStep->setText("1");
    ui->lineEditLenStep->setValidator(doubleValidator2);

    enableMotionButton(false);
}

void MainWindow::connection()
{
    connect(ui->pushButtonConnectServer, SIGNAL(clicked()),
            this, SLOT(buttonConnect()));

    connect(ui->pushButtonXn, SIGNAL(clicked(bool)),
            this, SLOT(moveXn()));
    connect(ui->pushButtonXp, SIGNAL(clicked(bool)),
            this, SLOT(moveXp()));

    connect(ui->pushButtonYn, SIGNAL(clicked(bool)),
            this, SLOT(moveYn()));
    connect(ui->pushButtonYp, SIGNAL(clicked(bool)),
            this, SLOT(moveYp()));

    connect(ui->pushButtonZn, SIGNAL(clicked(bool)),
            this, SLOT(moveZn()));
    connect(ui->pushButtonZp, SIGNAL(clicked(bool)),
            this, SLOT(moveZp()));

    connect(ui->pushButtonAn, SIGNAL(clicked(bool)),
            this, SLOT(moveAn()));
    connect(ui->pushButtonAp, SIGNAL(clicked(bool)),
            this, SLOT(moveAp()));

    connect(ui->pushButtonBn, SIGNAL(clicked(bool)),
            this, SLOT(moveBn()));
    connect(ui->pushButtonBp, SIGNAL(clicked(bool)),
            this, SLOT(moveBp()));

    connect(ui->pushButtonCn, SIGNAL(clicked(bool)),
            this, SLOT(moveCn()));
    connect(ui->pushButtonCp, SIGNAL(clicked(bool)),
            this, SLOT(moveCp()));

    connect(ui->pushButtonSetPos, SIGNAL(clicked(bool)),
            this, SLOT(setPosRobot()));

    connect(ui->pushButtonGoScan, SIGNAL(clicked(bool)),
            this, SLOT(gotoScan()));

    connect(ui->pushButtonStopScan, SIGNAL(clicked(bool)),
            this, SLOT(stopScan()));

}

void MainWindow::releaseResource()
{

}

void MainWindow::configArchitecture()
{
    //instance network module
    moduleNetwork_ = new ModuleNetwork(this);

    //connection between modules
    this->addObserver(moduleNetwork_);
    moduleNetwork_->addObserver(this);

    //instance xmlparsing module
    xmlParsing_ = new XMLparsing(this);
    xmlParsing_->addObserver(moduleNetwork_);
    xmlParsing_->addObserver(this);
    this->addObserver(xmlParsing_);
    moduleNetwork_->addObserver(xmlParsing_);

    //instance algorithme
    algo_ = new AlgorithmeScan(this);
    algo_->addObserver(this);
    algo_->addObserver(xmlParsing_);
    xmlParsing_->addObserver(algo_);
    this->addObserver(algo_);
}

void MainWindow::outputRobotData(RobotData robotData)
{
    ui->labelPosX->setText(QString("%1").arg(robotData.X));
    ui->labelPosY->setText(QString("%1").arg(robotData.Y));
    ui->labelPosZ->setText(QString("%1").arg(robotData.Z));

    ui->labelAngA->setText(QString("%1").arg(robotData.A));
    ui->labelAngB->setText(QString("%1").arg(robotData.B));
    ui->labelAngC->setText(QString("%1").arg(robotData.C));
}

void MainWindow::setPortNumber(quint16 port)
{
    ui->lineEditPort->setText(QString::number(port));
}

void MainWindow::updateStatusConnection()
{
    if(stateNetwork_)
    {
        statusConnection_
                ->setText(stateNetwork_->isStarted()?
                              "CONNECTED":"NOT CONNECTED");

        statusClinetConnection_->setText(stateNetwork_->hostAddressClient().isNull()?
                                             "NO CLIENT":
                                             stateNetwork_->hostAddressClient().toString()
                                             + " : " + QString("%1").arg(stateNetwork_->portClient()));


        statusErrorClient_->setText(stateNetwork_->errorString());

        ui->pushButtonConnectServer
                ->setText(stateNetwork_->isStarted()?
                              "Stop Server":"Run Server");
    }
}
