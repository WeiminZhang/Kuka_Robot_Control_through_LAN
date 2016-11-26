#include "algorithmescan.h"
#include <QDebug>

AlgorithmeScan::AlgorithmeScan(QObject *parent)
    : QObject(parent),
      isStarted_(false),
      robotIsOn_(false)
{
    clearData();
}

AlgorithmeScan::~AlgorithmeScan()
{

}

void AlgorithmeScan::onNotify(Event *event)
{
    if(event)
    {
        EventAlgorithm1 *eventAlg
                = dynamic_cast<EventAlgorithm1*>(event);
        if(eventAlg)
        {
            eventAlg->execute(this);
        }
        else
        {
            EventRobotDataExtractData *eventRobotDataExtractData
                    = dynamic_cast<EventRobotDataExtractData*>(event);
            if(eventRobotDataExtractData)
            {
                robotIsOn_ = eventRobotDataExtractData->robotData().isValid;
                qDebug() << "ALG " << robotIsOn_;
                if(robotIsOn_ && isStarted())
                {
                    qDebug() << "NEXT POINT ALG";
                    nextPoint();
                }
            }
        }
    }
}

void AlgorithmeScan::startAlgorithm(EventAlgorithm1Start *event)
{
    qDebug() << "START ALG"
             << event->x()
             << event->y()
             << event->z()
             << event->step();

    if(robotIsOn_)
    {
        setIsStarted(true);
        clearData();
        prepareAlgorithm(event->x(),
                         event->y(),
                         event->z(),
                         event->step());
        nextPoint();
    }

}

void AlgorithmeScan::stopAlgorithm(EventAlgorithm1Stop *event)
{
    Q_UNUSED(event)
    stopAlg();
}

void AlgorithmeScan::stopAlg()
{
    qDebug() << "STOP stopAlg";
    if(isStarted())
    {
        setIsStarted(false);
        notify(new EventAlgorithm1Status(posX_,
                                         posY_,
                                         posZ_,
                                         executed(),
                                         isStarted()));
    }
}

void AlgorithmeScan::config()
{

}

void AlgorithmeScan::connection()
{

}

void AlgorithmeScan::releaseResource()
{

}

void AlgorithmeScan::nextPoint()
{
    RobotData robotData{};


    robotData.X = step_;
    posX_ += step_;
    if(posX_ > x_)
    {
        robotData.X = -1 * (posX_-step_);
        posX_ = 0;

        robotData.Y = step_;
        posY_ += step_;
        if(posY_ > y_)
        {
            robotData.Y = -1 * (posY_-step_);
            posY_ = 0;

            robotData.Z = step_;
            posZ_ += step_;
            if(posZ_ > z_)
            {
                robotData.Z = -1 * (posZ_-step_);
                posZ_ = 0;

                stopAlg();
            }
        }
    }

    qDebug() << "NEXT " << robotData.X << robotData.Y << robotData.Z;

    notify(new EventRobotDataEmbedData(robotData));
    notify(new EventAlgorithm1Status(posX_,
                                     posY_,
                                     posZ_,
                                     executed(),
                                     isStarted()));
}

void AlgorithmeScan::prepareAlgorithm(double x,
                                      double y,
                                      double z,
                                      double step)
{
    x_ = x;
    y_ = y;
    z_ = z;
    step_ = step;
}

void AlgorithmeScan::clearData()
{
    x_ = 0;
    y_ = 0;
    z_ = 0;
    step_ = 0;

    posX_ = 0;
    posY_ = 0;
    posZ_ = 0;

    dirX_ = 0;
    dirY_ = 0;
    dirZ_ = 0;
}

bool AlgorithmeScan::isStarted() const
{
    return isStarted_;
}

double AlgorithmeScan::executed() const
{
    return (posZ_*x_*y_ + posY_*x_ + posX_)/(x_*y_*z_);
}

void AlgorithmeScan::setIsStarted(bool isStarted)
{
    isStarted_ = isStarted;
}
