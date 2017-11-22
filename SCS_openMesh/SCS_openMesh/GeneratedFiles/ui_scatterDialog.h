/********************************************************************************
** Form generated from reading UI file 'scatterDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCATTERDIALOG_H
#define UI_SCATTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qwebengineview.h"

QT_BEGIN_NAMESPACE

class Ui_scatterDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_Scatter;
    QHBoxLayout *horizontalLayout;
    QWebEngineView *preview;
    QGroupBox *groupBox_File;
    QGroupBox *groupBox_mode;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_4;
    QRadioButton *radioButton;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *showResult_Button;

    void setupUi(QDialog *scatterDialog)
    {
        if (scatterDialog->objectName().isEmpty())
            scatterDialog->setObjectName(QStringLiteral("scatterDialog"));
        scatterDialog->resize(920, 620);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scatterDialog->sizePolicy().hasHeightForWidth());
        scatterDialog->setSizePolicy(sizePolicy);
        scatterDialog->setMinimumSize(QSize(920, 620));
        scatterDialog->setMaximumSize(QSize(920, 620));
        scatterDialog->setBaseSize(QSize(920, 640));
        layoutWidget = new QWidget(scatterDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 11, 922, 608));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox_Scatter = new QGroupBox(layoutWidget);
        groupBox_Scatter->setObjectName(QStringLiteral("groupBox_Scatter"));
        groupBox_Scatter->setMinimumSize(QSize(900, 450));
        horizontalLayout = new QHBoxLayout(groupBox_Scatter);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        preview = new QWebEngineView(groupBox_Scatter);
        preview->setObjectName(QStringLiteral("preview"));
        preview->setMinimumSize(QSize(800, 460));

        horizontalLayout->addWidget(preview);


        verticalLayout->addWidget(groupBox_Scatter);

        groupBox_File = new QGroupBox(layoutWidget);
        groupBox_File->setObjectName(QStringLiteral("groupBox_File"));
        groupBox_File->setMinimumSize(QSize(920, 150));
        groupBox_File->setMaximumSize(QSize(16777215, 180));
        groupBox_mode = new QGroupBox(groupBox_File);
        groupBox_mode->setObjectName(QStringLiteral("groupBox_mode"));
        groupBox_mode->setGeometry(QRect(10, 20, 901, 91));
        layoutWidget1 = new QWidget(groupBox_mode);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(2, 20, 815, 60));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        radioButton_2 = new QRadioButton(layoutWidget1);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        gridLayout->addWidget(radioButton_2, 0, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(38, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 4, 1, 1);

        lineEdit = new QLineEdit(layoutWidget1);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 5, 1, 1);

        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 32));

        gridLayout->addWidget(pushButton, 0, 6, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 0, 7, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 0, 1, 1);

        radioButton = new QRadioButton(layoutWidget1);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        gridLayout->addWidget(radioButton, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(678, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 2, 1, 6);

        showResult_Button = new QPushButton(groupBox_File);
        showResult_Button->setObjectName(QStringLiteral("showResult_Button"));
        showResult_Button->setGeometry(QRect(790, 110, 75, 31));
        showResult_Button->raise();
        groupBox_mode->raise();

        verticalLayout->addWidget(groupBox_File);

        groupBox_File->raise();
        groupBox_Scatter->raise();

        retranslateUi(scatterDialog);

        QMetaObject::connectSlotsByName(scatterDialog);
    } // setupUi

    void retranslateUi(QDialog *scatterDialog)
    {
        scatterDialog->setWindowTitle(QApplication::translate("scatterDialog", "\344\273\277\347\234\237\347\273\223\346\236\234", 0));
        groupBox_Scatter->setTitle(QApplication::translate("scatterDialog", "\346\225\243\347\202\271\345\233\276", 0));
        groupBox_File->setTitle(QApplication::translate("scatterDialog", "\346\226\207\344\273\266", 0));
        groupBox_mode->setTitle(QApplication::translate("scatterDialog", "\346\250\241\345\274\217", 0));
        radioButton_2->setText(QApplication::translate("scatterDialog", "\344\273\277\347\234\237\347\273\223\346\236\234\345\261\225\347\244\272\357\274\210\346\240\241\346\255\243\357\274\211", 0));
        label->setText(QApplication::translate("scatterDialog", "\345\256\236\346\265\213\346\226\207\344\273\266", 0));
        pushButton->setText(QApplication::translate("scatterDialog", "\345\257\274\345\205\245", 0));
        radioButton->setText(QApplication::translate("scatterDialog", "\344\273\277\347\234\237\347\273\223\346\236\234\345\261\225\347\244\272", 0));
        showResult_Button->setText(QApplication::translate("scatterDialog", "\345\261\225\347\244\272", 0));
    } // retranslateUi

};

namespace Ui {
    class scatterDialog: public Ui_scatterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCATTERDIALOG_H
