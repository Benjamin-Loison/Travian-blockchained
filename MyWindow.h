#ifndef DEF_MYWINDOW
#define DEF_MYWINDOW

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>

class MyWindow : public QWidget
{
    Q_OBJECT

    public:
        MyWindow();

    private:
        void addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended = false),
             addTribeText(QLabel* tab, QString troopName, QString text);
        QLabel* m_paratext;
        QTabWidget* m_tabs;
};


#endif
