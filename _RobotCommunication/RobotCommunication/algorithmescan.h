#ifndef ALGORITHMESCAN_H
#define ALGORITHMESCAN_H
#include <QObject>
#include "subject.h"
#include "observer.h"
#include "robotdata.h"

class AlgorithmeScan :
        public QObject,
        public Observer,
        public Subject,
        public AbstractAlgorithm1
{
    Q_OBJECT
public:
    AlgorithmeScan(QObject *parent = 0);
    ~AlgorithmeScan();

    void onNotify(Event *event);

    bool isStarted() const;

private slots:

private:
    double executed()const;
    void setIsStarted(bool isStarted);
    void startAlgorithm(EventAlgorithm1Start *event);
    void stopAlgorithm(EventAlgorithm1Stop *event);
    void stopAlg();
    void config();
    void connection();
    void releaseResource();
    void nextPoint();
    void prepareAlgorithm(double x,
                          double y,
                          double z,
                          double step);

    bool robotIsOn_;
    bool isStarted_;

    void clearData();

    double x_;
    double y_;
    double z_;
    double step_;
    double posX_;
    double posY_;
    double posZ_;
    int dirX_;
    int dirY_;
    int dirZ_;
};

#endif // ALGORITHMESCAN_H
