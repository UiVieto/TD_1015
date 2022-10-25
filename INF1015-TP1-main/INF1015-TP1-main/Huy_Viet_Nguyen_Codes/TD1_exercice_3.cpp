/**
* Programme qui détermine la hauteur finale d'une balle après un certain nombre de rebond.
* \file   TD1_exercice_3.cpp
* \author Huy Viet Nguyen et Reda Rhanmouni (Section 5)
* \date   30 janvier 2022
* Créé le 20 janvier 2022
*/

#include <iostream>
#include <cmath>
using namespace std;

#define CONSTANTE_GRAVITE 9,81 //m/s


double prendreHauteurInitiale();
double prendreCoeficientRebond();
int prendreNombreRebond();
double calculerVitesseChute(double hauteur);
double calculerVitesseRebond(double vitesseChute, double coeficientRebond);
double calculerHauteurRebond(double vitesseRebond);


int main()
{
    double hauteur = prendreHauteurInitiale();
    double coeficientRebond = prendreCoeficientRebond();
    int nombreRebond = prendreNombreRebond();
    double vitesseRebond;

    for (int i = 0; i < nombreRebond; i++)
    {
        vitesseRebond = calculerVitesseRebond(calculerVitesseChute(hauteur), coeficientRebond);
        hauteur = calculerHauteurRebond(vitesseRebond);
    }
    cout << "La hauteur de la balle apres " << nombreRebond << " rebonds est de " << hauteur << "m.";

    return 0;
}


double prendreHauteurInitiale()
{
    double hauteurInitiale;
    cout << "Veuillez indiquer la hauteur initiale: ";
    cin >> hauteurInitiale;

    for (;;)
        if (hauteurInitiale < 0)
        {
            cout << "Veuillez indiquer une hauteur initiale strictement positive: ";
            cin >> hauteurInitiale;
        }
        else
            break;

    return hauteurInitiale;
}           

int prendreNombreRebond()
{
    int nombreRebond;
    cout << "Veuillez indiquer le nombre de rebonds: ";
    cin >> nombreRebond;

    for (;;)
        if (nombreRebond < 0)
        {
            cout << "Veuillez indiquer un nombre entier de rebonds strictement positif: ";
            cin >> nombreRebond;
        }
        else
            break;

    return nombreRebond;
}

double prendreCoeficientRebond()
{
    int coeficientRebond;
    cout << "Veuillez indiquer le coeficient de rebond: ";
    cin >> coeficientRebond;

    for (;;)
        if (coeficientRebond < 0 || coeficientRebond > 1)
        {
            cout << "Veuillez indiquer un coeficient de rebond strictement entre 0 et 1: ";
            cin >> coeficientRebond;
        }
        else
            break;

    return coeficientRebond;
}


double calculerVitesseChute(double hauteur)
{
    return sqrt((2 * CONSTANTE_GRAVITE * hauteur));
}

double calculerVitesseRebond(double vitesseChute, double coeficientRebond)
{
    return vitesseChute * coeficientRebond;
}

double calculerHauteurRebond(double vitesseRebond)
{
    return pow(vitesseRebond, 2.0) / (2 * CONSTANTE_GRAVITE);
}
