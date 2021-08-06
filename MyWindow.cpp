#include <QApplication> // why need this if it is also in the header ?!
#include <QtWidgets>
#include "MyWindow.h"
#include "QStringPlus.h"

MyWindow::MyWindow() : QWidget()
{
    //setFixedSize(200, 100);

    setWindowIcon(QIcon("travian.png"));
    setWindowTitle("Travian blockchained");

    m_paratext = new QLabel(QObject::tr("Great empires begin with important decisions ! Are you an attacker who loves competition ? Or is your time investment rather low ? Are you a team player who enjoys building up a thriving economy to forge the anvil ?"), this);

    m_tabs = new QTabWidget(this);
    m_tabs->setGeometry(30, 20, 240, 160);

    addTribe("gauls", "phalanx", "Low time requirements", "Loot protection and good defense", "Excellent, fast cavalry", "Well suited to new players");
    addTribe("romans", "legionnaire", "Moderate time requirements", "Can develop villages the fastest", "Very strong but expensive troops", "Hard to play for new players");
    addTribe("teutons", "clubswinger", "High time requirements", "Good at looting in early game", "Strong, cheap infantry", "For aggressive players");
}

void MyWindow::addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended)
{
    QLabel* tab = new QLabel; // isn't there any other way to do this ? (QWidget doesn't work here)
    QVBoxLayout* vbox = new QVBoxLayout;

    addTribeText(tab, troopName, timeRequirement);
    addTribeText(tab, troopName, speciality);
    addTribeText(tab, troopName, troopsTraining);
    addTribeText(tab, troopName, designedForUsers);

    m_tabs->addTab(tab, QIcon(tribeName + ".png"), tr("%1").arg(firstUppercase(tribeName)));
}

void MyWindow::addTribeText(QLabel* tab, QString troopName, QString text)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    QLabel* qText = new QLabel(tr("%1").arg(text));
    //qText->setPixmap(QPixmap(troopName + ".png"));
    QLabel* qIcon = new QLabel();
    qIcon->setPixmap(QPixmap(troopName + ".png"));
    hbox->addWidget(qIcon);
    hbox->addWidget(qText);
    //tab->setPixmap(QPixmap(troopName + ".png"));
    //QPixMap(troopName + ".png", vbox);
    //vbox->addWidget(new QIcon(troopName + ".png"));
    tab->setLayout(hbox);

    //tab->setAlignment(Qt::AlignLeft);
}
