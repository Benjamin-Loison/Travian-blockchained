#include <QApplication> // why need this if it is also in the header ?!
#include <QtWidgets>
#include "MyWindow.h"
#include "QStringPlus.h"

MyWindow::MyWindow() : QWidget()
{
    //setFixedSize(200, 100);

    setWindowIcon(QIcon("travian.png"));
    setWindowTitle("Travian blockchained");

    QVBoxLayout* vbox = new QVBoxLayout;
    m_paratext = new QLabel(QObject::tr("Great empires begin with important decisions ! Are you an attacker who loves competition ? Or is your time investment rather low ? Are you a team player who enjoys building up a thriving economy to forge the anvil ?"), this);
    m_paratext->setWordWrap(true);

    m_tabs = new QTabWidget();
    m_tabs->setGeometry(30, 20, 240, 160);
    vbox->addWidget(m_paratext);
    vbox->addWidget(m_tabs);
    this->setLayout(vbox);

    addTribe("gauls", "phalanx", "Low time requirements", "Loot protection and good defense", "Excellent, fast cavalry", "Well suited to new players");
    addTribe("romans", "legionnaire", "Moderate time requirements", "Can develop villages the fastest", "Very strong but expensive troops", "Hard to play for new players");
    addTribe("teutons", "clubswinger", "High time requirements", "Good at looting in early game", "Strong, cheap infantry", "For aggressive players");
}

void MyWindow::addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended)
{
    QWidget* tab = new QWidget; // isn't there any other way to do this ? (QWidget doesn't work here)
    QVBoxLayout* vbox = new QVBoxLayout;

    QWidget* subTabs = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;

    addTribeText(vbox, troopName, timeRequirement);
    addTribeText(vbox, troopName, speciality);
    addTribeText(vbox, troopName, troopsTraining);
    addTribeText(vbox, troopName, designedForUsers);

    //vbox->setSpacing(50);
    //vbox->setContentsMargins(0, 0, 0, 0);
    //vbox->setMargin(0);
    subTabs->setLayout(vbox);
    hbox->addWidget(subTabs);

    QLabel * qIcon = new QLabel();
    qIcon->setPixmap(QPixmap(tribeName + ".png"));
    hbox->addWidget(qIcon);

    tab->setLayout(hbox);

    m_tabs->addTab(tab, QIcon(tribeName + ".png"), tr("%1").arg(firstUppercase(tribeName)));
}

void MyWindow::addTribeText(QVBoxLayout* vbox, QString troopName, QString text)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    QWidget* qLine = new QWidget;
    QLabel * qText = new QLabel(tr("%1").arg(text)/*, tab*//*, qLine*/);
    QLabel * qIcon = new QLabel(/*qText*/);
    qIcon->setPixmap(QPixmap(troopName + ".png"));
    QStatusBar* qStatusBar = new QStatusBar;
    qStatusBar->addWidget(qIcon);
    qStatusBar->addWidget(qText);
    qStatusBar->setSizeGripEnabled(false);
    //hbox->addWidget(qIcon);
    //hbox->addWidget(qText);

    //qLine->setFrameShape(QFrame::NoFrame);
    //qLine->setLineWidth(0);
    //qLine->setLayout(hbox);
    vbox->addWidget(/*qLine*/qStatusBar/*, 0, Qt::AlignTop*/);
}
