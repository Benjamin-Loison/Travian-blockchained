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
    QLabel* m_paratext = new QLabel(QObject::tr("Great empires begin with important decisions ! Are you an attacker who loves competition ? Or is your time investment rather low ? Are you a team player who enjoys building up a thriving economy to forge the anvil ?"), this);
    m_paratext->setWordWrap(true);

    m_tabs = new QTabWidget();
    m_tabs->setGeometry(30, 20, 240, 160);

    QPushButton* confirmButton = new QPushButton(tr("Confirm"));

    vbox->addWidget(m_paratext);
    vbox->addWidget(m_tabs);
    vbox->addWidget(confirmButton);

    this->setLayout(vbox);

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));

    addTribe(QT_TR_NOOP("gauls"), "phalanx", tr("Low time requirements"), tr("Loot protection and good defense"), tr("Excellent, fast cavalry"), tr("Well suited to new players"));
    addTribe(QT_TR_NOOP("romans"), "legionnaire", tr("Moderate time requirements"), tr("Can develop villages the fastest"), tr("Very strong but expensive troops"), tr("Hard to play for new players"));
    addTribe(QT_TR_NOOP("teutons"), "clubswinger", tr("High time requirements"), tr("Good at looting in early game"), tr("Strong, cheap infantry"), tr("For aggressive players"));
}

void MyWindow::addTribe(QString tribeName, QString troopName, QString timeRequirement, QString speciality, QString troopsTraining, QString designedForUsers, bool recommended)
{
    QWidget* tab = new QWidget;
    QVBoxLayout* vbox = new QVBoxLayout;

    QWidget* subTabs = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;

    addTribeText(vbox, troopName, timeRequirement);
    addTribeText(vbox, troopName, speciality);
    addTribeText(vbox, troopName, troopsTraining);
    addTribeText(vbox, troopName, designedForUsers);

    subTabs->setLayout(vbox);
    hbox->addWidget(subTabs);

    QLabel * qIcon = new QLabel();
    qIcon->setPixmap(QPixmap(tribeName + ".png"));
    hbox->addWidget(qIcon);

    tab->setLayout(hbox);

    m_tabs->addTab(tab, QIcon(tribeName + ".png"), firstUppercase(tr(cStr(tribeName))));
}

void MyWindow::addTribeText(QVBoxLayout* vbox, QString troopName, QString text)
{
    QLabel* qText = new QLabel(text), // it's sad that tr here doesn't work "as expected"
          * qIcon = new QLabel();
    qIcon->setPixmap(QPixmap(troopName + ".png")); // there is maybe a way to directly make a picture but I'm learning Qt, be kind
    QStatusBar* qStatusBar = new QStatusBar; // same here, I didn't achieved to just use a horizontal layout
    qStatusBar->addWidget(qIcon);
    qStatusBar->addWidget(qText);
    qStatusBar->setSizeGripEnabled(false);

    vbox->addWidget(qStatusBar);
}
