#ifndef DEF_MYWINDOW_H
#define DEF_MYWINDOW_H

#include <QtWidgets>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

class MyWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MyWindow();

    protected:
        void resizeEvent(QResizeEvent* evt) override;

    public slots:
        void setChooseLocationGUI();
        void chooseLocation();
        void startGame(); // can't use a single void for all slots :'(
        void refreshLoop();

    private:
        void addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended = false),
             addTribeText(QVBoxLayout* vbox, QString troopName, QString text),
             manageBackground();
        QTabWidget* m_tabs;
        QLabel* setTitle(QString title);
};


#endif
