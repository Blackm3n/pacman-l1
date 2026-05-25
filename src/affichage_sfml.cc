#include "affichage_sfml.hh"
#include <iostream>
#include <algorithm>

affichage_pacman::affichage_pacman(unsigned int largeur, unsigned int hauteur, const std::filesystem::path& chemin_sprites, unsigned int zoom)
	: _fenetre(sf::VideoMode(largeur * _largeur_case * zoom, hauteur * _largeur_case * zoom), "Pacman++")
	, _vue(sf::FloatRect(0, 0, largeur * _largeur_case, hauteur * _largeur_case))
	, _clic_attente(false)
	, _clic_x(0)
	, _clic_y(0) {
	_fenetre.setView(_vue);
	_fenetre.setKeyRepeatEnabled(false);
	_fenetre.setVerticalSyncEnabled(false);
	_fenetre.setFramerateLimit(60);
	if (!_texture.loadFromFile((chemin_sprites / "sprites.png").string())) {
		std::cerr << "Fichier sprites.png introuvable.\nVous avez passé " << chemin_sprites << " comme paramètre à affichage_pacman. Il faut passer le répertoire contenant le fichier sprites.png.\n";
		exit(1);
	}
	for (unsigned int i = 0; i < 4; ++i) {
		_sprites_pacman[i].setTexture(_texture);
		_sprites_pacman[i].setTextureRect(sf::IntRect(static_cast<int>(i) * _largeur_sprite + _largeur_sprite * 2, 0, _largeur_sprite, _largeur_sprite));
	}
	for (unsigned int i = 0; i < 2; ++i) {
		_sprites_fantomes[i].setTexture(_texture);
		_sprites_fantomes[i].setTextureRect(sf::IntRect(static_cast<int>(i) * _largeur_sprite, 0, _largeur_sprite, _largeur_sprite));
	}
	_sprite_fruit.setTexture(_texture);
	_sprite_fruit.setTextureRect(sf::IntRect(_largeur_sprite * 6, 0, _largeur_sprite, _largeur_sprite));
	if (!_font.loadFromFile((chemin_sprites / "PressStart2P.ttf").string())) {
		std::cerr << "Fichier PressStart2P.ttg introuvable.\nVous avez passé " << chemin_sprites << " comme paramètre à affichage_pacman. Il faut passer le répertoire contenant le fichier PressStart2P.ttf.\n";
		exit(1);
	}
}

bool affichage_pacman::fenetre_ouverte() const {
	return _fenetre.isOpen();
}

bool affichage_pacman::recuperer_action_joueur(action_joueur& aj) {
	_action_joueur.fill(action_joueur::rien);
	sf::Event ev;
	while (_fenetre.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed)
			_fenetre.close();
		else if (ev.type == sf::Event::KeyPressed) {
			if (ev.key.code == sf::Keyboard::Left)
				_action_joueur[0] = action_joueur::gauche;
			else if (ev.key.code == sf::Keyboard::Right)
				_action_joueur[0] = action_joueur::droite;
			else if (ev.key.code == sf::Keyboard::Up)
				_action_joueur[0] = action_joueur::haut;
			else if (ev.key.code == sf::Keyboard::Down)
				_action_joueur[0] = action_joueur::bas;
		} else if (ev.type == sf::Event::MouseButtonPressed) {
			if (ev.mouseButton.button == sf::Mouse::Left) {
				_clic_attente = true;
				auto zoom = static_cast<unsigned int>(_fenetre.getSize().x / _vue.getSize().x);
				_clic_x = ev.mouseButton.x / zoom / _largeur_case;
				_clic_y = ev.mouseButton.y / zoom / _largeur_case;
			}
		}
	}
	if (_action_joueur[0] == action_joueur::rien)
		return false;
	else {
		aj = _action_joueur[0];
		return true;
	}
}

bool affichage_pacman::recuperer_clic_souris(unsigned int& x, unsigned int& y) {
	if (_clic_attente) {
		x = _clic_x;
		y = _clic_y;
		_clic_attente = false;
		return true;
	} else
		return false;
}

void affichage_pacman::affichage_commencer() {
	_fenetre.clear(sf::Color::Black);
}

void affichage_pacman::afficher_pacman(unsigned int x, unsigned int y, signed int dec, direction d) {
	if ((dec <= static_cast<signed int>(-_largeur_case) / 2) || (dec > static_cast<signed int>(_largeur_case) / 2)) {
		std::cerr << "afficher_pacman : dec doit être dans ]" << -_largeur_case << "," << _largeur_case << "]\n";
		exit(1);
	}
	auto isprite(static_cast<std::size_t>(d));
	auto axy(xyd_vers_xy(x, y, dec, d));
	_sprites_pacman[isprite].setPosition(axy.first, axy.second);
	_fenetre.draw(_sprites_pacman[isprite]);
}

void affichage_pacman::afficher_fantome(unsigned int x, unsigned int y, signed int dec, direction d, type_fantome tf) {
	if ((dec <= static_cast<signed int>(-_largeur_case) / 2) || (dec > static_cast<signed int>(_largeur_case) / 2)) {
		std::cerr << "afficher_fantome : dec doit être dans ]" << -_largeur_case << "," << _largeur_case << "]\n";
		exit(1);
	}
	auto isprite(static_cast<std::size_t>(tf));
	auto axy(xyd_vers_xy(x, y, dec, d));
	_sprites_fantomes[isprite].setPosition(axy.first, axy.second);
	_fenetre.draw(_sprites_fantomes[isprite]);
}

void affichage_pacman::afficher_pacgomme(unsigned int x, unsigned int y, type_pacgomme tg) {
	sf::Color couleur_pg(sf::Color::Green);
	unsigned int largeur(2);
	if (tg == type_pacgomme::super) {
		couleur_pg = sf::Color::Red;
		largeur *= 2;
	}
	sf::RectangleShape rs(sf::Vector2f(static_cast<float>(largeur), static_cast<float>(largeur)));
	rs.setFillColor(couleur_pg);
	rs.setPosition(static_cast<float>(x * _largeur_case + _largeur_case / 2 - largeur / 2), static_cast<float>(y * _largeur_case + _largeur_case / 2 - largeur / 2));
	_fenetre.draw(rs);
}

void affichage_pacman::afficher_mur(unsigned int x, unsigned int y, type_mur tm) {
	sf::RectangleShape rs(sf::Vector2f(_largeur_case, _largeur_case));
	switch (tm) {
	case type_mur::normal:
		rs.setFillColor(sf::Color::Blue);
		break;
	case type_mur::accessible_pacman:
		rs.setFillColor(sf::Color::Cyan);
		break;
	case type_mur::accessible_fantome:
		rs.setFillColor(sf::Color::Magenta);
		break;
	}
	rs.setPosition(static_cast<float>(x * _largeur_case), static_cast<float>(y * _largeur_case));
	_fenetre.draw(rs);
}

void affichage_pacman::afficher_fruit(unsigned int x, unsigned int y, signed dec, direction d) {
	auto axy(xyd_vers_xy(x, y, dec, d));
	_sprite_fruit.setPosition(axy.first, axy.second);
	_fenetre.draw(_sprite_fruit);
}

void affichage_pacman::afficher_invincibilite(float i) {
	sf::RectangleShape rs(sf::Vector2f(_largeur_case * 2, _largeur_case / 2));
	rs.setFillColor(sf::Color::Black);
	rs.setPosition(_vue.getSize().x - _largeur_case / 4 - _largeur_case * 2, _largeur_case / 4);
	_fenetre.draw(rs);
	rs.setFillColor(sf::Color::Yellow);
	rs.setPosition(_vue.getSize().x - _largeur_case / 4 - _largeur_case * 2 + 1, _largeur_case / 4 + 1);
	rs.setSize(sf::Vector2f(std::min(static_cast<float>(1), i) * (_largeur_case * 2 - 2), _largeur_case / 2 - 2));
	_fenetre.draw(rs);
}

void affichage_pacman::afficher_score(unsigned int s) {
	sf::Text text;
	text.setFont(_font);
	text.setString(std::to_string(s));
	text.setCharacterSize(8);
	text.setFillColor(sf::Color::White);
	text.setPosition(_largeur_case / 4, _largeur_case / 2 - 4);
	_fenetre.draw(text);
}

void affichage_pacman::afficher_resultat(type_resultat t) {
	sf::Text text;
	text.setFont(_font);
	switch (t) {
	case type_resultat::gagne:
		text.setString("VOUS AVEZ GAGNE !");
		break;
	case type_resultat::perdu:
		text.setString("GAME OVER");
		break;
	}
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.width / 2, textBounds.height / 2);
	text.setPosition(_vue.getSize().x / 2, _vue.getSize().y / 2);
	_fenetre.draw(text);
}

void affichage_pacman::afficher_chemin_element(unsigned int x, unsigned int y) {
	_chemin_courant_x = x;
	_chemin_courant_y = y;
	const int largeur(8);
	sf::RectangleShape rs(sf::Vector2f(static_cast<float>(largeur), static_cast<float>(largeur)));
	rs.setFillColor(sf::Color(255, 255, 255, 128));
	rs.setPosition(static_cast<float>(_chemin_courant_x * _largeur_case + _largeur_case / 2 - largeur / 2), static_cast<float>(y * _largeur_case + _largeur_case / 2 - largeur / 2));
	_fenetre.draw(rs);
}

void affichage_pacman::affichage_terminer() {
	_fenetre.display();
}

std::pair<float, float> affichage_pacman::xyd_vers_xy(unsigned int x, unsigned int y, signed int dec, direction d) {
	std::pair<float, float> res(static_cast<float>(x) * _largeur_case + (_largeur_case - _largeur_sprite) / 2, static_cast<float>(y) * _largeur_case + (_largeur_case - _largeur_sprite) / 2);
	switch (d) {
	case direction::droite:
	case direction::gauche:
		res.first += dec;
		break;
	case direction::bas:
	case direction::haut:
		res.second += dec;
		break;
	}
	return res;
}
