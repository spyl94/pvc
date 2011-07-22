#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <iostream>


class MaFenetre : public QWidget
{
    Q_OBJECT

    public:
    MaFenetre();

    public slots:
    void ouvrirDialogue();

    private:
    QPushButton *m_boutonDialogue;
};

struct t_cities{
    QString nom;
    long x;
    long y;
};

void traitement_ligne(QString ligne, long nb_ligne);
#endif // MAFENETRE_H
