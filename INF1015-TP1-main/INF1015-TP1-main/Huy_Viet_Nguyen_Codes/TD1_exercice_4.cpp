/**
* Programme qui approxime la valeur de pi par la méthode de monte Carlo
* \file   TD1_exercice_2.cpp
* \author Huy Viet Nguyen et Reda R
* \date   30 janvier 2022
* Créé le 21 janvier 2022
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

#define PI 3.141593

int prendreNombreIteration();
double genererValeurRandom();
double estimerPi(int nombrePointsInterieurCercle, int nombrePointsExterieurCercle);
double calculerEcartRelatif(double approximationPi);

void initialisation()
{
    srand(int(time(nullptr)));
}

int main()
{
    int nombreIterations = prendreNombreIteration();
    int nombrePointsInterieurCercle = 0;
    int nombrePointsExterieurCercle = 0;

    initialisation();

    while ((nombrePointsInterieurCercle + nombrePointsExterieurCercle) < nombreIterations)
    {
        double x = genererValeurRandom();
        double y = genererValeurRandom();

        if ((pow(x, 2) + pow(y, 2)) < 1)
        {
            nombrePointsInterieurCercle++;
            nombrePointsExterieurCercle++;
        }
         
        else
            nombrePointsExterieurCercle++;
    }

    double approximationPi = estimerPi(nombrePointsInterieurCercle, nombrePointsExterieurCercle);

    cout.precision(6);
    cout << "La valeur approximee de pi est " << approximationPi << " avec un ecart relatif de "
         << calculerEcartRelatif(approximationPi) << "%.";

    return 0;
}

double genererValeurRandom()
{

    return (__int64(rand() % 200) - 100) / 100.0;
}

int prendreNombreIteration()
{
    int nombreIterations;
    cout << "Veuillez donner le nombre d'iterations: ";
    cin >> nombreIterations;

    for (;;)
    {
        if (nombreIterations < 0)
        {
            cout << "Veuillez donner un nombre positif: ";
            cin >> nombreIterations;
        }
        else
            break;
    }
    return nombreIterations;
}

double estimerPi(int nombrePointsInterieurCercle, int nombrePointsExterieurCercle)
{
    return 4.0 * double(nombrePointsInterieurCercle) / double(nombrePointsExterieurCercle);
}

double calculerEcartRelatif(double approximationPi)
{
    return ((PI - approximationPi) / PI) * 100.0;
}
