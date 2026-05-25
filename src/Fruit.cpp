#include "Fruit.hpp"

Fruit::Fruit() : x(0), y(0), visible(false), timer(0) {}

void Fruit::apparaitre(int newX, int newY) {
    x = newX; y = newY;
    visible = true;
    timer = 0;
}

void Fruit::update() {
    if (visible) {
        timer++;
        if (timer > 20) visible = false; // Disparaît après 20 tours
    }
}

void Fruit::dessiner(affichage_pacman& ecran) const {
    if (visible) ecran.afficher_fruit(x, y); // Assure-toi que cette méthode existe dans affichage_sfml
}