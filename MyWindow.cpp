#include <QApplication> // why need this if it is also in the header ?!
#include <QtWidgets>
#include "MyWindow.h"

MyWindow::MyWindow() : QWidget()
{
    //setFixedSize(200, 100);

    setWindowIcon(QIcon("travian.png"));
    setWindowTitle("Travian blockchained");

    // 1 : Créer le QTabWidget
    QLabel* aparatext = new QLabel(QObject::tr("Great empires begin with important decisions ! Are you an attacker who loves competition ? Or is your time investment rather low ? Are you a team player who enjoys building up a thriving economy to forge the anvil ?"), this);

    QTabWidget *onglets = new QTabWidget(this);
    onglets->setGeometry(30, 20, 240, 160);

    // 2 : Créer les pages, en utilisant un widget parent pour contenir chacune des pages
    QWidget *page1 = new QWidget;
    QLabel *page3 = new QLabel; // Comme un QLabel est aussi un QWidget (il en hérite), on peut aussi s'en servir de page

    // 3 : Créer le contenu des pages de widgets

        // Page 1

        QLineEdit *lineEdit = new QLineEdit("Entrez votre nom");
        QPushButton *bouton1 = new QPushButton("Cliquez ici");

        QVBoxLayout *vbox1 = new QVBoxLayout;
        vbox1->addWidget(lineEdit);
        vbox1->addWidget(bouton1);

        page1->setLayout(vbox1);

        // Page 2

        QProgressBar *progress = new QProgressBar;
        progress->setValue(50);
        QSlider *slider = new QSlider(Qt::Horizontal);
        QPushButton *bouton3 = new QPushButton("Valider");

        // Page 3 (je ne vais afficher qu'une image ici, pas besoin de layout)

        page3->setPixmap(QPixmap("phalanx.png"));
        page3->setAlignment(Qt::AlignCenter);

    // 4 : ajouter les onglets au QTabWidget, en indiquant la page qu'ils contiennent

    onglets->addTab(page1, QIcon("gauls.png"), QObject::tr("Gauls"));
    onglets->addTab(page3, QIcon("teutons.png"), QObject::tr("Teutons"));
}
