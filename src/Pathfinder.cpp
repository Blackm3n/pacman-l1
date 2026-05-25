#include "Pathfinder.hpp"
#include "Ghost.hpp"
#include <cmath>
#include <queue>
#include <vector>

// Structure interne pour aider l'algorithme de calcul de chemin
struct Node {
    int x, y;
    affichage_pacman::direction dir_initiale;
};

// 1. L'ALGORITHME DE DEPLACEMENT (Celui qui avait disparu)
affichage_pacman::direction Pathfinder::calculerDirection(int startX, int startY, int targetX, int targetY, const Board& plateau) {
    if ((startX == targetX && startY == targetY) || plateau.isWall(targetX, targetY)) {
        return affichage_pacman::direction::droite; 
    }

    int w = plateau.getWidth();
    int h = plateau.getHeight();
    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    std::queue<Node> q;

    visited[startY][startX] = true;

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    affichage_pacman::direction dirs[] = {
        affichage_pacman::direction::haut,
        affichage_pacman::direction::bas,
        affichage_pacman::direction::gauche,
        affichage_pacman::direction::droite
    };

    // On explore les 4 cases autour de Pac-Man
    for (int i = 0; i < 4; ++i) {
        int nx = startX + dx[i];
        int ny = startY + dy[i];

        // Gestion des tunnels
        if (nx < 0) nx = w - 1; else if (nx >= w) nx = 0;
        if (ny < 0) ny = h - 1; else if (ny >= h) ny = 0;

        if (!plateau.isWall(nx, ny)) {
            visited[ny][nx] = true;
            if (nx == targetX && ny == targetY) return dirs[i];
            q.push({nx, ny, dirs[i]});
        }
    }

    // On cherche le chemin le plus court
    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        if (current.x == targetX && current.y == targetY) {
            return current.dir_initiale;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx < 0) nx = w - 1; else if (nx >= w) nx = 0;
            if (ny < 0) ny = h - 1; else if (ny >= h) ny = 0;

            if (!plateau.isWall(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                q.push({nx, ny, current.dir_initiale});
            }
        }
    }

    return affichage_pacman::direction::droite; 
}

// 2. LE CERVEAU DE L'IA (La nouveauté pour le Tournoi)
bool Pathfinder::trouverCibleIA(int pacX, int pacY, const Board& plateau, bool estInvincible, const std::vector<Ghost>& fantomes, int& targetX, int& targetY) {
    int minDist = 999999;
    bool cibleTrouvee = false;

    // MODE CHASSEUR : Si Pac-Man est invincible, il vise le fantôme le plus proche
    if (estInvincible) {
        for (const auto& fantome : fantomes) {
            if (!fantome.estMort()) {
                int dist = std::abs(fantome.getX() - pacX) + std::abs(fantome.getY() - pacY);
                if (dist < minDist) {
                    minDist = dist;
                    targetX = fantome.getX();
                    targetY = fantome.getY();
                    cibleTrouvee = true;
                }
            }
        }
        if (cibleTrouvee) return true; 
    }

    // MODE GLOUTON : Sinon, il cherche la pac-gomme la plus proche
    for (int y = 0; y < plateau.getHeight(); ++y) {
        for (int x = 0; x < plateau.getWidth(); ++x) {
            char cell = plateau.getCell(x, y);
            if (cell == '.' || cell == 'S') {
                int dist = std::abs(x - pacX) + std::abs(y - pacY); 
                if (dist < minDist) {
                    minDist = dist;
                    targetX = x;
                    targetY = y;
                    cibleTrouvee = true;
                }
            }
        }
    }
    
    return cibleTrouvee;
}