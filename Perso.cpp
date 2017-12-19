#include "Perso.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Texte.h"
using namespace sf;
using namespace std;


Graph::Graph(RenderWindow &fenetreRecois, TexteObj &texteRecois):fenetre(fenetreRecois), texte(texteRecois)
{
    image_Personnage.loadFromFile("files/perso.png");

    for (int a=0; a<image_Personnage.getSize().x; a++)
    {
        for (int b=0; b<image_Personnage.getSize().y; b++)
        {
                if (image_Personnage.getPixel(a,b) == Color(0,255,0))
                image_Personnage.setPixel(a,b, Color::Transparent);
        }
    }

    texture_Personnage.loadFromImage(image_Personnage);

    sprite_Corps.setTexture(texture_Personnage);
    sprite_Corps.setTextureRect(IntRect(0,0,20,24));
    sprite_Corps.setOrigin(Vector2f(1,0));

    sprite_Jambe.setTexture(texture_Personnage);
    sprite_Jambe.setTextureRect(IntRect(21,0,4,8));
    sprite_Jambe.setOrigin(Vector2f(1,0));

    sprite_tableauScore.setTexture(texture_Personnage);
    sprite_tableauScore.setTextureRect(IntRect(44,0,218,258));
    sprite_tableauScore.setOrigin(Vector2f(109,0));
    sprite_tableauScore.setPosition(Vector2f(229,50));
    sprite_tableauScore.setColor(Color(255,255,255,220));


    sprite_bonusUnPoint.setTexture(texture_Personnage);
    sprite_bonusUnPoint.setTextureRect(IntRect(26,0,16,10));
    sprite_bonusUnPoint.setOrigin(Vector2f(8,0));
    //sprite_bonusUnPoint.setColor(Color(0,0,0,20));

    sprite_selecteur.setTexture(texture_Personnage);
    sprite_selecteur.setTextureRect(IntRect(52,260,93,20));
    sprite_selecteur.setColor(Color(255,255,255,220));

    ///FAIRE DEPLACEMEENT LEVEL : DECALAGE DU PONT
    texture_Fond.loadFromFile("files/fond.jpg");
    sprite_Fond.setTexture(texture_Fond);
    sprite_Fond.setTextureRect(IntRect(0,0,456,600));



    sol.setFillColor(Color::Black);
    pont.setFillColor(Color::Black);
    pont.setOrigin(Vector2f(0,2));
    bonus.setFillColor(Color::Red);
    bonus.setSize(Vector2f(7,4));
    bonus.setOrigin(Vector2f((bonus.getSize().x-1)/2+1,0));

    nuage.setSize(Vector2f(5,455));
    nuage.setFillColor(Color::Blue);
    nuage.setPosition(Vector2f(71,0));





    animation_Jambes={0,0,0,0};
    animation_Bonus={0,0};
    animation_TableauScore={0,0};
    animation_Nuage={0,0};

    animation_Foulard.tile=0;
    for (int a=0; a<5; a++)
    {
        animation_Foulard.sprite_Foulard[a].setTexture(texture_Personnage);
        animation_Foulard.sprite_Foulard[a].setTextureRect(IntRect(0,30+a*27, 9,15));
    }
    animation_Foulard.direction=1;
    //animation_Foulard.tile=0;

    modeDeJeu=0;
}

void Graph :: afficherDecors(int phase)
{
    /*
    if (phase==mouvementPerso)
    {
        sprite_Fond.setTextureRect(IntRect((sprite_Fond.getTextureRect().left+1)%3199,0,456,600));
    }
    */
    sprite_Fond.setTextureRect(IntRect(sprite_Fond.getTextureRect().left,0,456,600));
    sprite_Fond.setPosition(0,0);
    fenetre.draw(sprite_Fond);
}

void Graph :: afficherPerso (int positX, int positY, int phase)
{
    sprite_Corps.setPosition(Vector2f(positX, positY + (int)(2*sin(animation_Jambes.avancement))));
  //  animation_Jambes.avancement+=0.5;

    if (animation_Foulard.clock.getElapsedTime().asMilliseconds()> (90-(phase==mouvementPerso)*70))
    {
        animation_Foulard.tile+=(animation_Foulard.direction*2-1);
        if (animation_Foulard.tile==4 || animation_Foulard.tile==0)
            animation_Foulard.direction = 1-animation_Foulard.direction;

        animation_Foulard.clock.restart();
        //animation_Foulard.tile=(animation_Foulard.tile+1)%5;
    }
    animation_Foulard.sprite_Foulard[animation_Foulard.tile].setPosition(Vector2f(positX-10, positY  + (int)(2*sin(animation_Jambes.avancement))));
    fenetre.draw(sprite_Corps);
    fenetre.draw(animation_Foulard.sprite_Foulard[animation_Foulard.tile]);
    if (phase == mouvementPerso)
    {
        animation_Jambes.avancement+=0.9;
        if (animation_Jambes.etat==0)
        {
            animation_Jambes.rotation+=5;
            if (animation_Jambes.rotation>=20)
                animation_Jambes.etat=1;
        }
        else
        {
            animation_Jambes.rotation-=5;
            if (animation_Jambes.rotation<-10)
                animation_Jambes.etat=0;
        }
        for (int a=0; a<2; a++)
        {
            sprite_Jambe.setRotation((a*2-1)*animation_Jambes.rotation);
            sprite_Jambe.setPosition(Vector2f(positX+3 + a*10, positY+21));
            fenetre.draw(sprite_Jambe);
        }
    }
    else if (phase == mouvementPont)
    {
        if (animation_Jambes.etat==0)
        {
            animation_Jambes.rotation+=10;
            if (animation_Jambes.rotation>=50)
                animation_Jambes.etat=1;
        }
        else if (animation_Jambes.etat==1)
        {
            animation_Jambes.rotation-=10;
            if (animation_Jambes.rotation<0)
            {
                animation_Jambes.etat=2;
                animation_Jambes.rotation=0;
            }
        }
        for (int a=0; a<2; a++)
        {
            if (a==0)
                sprite_Jambe.setRotation(0);
            else
                sprite_Jambe.setRotation(-animation_Jambes.rotation);
            sprite_Jambe.setPosition(Vector2f(positX+3 + a*(10 + 3*(animation_Jambes.etat!=2)), positY+21));
            fenetre.draw(sprite_Jambe);
        }
    }
    else
    {
        sprite_Jambe.setRotation(0);
        for (int a=0; a<2; a++)
        {
            sprite_Jambe.setPosition(Vector2f(positX+3 + a*10, positY+21));
            fenetre.draw(sprite_Jambe);
        }
    }

    if (phase==enAppuie || phase==attenteAppuie || phase == deplacementLevel)
    {
        if (animation_Jambes.tempsEnCours.getElapsedTime().asSeconds()>=animation_Jambes.tempsAtteindre)
        {
            if (animation_Jambes.avancement<2)
            {
                animation_Jambes.avancement+=0.6;
            }
            else
            {
                animation_Jambes.avancement=0;
                animation_Jambes.tempsEnCours.restart();
                animation_Jambes.tempsAtteindre = (rand()%2+1);
            }

        }
        //animation_Jambes.avancement+=0.6;
    }

}

void Graph:: afficherSol(int obstacleX, int obstacleSize, int obstacleX_Prec, int obstacleSize_Prec, int decalage, bool avecBonus)
{
    sol.setSize(Vector2f(obstacleSize_Prec, 140));
    sol.setPosition(Vector2f(obstacleX_Prec, 470 + decalage));
    fenetre.draw(sol);

    sol.setSize(Vector2f(obstacleSize, 140));
    sol.setPosition(Vector2f(obstacleX, 470 + decalage));

    bonus.setPosition(Vector2f(obstacleX+(int)(obstacleSize/2), 470+decalage));
    if (avecBonus && animation_Bonus.etat==0)
    {
        animation_Bonus.etat=1;
        animation_Bonus.avancement=4;
        sprite_bonusUnPoint.setPosition(Vector2f(obstacleX+(int)(obstacleSize/2), 440));
    }
    else if (animation_Bonus.etat)
    {
        sprite_bonusUnPoint.setColor(Color(0,0,0,animation_Bonus.avancement*63));
        sprite_bonusUnPoint.setPosition(Vector2f(obstacleX+(int)(obstacleSize/2), sprite_bonusUnPoint.getPosition().y-animation_Bonus.avancement));
        fenetre.draw(sprite_bonusUnPoint);
        animation_Bonus.avancement-=0.15;
        if (animation_Bonus.avancement<0)
            animation_Bonus.etat=0;

    }

    //sprite_bonusUnPoint.setPosition(Vector2f(obstacleX+(int)(obstacleSize/2), 400));

    fenetre.draw(sol);
    fenetre.draw(bonus);
}

void Graph :: afficherPont(int positPont, int taille, float rotation, int decalage)
{
    pont.setSize(Vector2f(taille, 3));
    pont.setRotation(rotation-90);
    pont.setPosition(Vector2f(positPont, 470 + decalage));
    fenetre.draw(pont);

}

void Graph :: resetAnimation()
{
    animation_Jambes={0,0,0,0,(rand()%2+1)};
    animation_Jambes.tempsEnCours.restart();

    animation_Foulard.clock.restart();
    animation_Foulard.direction=1;
    animation_Foulard.tile=0;
}

void Graph :: afficherTableauScore (int score, int meilleurScore, int modeDeJeuRecois)
{
    //system("pause");
    if (animation_TableauScore.etat==0)
    {
        modeDeJeu=modeDeJeuRecois;
        animation_TableauScore.etat=1;
        sprite_tableauScore.setPosition(Vector2f(229,108)); //50
        //sprite_selecteur.setPosition(Vector2f(200+(modeDeJeu==3)*12, 201+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement));
        animation_TableauScore.avancement = 5;
    }
    //system("pause");
    int touche(0);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        touche=1;
    else if (Keyboard::isKeyPressed(Keyboard::Right))
        touche=2;
    if (touche==1 || touche==2)
    {
        if (animation_TableauScore.etat==1)
        {
            modeDeJeu = (modeDeJeu+(touche==2))%4 - (touche==1);
            if (modeDeJeu<0)
                modeDeJeu+=4;
            //modeDeJeu= 1-modeDeJeu;
            animation_TableauScore.etat=2;
            sprite_selecteur.setTextureRect(IntRect(52+93*modeDeJeu,260,93,20));
        }
    }
    else if (animation_TableauScore.etat==2)
        animation_TableauScore.etat=1;



    sprite_tableauScore.setColor(Color(255,255,255,255-51*animation_TableauScore.avancement));
    sprite_selecteur.setColor(Color(255,255,255,255-51*animation_TableauScore.avancement));

    if (animation_TableauScore.avancement<0.2 && animation_TableauScore.avancement>0)
        animation_TableauScore.avancement=0;
    else if (animation_TableauScore.avancement>0)
        animation_TableauScore.avancement-=0.2;

    sprite_selecteur.setPosition(Vector2f(180, 206+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement));
    sprite_tableauScore.setPosition(Vector2f(229,sprite_tableauScore.getPosition().y-animation_TableauScore.avancement)); //50
    fenetre.draw(sprite_tableauScore);
    fenetre.draw(sprite_selecteur);

    texte.affTexteMiddle(string("SCORE"), 232, 50+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement, fenetre);
    texte.affTexteMiddle(string("MEILLEUR"), 232, 105+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement, fenetre);
    texte.affTexteMiddle(score, 232, 76+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement, fenetre);
    texte.affTexteMiddle(meilleurScore, 232, 131+sprite_tableauScore.getPosition().y-animation_TableauScore.avancement, fenetre);




}

void Graph :: resetAnimationTableauScore()
{
    animation_TableauScore.etat=0;
}

int Graph :: getModeDeJeu ()
{
    return modeDeJeu;
}
void Graph :: afficherMode (int modeDeJeuRecois, int phase)
{
    if (modeDeJeuRecois)
    {
        if (phase==attenteAppuie || phase == enAppuie)
        {
            nuage.setFillColor(Color::Blue);
            fenetre.draw(nuage);
        }
        else if (phase==mouvementPont && animation_Nuage.etat==0)
        {
            animation_Nuage.etat=1;
            animation_Nuage.avancement=5;
            nuage.setFillColor(Color::Blue);
            fenetre.draw(nuage);
        }
        else if (phase==mouvementPont && animation_Nuage.etat)
        {
            nuage.setFillColor(Color(0,0,255,51*animation_Nuage.avancement));
            fenetre.draw(nuage);
            if (animation_Nuage.avancement>0)
            {
                animation_Nuage.avancement-=0.2;
                if (animation_Nuage.avancement<0)
                    animation_Nuage.avancement=0;
            }
        }
        else if (phase == mouvementPerso && animation_Nuage.etat)
            animation_Nuage.etat=0;
    }



   // animation_Nuage
}
