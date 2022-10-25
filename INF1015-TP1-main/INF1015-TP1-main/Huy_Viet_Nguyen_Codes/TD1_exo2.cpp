/**
* Programme qui calcule le nombre de mois nécessaires pour rembourser un emprunt et la somme des intérêts payés au prêteur à partir
* de la somme d'argent empruntée, du taux d'intérêts annuel et du montant remboursé chaque mois.
* \file   TD1_exercice_2.cpp
* \author Huy Viet Nguyen et Reda R
* \date   30 janvier 2022
* Créé le 19 janvier 2022
*/

#include <iostream>
using namespace std;


double emprunterSommeArgent();
double indiquerTauxAnnuel(); //Le taux annuel est en pourcentage.
double indiquerMontantRembourseMensuel();
bool estSolvable(double pret, double tauxAnnuel, double montantRembourseMensuel);
double calculerInteretMensuel(double pret, double tauxAnnuel);


int main()
{
	double pret = emprunterSommeArgent();
	double tauxAnnuel = indiquerTauxAnnuel();
	double montantRembourseMensuel = indiquerMontantRembourseMensuel();

	if (estSolvable)
	{
		int nombreMois = 0;
		double interetPaye = 0;

		while (pret > 0)
		{
			nombreMois++;
			interetPaye += calculerInteretMensuel(pret, tauxAnnuel);
			pret = pret + calculerInteretMensuel(pret, tauxAnnuel) - montantRembourseMensuel;
		}
		cout << "Le pret a ete rembourser apres " << nombreMois << " mois et les interets payes sont de " << interetPaye << "$.";
	}
	
	else
		cout << "Il est impossible de rembourser le pret, car les interets sont superieurs ou egale au montant rembourse chaque mois.";
		
	return 0;
}


double emprunterSommeArgent()
{
	double pret;
	cout << "Veuillez indiquer le montant emprunte: ";
	cin >> pret;

	for (;;)
		if (pret <= 0)
		{
			cout << "Veuillez indiquer un montant superieur a 0$: ";
			cin >> pret;
		}
		else
			break;

	return pret;
}

double indiquerTauxAnnuel()
{
	double tauxAnnuel;
	cout << "Veuillez indiquer le taux annuel: ";
	cin >> tauxAnnuel;

	for (;;)
		if (tauxAnnuel < 0 || tauxAnnuel > 100)
		{
			cout << "Veuillez indiquer un taux annuel entre 0 et 100: ";
			cin >> tauxAnnuel;
		}
		else
			break;

	return tauxAnnuel;
}

double indiquerMontantRembourseMensuel()
{
	double montantRenbourseMensuel;
	cout << "Veuillez indiquer le montant rembourse: ";
	cin >> montantRenbourseMensuel;

	for (;;)
		if (montantRenbourseMensuel <= 0)
		{
			cout << "Veuillez indiquer un montant superieur a 0$: ";
			cin >> montantRenbourseMensuel;
		}
		else
			break;

	return montantRenbourseMensuel;
}

bool estSolvable(double pret, double tauxAnnuel, double montantRembourseMensuel)
{
	return (calculerInteretMensuel(pret, tauxAnnuel) <= montantRembourseMensuel) ? true : false;
}

double calculerInteretMensuel(double pret, double tauxAnnuel)
{
	return pret * tauxAnnuel / 1200;
}
