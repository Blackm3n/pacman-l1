#include "affichage_sfml.hh"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

namespace {
	struct Position {
		unsigned int x{};
		unsigned int y{};
	};

	struct Ghost {
		Position position{};
		affichage_pacman::direction direction{affichage_pacman::direction::droite};
	};

	bool is_walkable(char cellule) {
		return cellule != '*';
	}

	affichage_pacman::direction opposite(affichage_pacman::direction d) {
		switch (d) {
		case affichage_pacman::direction::droite:
			return affichage_pacman::direction::gauche;
		case affichage_pacman::direction::gauche:
			return affichage_pacman::direction::droite;
		case affichage_pacman::direction::haut:
			return affichage_pacman::direction::bas;
		case affichage_pacman::direction::bas:
			return affichage_pacman::direction::haut;
		}
		return affichage_pacman::direction::droite;
	}

	void avancer(Position& pos, affichage_pacman::direction d) {
		switch (d) {
		case affichage_pacman::direction::droite:
			++pos.x;
			break;
		case affichage_pacman::direction::gauche:
			--pos.x;
			break;
		case affichage_pacman::direction::bas:
			++pos.y;
			break;
		case affichage_pacman::direction::haut:
			--pos.y;
			break;
		}
	}

	bool dans_la_grille(const Position& pos, unsigned int largeur, unsigned int hauteur) {
		return pos.x < largeur && pos.y < hauteur;
	}
}

int main(int argc, char** argv) {
	const std::filesystem::path racine = (argc > 0)
		? std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path()
		: std::filesystem::current_path();
	const std::filesystem::path niveau = racine / "jeu1.txt";

	std::ifstream entree(niveau);
	if (!entree) {
		std::cerr << "Impossible d'ouvrir " << niveau << "\n";
		return 1;
	}

	unsigned int largeur = 0;
	unsigned int hauteur = 0;
	entree >> largeur >> hauteur;
	entree.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::vector<std::string> carte;
	carte.reserve(hauteur);
	for (unsigned int y = 0; y < hauteur; ++y) {
		std::string ligne;
		std::getline(entree, ligne);
		if (ligne.size() < largeur) {
			ligne.resize(largeur, ' ');
		}
		carte.push_back(ligne);
	}

	Position pacman{};
	entree >> pacman.x >> pacman.y;

	unsigned int nombre_fantomes = 0;
	entree >> nombre_fantomes;
	std::vector<Ghost> fantomes;
	fantomes.reserve(nombre_fantomes);
	for (unsigned int i = 0; i < nombre_fantomes; ++i) {
		Ghost fantome;
		entree >> fantome.position.x >> fantome.position.y;
		fantome.direction = affichage_pacman::direction::droite;
		fantomes.push_back(fantome);
	}

	std::vector<std::vector<bool>> pacgommes(hauteur, std::vector<bool>(largeur, false));
	unsigned int reste_a_manger = 0;
	for (unsigned int y = 0; y < hauteur; ++y) {
		for (unsigned int x = 0; x < largeur; ++x) {
			if (carte[y][x] == '.' || carte[y][x] == 'S') {
				pacgommes[y][x] = true;
				++reste_a_manger;
			}
		}
	}

	std::array<affichage_pacman::direction, 4> directions = {
		affichage_pacman::direction::droite,
		affichage_pacman::direction::bas,
		affichage_pacman::direction::gauche,
		affichage_pacman::direction::haut,
	};
	affichage_pacman::direction direction_courante = affichage_pacman::direction::droite;
	affichage_pacman::direction direction_demandee = direction_courante;

	std::mt19937 generateur(std::random_device{}());
	std::uniform_int_distribution<std::size_t> choix_direction(0, directions.size() - 1);

	affichage_pacman aff(largeur, hauteur, racine, 3);
	unsigned int score = 0;
	bool gagne = false;
	bool perdu = false;

	while (aff.fenetre_ouverte() && !gagne && !perdu) {
		affichage_pacman::action_joueur action;
		unsigned int clicx = 0;
		unsigned int clicy = 0;

		if (aff.recuperer_action_joueur(action)) {
			switch (action) {
			case affichage_pacman::action_joueur::droite:
				direction_demandee = affichage_pacman::direction::droite;
				break;
			case affichage_pacman::action_joueur::gauche:
				direction_demandee = affichage_pacman::direction::gauche;
				break;
			case affichage_pacman::action_joueur::haut:
				direction_demandee = affichage_pacman::direction::haut;
				break;
			case affichage_pacman::action_joueur::bas:
				direction_demandee = affichage_pacman::direction::bas;
				break;
			case affichage_pacman::action_joueur::rien:
				break;
			}
		}

		if (aff.recuperer_clic_souris(clicx, clicy) && dans_la_grille({clicx, clicy}, largeur, hauteur)) {
			pacman = {clicx, clicy};
		}

		Position prochaine = pacman;
		avancer(prochaine, direction_demandee);
		if (dans_la_grille(prochaine, largeur, hauteur) && is_walkable(carte[prochaine.y][prochaine.x])) {
			pacman = prochaine;
			direction_courante = direction_demandee;
		}

		if (pacgommes[pacman.y][pacman.x]) {
			pacgommes[pacman.y][pacman.x] = false;
			--reste_a_manger;
			++score;
		}

		for (auto& fantome : fantomes) {
			std::vector<affichage_pacman::direction> directions_possibles;
			for (const auto direction : directions) {
				Position candidate = fantome.position;
				avancer(candidate, direction);
				if (dans_la_grille(candidate, largeur, hauteur) && is_walkable(carte[candidate.y][candidate.x])) {
					directions_possibles.push_back(direction);
				}
			}
			if (!directions_possibles.empty()) {
				affichage_pacman::direction choix = directions_possibles[choix_direction(generateur) % directions_possibles.size()];
				fantome.direction = choix;
				avancer(fantome.position, choix);
			}
			if (fantome.position.x == pacman.x && fantome.position.y == pacman.y) {
				perdu = true;
			}
		}

		if (reste_a_manger == 0) {
			gagne = true;
		}

		aff.affichage_commencer();
		for (unsigned int y = 0; y < hauteur; ++y) {
			for (unsigned int x = 0; x < largeur; ++x) {
				if (carte[y][x] == '*') {
					aff.afficher_mur(x, y);
				}
				if (pacgommes[y][x]) {
					aff.afficher_pacgomme(x, y, carte[y][x] == 'S' ? affichage_pacman::type_pacgomme::super : affichage_pacman::type_pacgomme::normale);
				}
			}
		}

		for (const auto& fantome : fantomes) {
			aff.afficher_fantome(fantome.position.x, fantome.position.y, 0, fantome.direction, affichage_pacman::type_fantome::normal);
		}

		aff.afficher_pacman(pacman.x, pacman.y, 0, direction_courante);
		aff.afficher_score(score);
		aff.affichage_terminer();
	}

	if (gagne) {
		aff.affichage_commencer();
		for (unsigned int y = 0; y < hauteur; ++y) {
			for (unsigned int x = 0; x < largeur; ++x) {
				if (carte[y][x] == '*') {
					aff.afficher_mur(x, y);
				}
			}
		}
		aff.afficher_resultat(affichage_pacman::type_resultat::gagne);
		aff.affichage_terminer();
		while (aff.fenetre_ouverte()) {
			affichage_pacman::action_joueur action;
			if (aff.recuperer_action_joueur(action)) {
				(void)action;
			}
		}
	}

	if (perdu) {
		aff.affichage_commencer();
		for (unsigned int y = 0; y < hauteur; ++y) {
			for (unsigned int x = 0; x < largeur; ++x) {
				if (carte[y][x] == '*') {
					aff.afficher_mur(x, y);
				}
			}
		}
		aff.afficher_resultat(affichage_pacman::type_resultat::perdu);
		aff.affichage_terminer();
		while (aff.fenetre_ouverte()) {
			affichage_pacman::action_joueur action;
			if (aff.recuperer_action_joueur(action)) {
				(void)action;
			}
		}
	}

	return 0;
}
