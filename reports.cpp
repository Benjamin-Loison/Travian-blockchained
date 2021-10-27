#include "reports.h"
#include "main.h"
#include "QPlus.h"

void setReportsScreen()
{
    screenView = SCREEN_VIEW_REPORTS;
}

// copied from getStatisticsScreen
QWidget* getReportsScreen()
{
    QWidget* screen = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout,
               * allTabVBox = new QVBoxLayout;
    QLabel* reports = new QLabel(QObject::tr("Reports"));
    QTabWidget* tabs = new QTabWidget;
    QWidget* allTabContent = new QWidget,
           //* surroundingTabContent = new QWidget,
           * allTabGridContent = new QWidget;
    QGridLayout* allGrid = new QGridLayout; // could add borders lines
    allGrid->addWidget(new QLabel(QObject::tr("Subject")), 0, 0);
    allGrid->addWidget(new QLabel(QObject::tr("Received")), 0, 1);

    QWidget* subject = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(getQLabel("reports/read.png")); // or unread.jpg
    hbox->addWidget(new QLabel("a subject")); // or unread.jpg
    hbox->addStretch();
    subject->setLayout(hbox);
    allGrid->addWidget(subject, 1, 0);
    allGrid->addWidget(new QLabel("11.09.21, 16:07"), 1, 1);

    allTabGridContent->setLayout(allGrid);
    allTabVBox->addWidget(allTabGridContent);
    allTabVBox->addStretch();
    allTabContent->setLayout(allTabVBox);

    tabs->addTab(allTabContent, QObject::tr("All"));
    //tabs->addTab(surroundingTabContent, QObject::tr("Surrounding"));
    // https://answers.travian.com/index.php?aid=99#sharing
    // max distance from my reports: 21.21 - maybe all
    // for each (location, distance, received (date)): (following not exhaustive just from my reports)
    // oasis plundered
    // oasis conquered by X
    // new village founded by X (distinction for first or others but this pieces of information are redundant with profile)
    // a fight took place in Village Y of X
    // X renamed village from Y to Z (but this pieces of information are redundant with profile)
    // X left the alliance Y
    // alliance switch ?!

    vbox->addWidget(reports);
    vbox->addWidget(tabs);
    screen->setLayout(vbox);
    QColor backgroundCurrentlyBuilding = QColor(210, 189, 161);
    setColor(screen, backgroundCurrentlyBuilding);

    return screen;
}
