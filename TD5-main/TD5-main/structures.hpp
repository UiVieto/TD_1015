// Solutionnaire du TD3 INF1015 hiver 2021
// Par Francois-R.Boyer@PolyMtl.ca
#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	void detruire(bool possedeLesFilms = false);
	Film*& operator[] (int index) { return elements[index]; }
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template<typename T>
class IterateurListe;

template <typename T>
class Liste {
public:
	Liste() = default;
	explicit Liste(int capaciteInitiale) :  // explicit n'est pas matière à ce TD, mais c'est un cas où c'est bon de l'utiliser, pour ne pas qu'il construise implicitement une Liste à partir d'un entier, par exemple "maListe = 4;".
		capacite_(capaciteInitiale),
		elements_(make_unique<shared_ptr<T>[]>(capacite_))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite_(autre.nElements_),
		nElements_(autre.nElements_),
		elements_(make_unique<shared_ptr<T>[]>(nElements_))
	{
		for (int i = 0; i < nElements_; ++i)
			elements_[i] = autre.elements_[i];
	}
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simple avec unique_ptr...
	Liste<T>& operator= (Liste<T>&&) noexcept = default;  // Utilisé pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements_ < capacite_);  // Comme dans le TD précédent, on ne demande pas la réallocation pour ListeActeurs...
		elements_[nElements_++] = move(element);
	}

	// Noter que ces accesseurs const permettent de modifier les éléments; on pourrait vouloir des versions const qui retournent des const shared_ptr, et des versions non const qui retournent des shared_ptr.
	shared_ptr<T>& operator[] (int index) const { return elements_[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements_.get(), nElements_); }

	IterateurListe<T> begin();
	IterateurListe<T> end();

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
using ListeActeurs = Liste<Acteur>;

template<typename T>
class IterateurListe
{
public:
	IterateurListe(Liste<T>* pointeurliste, int position = NULL);

	shared_ptr<T>& operator*();
	IterateurListe<T>& operator++();
	bool operator==(const IterateurListe<T>& iterateur) const;
	bool operator!=(const IterateurListe<T>& iterateur) const;
private:
	int position_;
	Liste<T>* pointeurListe_;
	friend class Liste<T>;
};

class Affichable
{
public:
	virtual ostream& afficher(ostream& os) const = 0;
};

class Item : public Affichable {
public:
	Item() = default;
	Item(const Item& item) = delete;
	virtual ~Item() = default;

	virtual ostream& afficher(ostream& os) const = 0;
	friend ostream& operator<< (ostream& os, const Item& item);

	string titre = "Item";
	int anneeSortie = 0;
};


struct Film : virtual public Item
{
	Film() = default;
	Film(const Film& film);
	virtual ~Film() = default;
	ostream& afficher(ostream& os) const override;

	string realisateur; // Nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int recette = 0; // Recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Livre : virtual public Item
{
	virtual ~Livre() = default;
	ostream& afficher(ostream& os) const override;
	string auteur;
	int ventes = 0, nPages = 0;
};

struct FilmLivre : public Film, public Livre
{
	FilmLivre(const FilmLivre& filmLivre);
	FilmLivre(const Film& film, const Livre& livre);
	virtual ~FilmLivre() = default;
	ostream& afficher(ostream& os) const override;
};


struct Acteur
{
	string nom; int anneeNaissance = 0; char sexe = '\0';
};

struct ComparateurItem {
	bool operator () (const shared_ptr<Item>& unItem, const shared_ptr<Item>& autreItem) const
	{
		return unItem->titre < autreItem->titre;
	}
};
