#ifndef PVC_H
#define PVC_H

#include<QtGui>


class PVC : public QObject
{
    Q_OBJECT
private:
    QString* nom;
    double* x;
    double* y;
    long nombreVilles;
    long nombreIndividus;
    long** pop;
    long *meilleurIndividu;
    long nombreSimulation;

public:
    PVC();

    void nouveau(long i, long nb_indiv);
    void reload(long nb_villes, long nb_indiv);
    void changerDepart(long i);

    void afficherMeilleur();
    void randomPop();
    void methodeElitisme();    
    void methodeTournoi();
    void methodeHasard();

    void croisement(long* alive);
    void mutation(long nombreMutations);
    void restaureBest();
    void deleteDoublons(long debut, long largeur, long* fils, long alive);

    void setNom(long i, QString nouveau);
    void setX(long i, double k);
    void setY(long i, double k);
    long getNombreVilles();
    double getX(long i);
    double getY(long i);

    double distance(long j);
    void melanger(long* cities);

    QString getNom(long i);
    QString fin();
};

#endif // PVC_H
