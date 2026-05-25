#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include "affichage_sfml.hh"
#include "Board.hpp"
#include <vector>

class Ghost; // Déclaration anticipée pour éviter les erreurs d'inclusion

class Pathfinder {
public:
    static affichage_pacman::direction calculerDirection(int startX, int startY, int targetX, int targetY, const Board& plateau);
    
    // NOUVEAU : Le cerveau de prise de décision pour l'IA
    static bool trouverCibleIA(int pacX, int pacY, const Board& plateau, bool estInvincible, const std::vector<Ghost>& fantomes, int& targetX, int& targetY);
};

#endif