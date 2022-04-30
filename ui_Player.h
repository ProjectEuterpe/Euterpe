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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Player
{
public:
    QGridLayout *gridLayout;
    QFrame *group_play;
    QVBoxLayout *verticalLayout;
    QVideoWidget *video_widget;
    QGroupBox *control_pad;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *progress_slider;
    QLabel *time_label;
    QCheckBox *loop;
    QCheckBox *rewind;
    QComboBox *rate;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *open;
    QSpacerItem *spacer1;
    QPushButton *prev;
    QPushButton *play;
    QPushButton *next;
    QPushButton *stop;
    QPushButton *info;
    QSpacerItem *space2;
    QPushButton *volume_icon;
    QSlider *volume_slider;
    QFrame *group_lib;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QTableView *tableView;

    void setupUi(QWidget *Player)
    {
        if (Player->objectName().isEmpty())
            Player->setObjectName(QString::fromUtf8("Player"));
        Player->resize(950, 622);
        Player->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(Player);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        group_play = new QFrame(Player);
        group_play->setObjectName(QString::fromUtf8("group_play"));
        verticalLayout = new QVBoxLayout(group_play);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        video_widget = new QVideoWidget(group_play);
        video_widget->setObjectName(QString::fromUtf8("video_widget"));

        verticalLayout->addWidget(video_widget);

        control_pad = new QGroupBox(group_play);
        control_pad->setObjectName(QString::fromUtf8("control_pad"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(control_pad->sizePolicy().hasHeightForWidth());
        control_pad->setSizePolicy(sizePolicy);
        control_pad->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(control_pad);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_2 = new QFrame(control_pad);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        progress_slider = new QSlider(frame_2);
        progress_slider->setObjectName(QString::fromUtf8("progress_slider"));
        progress_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(progress_slider);

        time_label = new QLabel(frame_2);
        time_label->setObjectName(QString::fromUtf8("time_label"));
        time_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(time_label);

        loop = new QCheckBox(frame_2);
        loop->setObjectName(QString::fromUtf8("loop"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(loop->sizePolicy().hasHeightForWidth());
        loop->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/singleLoop-play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        loop->setIcon(icon);

        horizontalLayout_2->addWidget(loop);

        rewind = new QCheckBox(frame_2);
        rewind->setObjectName(QString::fromUtf8("rewind"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/rewind-play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        rewind->setIcon(icon1);

        horizontalLayout_2->addWidget(rewind);

        rate = new QComboBox(frame_2);
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->addItem(QString());
        rate->setObjectName(QString::fromUtf8("rate"));
        sizePolicy.setHeightForWidth(rate->sizePolicy().hasHeightForWidth());
        rate->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(rate);


        verticalLayout_2->addWidget(frame_2);

        frame = new QFrame(control_pad);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        open = new QPushButton(frame);
        open->setObjectName(QString::fromUtf8("open"));

        horizontalLayout->addWidget(open);

        spacer1 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacer1);

        prev = new QPushButton(frame);
        prev->setObjectName(QString::fromUtf8("prev"));
        prev->setEnabled(false);
        prev->setMinimumSize(QSize(40, 40));
        prev->setMaximumSize(QSize(40, 40));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/previous-step.svg"), QSize(), QIcon::Normal, QIcon::Off);
        prev->setIcon(icon2);
        prev->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(prev);

        play = new QPushButton(frame);
        play->setObjectName(QString::fromUtf8("play"));
        play->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(play->sizePolicy().hasHeightForWidth());
        play->setSizePolicy(sizePolicy3);
        play->setMinimumSize(QSize(40, 40));
        play->setMaximumSize(QSize(40, 40));
        QIcon icon3;
        QString iconThemeName = QString::fromUtf8("playAndpause");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8(":/images/circle-play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        }
        play->setIcon(icon3);
        play->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(play);

        next = new QPushButton(frame);
        next->setObjectName(QString::fromUtf8("next"));
        next->setEnabled(false);
        next->setMinimumSize(QSize(40, 40));
        next->setMaximumSize(QSize(40, 40));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/next-step.svg"), QSize(), QIcon::Normal, QIcon::Off);
        next->setIcon(icon4);
        next->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(next);

        stop = new QPushButton(frame);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setEnabled(false);

        horizontalLayout->addWidget(stop);

        info = new QPushButton(frame);
        info->setObjectName(QString::fromUtf8("info"));
        sizePolicy.setHeightForWidth(info->sizePolicy().hasHeightForWidth());
        info->setSizePolicy(sizePolicy);
        info->setMinimumSize(QSize(26, 26));
        info->setMaximumSize(QSize(26, 26));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/media-info.svg"), QSize(), QIcon::Normal, QIcon::Off);
        info->setIcon(icon5);
        info->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(info);

        space2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(space2);

        volume_icon = new QPushButton(frame);
        volume_icon->setObjectName(QString::fromUtf8("volume_icon"));
        volume_icon->setMinimumSize(QSize(26, 26));
        volume_icon->setMaximumSize(QSize(26, 26));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/volume-open.svg"), QSize(), QIcon::Normal, QIcon::Off);
        volume_icon->setIcon(icon6);
        volume_icon->setIconSize(QSize(18, 18));

        horizontalLayout->addWidget(volume_icon);

        volume_slider = new QSlider(frame);
        volume_slider->setObjectName(QString::fromUtf8("volume_slider"));
        sizePolicy2.setHeightForWidth(volume_slider->sizePolicy().hasHeightForWidth());
        volume_slider->setSizePolicy(sizePolicy2);
        volume_slider->setMaximum(1000);
        volume_slider->setValue(1000);
        volume_slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(volume_slider);


        verticalLayout_2->addWidget(frame);


        verticalLayout->addWidget(control_pad);


        gridLayout->addWidget(group_play, 0, 1, 1, 1);

        group_lib = new QFrame(Player);
        group_lib->setObjectName(QString::fromUtf8("group_lib"));
        group_lib->setMaximumSize(QSize(250, 16777215));
        group_lib->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_3 = new QVBoxLayout(group_lib);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(group_lib);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label);

        tableView = new QTableView(group_lib);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMaximumSize(QSize(400, 16777215));

        verticalLayout_3->addWidget(tableView);


        gridLayout->addWidget(group_lib, 0, 0, 1, 1);


        retranslateUi(Player);

        rate->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Player);
    } // setupUi

    void retranslateUi(QWidget *Player)
    {
        Player->setWindowTitle(QCoreApplication::translate("Player", "Euterpe", nullptr));
        control_pad->setTitle(QString());
        time_label->setText(QCoreApplication::translate("Player", "--.-- / --.--", nullptr));
        loop->setText(QString());
        rewind->setText(QString());
        rate->setItemText(0, QCoreApplication::translate("Player", "0.25x", nullptr));
        rate->setItemText(1, QCoreApplication::translate("Player", "0.5x", nullptr));
        rate->setItemText(2, QCoreApplication::translate("Player", "1x", nullptr));
        rate->setItemText(3, QCoreApplication::translate("Player", "1.5x", nullptr));
        rate->setItemText(4, QCoreApplication::translate("Player", "2x", nullptr));

        rate->setCurrentText(QCoreApplication::translate("Player", "1x", nullptr));
        open->setText(QCoreApplication::translate("Player", "OPEN", nullptr));
        prev->setText(QString());
        play->setText(QString());
        next->setText(QString());
        stop->setText(QCoreApplication::translate("Player", "STOP", nullptr));
        info->setText(QString());
        volume_icon->setText(QString());
        label->setText(QCoreApplication::translate("Player", "\345\252\222\344\275\223\345\272\223", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Player: public Ui_Player {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYER_H
