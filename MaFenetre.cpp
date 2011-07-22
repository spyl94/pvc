// MaFenetre.cpp

#include "MaFenetre.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


MaFenetre::MaFenetre() : QWidget()
{
    //setFixedSize(230, 120);

    m_boutonDialogue = new QPushButton("Ouvrir la boîte de dialogue", this);
    m_boutonDialogue->move(40, 50);
    this->setWindowTitle("Projet 4, le voyageur de commerce");

    QObject::connect(m_boutonDialogue, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
}

void MaFenetre::ouvrirDialogue()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier texte", QString(), "Text files (*.txt)");
    QFile file(fichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Fichier", "Impossible d'ouvrir: " + fichier);
    }
    QTextStream flux(&file);

    QString ligne;
    long nb_ligne=0;
    while(! flux.atEnd())
    {
        ligne = file.readLine();
        if (ligne != NULL)
        {
            nb_ligne++;
            ligne = ligne.trimmed();
            qDebug() << ligne;
            //traitement_ligne(ligne, nb_ligne);
        }

    }

}

void traitement_ligne(QString ligne, long nb_ligne)
{
    if(nb_ligne==1)
    {
        qDebug() << "premiere ligne";
    }
}

