#include "videoprocessor.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"

VideoProcessor::VideoProcessor(QObject *parent) : QObject(parent)
{

}

void VideoProcessor::startVideo() {

    tennis = cv::CascadeClassifier("../project3/tennisball2.xml");
    if(!tennis.load("../project3/tennisball2.xml")) {
        std::cerr << "Error loading xml" << std::endl;
        return;
    }


    minThresh = 120;
    maxThresh = 255;
    cv::VideoCapture camera("../project3/videos/Sample.mp4");
    cv::Mat inFrame, outFrame, grayFrame, threshFrame, outCan, maskCopy, copycopy;
    stopped = false;
    camera.set(cv::CAP_PROP_FPS, 30);

    while(camera.isOpened() && !stopped) {
        camera >> inFrame;
        if(inFrame.empty())
            continue;

        cv::resize(inFrame, inFrame, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
        outFrame = inFrame.clone();
        grayFrame = inFrame.clone();
        outCan = inFrame.clone();
        maskCopy = inFrame.clone();
        copycopy = inFrame.clone();
        //Adjust outFrame here instead of clone


        //Masking Code
        cv::Mat mask = cv::Mat::zeros(outCan.size(), outCan.type());
        cv::rectangle(copycopy, cv::Point(0,480), cv::Point(640,350), cv::Scalar(0,0,0),cv::FILLED);
        cv::rectangle(copycopy, cv::Point(0,0), cv::Point(640,100), cv::Scalar(0,0,0),cv::FILLED);
        outFrame.copyTo(outCan,mask);

        //End

        cv::cvtColor(copycopy, grayFrame, cv::COLOR_RGB2GRAY);
        cv::threshold(grayFrame, threshFrame, minThresh, maxThresh, thresholdType);

        std::vector<cv::Vec4i> hierarchy;
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(threshFrame,contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        cv::drawContours(outCan,contours,-1,cv::Scalar(0,255,0),2);

        std::vector<std::vector<cv::Point>> poly(contours.size());
        std::vector<cv::Rect> boundRect(contours.size());
        std::vector<cv::Point2f>centers(contours.size());
        std::vector<float>radius(contours.size());


        std::vector<cv::Rect> ball;

        tennis.detectMultiScale(outCan, ball, 1.025, 3, 0, cv::Size(60,60));

        // Detecting object with classifier
        for(size_t i = 0;i < ball.size();++i) {
            cv::Point p1(ball[i].x, ball[i].y);
            cv::Point p2(ball[i].x + ball[i].width, ball[i].y + ball[i].height);
            cv::rectangle(outCan, p1, p2, cv::Scalar(0,255,0), 0, 0 ,0);
            cv::putText(outCan, "TENNIS BALL", p1, cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,255,0), 2, false);
        }

        for(size_t i = 0;i < contours.size();++i) {
                approxPolyDP(contours[i], poly[i],3,true);
                boundRect[i] = boundingRect(poly[i]);
                cv::minEnclosingCircle(poly[i],centers[i],radius[i]);
        }

        cv::RNG rng(12345);
        for(size_t i = 0;i < contours.size();++i) {
            cv::Scalar color = cv::Scalar(rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256));
            cv::drawContours(outCan, poly, (int)i, color);
            cv::rectangle(outCan, boundRect[i].tl(), boundRect[i].br(),color,2);

            cv::circle(outCan,centers[i],(int)radius[i], color,2);
        }


        QImage faceImage = QImage(outFrame.data,
                                  outFrame.cols,
                                  outFrame.rows,
                                  outFrame.step,
                                  QImage::Format_RGB888)
                                    .rgbSwapped();

        QImage grayImage = QImage(threshFrame.data,
                                  threshFrame.cols,
                                  threshFrame.rows,
                                  threshFrame.step,
                                  QImage::Format_Grayscale8);

        QImage contImage = QImage(outCan.data,
                                  outCan.cols,
                                  outCan.rows,
                                  outCan.step,
                                  QImage::Format_RGB888).rgbSwapped();


        emit outDisplay(QPixmap::fromImage(faceImage));
        emit grayDisplay(QPixmap::fromImage(grayImage));
        emit contDisplay(QPixmap::fromImage(contImage));
    }
}

void VideoProcessor::stopVideo() {
    stopped = true;
}

void VideoProcessor::setThreshType(int index) {
    qDebug() << "changing threshold";
    if (index == 0) {
        thresholdType = cv::THRESH_BINARY;
    }
    else if (index == 1) {
        thresholdType = cv::THRESH_BINARY_INV;
    }
    else if (index == 2) {
        thresholdType = cv::THRESH_TRUNC;
    }
    else if (index == 3) {
        thresholdType = cv::THRESH_TOZERO;
    }
    else if (index == 4) {
        thresholdType = cv::THRESH_TOZERO_INV;
    }
}

void VideoProcessor::setThreshVal(int value) {
    minThresh = value;
}

void VideoProcessor::setMaxVal(int value) {
    maxThresh = value;
}

