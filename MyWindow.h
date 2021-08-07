#ifndef DEF_MYWINDOW
#define DEF_MYWINDOW

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

    public slots:
        void setChooseLocationGUI();
        void chooseLocation();
        void startGame(); // can't use a single void for all slots :'(

    private:
        void addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended = false),
             addTribeText(QVBoxLayout* vbox, QString troopName, QString text);
        QTabWidget* m_tabs;
        QLabel* setTitle(QString title);
};


#endif
