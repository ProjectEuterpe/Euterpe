/********************************************************************************
** Form generated from reading UI file 'Player.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYER_H
#define UI_PLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Player
{
public:
    QGridLayout *gridLayout;
    QGroupBox *group_lib;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *group_libTitle;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QPushButton *open;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGroupBox *group_play;
    QVBoxLayout *verticalLayout;
    QVideoWidget *video_widget;
    QGroupBox *control_pad;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *progress_slider;
    QCheckBox *loop;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *time_label;
    QSpacerItem *spacer1;
    QPushButton *btn_play_order;
    QCheckBox *rewind;
    QPushButton *prev;
    QPushButton *play;
    QPushButton *next;
    QPushButton *stop;
    QPushButton *info;
    QSpacerItem *spacer2;
    QPushButton *btn_volume;
    QSlider *volume_slider;
    QComboBox *rate;

    void setupUi(QWidget *Player)
    {
        if (Player->objectName().isEmpty())
            Player->setObjectName(QString::fromUtf8("Player"));
        Player->resize(950, 650);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Player->sizePolicy().hasHeightForWidth());
        Player->setSizePolicy(sizePolicy);
        Player->setMinimumSize(QSize(800, 550));
        Player->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(Player);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        group_lib = new QGroupBox(Player);
        group_lib->setObjectName(QString::fromUtf8("group_lib"));
        group_lib->setMinimumSize(QSize(250, 0));
        group_lib->setMaximumSize(QSize(250, 16777215));
        group_lib->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_3 = new QVBoxLayout(group_lib);
        verticalLayout_3->setSpacing(7);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, 11, 11);
        group_libTitle = new QGroupBox(group_lib);
        group_libTitle->setObjectName(QString::fromUtf8("group_libTitle"));
        horizontalLayout_3 = new QHBoxLayout(group_libTitle);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        label = new QLabel(group_libTitle);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label);

        open = new QPushButton(group_libTitle);
        open->setObjectName(QString::fromUtf8("open"));
        open->setMaximumSize(QSize(28, 28));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/file-import.svg"), QSize(), QIcon::Normal, QIcon::Off);
        open->setIcon(icon);
        open->setIconSize(QSize(18, 18));

        horizontalLayout_3->addWidget(open);


        verticalLayout_3->addWidget(group_libTitle);

        scrollArea = new QScrollArea(group_lib);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 224, 561));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);


        gridLayout->addWidget(group_lib, 0, 0, 1, 1);

        group_play = new QGroupBox(Player);
        group_play->setObjectName(QString::fromUtf8("group_play"));
        group_play->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(group_play);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        video_widget = new QVideoWidget(group_play);
        video_widget->setObjectName(QString::fromUtf8("video_widget"));

        verticalLayout->addWidget(video_widget);

        control_pad = new QGroupBox(group_play);
        control_pad->setObjectName(QString::fromUtf8("control_pad"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(control_pad->sizePolicy().hasHeightForWidth());
        control_pad->setSizePolicy(sizePolicy1);
        control_pad->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(control_pad);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, -1, 1, -1);
        frame_2 = new QFrame(control_pad);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        progress_slider = new QSlider(frame_2);
        progress_slider->setObjectName(QString::fromUtf8("progress_slider"));
        progress_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(progress_slider);

        loop = new QCheckBox(frame_2);
        loop->setObjectName(QString::fromUtf8("loop"));
        loop->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(loop->sizePolicy().hasHeightForWidth());
        loop->setSizePolicy(sizePolicy2);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/play-onlyOnce.svg"), QSize(), QIcon::Normal, QIcon::Off);
        loop->setIcon(icon1);

        horizontalLayout_2->addWidget(loop);


        verticalLayout_2->addWidget(frame_2);

        frame = new QFrame(control_pad);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        time_label = new QLabel(frame);
        time_label->setObjectName(QString::fromUtf8("time_label"));
        time_label->setMinimumSize(QSize(80, 0));
        time_label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(time_label);

        spacer1 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacer1);

        btn_play_order = new QPushButton(frame);
        btn_play_order->setObjectName(QString::fromUtf8("btn_play_order"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btn_play_order->sizePolicy().hasHeightForWidth());
        btn_play_order->setSizePolicy(sizePolicy3);
        btn_play_order->setMaximumSize(QSize(26, 26));
        btn_play_order->setLayoutDirection(Qt::LeftToRight);
        btn_play_order->setAutoFillBackground(false);
        btn_play_order->setIcon(icon1);
        btn_play_order->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(btn_play_order);

        rewind = new QCheckBox(frame);
        rewind->setObjectName(QString::fromUtf8("rewind"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/rewind-play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        rewind->setIcon(icon2);

        horizontalLayout->addWidget(rewind);

        prev = new QPushButton(frame);
        prev->setObjectName(QString::fromUtf8("prev"));
        prev->setEnabled(false);
        prev->setMinimumSize(QSize(40, 40));
        prev->setMaximumSize(QSize(40, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/step-previous.svg"), QSize(), QIcon::Normal, QIcon::Off);
        prev->setIcon(icon3);
        prev->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(prev);

        play = new QPushButton(frame);
        play->setObjectName(QString::fromUtf8("play"));
        play->setEnabled(false);
        sizePolicy3.setHeightForWidth(play->sizePolicy().hasHeightForWidth());
        play->setSizePolicy(sizePolicy3);
        play->setMinimumSize(QSize(40, 40));
        play->setMaximumSize(QSize(40, 40));
        QIcon icon4;
        QString iconThemeName = QString::fromUtf8("playAndpause");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8(":/images/circle-play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        }
        play->setIcon(icon4);
        play->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(play);

        next = new QPushButton(frame);
        next->setObjectName(QString::fromUtf8("next"));
        next->setEnabled(false);
        next->setMinimumSize(QSize(40, 40));
        next->setMaximumSize(QSize(40, 40));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/step-next.svg"), QSize(), QIcon::Normal, QIcon::Off);
        next->setIcon(icon5);
        next->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(next);

        stop = new QPushButton(frame);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setEnabled(false);

        horizontalLayout->addWidget(stop);

        info = new QPushButton(frame);
        info->setObjectName(QString::fromUtf8("info"));
        sizePolicy1.setHeightForWidth(info->sizePolicy().hasHeightForWidth());
        info->setSizePolicy(sizePolicy1);
        info->setMinimumSize(QSize(0, 0));
        info->setMaximumSize(QSize(26, 26));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/media-info.svg"), QSize(), QIcon::Normal, QIcon::Off);
        info->setIcon(icon6);
        info->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(info);

        spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacer2);

        btn_volume = new QPushButton(frame);
        btn_volume->setObjectName(QString::fromUtf8("btn_volume"));
        btn_volume->setMinimumSize(QSize(26, 26));
        btn_volume->setMaximumSize(QSize(26, 26));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/volume-open.svg"), QSize(), QIcon::Normal, QIcon::Off);
        btn_volume->setIcon(icon7);
        btn_volume->setIconSize(QSize(18, 18));

        horizontalLayout->addWidget(btn_volume);

        volume_slider = new QSlider(frame);
        volume_slider->setObjectName(QString::fromUtf8("volume_slider"));
        sizePolicy2.setHeightForWidth(volume_slider->sizePolicy().hasHeightForWidth());
        volume_slider->setSizePolicy(sizePolicy2);
        volume_slider->setMaximum(1000);
        volume_slider->setValue(1000);
        volume_slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(volume_slider);

        rate = new QComboBox(frame);
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->setObjectName(QString::fromUtf8("rate"));
        sizePolicy1.setHeightForWidth(rate->sizePolicy().hasHeightForWidth());
        rate->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(rate);


        verticalLayout_2->addWidget(frame);


        verticalLayout->addWidget(control_pad);


        gridLayout->addWidget(group_play, 0, 1, 1, 1);


        retranslateUi(Player);

        rate->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Player);
    } // setupUi

    void retranslateUi(QWidget *Player)
    {
        Player->setWindowTitle(QCoreApplication::translate("Player", "Euterpe", nullptr));
        group_libTitle->setTitle(QString());
        label->setText(QCoreApplication::translate("Player", "\345\252\222\344\275\223\345\272\223", nullptr));
        open->setText(QString());
        control_pad->setTitle(QString());
        loop->setText(QString());
        time_label->setText(QCoreApplication::translate("Player", "--.-- / --.--", nullptr));
        btn_play_order->setText(QString());
        rewind->setText(QString());
        prev->setText(QString());
        play->setText(QString());
        next->setText(QString());
        stop->setText(QCoreApplication::translate("Player", "STOP", nullptr));
        info->setText(QString());
        btn_volume->setText(QString());
        rate->setItemText(0, QCoreApplication::translate("Player", "0.25x", nullptr));
        rate->setItemText(1, QCoreApplication::translate("Player", "0.5x", nullptr));
        rate->setItemText(2, QCoreApplication::translate("Player", "1x", nullptr));
        rate->setItemText(3, QCoreApplication::translate("Player", "1.5x", nullptr));
        rate->setItemText(4, QCoreApplication::translate("Player", "2x", nullptr));

        rate->setCurrentText(QCoreApplication::translate("Player", "1x", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Player: public Ui_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_H
