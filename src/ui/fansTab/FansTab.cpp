#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include "FansTab.h"

FansTab::FansTab(QWidget *parent) {

    QHBoxLayout *cpuSectionLayout = new QHBoxLayout;
    QHBoxLayout *gpuSectionLayout = new QHBoxLayout;

    QVector<QSlider *> *cpuSliders = new QVector<QSlider *>(8);
    for (int i = 0; i < 8; i++) {
        QWidget *sliderBox = new QWidget;
        QVBoxLayout *sliderBoxLayout = new QVBoxLayout;
        QLabel *tempValue = new QLabel(QString::number(35 + 10 * i) + tr("°C"));
        tempValue->setStyleSheet("QLabel { width : 4em }");
        QSlider *qSlider = new QSlider(Qt::Orientation::Vertical);
        cpuSliders->insert(i, qSlider);
        sliderBoxLayout->addWidget(tempValue);
        sliderBoxLayout->addWidget(qSlider);
        sliderBoxLayout->addStretch(1);
        sliderBox->setLayout(sliderBoxLayout);
        cpuSectionLayout->addWidget(sliderBox);
    }
    cpuRPM = new QLabel(/*"CPU RPM: 2000"*/);

    cpuSectionLayout->addWidget(cpuRPM);

    cpuSectionLayout->addStretch(1);

    QVector<QSlider *> *gpuSliders = new QVector<QSlider *>(8);
    for (int i = 0; i < 8; i++) {
        QWidget *sliderBox = new QWidget;
        QVBoxLayout *sliderBoxLayout = new QVBoxLayout;
        QLabel *tempValue = new QLabel(QString::number(35 + 10 * i) + tr("°C"));
        tempValue->setStyleSheet("QLabel { width : 4em }");
        QSlider *qSlider = new QSlider(Qt::Orientation::Vertical);
        gpuSliders->insert(i, qSlider);
        sliderBoxLayout->addWidget(tempValue);
        sliderBoxLayout->addWidget(qSlider);
        sliderBoxLayout->addStretch(1);
        sliderBox->setLayout(sliderBoxLayout);
        gpuSectionLayout->addWidget(sliderBox);
    }

    gpuRPM = new QLabel(/*"GPU RPM: 2000"*/);

    gpuSectionLayout->addWidget(gpuRPM);

    gpuSectionLayout->addStretch(1);

    QGroupBox *cpuSection = new QGroupBox(tr("CPU"));
    QGroupBox *gpuSection = new QGroupBox(tr("GPU"));
    cpuSection->setLayout(cpuSectionLayout);
    gpuSection->setLayout(gpuSectionLayout);

    QVBoxLayout *container = new QVBoxLayout;
    container->addWidget(cpuSection);
    container->addWidget(gpuSection);
    container->addStretch(1);

    setLayout(container);
}

void FansTab::refresh() {
    RY.refreshTable();

    cpuRPM->setText(QString::asprintf("%.2f°C, %ld RPM",
                                      RY.getCpuTemp(),
                                      AcpiControlSingleton::getInstance().getCpuFanSpeed()));
    gpuRPM->setText(QString::number(AcpiControlSingleton::getInstance().getGpuFanSpeed()) + " RPM");
}
