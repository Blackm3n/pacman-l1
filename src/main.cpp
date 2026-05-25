#include "affichage_sfml.hh"
#include "Board.hpp"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "Pathfinder.hpp"
#include "Fruit.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <SFML/System/Sleep.hpp>

// --- FONCTIONS DE GESTION DU SCORE ---
int lireHighScore() {
    std::ifstream fichier("../highscore.txt");
    int hs = 0;
    if (fichier.is_open()) fichier >> hs;
    return hs;
}

void sauvegarderHighScore(int hs) {
    std::ofstream fichier("../highscore.txt");
    if (fichier.is_open()) fichier << hs;
}

void reinitialiserHighScore() {
    std::ofstream fichier("../highscore.txt");
    if (fichier.is_open()) fichier << 0;
}

// --- PROGRAMME PRINCIPAL ---
int main() {
    std::srand(std::time(nullptr)); 
    bool quitterProgramme = false;

    // LA GRANDE BOUCLE DU MENU
    while (!quitterProgramme) {
        int meilleurScore = lireHighScore();
        
        std::cout << "\n=======================================" << std::endl;
        std::cout << "         PAC-MAN (MENU PRINCIPAL)      " << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "   Meilleur Score Absolu : " << meilleurScore << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "  1. Jouer (Controle Humain)" << std::endl;
        std::cout << "  2. Jouer (Mode Spectateur IA)" << std::endl;
        std::cout << "  3. Reinitialiser le meilleur score" << std::endl;
        std::cout << "  4. Quitter le jeu" << std::endl;
        std::cout << "=======================================" << std::endl;
        std::cout << "Votre choix (1-4) : ";
        
        int choix = 0;
        std::cin >> choix;

        // GESTION DES CHOIX DU MENU
        if (choix == 4) {
            quitterProgramme = true;
            std::cout << "Fermeture du programme..." << std::endl;
            break;
            
        } else if (choix == 3) {
            reinitialiserHighScore();
            std::cout << ">>> Meilleur score efface avec succes ! <<<\n" << std::endl;
            continue; // Relance le menu pour afficher le score à 0
            
        } else if (choix == 1 || choix == 2) {
            // C'EST PARTI POUR LE JEU !
            bool modeTournoiIA = (choix == 2);

            Board plateau;
            if (!plateau.loadLevel("../jeu1.txt")) {
                std::cout << "Erreur de chargement du niveau !" << std::endl;
                return -1;
            }

            // La fenêtre ne s'ouvre que lorsqu'on lance une partie
            affichage_pacman ecran(plateau.getWidth(), plateau.getHeight(), "..", 2);
            Pacman joueur(plateau.getPacmanStartX(), plateau.getPacmanStartY());
            
            std::vector<Ghost> fantomes;
            for (int i = 0; i < plateau.getNumGhosts(); ++i) {
                auto [gx, gy] = plateau.getGhostStart(i);
                affichage_pacman::type_fantome type = (i % 2 == 0) ? affichage_pacman::type_fantome::normal : affichage_pacman::type_fantome::peureux;
                fantomes.push_back(Ghost(gx, gy, type));
            }

            Fruit fruit;
            int tourCompteur = 0;
            unsigned int targetX = 0, targetY = 0;
            bool autoModeSouris = false; 

            // LA BOUCLE DE JEU CLASSIQUE
            while (ecran.fenetre_ouverte()) {
                
                // --- 1. L'IA DE PAC-MAN (Ou contrôle humain) ---
                if (ecran.recuperer_clic_souris(targetX, targetY)) autoModeSouris = true;

                affichage_pacman::action_joueur actionJoueur = affichage_pacman::action_joueur::rien;
                if (ecran.recuperer_action_joueur(actionJoueur) && actionJoueur != affichage_pacman::action_joueur::rien) {
                    autoModeSouris = false; 
                }
                
                if (modeTournoiIA) {
                    int cibleX = 0, cibleY = 0;
                    if (Pathfinder::trouverCibleIA(joueur.getX(), joueur.getY(), plateau, joueur.estInvincible(), fantomes, cibleX, cibleY)) {
                        affichage_pacman::direction nextDir = Pathfinder::calculerDirection(joueur.getX(), joueur.getY(), cibleX, cibleY, plateau);
                        if (nextDir == affichage_pacman::direction::haut) actionJoueur = affichage_pacman::action_joueur::haut;
                        else if (nextDir == affichage_pacman::direction::bas) actionJoueur = affichage_pacman::action_joueur::bas;
                        else if (nextDir == affichage_pacman::direction::gauche) actionJoueur = affichage_pacman::action_joueur::gauche;
                        else actionJoueur = affichage_pacman::action_joueur::droite;
                    }
                    joueur.deplacer(actionJoueur, plateau);
                } else {
                    if (autoModeSouris) {
                        affichage_pacman::direction nextDir = Pathfinder::calculerDirection(joueur.getX(), joueur.getY(), targetX, targetY, plateau);
                        affichage_pacman::action_joueur actionAuto = affichage_pacman::action_joueur::rien;
                        
                        if (nextDir == affichage_pacman::direction::haut) actionAuto = affichage_pacman::action_joueur::haut;
                        else if (nextDir == affichage_pacman::direction::bas) actionAuto = affichage_pacman::action_joueur::bas;
                        else if (nextDir == affichage_pacman::direction::gauche) actionAuto = affichage_pacman::action_joueur::gauche;
                        else if (nextDir == affichage_pacman::direction::droite) actionAuto = affichage_pacman::action_joueur::droite;
                        
                        joueur.deplacer(actionAuto, plateau);
                        if (joueur.getX() == targetX && joueur.getY() == targetY) autoModeSouris = false;
                    } else if (actionJoueur != affichage_pacman::action_joueur::rien) {
                        joueur.deplacer(actionJoueur, plateau);
                    }
                }

                // --- 2. LOGIQUE FANTOMES ET JEU ---
                bool etaitInvincible = joueur.estInvincible(); 
                joueur.decrementerInvincibilite(); 
                
                if (etaitInvincible && !joueur.estInvincible()) {
                    std::cout << "\n/!\\ ATTENTION : Fin de l'invincibilite ! /!\\\n" << std::endl;
                }
                
                tourCompteur++;
                if (tourCompteur % 4 != 0) {
                    for (auto& fantome : fantomes) {
                        fantome.deplacer(plateau, joueur.getX(), joueur.getY(), joueur.estInvincible());
                    }
                }
                
                if (tourCompteur % 50 == 0) {
                    int fx, fy;
                    do {
                        fx = std::rand() % plateau.getWidth();
                        fy = std::rand() % plateau.getHeight();
                    } while (plateau.isWall(fx, fy) || (joueur.getX() == fx && joueur.getY() == fy));
                    fruit.apparaitre(fx, fy);
                }
                
                fruit.update();
                if (fruit.estVisible() && joueur.getX() == fruit.getX() && joueur.getY() == fruit.getY()) {
                    fruit.manger();
                    joueur.ajouterScore(25);
                }

                // --- 3. L'ARBITRE (Collisions) ---
                bool gameOver = false;
                for (auto& fantome : fantomes) { 
                    if (!fantome.estMort() && joueur.getX() == fantome.getX() && joueur.getY() == fantome.getY()) {
                        if (joueur.estInvincible()) {
                            joueur.ajouterScore(200);
                            fantome.tuer(); 
                            std::cout << "Fantome avale ! (+200 pts)" << std::endl;
                        } else {
                            gameOver = true; 
                            break;
                        }
                    }
                }
                
                // --- 4. L'ARBITRE FINAL (HUD & Arrêt) ---
                int gommesRestantes = plateau.getGommesRestantes();
                
                if (joueur.estInvincible()) {
                    std::cout << ">>> [SUPER-POUVOIR] Fin dans : " << joueur.getTimerInvincibilite() << " pas... <<<         \r" << std::flush;
                } else if (gommesRestantes > 0 && gommesRestantes <= 5) {
                    std::cout << "RADAR -> Il reste " << gommesRestantes << " gomme(s) !                               \r" << std::flush;
                } else {
                    std::cout << "                                                                 \r" << std::flush;
                }

                if (gameOver || gommesRestantes == 0) {
                    std::cout << "\n\n==================================" << std::endl;
                    if (gameOver) {
                        std::cout << "======      GAME OVER       ======" << std::endl;
                        std::cout << ">> CAUSE : Devore par un fantome ! <<" << std::endl;
                    } else {
                        std::cout << "======      VICTOIRE !      ======" << std::endl;
                        std::cout << ">> CAUSE : Labyrinthe nettoye !   <<" << std::endl;
                    }
                    std::cout << "==================================" << std::endl;
                    std::cout << "Score Final : " << joueur.getScore() << std::endl;
                    
                    if (joueur.getScore() > meilleurScore) {
                        std::cout << ">>> NOUVEAU RECORD ABSOLU ! <<<" << std::endl;
                        sauvegarderHighScore(joueur.getScore());
                    }

                    sf::sleep(sf::seconds(3)); 
                    break; // Cette commande ferme la partie en cours, et nous renvoie au Menu Principal
                }

                // --- 5. AFFICHAGE ---
                ecran.affichage_commencer();
                for (int y = 0; y < plateau.getHeight(); ++y) {
                    for (int x = 0; x < plateau.getWidth(); ++x) {
                        char c = plateau.getCell(x, y);
                        if (c == '*') ecran.afficher_mur(x, y, affichage_pacman::type_mur::normal);
                        else if (c == '.') ecran.afficher_pacgomme(x, y, affichage_pacman::type_pacgomme::normale);
                        else if (c == 'S') ecran.afficher_pacgomme(x, y, affichage_pacman::type_pacgomme::super);
                    }
                }
                fruit.dessiner(ecran);
                for (const auto& fantome : fantomes) fantome.dessiner(ecran);
                joueur.dessiner(ecran);
                
                ecran.afficher_score(joueur.getScore());
                ecran.affichage_terminer();

                sf::sleep(sf::milliseconds(150)); 
            } // Fin de la boucle de la partie
        } else {
            std::cout << "Choix invalide. Veuillez taper 1, 2, 3 ou 4.\n" << std::endl;
            // On nettoie la mémoire du terminal au cas où l'utilisateur tape une lettre par erreur
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    } // Fin de la grande boucle Menu

    return 0;
}