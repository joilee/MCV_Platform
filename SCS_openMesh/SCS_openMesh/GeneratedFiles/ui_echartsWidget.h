/********************************************************************************
** Form generated from reading UI file 'echartsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECHARTSWIDGET_H
#define UI_ECHARTSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qwebengineview.h"

QT_BEGIN_NAMESPACE

class Ui_echartsWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QWebEngineView *preview;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *echartsWidget)
    {
        if (echartsWidget->objectName().isEmpty())
            echartsWidget->setObjectName(QStringLiteral("echartsWidget"));
        echartsWidget->resize(900, 650);
        verticalLayout_3 = new QVBoxLayout(echartsWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(echartsWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(900, 450));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        preview = new QWebEngineView(groupBox);
        preview->setObjectName(QStringLiteral("preview"));
        preview->setMinimumSize(QSize(800, 450));

        horizontalLayout->addWidget(preview);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(echartsWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(900, 0));
        groupBox_2->setMaximumSize(QSize(16777215, 100));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(0, 32));

        verticalLayout->addWidget(pushButton);


        gridLayout->addLayout(verticalLayout, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(echartsWidget);

        QMetaObject::connectSlotsByName(echartsWidget);
    } // setupUi

    void retranslateUi(QWidget *echartsWidget)
    {
        echartsWidget->setWindowTitle(QApplication::translate("echartsWidget", "\346\225\243\347\202\271\345\233\276", 0));
        groupBox->setTitle(QApplication::translate("echartsWidget", "Charts", 0));
        groupBox_2->setTitle(QApplication::translate("echartsWidget", "File", 0));
        label->setText(QApplication::translate("echartsWidget", "\346\226\207\344\273\266\345\257\274\345\205\245", 0));
        pushButton->setText(QApplication::translate("echartsWidget", "\350\256\241\347\256\227\346\225\260\346\215\256", 0));
    } // retranslateUi

};

namespace Ui {
    class echartsWidget: public Ui_echartsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECHARTSWIDGET_H
