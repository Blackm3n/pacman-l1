#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <array>

class affichage_pacman {
public:
	enum class direction {
		droite,
		bas,
		gauche,
		haut
	};
	enum class type_fantome {
		normal,
		peureux
	};
	enum class action_joueur {
		droite,
		bas,
		gauche,
		haut,
		rien
	};
	enum class type_mur {
		normal,
		accessible_pacman,
		accessible_fantome
	};
	enum class type_pacgomme {
		normale,
		super
	};
	enum class type_resultat {
		gagne,
		perdu
	};

public:
	affichage_pacman(unsigned int largeur, unsigned int hauteur, std::filesystem::path const& chemin_sprites, unsigned int zoom);
	bool fenetre_ouverte() const;
	bool recuperer_action_joueur(action_joueur& aj);
	bool recuperer_clic_souris(unsigned int& x, unsigned int& y);
	void affichage_commencer();
	void afficher_pacman(unsigned int x, unsigned int y, signed int dec, direction d);
	void afficher_fantome(unsigned int x, unsigned int y, signed int dec, direction d, type_fantome tf);
	void afficher_pacgomme(unsigned int x, unsigned int y, type_pacgomme tg);
	void afficher_mur(unsigned int x, unsigned int y, type_mur tm = type_mur::normal);
	void afficher_fruit(unsigned int x, unsigned int y, signed int dec = 0, direction d = direction::droite);
	void afficher_invincibilite(float i);
	void afficher_score(unsigned int s);
	void afficher_resultat(type_resultat t);
	void afficher_chemin_element(unsigned int x, unsigned int y);
	void affichage_terminer();

private:
	std::pair<float, float> xyd_vers_xy(unsigned int x, unsigned int y, signed int dec, direction d);

private:
	sf::RenderWindow _fenetre;
	sf::View _vue;
	sf::Texture _texture;
	sf::Font _font;
	std::array<sf::Sprite, 4> _sprites_pacman;
	std::array<sf::Sprite, 2> _sprites_fantomes;
	sf::Sprite _sprite_fruit;
	std::array<action_joueur, 1> _action_joueur;
	bool _clic_attente;
	unsigned int _clic_x;
	unsigned int _clic_y;
	unsigned int _chemin_courant_x;
	unsigned int _chemin_courant_y;
	static const unsigned int _largeur_sprite = 20;
	static const unsigned int _largeur_case = 26;
};
