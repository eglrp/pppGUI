/********************************************************************************
** Form generated from reading UI file 'dialogpost.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPOST_H
#define UI_DIALOGPOST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DialogPost
{
public:

    void setupUi(QDialog *DialogPost)
    {
        if (DialogPost->objectName().isEmpty())
            DialogPost->setObjectName(QStringLiteral("DialogPost"));
        DialogPost->resize(400, 300);

        retranslateUi(DialogPost);

        QMetaObject::connectSlotsByName(DialogPost);
    } // setupUi

    void retranslateUi(QDialog *DialogPost)
    {
        DialogPost->setWindowTitle(QApplication::translate("DialogPost", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialogPost: public Ui_DialogPost {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPOST_H
