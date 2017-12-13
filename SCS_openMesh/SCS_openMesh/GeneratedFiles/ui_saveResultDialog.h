/********************************************************************************
** Form generated from reading UI file 'saveResultDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVERESULTDIALOG_H
#define UI_SAVERESULTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_saveDialog
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QComboBox *pciComboBox;
    QRadioButton *multiRadioButton;
    QDialogButtonBox *buttonBox;
    QRadioButton *singleRadioButton;

    void setupUi(QDialog *saveDialog)
    {
        if (saveDialog->objectName().isEmpty())
            saveDialog->setObjectName(QStringLiteral("saveDialog"));
        saveDialog->setWindowModality(Qt::ApplicationModal);
        saveDialog->resize(240, 116);
        widget = new QWidget(saveDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(15, 22, 211, 81));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pciComboBox = new QComboBox(widget);
        pciComboBox->setObjectName(QStringLiteral("pciComboBox"));

        gridLayout->addWidget(pciComboBox, 0, 1, 1, 1);

        multiRadioButton = new QRadioButton(widget);
        multiRadioButton->setObjectName(QStringLiteral("multiRadioButton"));

        gridLayout->addWidget(multiRadioButton, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 2);

        singleRadioButton = new QRadioButton(widget);
        singleRadioButton->setObjectName(QStringLiteral("singleRadioButton"));

        gridLayout->addWidget(singleRadioButton, 0, 0, 1, 1);

        buttonBox->raise();
        pciComboBox->raise();
        singleRadioButton->raise();
        multiRadioButton->raise();
        singleRadioButton->raise();

        retranslateUi(saveDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), saveDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), saveDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(saveDialog);
    } // setupUi

    void retranslateUi(QDialog *saveDialog)
    {
        saveDialog->setWindowTitle(QApplication::translate("saveDialog", "\344\277\235\345\255\230\347\273\223\346\236\234\346\226\207\344\273\266", 0));
        multiRadioButton->setText(QApplication::translate("saveDialog", "\346\211\271\351\207\217\344\277\235\345\255\230\346\211\200\346\234\211\346\226\207\344\273\266", 0));
        singleRadioButton->setText(QApplication::translate("saveDialog", "\345\215\225\344\270\252Cell\344\277\235\345\255\230", 0));
    } // retranslateUi

};

namespace Ui {
    class saveDialog: public Ui_saveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVERESULTDIALOG_H
