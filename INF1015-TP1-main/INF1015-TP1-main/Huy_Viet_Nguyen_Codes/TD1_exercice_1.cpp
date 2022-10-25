/**
* Programme qui détermine si un nombre entier donné est premier ou non.
* \file   TD1_exercice_1.cpp
* \author Huy Viet Nguyen et Reda R
* \date   30 janvier 2022
* Créé le 18 janvier 2022
*/

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


int prendreNombreEntier();
bool estDivisible(int dividende, int diviseur);
vector<int> trouverDiviseurPertinent(int nombreEntier);


int main()
{
	int nombreEntierDonne = prendreNombreEntier();
	
	if (nombreEntierDonne == 1)
	{
		cout << "1 n'est pas un nombre premier, car il n'est divisible que par une seule valeur entiere positive et non deux, soit lui-meme.";
	}

	else
	{
		for (int i : trouverDiviseurPertinent(nombreEntierDonne))
		{
			if (estDivisible(nombreEntierDonne, i) && i != nombreEntierDonne)
			{
				cout << nombreEntierDonne << " n'est pas premier car il est divisible par " << i << ".";
				break;
			}
		}
		cout << nombreEntierDonne <<" est premier.";
	}
	return 0;
}


int prendreNombreEntier()
{
	int nombreEntier;
	cout << "Veuillez entrer un nombre entier:";
	cin >> nombreEntier;

	return nombreEntier;
}

bool estDivisible(int dividende, int diviseur)
{
	return (dividende % diviseur == 0) ? true : false;
}

vector<int> trouverDiviseurPertinent(int nombreEntier)
{	
	vector<int> diviseurs;
		
	if (nombreEntier % 2 == 0)
		diviseurs = { 2 };
	else
	{
		diviseurs = { 3 };
		{
			for (int i = 5; i <= int(sqrt(nombreEntier)); i += 2)
				diviseurs.push_back(i);
		}
	}

	return diviseurs;
}
