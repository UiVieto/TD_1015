/**
* Programme qui crée un dictionnaire en tant qu'array de strutures à partir d'un
* fichier texte et qui affiche le mot le plus long du dictionnaire.
* \file   TD1_exercice_6.cpp
* \author Huy Viet Nguyen et Reda Rhanmouni (Section 5)
* \date   30 janvier 2022
* Créé le 23 janvier 2022
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Mot 
{
    string mot;
    string nature;
    string definition;
};

int trouverNombreMotsDictionnaire();
vector<Mot> creerDictionnaire();
Mot trouverMotPlusLong(vector<Mot>* dictionnaire);
void afficherMot(Mot* mot);


int main()
{
    vector<Mot> dictionnaire = creerDictionnaire();
    Mot motPlusLong = trouverMotPlusLong(&dictionnaire);

    afficherMot(&motPlusLong);

    return 0;
}


int trouverNombreMotsDictionnaire()
{
    ifstream fichier("dictionnaire.txt");
    string ligne;
    int nombreMots = 0;

    while (std::getline(fichier, ligne))
    {
        nombreMots++;
    }

    return nombreMots;
}

vector<Mot> creerDictionnaire()
{
    ifstream fichier("dictionnaire.txt");
    string element;

    vector<Mot> dictionnaire;

    string mot;
    string nature;
    string definition;

    for (int i = 1; i <= trouverNombreMotsDictionnaire(); i++)
    {
        Mot nouveauMot;

        std::getline(fichier, mot, '\t');
        nouveauMot.mot = mot;

        std::getline(fichier, nature, '\t');
        nouveauMot.nature = nature;

        std::getline(fichier, definition, '\n');
        nouveauMot.definition = definition;

        (dictionnaire).push_back(nouveauMot);
    }

    return dictionnaire;
}

Mot trouverMotPlusLong(vector<Mot>* dictionnaire)
{   
    int nombreMots = trouverNombreMotsDictionnaire();
    Mot motPlusLong = (*dictionnaire)[0];

    for (int i = 1; i < nombreMots; i++)
    {
        if ((motPlusLong.mot).size() > (((*dictionnaire)[i]).mot).size())
            continue;
        else
            motPlusLong = (*dictionnaire)[i];
    }

    return motPlusLong;
}

void afficherMot(Mot* mot)
{
    cout << (*mot).mot << " (" << (*mot).nature << ") : " << (*mot).definition;
}
