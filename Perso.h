#ifndef PERSO_H
#define PERSO_H
#include <SFML/Graphics.hpp>
#include "Texte.h"
typedef struct Animation Animation;
struct Animation
{
    int tileEnCours;
    int rotation;
    int etat;
    float avancement;
    int tempsAtteindre;
    sf::Clock tempsEnCours;

};

typedef struct AnimationBIS AnimationBIS;
struct AnimationBIS
{
    int etat;
    float avancement;
};

typedef struct AnimationFoulard AnimationFoulard;
struct AnimationFoulard
{
    int tile;
    sf::Clock clock;
    sf::Sprite sprite_Foulard[5];
    bool direction;
};

enum phaseDeConstruction{attenteAppuie, enAppuie, mouvementPont, mouvementPerso, enumEtatVictoire, persoTombe, tremblement, deplacementLevel, tableauScore};
enum etatVict{passed, perdu, bonus};

class Graph
{
    public:
        Graph(sf::RenderWindow &fenetreRecois, TexteObj &texteRecois);
        void afficherPerso(int positX, int positY, int phase);
        void afficherSol(int obstacleX, int obstacleSize, int obstacleX_Prec, int obstacleSize_Prec, int decalage, bool avecBonus);
        void afficherDecors(int phase);
        void afficherPont(int positPont, int taille, float rotation, int decalage=0);
        void resetAnimation();
        void afficherTableauScore (int score, int meilleurScore, int modeDeJeuRecois);
        void resetAnimationTableauScore();
        int getModeDeJeu();
        void afficherMode (int modeDeJeuRecois, int phase);

    protected:
        sf::RenderWindow &fenetre;

        int modeDeJeu;

        sf::Image image_Personnage;
        sf::Texture texture_Personnage;
        sf::Sprite sprite_Jambe, sprite_Corps;

        sf::Sprite sprite_tableauScore;
        sf::Sprite sprite_bonusUnPoint;
        sf::Sprite sprite_selecteur;

        sf::Texture texture_Fond;
        sf::Sprite sprite_Fond;

        sf::RectangleShape sol;
        sf::RectangleShape pont;
        sf::RectangleShape bonus;
        sf::RectangleShape nuage;

        Animation animation_Jambes;
        AnimationBIS animation_Bonus;
        AnimationBIS animation_TableauScore;
        AnimationBIS animation_Nuage;

        AnimationFoulard animation_Foulard;


        TexteObj &texte;

};

#endif // PERSO_H
