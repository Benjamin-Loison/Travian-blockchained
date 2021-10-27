#include "profile.h"
#include "main.h"
#include "resources.h"
#include "QPlus.h"
#include "QStringPlus.h"
#include <QHBoxLayout>
#include <QLabel>

void seeVillage()
{
    screenView = SCREEN_VIEW_VILLAGE;
}

QWidget* getProfileScreen()
{
    QWidget* screen = new QWidget,
           * profile = new QWidget,
           * grid = new QWidget;
    QHBoxLayout* hbox = new QHBoxLayout;
    QVBoxLayout* vbox = new QVBoxLayout;

    QLabel* title = new QLabel(nickname);
    QColor backgroundCurrentlyBuilding = QColor(210, 189, 161); // could rename this variable (almost) everywhere it is used
    setColor(title, backgroundCurrentlyBuilding);
    vbox->addWidget(title);
    vbox->addWidget(new QLabel("<b>" + QObject::tr("Details") + "</b>"));
    vbox->addWidget(new QLabel(QObject::tr("Tribe") + ": " +  translator.translate("tribes", getTribe().toStdString().c_str())));
    vbox->addWidget(new QLabel(QObject::tr("Villages") + ": " + QString::number(1)));
    vbox->addWidget(new QLabel("<b>" + QObject::tr("Villages") + "</b>"));

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel(QObject::tr("Name")), 0, 0);
    gridLayout->addWidget(new QLabel(QObject::tr("Inhabitants")), 0, 1);
    gridLayout->addWidget(new QLabel(QObject::tr("Coordinates")), 0, 2);

    //QString villageName = firstUppercase(QObject::tr("%1's village").arg(nickname)) + " <font color=\"grey\">(" + QObject::tr("Capital") + ")</font>";
    QPushButton* villageButton = new QPushButton(firstUppercase(QObject::tr("%1's village").arg(nickname)) + " (" + QObject::tr("Capital") + ")"); // would be cool to make text clickable

    QObject::connect(villageButton, &QPushButton::clicked, [](){ seeVillage(); });

    //gridLayout->addWidget(new QLabel(villageName), 1, 0);
    gridLayout->addWidget(villageButton, 1, 0);
    gridLayout->addWidget(new QLabel(QString::number(population)), 1, 1);
    gridLayout->addWidget(new QLabel(getCoordinatesStr()), 1, 2);

    grid->setLayout(gridLayout);
    vbox->addWidget(grid);
    profile->setLayout(vbox);

    hbox->addStretch();
    hbox->addWidget(profile);
    hbox->addStretch();
    screen->setLayout(hbox);

    setColor(profile, Qt::white);

    return screen;
}
