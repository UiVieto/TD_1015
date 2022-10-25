/*Ce programme permet de se familiariser avec les méthodes virtuelles, les objets polymorphes, l’héritage simple et multiple.
  Complété à partir du Solutionnaire du TD3 INF1015 hiver 2021 de Francois-R.Boyer@PolyMtl.ca
					Reda Rhanmouni  2087548
					Huy Viet Nguyen 2136378                   */


#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <vector>
#include <forward_list>
#include <map>
#include <set>

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).
using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}


void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film * [nouvelleCapacite];

	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements_ sera zéro si elements_ est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}

	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}

span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}


shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->acteurs.enSpan()) {
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}

shared_ptr<Acteur> lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = lireUint16(fichier);
	acteur.sexe = lireUint8(fichier);

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(move(acteur));  // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->titre = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = lireUint16(fichier);
	film->recette = lireUint16(fichier);
	auto nActeurs = lireUint8(fichier);
	film->acteurs = ListeActeurs(nActeurs);  // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	cout << "Création Film " << film->titre << endl;

	for ([[maybe_unused]] auto i : range(nActeurs)) {  // On peut aussi mettre nElements_ avant et faire un span, comme on le faisait au TD précédent.
		film->acteurs.ajouter(lireActeur(fichier, listeFilms));
	}

	return film;
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = lireUint16(fichier);

	ListeFilms listeFilms;
	for ([[maybe_unused]] int i : range(nElements)) { //NOTE: On ne peut pas faire un span simple avec ListeFilms::enSpan car la liste est vide et on ajoute des éléments à mesure.
		listeFilms.ajouterFilm(lireFilm(fichier, listeFilms));
	}

	return listeFilms;
}

void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film* film : enSpan())
			delete film;
	delete[] elements;
}


ostream& operator<< (ostream& os, const Acteur& acteur)
{
	return os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

// Fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
//[
ostream& operator<< (ostream& os, const Film& film)
{
	os << "Titre: " << film.titre << endl;
	os << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
	os << "  Recette: " << film.recette << "M$" << endl;

	os << "Acteurs:" << endl;
	for (const shared_ptr<Acteur>& acteur : film.acteurs.enSpan())
		os << *acteur;
	return os;
}
//]

// Pas demandé dans l'énoncé de tout mettre les affichages avec surcharge, mais pourquoi pas.
ostream& operator<< (ostream& os, const ListeFilms& listeFilms)
{
	static const string ligneDeSeparation = //[
		"\033[32m────────────────────────────────────────\033[0m\n";
	os << ligneDeSeparation;
	for (const Film* film : listeFilms.enSpan()) {
		os << *film << ligneDeSeparation;
	}
	return os;
}

ostream& Item::afficher(ostream& os) const {
	os << titre;

	return os;
}

ostream& Film::afficher(ostream& os) const {
	Item::afficher(os);
	os << ", par " << realisateur << endl;
	return os;
}

ostream& Livre::afficher(ostream& os) const {
	Item::afficher(os);
	os << ", de " << auteur << endl;

	return os;
}

void ajouterLivres(string nomFichierLivres, vector<shared_ptr<Item>>& bibliotheque)
{
	ifstream Livres(nomFichierLivres);
	string element;
	while (getline(Livres, element, '\t'))
	{
		cout << "Creation du livre: " << element << endl;
		shared_ptr<Livre> livre = make_unique<Livre>();

		livre->titre = element;

		getline(Livres, element, '\t');
		livre->anneeSortie = stoi(element);

		getline(Livres, element, '\t');
		livre->auteur = element;

		getline(Livres, element, '\t');
		livre->ventes = stoi(element);

		getline(Livres, element, '\n');
		livre->nPages = stoi(element);

		cout << "Ajout de:" << livre->titre << endl;
		bibliotheque.push_back(move(livre));
	}
}

Film::Film(const Film& film)
{
	this->acteurs = Liste(film.acteurs);
	this->titre = film.titre;
	this->anneeSortie = film.anneeSortie;
	this->realisateur = film.realisateur;
	this->recette = film.recette;
}

ostream& operator<< (ostream& os, const Item& item) {
	return item.afficher(os);
}

FilmLivre::FilmLivre(const Film& film, const Livre& livre)
{
	this->acteurs = Liste(film.acteurs);
	this->anneeSortie = film.anneeSortie;
	this->titre = film.titre;
	this->realisateur = film.realisateur;
	this->recette = film.recette;
	this->auteur = livre.auteur;
	this->nPages = livre.nPages;
	this->ventes = livre.ventes;
}

ostream& FilmLivre::afficher(ostream& os) const
{
	os << Film::titre << ", ";
	os << "par " << realisateur << " de " << auteur << endl;

	return os;
}

//Fonction remplacée par une fonction générique.
//void afficherListeItems(vector<unique_ptr<Item>>& listeItems)
//{
	//for (unique_ptr<Item>& item : listeItems) {
		//cout << "=====================================================" << endl;
		//cout << *item;
	//}
//}

FilmLivre::FilmLivre(const FilmLivre& filmLivre)
{
	this->acteurs = Liste(filmLivre.acteurs);
	this->anneeSortie = filmLivre.anneeSortie;
	this->titre = filmLivre.titre;
	this->realisateur = filmLivre.realisateur;
	this->recette = filmLivre.recette;
	this->auteur = filmLivre.auteur;
	this->nPages = filmLivre.nPages;
	this->ventes = filmLivre.ventes;
}

/*--------------------------TD5--------------------------------*/
template<typename Conteneur>
void afficherListeItems(const Conteneur& conteneur) {
	for (const auto& element : conteneur)
		cout << *element;
}

template<typename T>
IterateurListe<T>::IterateurListe(Liste<T>* pointeurListe, int position) {
	pointeurListe_ = pointeurListe;
	position_ = position;
}

template<typename T>
shared_ptr<T>& IterateurListe<T>::operator*() {
	return (*pointeurListe_)[position_];
}

template<typename T>
IterateurListe<T>& IterateurListe<T>::operator++() {
	++position_;
	return *this;
}

template<typename T>
bool IterateurListe<T>::operator==(const IterateurListe<T>& iterateur) const {
	return position_ == iterateur.position_ && pointeurListe_ == iterateur.pointeurListe_;
}

template<typename T>
bool IterateurListe<T>::operator!=(const IterateurListe<T>& iterateur) const {
	return !(*this == iterateur);
}

template<typename T>
IterateurListe<T> Liste<T>::begin() {
	return IterateurListe<T>(this, NULL);
}

template<typename T>
IterateurListe<T> Liste<T>::end() {
	return IterateurListe<T>(this, nElements_);
}

bool operator<(const Item& item, const Item& autreItem) {
	return item.titre < autreItem.titre;
}


int main()
{
#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");

	/*---------------------TD4----------------------*/

	cout << ligneDeSeparation;
	cout << "Creation bibliotheque:" << endl;
	vector<shared_ptr<Item>> bibliotheque;

	for (Film* film : listeFilms.enSpan()) {
		cout << "Ajout de: " << film->titre << endl;
		bibliotheque.push_back(make_shared<Film>(*film));
	}

	ajouterLivres("Livres.txt", bibliotheque);
	cout << ligneDeSeparation;

	cout << "Affichage de la bibliotheque:" << endl;
	afficherListeItems(bibliotheque);
	cout << ligneDeSeparation;

	FilmLivre hobbit(*dynamic_cast<Film*>(bibliotheque[4].get()), *dynamic_cast<Livre*>(bibliotheque[9].get()));
	bibliotheque.push_back(make_shared<FilmLivre>(hobbit));
	cout << *bibliotheque[12];

	/*---------------------TD5----------------------*/
	//1. Listes liées et itérateurs
	forward_list<shared_ptr<Item>> listeItems(bibliotheque.begin(), bibliotheque.end());

	forward_list<shared_ptr<Item>> autreListeItems;

	/*Algorithme qui copie les elements de la bibliotheque dans une autre liste à l'aide du
	  push_front. Puisqu'on fait n push_front (n = nombre d'éléments dans la bibliotheque) et que
	  le push_front a une complexité constante, la complexité de l'algorithme est O(n).*/
	for (shared_ptr<Item> item : bibliotheque)
		autreListeItems.push_front(item);

	for (shared_ptr<Item> item : autreListeItems)
		cout << *item;

	cout << ligneDeSeparation;

	for (auto&& acteur : listeFilms[0]->acteurs)
		cout << *acteur;

	//2. Conteneurs
	cout << ligneDeSeparation;
	map<string, shared_ptr<Item>> bibliothequeTrie;

	for (shared_ptr<Item> item : bibliotheque)
		bibliothequeTrie[item->titre] = item;

	for (auto item : bibliothequeTrie)
		cout << *item.second;

	//2.1
	set<shared_ptr<Item>,ComparateurItem> bibliothequeOrdo;
	for (shared_ptr<Item> item : bibliotheque)
		bibliothequeOrdo.insert(item);



	//3. Algorithmes

	// Détruire tout avant de terminer le programme.
	listeFilms.detruire(true);

	return 0;
}
