/********************************************************************************
** Form generated from reading UI file 'correctionWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORRECTIONWIDGET_H
#define UI_CORRECTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_correctionWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *correctionWidget)
    {
        if (correctionWidget->objectName().isEmpty())
            correctionWidget->setObjectName(QStringLiteral("correctionWidget"));
        correctionWidget->resize(246, 249);
        verticalLayout_2 = new QVBoxLayout(correctionWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(correctionWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new QTableWidget(groupBox);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        verticalLayout_2->addWidget(groupBox);


        retranslateUi(correctionWidget);

        QMetaObject::connectSlotsByName(correctionWidget);
    } // setupUi

    void retranslateUi(QWidget *correctionWidget)
    {
        correctionWidget->setWindowTitle(QApplication::translate("correctionWidget", "\345\217\202\346\225\260\345\257\271\346\257\224", 0));
        groupBox->setTitle(QApplication::translate("correctionWidget", "\345\217\202\346\225\260", 0));
    } // retranslateUi

};

namespace Ui {
    class correctionWidget: public Ui_correctionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORRECTIONWIDGET_H
