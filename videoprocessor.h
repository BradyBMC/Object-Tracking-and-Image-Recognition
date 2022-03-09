#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <opencv4/opencv2/opencv.hpp>
#include <QDebug>

class VideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit VideoProcessor(QObject *parent = nullptr);
    void setThreshType(int index);
    void setThreshVal(int value);
    void setMaxVal(int value);

signals:
    void outDisplay(QPixmap pixmap);
    void grayDisplay(QPixmap pixmap);
    void contDisplay(QPixmap pixmap);

public slots:
    void startVideo();
    void stopVideo();
private:
    cv::CascadeClassifier tennis;
    bool stopped;
    int thresholdType = cv::THRESH_BINARY;
    double minThresh;
    double maxThresh;
};

#endif // VIDEOPROCESSOR_H
