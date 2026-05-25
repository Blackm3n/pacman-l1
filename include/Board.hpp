#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>

class Board {
private:
    std::vector<std::string> grid;
    int width;
    int height;
    int startPacmanX;
    int startPacmanY;

    int numGhosts;
    std::vector<std::pair<int, int>> ghostStarts;
public:
    Board();
    
    int getGommesRestantes() const;

    bool isWall(int x, int y) const;
    void eatDot(int x, int y);

    int getNumGhosts() const;
    std::pair<int, int> getGhostStart(int index) const;
    // Charge le fichier jeu1.txt
    bool loadLevel(const std::string& filename);
    
    // Accesseurs
    int getWidth() const;
    int getHeight() const;
    int getPacmanStartX() const;
    int getPacmanStartY() const;
    
    // Permet de lire une case (pour l'affichage)
    char getCell(int x, int y) const;
};

#endif