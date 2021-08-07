#ifndef DEF_MYWINDOW
#define DEF_MYWINDOW

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

class MyWindow : public QWidget
{
    Q_OBJECT

    public:
        MyWindow();

    private:
        void addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended = false),
             addTribeText(QVBoxLayout* vbox, QString troopName, QString text);
        QTabWidget* m_tabs;
};


#endif
