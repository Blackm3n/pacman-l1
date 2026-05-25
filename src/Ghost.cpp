#include "Ghost.hpp"
#include "Board.hpp"
#include "Pathfinder.hpp"
#include <vector>
#include <cstdlib>

// NOUVEAU : On initialise le respawnTimer à 0 (le fantôme est vivant au début)
Ghost::Ghost(int startX, int startY, affichage_pacman::type_fantome typeFantome) 
    : x(startX), y(startY), startX(startX), startY(startY), dir(affichage_pacman::direction::droite), type(typeFantome), respawnTimer(0) {}

void Ghost::deplacer(const Board& plateau, int pacmanX, int pacmanY, bool pacmanInvincible) {
    // --- GESTION DU COOLDOWN ---
    if (respawnTimer > 0) {
        respawnTimer--; // Le temps s'écoule
        return; // On arrête la fonction ici : le fantôme ne bouge pas !
    }

    // --- INTELLIGENCE ARTIFICIELLE ---
    if (type == affichage_pacman::type_fantome::normal && !pacmanInvincible) {
        dir = Pathfinder::calculerDirection(x, y, pacmanX, pacmanY, plateau);
        
        if (dir == affichage_pacman::direction::haut) y--;
        else if (dir == affichage_pacman::direction::bas) y++;
        else if (dir == affichage_pacman::direction::gauche) x--;
        else if (dir == affichage_pacman::direction::droite) x++;
    } else {
        std::vector<affichage_pacman::direction> directions_possibles;
        if (!plateau.isWall(x, y - 1)) directions_possibles.push_back(affichage_pacman::direction::haut);
        if (!plateau.isWall(x, y + 1)) directions_possibles.push_back(affichage_pacman::direction::bas);
        if (!plateau.isWall(x - 1, y)) directions_possibles.push_back(affichage_pacman::direction::gauche);
        if (!plateau.isWall(x + 1, y)) directions_possibles.push_back(affichage_pacman::direction::droite);

        if (!directions_possibles.empty()) {
            int choix = std::rand() % directions_possibles.size();
            dir = directions_possibles[choix];
            if (dir == affichage_pacman::direction::haut) y--;
            else if (dir == affichage_pacman::direction::bas) y++;
            else if (dir == affichage_pacman::direction::gauche) x--;
            else if (dir == affichage_pacman::direction::droite) x++;
        }
    }

    if (x < 0) x = plateau.getWidth() - 1;
    if (x >= plateau.getWidth()) x = 0;
    if (y < 0) y = plateau.getHeight() - 1;
    if (y >= plateau.getHeight()) y = 0;
}

void Ghost::dessiner(affichage_pacman& ecran) const {
    // NOUVEAU : S'il est en cooldown, on ne le dessine pas (il est invisible)
    if (respawnTimer > 0) return; 
    
    ecran.afficher_fantome(x, y, 0, dir, type);
}

void Ghost::tuer() {
    x = startX;
    y = startY;
    respawnTimer = 20; // NOUVEAU : Il restera bloqué et invisible pendant 20 tours (environ 5 secondes)
}

bool Ghost::estMort() const {
    return respawnTimer > 0;
}

int Ghost::getX() const { return x; }
int Ghost::getY() const { return y; }