#pragma once

#include <cstdint>
#include <array>

// Version de l'étape 4. Les seules modifications sont en bas du fichier.

// Vous ne devriez pas avoir à modifier ce fichier. Il contient des déclarations de types et de sous-programmes qui ont pour but d'être communs à tous, et pour cette raison, ne doivent pas être modifiés. Si vous voyez une erreur, si vous avez une suggestion de modification, bien sûr, vous pouvez me le signaler, et je publierai une version mise à jour.
// Ce fichier d'entêtes déclare un certain nombre de sous-programmes, mais je ne vous fournis pas de code pour ces sous-programmes qui "gèrent" une partie et permettent au code des IA d'accéder à l'état du jeu. C'est une des choses que vous devez faire dans l'étape 3, fournir le code pour ces sous-programmes. Ce code, vous allez l'écrire dans un fichier ia_arbitre_X.cpp (ou .cc), le "X" sera remplacé par votre numéro de binôme (le chiffre initial du canal discord spécifique à votre binôme).

// Dans ce fichier, tous les types sont préfixés par ia pour qu'il n'ait pas d'ambiguïté avec les types que vous avez déjà définis.
// (Pour aller plus loin : ce n'est pas forcément une bonne pratique de faire cela, C++ permet de définir des espaces de noms (namespaces) et il serait sans doute plus élégant de faire ça, mais je n'ai pas voulu introduire cette notion, car je préfère au maximum rester sur des notions que vous connaissez déjà, même si à l'occasion, on peut voir de nouveaux concepts dans le cadre de cette unité.)

// Ce type représenta une position sur le plateau, elle est aussi utilisée pour représenter la taille du plateau.
// Note : std::uint8_t n'est rien d'autre qu'un entier, non signé (le "u" signifie unsigned) codé sur 8 bits (en d'autres termes, les valeurs admises sont [0,255].
struct ia_position {
	std::uint8_t x;
	std::uint8_t y;
};

// La valeur stop n'est normalement pas permise pour un fantôme. Et un pacman non plus ne s'arrêtera pas. Par contre, au tout début de partie, le fantôme est arrêté à la position initiale, et donc sa direction est "ia_direction::stop".
enum class ia_direction {
	haut,
	bas,
	droite,
	gauche,
	stop
};

enum class ia_plateau_contenu {
	vide,
	pacgomme,
	superpacgomme,
	mur_normal,
	mur_accessible_fantome,
	mur_accessible_pacman
};

// Cette struct définit les informations sur un élément mobile, c'est à dire le Pacman et les fantômes. Pacman et fantômes ont des informations "communes", elles sont déclarées ici. Les deux struct plus bas rajoutent (ou pas) des informations supplémentaires pour le Pacman / les fantômes.
struct ia_mobile_info {
	ia_position position;
	ia_direction direction;
	signed int decalage; // Il s'agit ici du décalage qui est ce que nous avons défini à l'étape 2.
	bool vivant;
};

// Note : On ne vous a pas encore expliqué en cours la syntaxe ci dessus. Elle permet de définir une nouvelle structure (ia_pacman_info) qui contient tout ce qu'il y a dans la strcuture ia_mobile_info (position, direction, decalage, vivant), et contient un champ supplémentaire : invincibilite.
struct ia_pacman_info : public ia_mobile_info {
	unsigned int invincibilite;
};

// Note : Ici, c'est la même chose, on définit une nouvelle struct, ia_fantome_info, qui contient tout ce qu'il y a dans ia_mobile_info... et rien en plus (mais peut-être pourrait on plus tard ajouter des champs utiles à l'IA de fantôme).
struct ia_fantome_info : public ia_mobile_info {
};

struct ia_jeu;

// Les 6 fonctions, dont le nom commence par "ia_jeu", ci-dessous vont pouvoir être appelées par l'IA pour connaître l'état de la partie (le plateau, où sont les fantomes, le pacman). Chacune de ces fonctions prend, comme vous le voyez, un premier paramètre qui est toujours le même, un "ia_jeu*". Mais vous le voyez ci-dessus, la struct ia_jeu est définie d'une façon un peu particulière (que vous n'avez pas encore vue en cours, et qui s'appelle une "déclaration avancée"). Cette déclaration avancée ne signifie pas que la struct ia_jeu ne contient rien. Elle signifie que ia_jeu est une struct, et donc on va pouvoir déclarer des sous-programmes qui prennent comme argument un pointeur sur ia_jeu MAIS que la struct ia_jeu sera définie plus tard. Mais où définir cette structure ? Comme ce fichier ia.hh doit être exactement le même pour tous les binômes, vous ne pouvez pas le faire dans ce fichier. Vous allez donc créer un fichier ia_arbitre_0.hh qui déclarera cette structure (dont le nom est fixé : ia_jeu) : comme champs de cette structure vous pouvez mettre ce que vous voulez, mais a priori, vous allez déclarer un pointeur sur une structure (que vous avez déjà écrite en étape 1 et 2) qui représente la totalité de la partie. Il y a un exemple sur la page Moodle.
// Quel est l'intérêt de faire ça ? On peut déclarer les sous-programmes ci-dessous, qui auront exactement la même signature pour chacun des binômes et chacun d'entre vous pourra à la fois :
// - Ecrire un moteur qui fournit un code pour chacune de ces fonctions (dans un fichier ia.cpp que vous devrez écrire, ainsi qu'un main lançant la partie). Comment écrire ces fonctions ? Ne réécrivez pas le code que vous avez déjà mis au point pour l'étape 2. Certes le code que vous avez écrit n'utilise pas les structures de données (dont le nom commence par ia_) qui sont définies ici et qui ont pour but d'être COMMUNES à chacun des binômes, votre code utilise VOS structures de données. L'objectif est donc d'écrire un code, pour chacune des fonctions ci-dessous, relativement court qui a simplement pour objectif de "convertir" les structures de données communes en structures de données partagées. Prenons la première de ces fonctions : ia_plateau_taille. Vous avez tous déjà écrit la gestion de plateau de jeu de taille variable. Cette taille est représentée, selon les binômes par un couple d'entiers ou par une structure contenant deux entiers. La fonction ia_jeu_plateau_taille retournera donc une valeur de type ia_position construite à partir du couple d'entiers ou des deux valeurs contenues dans votre structure de données spécifique.
// - Ecrire une IA (c'est expliqué plus bas dans ce fichier) qui appelle ces sous programmes, et chacune de vos IA, qui appeleront ces sous-programmes là devrait être utilisable sur le moteur de n'importe quel binôme. En conséquence, ceci nous permettra de lancer le tournoi à la fin de l'étape 4, qui ne sera rien d'autre qu'un moteur (écrit par moi) qui fournit dont un code pour chacune des fonctions ci-dessous, appelées depuis vos codes d'IA.

// Accès à la taille du plateau.
ia_position ia_jeu_plateau_taille(ia_jeu* iaj);
// Accès au contenu d'une case du plateau.
ia_plateau_contenu ia_jeu_plateau_contenu(ia_jeu* iaj, ia_position const& p);
// Accès au nombre de fantômes du jeu. Attention, il s'agit du nombre de fantômes au début de la partie, certains peuvent avoir disparu depuis, voir fonction suivante.
std::uint8_t ia_jeu_nombre_fantomes(ia_jeu* iaj);
// Accès à un fantôme, la valeur f doit être comprise entre 0 et l'entier retourné par ia_jeu_nombre_fantomes -1. Attention, le fantôme peut être mort (vivant == false).
ia_fantome_info ia_jeu_acces_fantome(ia_jeu* iaj, std::uint8_t f);
// Accès au pacman.
ia_pacman_info ia_jeu_acces_pacman(ia_jeu* iaj);
// Accès à l'horloge du jeu. Ce n'est rien d'autre qu'un entier qui commence à 0 au début de la partie est qui est incrémenté à chaque tour de jeu.
unsigned int ia_jeu_acces_horloge(ia_jeu* iaj);

// Il y aura deux types d'IA, celle de pacman (étape 4) et celle de fantôme (étape 3).
enum class ia_type {
	pacman,
	fantome
};

// Cette structure définit quel élément mobile est géré par une IA, le pacman (si type == ia_type::pacman) ou un fantôme (si type == ia_type::fantome). Dans ce dernier cas, fantome_id est le numéro du fantôme géré par l'IA.
struct ia_info {
	ia_type type;
	std::size_t fantome_id;
};

// Les IA vont produire des chemins. Un chemin est une suite de directions à prendre (produite par l'IA, et appliquées par le moteur de jeu aux étapes suivantes). Un chemin contient, comme vous le voyez, 50 directions au maximum (mais il peut en contenir moins, sa longueur est donnée par le champ longueur). Bien évidemment, cela ne veut pas dire que l'IA devra retourner un "long" chemin et ensuite ce chemin est suivi, au contraire, à chaque tour de jeu (quand l'élément mobile est à nouveau au centre d'une case), le moteur rappellera l'IA pour savoir si le chemin produit doit être mis à jour ou si le chemin produit par l'appel précédent à l'IA peut être poursuivi.
const std::size_t IA_CHEMIN_MAX = 50;
struct ia_chemin {
	std::array<ia_direction, IA_CHEMIN_MAX> chemin;
	std::size_t longueur;
};

// Pour participer au tournoi, chacun d'entre vous devra fournir UNE fonction ayant le nom et signature ci-dessous dans lequel le "X" sera remplacé par votre numéro de binôme (le chiffre initial du canal discord spécifique à votre binôme). Cette fonction recevra (de la part du moteur) un premier paramètre "ia_jeu*" lui permettant d'appeler les fonctions dont le nom commence par "ia_jeu" décrites ci-dessus. En deuxième paramètre, ce sera l'élément géré par l'IA (un fantôme et si oui, lequel ou le pacman), le chemin actuellement suivi par l'élément, et ce chemin (passé par référence comme vous le voyez) pourra être modifié par l'IA pour définir un nouveau déplacement. Si la fonction modifie ce chemin, elle devra retourner true (pour signaler qu'il y a un nouveau chemin à suivre), et elle retourera false si le chemin actuel doit continue à être suivi. Si l'IA décide de ne "rien faire", il n'y a donc rien à faire, le chemin "actuel" continue d'être suivi, il suffit de feiare un "return false;". Cela peut permettre par exemple à une IA de générer à un moment donner un chemin "plus ou moins long", et tant que ce chemin n'est pas vide, elle ne fera rien d'autre (if (c.longueur >= 1) return false; else { ... return true; }).
// bool ia_binomeX_executer(ia_jeu* iaj, ia_info iai, ia_chemin& c);

// Cette fonction ia_binomeX_executer ne sera PAS déclarée dans ce fichier, et son code ne sera pas donné dans ia_arbitre_X.cc : elle sera déclarée dans ia_X.hh et son code sera donné dans ia_X.cc (ou cpp). Bien évidemment, la déclaration de la fonction a besoin des déclarations qui sont données ici (ia_jeu, ia_info, ia_chemin) donc dans votre fichier ia_X.hh, vous ferez un #include "ia.hh". De cette façon, on conserve ce fichier ia.hh totalement commun à tous les binômes, le code des fonctions "arbitre" de chacun des binômes est dans ia_arbitre_X.cc, et le code de la fonction d'IA est dans ia_X.hh ia_X.cc.

// =====================================
// Inutile de lire ce qu'il y a au dessous de cette ligne pour l'étape 3, cela concerne l'étape 4, nous verrons cela la semaine prochaine.
// Modifications pour rajouter le stockage de données dans l'arbitre à destination des IA.

// Du point de vue de l'arbitre, on devra pouvoir mémoriser pour chaque valeur de ia_info, un pointeur non typé (void*). En début de partie, l'arbitre appellera, pour chaque élément géré par une IA la fonction ia_binome_X_memoire_creer qui retournera un void* pointant sur une structure allouée dynamiquement par new, mémorisant les données de l'IA. En fin de partie, l'arbitre appellera ia_binome_X_memoire_detruire pour que l'IA libère (par delete) ladite structure. A chaque appel de la fonction d'IA (ia_binomeX_executer), l'IA pourra eppeler ia_jeu_memoire_acceder pour accéder au "void *" (et ce sera au code de l'IA de convertir ce pointeur non typé en un pointeur typé).
// Résumons : cette fonction ia_jeu_memoire_acceder doit être fournie par l'arbitre qui doit aussi appeler ia_binomeX_memoire_creer et ia_binome_X_memoire_detruire (respectivement en début et fin de partie).
void* ia_jeu_memoire_acceder(ia_jeu* iaj, ia_info iai);
// Cette fonction doit être mise dans le même fichier que ia_binomeX_executer.
// void* ia_binomeX_memoire_creer(ia_jeu* iaj, ia_info iai);
// Cette fonction doit être mise dans le même fichier que ia_binomeX_executer.
// void ia_binomeX_memoire_detruire(ia_jeu* iaj, ia_info iai);
