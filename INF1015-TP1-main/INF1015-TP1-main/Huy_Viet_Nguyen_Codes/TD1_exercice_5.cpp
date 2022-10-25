/**
* Programme qui retourne un tableau avec les nombres pairs en premier à partir d'un tableau initial de taille 10.
* \file   TD1_exercice_5.cpp
* \author Huy Viet Nguyen et Reda Rhanmouni (Section 5)
* \date   30 janvier 2022
* Créé le 23 janvier 2022
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int TABLEAU_INITIAL[10] = { 7, 4, 7, 8, 4, 6, 3, 9, 6, 11 };

bool estPair(int nombreEntier);


int main()
{
    int tableauTrie[10];
    vector<int> nombreImpairs;
    int positionAjout = 0;

    cout << "Le tableau trie est: ";

    for (int i : TABLEAU_INITIAL)
    {
        if (estPair(i))
        {
            tableauTrie[positionAjout] = i;
            positionAjout++;
        }

        else
        {
            nombreImpairs.push_back(i);
        }
    }

    for (int i : nombreImpairs)
    {
        tableauTrie[positionAjout] = i;
        positionAjout++;
    }

    cout << "{ ";

    for (int i = 0; i < 8; i++)
    {
        cout << tableauTrie[i] << ", ";
    }

    cout << tableauTrie[9] << " }";

    return 0;
}


bool estPair(int nombreEntier)
{
    return (nombreEntier % 2 == 0) ? true : false;
}
