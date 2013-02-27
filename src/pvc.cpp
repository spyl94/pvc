#include "pvc.h"
#include "mainwindow.h"
#include "time.h"
#include<QtGui>
#include <iostream>
using namespace std;

PVC::PVC()
{
    nombreSimulation = 0;
}
void PVC::nouveau(long nb_villes, long nb_indiv) //appelé lors de l'ouverture d'un fichier
{
    //allocation dynamique des paramètres.
    this->nombreIndividus= nb_indiv;
    this->nombreVilles= nb_villes;
    this->nom = new QString[nb_villes];
    this->x = new double[nb_villes];
    this->y = new double[nb_villes];
    this->meilleurIndividu = new long[nb_villes];
    this->pop = new long*[nb_indiv];
    for (long i = 0 ; i<nb_indiv ; i++)
    {
         this->pop[i] = new long[nb_villes];
    }
}
void PVC::reload(long nb_villes, long nb_indiv) //appelé lors d'un changement dans les options
{
    //charge les nouveaux paramètres.
    this->nombreIndividus= nb_indiv;
    this->meilleurIndividu = new long[nb_villes];
    this->pop = new long*[nb_indiv];
    for (long i = 0 ; i<nb_indiv ; i++)
    {
         this->pop[i] = new long[nb_villes];
    }
}
void PVC::changerDepart(long i)
{
    /* on inverse les données de i avec celles de 0 */
    swap(this->nom[i], this->nom[0]);
    swap(this->x[i], this->x[0]);
    swap(this->y[i], this->y[0]);
}

/* Méthodes get utilisées pour l'affichage de MainWindow*/
long PVC::getNombreVilles()
{
    return this->nombreVilles;
}
QString PVC::getNom(long i)
{
    return this->nom[i];
}
double PVC::getX(long i)
{
    return this->x[i];
}
double PVC::getY(long i)
{
    return this->y[i];
}

/* Méthodes set utilisées pour récupèrer les valeurs du fichier texte */
void PVC::setNom(long i, QString nouveau)
{
    this->nom[i]= nouveau;
}
void PVC::setX(long i, double k)
{
    this->x[i]= k;
}
void PVC::setY(long i, double k)
{
    this->y[i]= k;
}


/* Méthode permettant de génèrer une population d'individu pseudo aléatoire */
void PVC::randomPop()
{
    //On créé un tableau contenant toutes les villes sauf la ville de départ.
    long* cities;
    cities = new long[this->nombreVilles-1];
    for (long i = 0; i < this->nombreVilles-1 ; i++)
    {
        cities[i]=i+1;
    }
    for (long i = 0 ;  i< this->nombreIndividus ; i++)
        {
            this->pop[i][0] = 0;
            melanger(cities);
            for (long j = 1 ; j<this->nombreVilles ; j++)
            {
                this->pop[i][j]=cities[j-1];
            }
        }
    delete(cities);
}
void PVC::melanger(long* cities)
{
    long nombre_tire=0;
    for(long i = 0; i<this->nombreVilles-1 ;i++)
    {
       nombre_tire=rand()%(this->nombreVilles-1);
       swap(cities[i],cities[nombre_tire]);
    }
}
void PVC::afficherMeilleur()
{
    double *tab;
    tab = new double[this->nombreIndividus];
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        tab[i]=this->distance(i);   //Distance
    }
    bool tab_en_ordre = false;
    long taille=this->nombreIndividus;
    while(!tab_en_ordre)
    {
        tab_en_ordre = true;
        for(int i=0 ; i < taille-1 ; i++)
        {
            if(tab[i] > tab[i+1])
            {
                //On inverse la distance ET l'identifiant.
                swap(tab[i],tab[i+1]);
                tab_en_ordre = false;
            }
        }
        taille--;
    }
    qDebug() << "Meilleur individu: " << tab[0];
    delete(tab);
}

/* Methodes Sélection */
void PVC::methodeElitisme() //sélection élitiste
{
    double *tab;
    long *id;
    long *alive; //Contient les survivants

    tab = new double[this->nombreIndividus];
    id = new long[this->nombreIndividus];
    alive = new long[this->nombreIndividus/2];
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        tab[i]=this->distance(i);   //Distance
        id[i]=i;    //Identifiant
    }
    bool tab_en_ordre = false;
    long taille=this->nombreIndividus;
    while(!tab_en_ordre)
    {
        tab_en_ordre = true;
        for(long i=0 ; i < taille-1 ; i++)
        {
            if(tab[i] > tab[i+1])
            {
                //On inverse la distance ET l'identifiant.
                swap(tab[i],tab[i+1]);
                swap(id[i],id[i+1]);
                tab_en_ordre = false;
            }
        }
        taille--;
    }
    long j =0; //compteur survivants
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        if (i >= this->nombreIndividus/2 )
        {
            //qDebug() << tab[i] << "individu n:" << id[i] << " éliminé";
        }
        else
        {
            //On stock l'identidiant des survivants
            alive[j]=id[i];
            j++;    //On incrémente le compteur de survivants.
        }
    }
    //On enregistre le meilleur individu
    for (long i = 0; i < this->nombreVilles ; i++)
    {
        this->meilleurIndividu[i]= this->pop[id[0]][i];
    }
    //On envoit les survivants à la fonction croisement
    this->croisement(alive);
    delete(tab);
    delete(alive);
    delete(id);
}

void PVC::methodeTournoi() //sélection par tournoi
{
    long id1 = 0;
    long id2 = 0;
    long* alive;
    double *tab;
    long idBest;
    alive = new long[this->nombreIndividus/2]; //stock les id des vivants
    for ( long i = 0 ; i < this->nombreIndividus/2 ; i++)
    {
        id1= rand() % this->nombreIndividus;
        id2= rand() % this->nombreIndividus;
        if (this->distance(id1) <= this->distance(id2))
        {
            alive[i]= id1;
        }
        else
        {
            alive[i] = id2;
        }
    }
    //On enregistre le meilleur individu.
    tab = new double[this->nombreIndividus];
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        tab[i]=this->distance(i);   //Distance
    }
    double min = tab[0];
    idBest=0;
    for (long i = 1; i < this->nombreIndividus ; i++)
    {
        if(min > tab[i])
        {
            min = tab[i];
            idBest=i;
        }
    }
    for (long i = 0; i < this->nombreVilles ; i++)
    {
        this->meilleurIndividu[i]= this->pop[idBest][i];
    }
    //On envoit les survivants à la fonction croisement
    this->croisement(alive);
    delete(tab);
    delete(alive);
}

void PVC::methodeHasard() //sélection par tournoi
{
    long id = 0;
    long* alive;
    double *tab;
    long idBest;
    alive = new long[this->nombreIndividus/2]; //stock les id des vivants
    for ( long i = 0 ; i < this->nombreIndividus/2 ; i++)
    {
        id= rand() % this->nombreIndividus;
        alive[i]= id;
    }
    //On enregistre le meilleur individu.
    tab = new double[this->nombreIndividus];
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        tab[i]=this->distance(i);   //Distance
    }
    double min = tab[0];
    idBest=0;
    for (long i = 1; i < this->nombreIndividus ; i++)
    {
        if(min > tab[i])
        {
            min = tab[i];
            idBest=i;
        }
    }
    for (long i = 0; i < this->nombreVilles ; i++)
    {
        this->meilleurIndividu[i]= this->pop[idBest][i];
    }
    //On envoit les survivants à la fonction croisement
    this->croisement(alive);
    delete(tab);
    delete(alive);
}
/* Croisement */
void PVC::croisement(long* alive)
{
    long debut = 0;
    long largeur = 0;
    long **popTmp;
    long individuHasard = 0;
    long cptpop=0;

    popTmp = new long*[this->nombreIndividus];
    for (long i = 0 ; i < this->nombreIndividus ; i++)
    {
         popTmp[i] = new long[this->nombreVilles];
    }

    for (long cpt=0; cpt < this->nombreIndividus/2; cpt++)
    {
        /* deux paramètres entiers tirés aléatoirement */
        debut = rand()%(this->nombreVilles); //la première position à faire l'objet du croisement
        do
        {
            largeur=rand()% (this->nombreVilles-debut +1); //et le nombre de villes croisées
        }while(largeur < 1);

        //copie des parents dans les fils
        do{
             individuHasard = rand()%(this->nombreIndividus/2); //Un autre survivant au hasard
        }while(individuHasard==cpt);
        for (long i = 0 ; i < this->nombreVilles; i++)
        {
            popTmp[cptpop][i]  =   this->pop[alive[cpt]][i]; // Un individu survivant
            popTmp[cptpop+1][i]=   this->pop[alive[individuHasard]][i];
        }
        /*croisement -> Obtention des enfant temporaires */
        for (long i=debut; i <debut+largeur; i++)
        {
           popTmp[cptpop][i]  = this->pop[ alive[individuHasard]][i];
           popTmp[cptpop+1][i]= this->pop[ alive[cpt] ][i];
        }
        /* Dernière étape: supprimer les villes en double dans l'ordre du parent */
        deleteDoublons(debut,largeur, popTmp[cptpop], alive[cpt]);
        deleteDoublons(debut,largeur, popTmp[cptpop+1], alive[individuHasard]);
        cptpop+=2;  //On incrémente de 2 le cpt de population (on retrouve ainsi une population de X)
    }
    //On copie la nouvelle population dans la précédente
    for (long i = 0 ; i < this->nombreIndividus ; i++)
    {
        for (long j =0 ; j < this->nombreVilles ; j++)
        {
            pop[i][j] = popTmp[i][j];
        }
    }
    //On libère la mémoire.
    for (long i = 0 ; i < this->nombreIndividus ; i++)
    {
         delete(popTmp[i]);
    }
     delete(popTmp);
}
void PVC::deleteDoublons(long debut, long largeur, long* fils, long alive)
{
    long k =0; //cpt de doubles
    long* doublons;
    doublons = new long[this->nombreVilles];
    for (long i=0; i <debut; i++)
    {
        for (long j=debut; j < debut+largeur; j++)
        {
            if(fils[i] == fils[j])
            {
                //si une valeur est la même que dans l'intervalle du croisement
                //je retiens sa position.
               doublons[k]=i;
               k++;
            }
        }
    }
    for (long i=debut+largeur ; i < this->nombreVilles; i++)
    {
        for (long j=debut; j < debut+largeur; j++)
        {
            if(fils[i] == fils[j])
            {
               doublons[k]=i;
               k++;
            }
        }
    }
    if ( k > 0 ) //si on a des remplacements à faire
    {
        long l =0; //cpt de doublons remplacé
        for (long j=debut; j < debut+largeur; j++)
        {
            bool present = false;
            for (long i=debut; i < debut+largeur; i++)
            {
                if (this->pop[alive][j] == fils[i])
                {
                    present = true;
                }
            }
            if(!present)
            {
               fils[ doublons[l] ] = this->pop[ alive ][j];
               l++;
            }
        }
    }
    delete(doublons);
}

void PVC::restaureBest()
{
    long invididuMalchanceux = rand()%this->nombreIndividus;
    for (long j =0 ; j < this->nombreVilles ; j++)
    {
        this->pop[invididuMalchanceux][j] = meilleurIndividu[j];
    }
}

/* Méthode appelée pour génèrer autant de mutations que reçu en paramètre */
void PVC::mutation(long nombreMutations)
{
    long k = 0; //identifiant de l'individu
    long i = 0; //ville1
    long j=0;   //ville2
    while(nombreMutations != 0)
    {
        k = rand()% this->nombreIndividus;
        i = rand()%(this->nombreVilles-1);
        i++; // On ne peut pas modifier la ville de départ
        do
        {
            j=rand()%(this->nombreVilles-1);
            j++;
        }while(i==j);
        swap(this->pop[k][i],this->pop[k][j]);
        nombreMutations--;
    }
}

/* Méthode qui retourne la distance d'un individu reçu en paramètre */
double PVC::distance(long j)
{
    double d=0;
    for (long i =0; i < this->nombreVilles ; i++)
    {
        if (i != this->nombreVilles-1)
        {
            d+= sqrt( ( this->x[this->pop[j][i+1]] -  this->x[this->pop[j][i]] )*( this->x[this->pop[j][i+1]] -  this->x[this->pop[j][i]] ) + ( this->y[this->pop[j][i+1]] -  this->y[this->pop[j][i]] )*( this->y[this->pop[j][i+1]] -  this->y[this->pop[j][i]] ) );
        }
        else
        {
            d+= sqrt( ( this->x[this->pop[j][i]] -  this->x[this->pop[j][0]] )*( this->x[this->pop[j][i]] -  this->x[this->pop[j][0]] ) + ( this->y[this->pop[j][i]] -  this->y[this->pop[j][0]] )*( this->y[this->pop[j][i]] -  this->y[this->pop[j][0]] ) );
        }
    }
    return d;
}


QString PVC::fin()
{
    this->nombreSimulation++;
    double *tab;
    tab = new double[this->nombreIndividus];
    for(long i =0 ; i < this->nombreIndividus; i++)
    {
        tab[i]=this->distance(i);
    }
    double distanceMini=tab[0];
    long id=0;
    for(long i=1 ; i < this->nombreIndividus-1 ; i++)
    {
        if(tab[i+1] > tab[i])
        {
           distanceMini=tab[i+1];
           id=i+1;
        }
    }
    QString nomFichier = "simulation";
    nomFichier += QString::number(this->nombreSimulation);
    nomFichier += ".txt";

    QFile fichier_fin(nomFichier);
    fichier_fin.open(QIODevice::WriteOnly);//On ouvre
    QTextStream flux(&fichier_fin);
    flux<<"Distance minimale: "<<distanceMini << "\n";
    for (long i = 0 ; i < this->nombreVilles ; i++)
    {
        flux<<this->getNom(pop[id][i])<< "\n";
    }
    fichier_fin.close();

    QFile fichier_debug("debug.txt");
    fichier_debug.open(QIODevice::WriteOnly);//On ouvre
    QTextStream fluxdeb(&fichier_debug);
    for (long j = 0; j < this->nombreIndividus ; j++)
    {
        fluxdeb<<"\n ------ \n";
        fluxdeb << "individu n"<< j << "\n";
        for (long i = 0 ; i < this->nombreVilles ; i++)
        {
            fluxdeb<<i<< "/" <<this->getNom(pop[j][i])<< "\n";
        }
    }
    fichier_debug.close();
    delete(tab);
    return nomFichier;

}

