#include "Pacman.hpp"
#include "Board.hpp"

Pacman::Pacman(int startX, int startY) : x(startX), y(startY), dir(affichage_pacman::direction::droite), score(0), invincibiliteTimer(0) {}

void Pacman::deplacer(affichage_pacman::action_joueur action, Board& plateau) {
    if (action == affichage_pacman::action_joueur::gauche) dir = affichage_pacman::direction::gauche;
    else if (action == affichage_pacman::action_joueur::droite) dir = affichage_pacman::direction::droite;
    else if (action == affichage_pacman::action_joueur::haut) dir = affichage_pacman::direction::haut;
    else if (action == affichage_pacman::action_joueur::bas) dir = affichage_pacman::direction::bas;

    int nextX = x;
    int nextY = y;
    if (dir == affichage_pacman::direction::gauche) nextX--;
    else if (dir == affichage_pacman::direction::droite) nextX++;
    else if (dir == affichage_pacman::direction::haut) nextY--;
    else if (dir == affichage_pacman::direction::bas) nextY++;

    if (nextX < 0) nextX = plateau.getWidth() - 1;
    if (nextX >= plateau.getWidth()) nextX = 0;
    
    // --- NOUVEAU : Le tunnel vertical (Haut / Bas) ---
    if (nextY < 0) nextY = plateau.getHeight() - 1;
    if (nextY >= plateau.getHeight()) nextY = 0;

    if (!plateau.isWall(nextX, nextY)) {
        x = nextX;
        y = nextY;

        char caseActuelle = plateau.getCell(x, y);
        if (caseActuelle == '.') {
            score += 10;
            plateau.eatDot(x, y);
        } else if (caseActuelle == 'S') {
            score += 50;
            plateau.eatDot(x, y);
            invincibiliteTimer = 30; // 30 tours d'invincibilité
        }
    }
}

void Pacman::dessiner(affichage_pacman& ecran) const {
    ecran.afficher_pacman(x, y, 0, dir);
}

int Pacman::getScore() const { return score; }
int Pacman::getX() const { return x; }
int Pacman::getY() const { return y; }

void Pacman::ajouterScore(int points) { score += points; }
bool Pacman::estInvincible() const { return invincibiliteTimer > 0; }
void Pacman::decrementerInvincibilite() { if (invincibiliteTimer > 0) invincibiliteTimer--; }

// NOUVEAU : On retourne la valeur exacte
int Pacman::getTimerInvincibilite() const { 
    return invincibiliteTimer; 
}