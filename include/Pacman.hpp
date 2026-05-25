#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "affichage_sfml.hh"

class Board;

class Pacman {
private:
    int x, y;
    affichage_pacman::direction dir;
    int score;
    int invincibiliteTimer; // Chronomètre d'invincibilité

public:
    Pacman(int startX, int startY);
    void deplacer(affichage_pacman::action_joueur action, Board& plateau);
    void dessiner(affichage_pacman& ecran) const;
    
    int getScore() const;
    int getX() const;
    int getY() const;
    
    // Nouvelles méthodes pour le super-pouvoir
    void ajouterScore(int points);
    bool estInvincible() const;
    void decrementerInvincibilite();
    
    // NOUVEAU : Fonction pour lire le chronomètre
    int getTimerInvincibilite() const; 
};

#endif