#ifndef GHOST_HPP
#define GHOST_HPP

#include "affichage_sfml.hh"

class Board;

class Ghost {
private:
    int x, y;
    int startX, startY;
    affichage_pacman::direction dir;
    affichage_pacman::type_fantome type;
    
    // NOUVEAU : Le chronomètre avant la résurrection
    int respawnTimer; 

public:
    Ghost(int startX, int startY, affichage_pacman::type_fantome typeFantome);
    
    void deplacer(const Board& plateau, int pacmanX, int pacmanY, bool pacmanInvincible);
    void dessiner(affichage_pacman& ecran) const;
    void tuer(); 
    
    // NOUVEAU : Permet à l'arbitre de savoir s'il faut ignorer ce fantôme
    bool estMort() const; 

    int getX() const;
    int getY() const;
};

#endif