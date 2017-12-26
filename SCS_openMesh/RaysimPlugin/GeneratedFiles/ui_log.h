/********************************************************************************
** Form generated from reading UI file 'log.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_H
#define UI_LOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_logForm
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTextBrowser *logBrowser;

    void setupUi(QWidget *logForm)
    {
        if (logForm->objectName().isEmpty())
            logForm->setObjectName(QStringLiteral("logForm"));
        logForm->resize(400, 300);
        horizontalLayout = new QHBoxLayout(logForm);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        logBrowser = new QTextBrowser(logForm);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));

        verticalLayout->addWidget(logBrowser);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(logForm);

        QMetaObject::connectSlotsByName(logForm);
    } // setupUi

    void retranslateUi(QWidget *logForm)
    {
        logForm->setWindowTitle(QApplication::translate("logForm", "\350\256\241\347\256\227\346\227\245\345\277\227", 0));
    } // retranslateUi

};

namespace Ui {
    class logForm: public Ui_logForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_H
