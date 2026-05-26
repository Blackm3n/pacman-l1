#include "Board.hpp"
#include <iostream>
#include <fstream>
#include <string>

Board::Board() : width(0), height(0), startPacmanX(7), startPacmanY(7), numGhosts(0) {}

bool Board::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << filename << std::endl;
        return false;
    }

    // 1. Lire les dimensions
    file >> width >> height;

    std::string line;
    // 2. Avancer jusqu'à trouver la première ligne de la carte (qui contient un mur '*' ou un espace)
    while (std::getline(file, line)) {
        if (!line.empty() && (line[0] == '*' || line[0] == ' ')) {
            grid.push_back(line);
            break;
        }
    }

    // 3. Lire le reste des lignes du labyrinthe
    for (int i = 1; i < height; ++i) {
        std::getline(file, line);
        grid.push_back(line);
    }

    // 4. Lire la position de Pacman en toute sécurité
    if (!(file >> startPacmanX >> startPacmanY)) {
        startPacmanX = 7;
        startPacmanY = 7;
    }

    // 5. Lire les fantômes
    if (file >> numGhosts) {
        for (int i = 0; i < numGhosts; ++i) {
            int gx, gy;
            if (file >> gx >> gy) {
                ghostStarts.push_back({gx, gy});
            }
        }
    }

    file.close();
    return true;
}

int Board::getWidth() const { return width; }
int Board::getHeight() const { return height; }
int Board::getPacmanStartX() const { return startPacmanX; }
int Board::getPacmanStartY() const { return startPacmanY; }
int Board::getNumGhosts() const { return numGhosts; }

std::pair<int, int> Board::getGhostStart(int index) const { 
    if (index >= 0 && index < ghostStarts.size()) return ghostStarts[index];
    return {0, 0}; // Sécurité
}

bool Board::isWall(int x, int y) const {
    if (y < 0 || y >= height || x < 0 || x >= width) return true;
    return grid[y][x] == '*'; 
}

void Board::eatDot(int x, int y) {
    if (y >= 0 && y < height && x >= 0 && x < width) {
        if (grid[y][x] == '.' || grid[y][x] == 'S') {
            grid[y][x] = ' '; 
        }
    }
}

char Board::getCell(int x, int y) const {
    if (y < 0 || y >= height || x < 0 || x >= width) return ' ';
    return grid[y][x];
}

int Board::getGommesRestantes() const {
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == '.' || grid[y][x] == 'S') {
                count++;
            }
        }
    }
    return count;
}
