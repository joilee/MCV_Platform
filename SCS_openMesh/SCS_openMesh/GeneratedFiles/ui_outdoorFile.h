/********************************************************************************
** Form generated from reading UI file 'outdoorFile.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTDOORFILE_H
#define UI_OUTDOORFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_outdoorFile
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit_name;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_2;
    QPushButton *buildingButton;
    QPushButton *pushButton_deleteV;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *checkBox_4;
    QPushButton *heightButton;
    QPushButton *pushButton_deleteH;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox_3;
    QPushButton *altitudeButton;
    QPushButton *pushButton_deleteP;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_LoadFile;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *outdoorFile)
    {
        if (outdoorFile->objectName().isEmpty())
            outdoorFile->setObjectName(QStringLiteral("outdoorFile"));
        outdoorFile->resize(321, 370);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(outdoorFile->sizePolicy().hasHeightForWidth());
        outdoorFile->setSizePolicy(sizePolicy);
        layoutWidget = new QWidget(outdoorFile);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 295, 251));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 20, 177, 22));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEdit_name = new QLineEdit(widget);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));

        horizontalLayout->addWidget(lineEdit_name);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setCheckable(true);

        horizontalLayout_3->addWidget(checkBox_2);

        buildingButton = new QPushButton(groupBox_3);
        buildingButton->setObjectName(QStringLiteral("buildingButton"));

        horizontalLayout_3->addWidget(buildingButton);

        pushButton_deleteV = new QPushButton(groupBox_3);
        pushButton_deleteV->setObjectName(QStringLiteral("pushButton_deleteV"));

        horizontalLayout_3->addWidget(pushButton_deleteV);


        verticalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(layoutWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_4);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        checkBox_4 = new QCheckBox(groupBox_4);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setCheckable(true);

        horizontalLayout_5->addWidget(checkBox_4);

        heightButton = new QPushButton(groupBox_4);
        heightButton->setObjectName(QStringLiteral("heightButton"));

        horizontalLayout_5->addWidget(heightButton);

        pushButton_deleteH = new QPushButton(groupBox_4);
        pushButton_deleteH->setObjectName(QStringLiteral("pushButton_deleteH"));

        horizontalLayout_5->addWidget(pushButton_deleteH);


        verticalLayout->addWidget(groupBox_4);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        checkBox_3 = new QCheckBox(groupBox_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setCheckable(true);

        horizontalLayout_2->addWidget(checkBox_3);

        altitudeButton = new QPushButton(groupBox_2);
        altitudeButton->setObjectName(QStringLiteral("altitudeButton"));

        horizontalLayout_2->addWidget(altitudeButton);

        pushButton_deleteP = new QPushButton(groupBox_2);
        pushButton_deleteP->setObjectName(QStringLiteral("pushButton_deleteP"));

        horizontalLayout_2->addWidget(pushButton_deleteP);


        verticalLayout->addWidget(groupBox_2);

        layoutWidget2 = new QWidget(outdoorFile);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(130, 310, 164, 31));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        pushButton_LoadFile = new QPushButton(layoutWidget2);
        pushButton_LoadFile->setObjectName(QStringLiteral("pushButton_LoadFile"));
        pushButton_LoadFile->setCheckable(false);

        horizontalLayout_4->addWidget(pushButton_LoadFile);

        horizontalSpacer_4 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        pushButton_cancel = new QPushButton(layoutWidget2);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));

        horizontalLayout_4->addWidget(pushButton_cancel);


        retranslateUi(outdoorFile);

        pushButton_LoadFile->setDefault(false);


        QMetaObject::connectSlotsByName(outdoorFile);
    } // setupUi

    void retranslateUi(QDialog *outdoorFile)
    {
        outdoorFile->setWindowTitle(QApplication::translate("outdoorFile", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("outdoorFile", "\345\267\245\347\250\213\345\220\215", 0));
        label->setText(QApplication::translate("outdoorFile", "\345\234\260\345\233\276\345\220\215", 0));
        groupBox_3->setTitle(QApplication::translate("outdoorFile", "\345\257\274\345\205\245\345\273\272\347\255\221\347\211\251\346\226\207\344\273\266", 0));
        checkBox_2->setText(QApplication::translate("outdoorFile", "\347\212\266\346\200\201", 0));
        buildingButton->setText(QApplication::translate("outdoorFile", "\346\211\223\345\274\200", 0));
        pushButton_deleteV->setText(QApplication::translate("outdoorFile", "\345\210\240\351\231\244", 0));
        groupBox_4->setTitle(QApplication::translate("outdoorFile", "\345\257\274\345\205\245\345\273\272\347\255\221\347\211\251\351\253\230\345\272\246\346\226\207\344\273\266", 0));
        checkBox_4->setText(QApplication::translate("outdoorFile", "\347\212\266\346\200\201", 0));
        heightButton->setText(QApplication::translate("outdoorFile", "\346\211\223\345\274\200", 0));
        pushButton_deleteH->setText(QApplication::translate("outdoorFile", "\345\210\240\351\231\244", 0));
        groupBox_2->setTitle(QApplication::translate("outdoorFile", "\345\257\274\345\205\245\346\265\267\346\213\224\346\226\207\344\273\266", 0));
        checkBox_3->setText(QApplication::translate("outdoorFile", "\347\212\266\346\200\201", 0));
        altitudeButton->setText(QApplication::translate("outdoorFile", "\346\211\223\345\274\200", 0));
        pushButton_deleteP->setText(QApplication::translate("outdoorFile", "\345\210\240\351\231\244", 0));
        pushButton_LoadFile->setText(QApplication::translate("outdoorFile", "OK", 0));
        pushButton_cancel->setText(QApplication::translate("outdoorFile", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class outdoorFile: public Ui_outdoorFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTDOORFILE_H
