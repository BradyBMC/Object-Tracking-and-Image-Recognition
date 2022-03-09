#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the slider labels
    ui->ThreshValLabel->setText(QString("120"));
    ui->MaxValLabel->setText(QString("255"));

    // Add options for combobox
    ui->ThresholdTypeComboBox->addItem(QString("Binary"));
    ui->ThresholdTypeComboBox->addItem(QString("Binary Inverted"));
    ui->ThresholdTypeComboBox->addItem(QString("Truncated"));
    ui->ThresholdTypeComboBox->addItem(QString("Threshold to Zero"));
    ui->ThresholdTypeComboBox->addItem(QString("Threshold to Zero Inverted"));

    // set up the sliders
    ui->ThresholdValueSlider->setMinimum(0);
    ui->ThresholdValueSlider->setMaximum(255);
    ui->MaxValSlider->setMinimum(0);
    ui->MaxValSlider->setMaximum(255);

    processor = new VideoProcessor();
    processor->moveToThread(new QThread(this));

    // catch signal that the user has chosen from the drop down

    connect(ui->ThresholdTypeComboBox, SIGNAL(activated(int)), this, SLOT(changeThresh(int)));

    // catch signals that the user has moved the slider

    connect(ui->ThresholdValueSlider, SIGNAL(sliderReleased()), this, SLOT(changeThreshVal()));

    connect(ui->MaxValSlider, SIGNAL(sliderReleased()), this, SLOT(changeMaxVal()));

    connect(processor->thread(), &QThread::started, processor, &VideoProcessor::startVideo);

    connect(processor->thread(), &QThread::finished, processor, &QThread::deleteLater);

    connect(processor, &VideoProcessor::outDisplay, ui->outImageLabel, &QLabel::setPixmap);

    connect(processor, &VideoProcessor::grayDisplay, ui->outGrayLabel, &QLabel::setPixmap);

    connect(processor, &VideoProcessor::contDisplay, ui->outContourLabel, &QLabel::setPixmap);

    processor->thread()->start();
}

MainWindow::~MainWindow()
{
    processor->stopVideo();
    processor->thread()->quit();
    processor->thread()->wait();
    delete ui;
}

void MainWindow::changeThresh(int index) {
    // change the threshold type
    processor->setThreshType(index);
}

void MainWindow::changeThreshVal() {
    processor->setThreshVal(ui->ThresholdValueSlider->value());
    ui->ThreshValLabel->setText(QString::number(ui->ThresholdValueSlider->value()));
}

void MainWindow::changeMaxVal() {
    processor->setMaxVal(ui->MaxValSlider->value());
    ui->MaxValLabel->setText(QString::number(ui->MaxValSlider->value()));
}

