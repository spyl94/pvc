#include "mainwindow.h"
#include "pvc.h"

MainWindow::MainWindow()
{
    // Génération des widgets de la fenêtre principale
    creerActions();
    creerMenus();
    creerBarresOutils();
    creerBarreEtat();
    lancerEnable=false; //On ne peut pas lancer tant qu'on a pas ouvert de fichier.

    //Définition de la fenetre principale
    setMaximumSize(400,240);//Dimensionnement de la fenetre
    setMinimumSize(400,240);//idem
    setWindowTitle("Projet 4: Voyageur de commerce");//Définition du titre la fenetre

    tableWidget = new QTableWidget(40, 3, this);
    QStringList colonnes;
    colonnes << "   Nom de la ville   " <<"Abscisse" << "Ordonnée";
    tableWidget->setHorizontalHeaderLabels(colonnes);
    tableWidget->horizontalHeader()->setResizeMode ( QHeaderView::Stretch);
    setCentralWidget(tableWidget);
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Définition de la fenetre Options
    fenetreOptions.setWindowTitle("Options");//Définition du titre de la fenêtre secondaire "Options"
    fenetreOptions.setWindowIcon(QIcon("logo.png"));//Définition de l'icone de la fenêtre secondaire "Options"


    fenetrePropos.setWindowTitle("A propos");//Définition du titre de la fenêtre secondaire "A propos"
    fenetrePropos.setMinimumSize(600,150);//idem

    //Définition des champs d'entrées de la fenetre Options
    cardinalite_pop = new QSpinBox;
    tauxMutation = new QDoubleSpinBox;
    nbIteration = new QSpinBox;
    cardinalite_pop->setRange(2,10000);
    cardinalite_pop->setSingleStep(2);
    tauxMutation->setRange(0.00,1.00);
    tauxMutation->setSingleStep(0.10);
    nbIteration->setRange(1,1000000);

    methodeSelection = new QComboBox;
    methodeSelection->addItem("Elitiste");
    methodeSelection->addItem("Tournoi");
    methodeSelection->addItem("Hasard");

    //Ajout du bouton "Enregistrer"
    valider = new QPushButton(this);
    valider->setText("Enregistrer les modifications");

    //Mise en page de la Fenetre Options
    QVBoxLayout *layout = new QVBoxLayout(this);//Nouveau layout vertical
    QFormLayout *formLayout = new QFormLayout(this);//Nouveau layout de formulaire
    formLayout->addRow("Cardinalité de la population", cardinalite_pop);
    formLayout->addRow("Taux de mutation", tauxMutation);
    formLayout->addRow("Nombre d'itération", nbIteration);
    formLayout->addRow("Methode de sélection:", methodeSelection);
    QHBoxLayout *hLayout = new QHBoxLayout(this);//Nouveau layout horizontal
    hLayout->addWidget(valider);//Ajout du bouton valider au layout horizontal
    layout->addLayout(formLayout);//Ajout du layout de formulaire au layout vertical
    layout->addLayout(hLayout);//Ajout du layout horizontal au layout vertical
    fenetreOptions.setLayout(layout);//Ajout du layout vertical à la fenetre Options

    verifConfig();
    messageStatus->setText("Veuillez sélectionner un fichier...");
}
MainWindow::~MainWindow()
{

}
/* Méthodes d'initialisation de la GUI */
void MainWindow::creerActions()
{
    actionOuvrirFichier = new QAction(tr("&Ouvrir Fichier"), this);
    actionOuvrirFichier->setShortcut(tr("Ctrl+T"));
    connect(actionOuvrirFichier, SIGNAL(triggered()), this, SLOT(ouvrirFichier()));

    actionLancer = new QAction(tr("&Lancer"), this);
    actionLancer->setShortcut(tr("Ctrl+N"));
    connect(actionLancer, SIGNAL(triggered()), this, SLOT(lancer()));

    actionQuitter = new QAction(tr("&Quitter"), this);
    actionQuitter->setShortcut((tr("Ctrl+Q")));
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    actionOptions = new QAction("&Options", this);
    connect(actionOptions, SIGNAL(triggered()), this, SLOT(ouvrirOptions()));

    actionPropos = new QAction("&À propos du projet", this);
    connect(actionPropos, SIGNAL(triggered()), this, SLOT(ouvrirPropos()));

    actionChangerDepart = new QAction("&Changer Départ", this);
    connect(actionChangerDepart, SIGNAL(triggered()), this, SLOT(ouvrirChangerDepart()));

}
void MainWindow::creerMenus()
{
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");

        menuFichier->addAction(actionOuvrirFichier);
        menuFichier->addAction(actionLancer);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);

    QMenu *menuEdition = menuBar()->addMenu("&Edition");//Définiton du menu "Edition"

          menuEdition->addAction(actionOptions);//Ajout du sous menu "Options" au menu "Edition"
          menuEdition->addAction(actionChangerDepart);

    QMenu *menuAPropos = menuBar()->addMenu("&?");//Définition du  menu "?"

           menuAPropos->addAction(actionPropos);//Ajout du sous menu "A propos du projet" au menu "?"
}
void MainWindow::creerBarresOutils()
{
    QToolBar *toolBarNavigation = addToolBar(tr("Navigation"));

        toolBarNavigation->addAction(actionOuvrirFichier);
        toolBarNavigation->addAction(actionOptions);
        toolBarNavigation->addAction(actionChangerDepart);
        toolBarNavigation->addAction(actionLancer);
        toolBarNavigation->addAction(actionQuitter);
}
void MainWindow::creerBarreEtat()
{
    //Ajout de la barre d'état à la fenetre principale
    barreEtat = statusBar();//Définition de la barre d'état
    messageStatus = new QLabel;//Définition du message d'état
    barreEtat->addWidget(messageStatus);//Ajout du message de statut à la barre d'état

    progression = new QProgressBar(this);
    progression->setAlignment(Qt::AlignRight);
    progression->setVisible(false);
    progression->setMaximumHeight(14);
    statusBar()->addWidget(progression, 1);
}

/* SLOTS */
void  MainWindow::ouvrirOptions()
{
    if (!this->lancerEnable)
    {
        QMessageBox::critical(this, "Echec du lancement", "Vous devez charger un fichier avant de pouvoir modifier les options!");
        return;
    }
    else
    {
        fenetreOptions.show();
        connect(valider, SIGNAL(clicked()), this, SLOT(saveOptions()));//En cas de clic sur Enregistrer, on enregistre les parametres
    }
}
void  MainWindow::ouvrirFichier()
{
    messageStatus->setText("Sélection du fichier");
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier texte", QString(), "Text files (*.txt)");
    QFile file(fichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Fichier", "Impossible d'ouvrir le fichier!");
        messageStatus->setText("Prêt");
        return;
    }
    QTextStream flux(&file);

    QString ligne;
    long i=0;
    ligne = file.readLine();
    this->nombreVilles = ligne.toInt();
    this->c_pvc.nouveau(this->nombreVilles , this->cardinalite_pop->value());
    while(! flux.atEnd())
    {
        ligne = file.readLine();
        if (ligne != "" && ligne != "\n" )
        {
            ligne = ligne.trimmed();
            QStringList list;
            list = ligne.split("\t");
            this->c_pvc.setNom(i, list.at(0));
            this->c_pvc.setX(i, list.at(1).toDouble());
            this->c_pvc.setY(i, list.at(2).toDouble());
            i++;
        }
    }
    displayTab();
    messageStatus->setText("Prêt");
    this->lancerEnable=true; //Permet d'autoriser le lancer de l'algo
}
void  MainWindow::ouvrirChangerDepart()
{
    if (!this->lancerEnable)
    {
         QMessageBox::critical(this, "Echec du lancement", "Vous devez charger un fichier avant!");
      return;
    }
    QStringList villes;
    for (long i =0 ; i < this->nombreVilles ; i++)
    {
        villes.append(c_pvc.getNom(i));
    }
    bool ok=false;
   QString nouveauDepart = QInputDialog::getItem(this, "Ville de départ", "Quelle est la nouvelle ville de départ ?", villes,0,false,&ok);
    if (ok)
    {
        for (long i =0 ; i < this->nombreVilles ; i++)
        {
            if(nouveauDepart == c_pvc.getNom(i))
            {
                c_pvc.changerDepart(i);
                displayTab();
            }
        }
    }
}
void  MainWindow::ouvrirPropos()
{
    label = new QLabel("Programme de résolution du voyageur de commerce <br />Réalisé dans le cadre d'un projet de l'<a href=\"http://www.efrei.fr\">EFREI</a> par Spyl et Alexian (ou pas ^^).<br /><br />Le fichier à ouvrir doit respecter le format suivant: <br />La première ligne du fichier contient juste un entier : le nombre de villes du fichier. <br /> Ensuite, chaque ligne décrit une ville avec trois informations séparées par des séries de tabulations (caractère '\\t') : <br /> Tout d'abord le nom (au plus 20 caractères), puis deux réels, l'abscisse et l'ordonnée. ",&fenetrePropos);
    label->move(30, 20);
    fenetrePropos.show();
}
void MainWindow::lancer()
{
    if (!this->lancerEnable)
    {
        QMessageBox::critical(this, "Echec du lancement", "Vous devez charger un fichier avant de pouvoir lancer !");
        return;
    }
    else
    {
        /* Initialisation de l'affichage */
        messageStatus->setText("Lancement en cours...");
        progression->setRange(0,this->nbIteration->value());
        progression->setValue(0);
        progression->setVisible(true);

        /* Lancement de l'algorithme */
        time.start();
        c_pvc.randomPop();
        for (long i = 0; i < this->nbIteration->value(); i++)
        {
            switch(methodeSelection->currentIndex())
            {
            case 0: //Elitisme
                c_pvc.methodeElitisme();
                break;
            case 1: //Tournoi
                c_pvc.methodeTournoi();
                break;
            case 2:
                c_pvc.methodeHasard();
            default:
                break;
            }
            c_pvc.mutation( this->tauxMutation->value() * this->cardinalite_pop->value() ); //On envoit le nombre de mutations
            c_pvc.restaureBest();
            c_pvc.afficherMeilleur();
            progression->setValue(i+1);
        }
        messageStatus->setText("Terminée    ");
        displayFin( c_pvc.fin() );
        progression->setVisible(false);
        messageStatus->setText("Prêt");
    }
}

/* Méthodes d'affichage */
void  MainWindow::displayFin(QString nomFichier)    //affiche le résultat de la simulation
{
    QString message= "La simulation s'est déroulée avec succès! \n";
    message += "Le temps d'éxécution a été de: ";
    message += QString::number(time.elapsed());
    message += "ms\nLa simulation a été enregistrée dans le fichier: ";
    message += nomFichier;
    QMessageBox::information(this, "Simulation terminée!", message);
    this->lancerEnable=true; //Permet d'autoriser le lancer de l'algo
}
void  MainWindow::displayTab()  //affiche le contenu du fichier "carte"
{
    tableWidget = new QTableWidget( this->c_pvc.getNombreVilles(), 3, this);
    QStringList colonnes;
    colonnes << "   Nom de la ville   " <<"Abscisse" << "Ordonnée";
    tableWidget->setHorizontalHeaderLabels(colonnes);
    tableWidget->horizontalHeader()->setResizeMode ( QHeaderView::Stretch);
    for (long i = 0 ; i < c_pvc.getNombreVilles(); i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(
          this->c_pvc.getNom(i) ));
       tableWidget->setItem(i, 0, newItem);
       QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(
         this->c_pvc.getX(i)  ));
       tableWidget->setItem(i, 1, newItem2);
       QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(
         this->c_pvc.getY(i) ));
       tableWidget->setItem(i, 2, newItem3);
    }
    setCentralWidget(tableWidget);
}

/* Méthodes de configuration */
void MainWindow::verifConfig()
{
    messageStatus->setText("Initialisation");//Définition du message d'état
    QFile fichier_conf("conf.conf");//Ouverture du fichier de configuration
        if (fichier_conf.open(QIODevice::ReadOnly | QIODevice::Text) && fichier_conf.size()!=0)//Si le fichier de configuration existe et n'est pas vide
        {
            long i=0;
            QTextStream flux_conf(&fichier_conf);
            while(i<4)//On lit les 4 premières lignes
            {
                conf[i] = flux_conf.readLine();//On met le contenu du fichier de configuration dans un tableau
                if(conf[i]=="")//Si une ligne est vide, on renvoie une erreur
                {
                    cardinalite_pop->setValue(10);
                    tauxMutation->setValue(0.50);
                    nbIteration->setValue(50);
                    methodeSelection->setCurrentIndex(0);
                    saveOptions();
                    messageStatus->setText("Erreur dans le fichier conf.conf: valeurs remisent par défaut");
                    return;
                }
                i++;
            }
        }
        else
        {
            cardinalite_pop->setValue(10);
            tauxMutation->setValue(0.50);
            nbIteration->setValue(50);
            methodeSelection->setCurrentIndex(0);
            saveOptions();
            messageStatus->setText("Fichier conf.conf introuvable: valeurs remisent par défaut");
            return;
        }
        cardinalite_pop->setValue(conf[0].toInt());
        tauxMutation->setValue(conf[1].toDouble());
        nbIteration->setValue(conf[2].toInt());
        methodeSelection->setCurrentIndex(conf[3].toInt());
        messageStatus->setText("Prêt");
}
void  MainWindow::saveOptions()
{
    QFile fichier_conf("conf.conf");
    fichier_conf.open(QIODevice::WriteOnly);//On ouvre et on efface le fichier de configuration
    QTextStream flux(&fichier_conf);
    flux<<cardinalite_pop->value()<<'\n'<<tauxMutation->value()<<'\n'<<nbIteration->value()<<'\n'<<methodeSelection->currentIndex();//On enregistre la configuration dans le fichier
    fichier_conf.close();
    fenetreOptions.close();
    verifConfig();
    this->c_pvc.reload(this->nombreVilles, cardinalite_pop->value());
}

