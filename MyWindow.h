#ifndef DEF_MYWINDOW_H
#define DEF_MYWINDOW_H

#include <QtWidgets>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include "troops.h"

class MyWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MyWindow();
        void manageBackground(),
             setChooseTribeGUI();

    protected:
        void resizeEvent(QResizeEvent* evt) override;

    public slots:
        void setChooseLocationGUI();
        void startGame(bool isRestoring = false); // can't use a single void for all slots :'(
        void refreshLoop(/*bool refresh = false*/); // connect in MyWindow doesn't seem to work according to Qt logs otherwise
    private:
        void addTribe(tribeEnum tribe, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended = false),
             addTribeText(QVBoxLayout* vbox, QString tribeName, QString troopName, QString text);
        void keyPressEvent(QKeyEvent* pe) override;
        QTabWidget* m_tabs;
        QLabel* setTitle(QString title);
        QLineEdit* nicknameLineEdit;
};


#endif
