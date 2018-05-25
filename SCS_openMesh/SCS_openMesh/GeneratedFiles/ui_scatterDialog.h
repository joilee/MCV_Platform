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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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


        retranslateUi(scatterDialog);

        QMetaObject::connectSlotsByName(scatterDialog);
    } // setupUi

    void retranslateUi(QDialog *scatterDialog)
    {
        scatterDialog->setWindowTitle(QApplication::translate("scatterDialog", "\344\273\277\347\234\237\347\273\223\346\236\234", 0));
        groupBox_Scatter->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class scatterDialog: public Ui_scatterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCATTERDIALOG_H
