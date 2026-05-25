#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "affichage_sfml.hh"

class Fruit {
private:
    int x, y;
    bool visible;
    int timer; // Compte le nombre de tours

public:
    Fruit();
    void apparaitre(int newX, int newY);
    void update(); // Gère le timer
    void dessiner(affichage_pacman& ecran) const;
    
    bool estVisible() const { return visible; }
    int getX() const { return x; }
    int getY() const { return y; }
    void manger() { visible = false; }
};

#endif