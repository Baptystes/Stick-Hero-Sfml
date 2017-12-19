#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include <cmath>
#include "Texte.h"
#include "Perso.h"
#include <string>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>


#define vitesseDeplacementLVL 7
#define vitesseCreationPONT 7
using namespace sf;

typedef struct Decalage Decalage;
struct Decalage
{
    float avancement;
    float decalageEnCours;

};
void startGame (int &positX, int &positY, int &obstacleX, int &obstacleSize, int &obstacleX_Prec, int &obstacleSize_Prec, int &pont_Taille, int &pont_Posit, int &pont_Taille_Anc, int &pont_Posit_Anc, int &phase, int checkDeplacement[3], Decalage &decalage, int &score);

int main()
{
    srand(time(NULL));

    FILE *fichierScore = NULL;
    fichierScore=fopen("files/scores.txt", "r+");

    if (fichierScore==NULL)
    {

        fichierScore=fopen("files/scores.txt", "w");
        fclose(fichierScore);
        fichierScore=fopen("files/scores.txt", "r+");
        fputs("0 0 0 0", fichierScore);
    }
    rewind(fichierScore);


    RenderWindow fenetre(VideoMode(456,600,32), "StickHero!");  //338
    fenetre.setFramerateLimit(60);
    TexteObj texte;
    Graph graph(fenetre, texte);

    Event event;
    SoundBuffer buffer_pontGrandie;
    buffer_pontGrandie.loadFromFile("files/stickGrandie.wav");
    Sound son_PontGrandie;
    son_PontGrandie.setBuffer(buffer_pontGrandie);
    son_PontGrandie.setLoop(1);

    SoundBuffer buffer_pontPousse;
    buffer_pontPousse.loadFromFile("files/stickPousse.wav");
    Sound son_PontPousse;
    son_PontPousse.setBuffer(buffer_pontPousse);

    SoundBuffer buffer_pontTombe;
    buffer_pontTombe.loadFromFile("files/stickTombe.wav");
    Sound son_PontTombe;
    son_PontTombe.setBuffer(buffer_pontTombe);

    SoundBuffer buffer_score;
    buffer_score.loadFromFile("files/score.wav");
    Sound son_Score;
    son_Score.setBuffer(buffer_score);

    SoundBuffer buffer_mort;
    buffer_mort.loadFromFile("files/mort.wav");
    Sound son_Mort;
    son_Mort.setBuffer(buffer_mort);

    SoundBuffer buffer_bonus;
    buffer_bonus.loadFromFile("files/bonus.wav");
    Sound son_Bonus;
    son_Bonus.setBuffer(buffer_bonus);

    fenetre.pollEvent(event);
    bool quitter=0;

    int positX(50), positY(441), positFinale(0);
    int obstacleX(200), obstacleSize(50);
    int obstacleX_Prec(0), obstacleSize_Prec(200);

    int pont_Taille(0);
    int pont_Posit(0);

    int pont_Taille_Anc(0);
    int pont_Posit_Anc(0);

    float pont_Rotation(0);
    float pont_Gravity;
    Clock pont_Temps;

    int phase(attenteAppuie);
    int checkDeplacement[3];
    for (int a=0; a<3; a++)
        checkDeplacement[a]=0;
    int somme=0;
    int pont_Ecart;

    int etatVictoire;

    //int decalage=0;
    float decalage_phase=0;
    float decalate_enCours=0;

    int score, meilleurScore[4];
    int obtientBonus;
    for (int a=0; a<4; a++)
    {
        fscanf(fichierScore, "%d", &obtientBonus);
        meilleurScore[a] = obtientBonus;
    }

    fclose(fichierScore);

    int modeDeJeu(0);
    Clock clock;
    int var_Etat=0;

    Decalage decalage;
    decalage = {0.1,0};
    obtientBonus=0;
    //obstacleX_Prec = positX - (obstacleSize_Prec-24);

    startGame (positX,positY,obstacleX,obstacleSize,obstacleX_Prec,obstacleSize_Prec,pont_Taille, pont_Posit,pont_Taille_Anc,pont_Posit_Anc,phase, checkDeplacement, decalage, score);
    while (!quitter)
    {
        fenetre.pollEvent(event);
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)  //(event.type == Event::Closed)// || (
            quitter=1;

        if (Keyboard::isKeyPressed(Keyboard::Space) && phase==attenteAppuie)
        {
            phase = enAppuie;
            pont_Taille=0;
            pont_Posit = positX+24;
            pont_Rotation=0;
            son_PontGrandie.play();
        }
///
        if (phase==enAppuie && Keyboard::isKeyPressed(Keyboard::Space))// && pont_Taille<(obstacleX+(int)(obstacleSize/2)-pont_Posit)))
        {
            pont_Taille+= (pont_Taille<600)*vitesseCreationPONT;

        }
        else if (phase==enAppuie && (!Keyboard::isKeyPressed(Keyboard::Space)))
        {
            // pont_Taille  = obstacleX+(int)(obstacleSize/2)-pont_Posit;
///
            phase=mouvementPont;
            son_PontGrandie.stop();
            pont_Temps.restart();
            pont_Gravity=0;
            somme=0;
            graph.resetAnimation();
            var_Etat=0;
        }
        if (phase==mouvementPont && pont_Rotation< 84)
        {
            if (pont_Temps.getElapsedTime().asMicroseconds()>5 && somme==0)
            {
                son_PontPousse.play();
                somme=1;
            }
            if  (pont_Temps.getElapsedTime().asMilliseconds()>100)
            {
                pont_Gravity+=0.14;
                pont_Rotation+=pont_Gravity;
            }
        }
        else if (phase==mouvementPont && pont_Rotation>=84)
        {

            pont_Rotation=90;
            graph.resetAnimation();
            son_PontTombe.play();
            phase=mouvementPerso;
            if ((pont_Posit+pont_Taille >= obstacleX) && (pont_Posit+pont_Taille <= (obstacleX+obstacleSize)))
            {

                if ((pont_Posit+pont_Taille >= (obstacleX + (int)(obstacleSize/2)-3)) && (pont_Posit+pont_Taille <= (obstacleX + (int)(obstacleSize/2)+3)))
                    obtientBonus=1;
                //else
                   // obtientBonus=1;
                positFinale = (obstacleX+obstacleSize-24);
                etatVictoire=passed;
            }
            else
            {
                if ((pont_Posit+pont_Taille)>458)
                    positFinale = 462;
                else
                    positFinale = pont_Posit+pont_Taille;
                etatVictoire=perdu;
            }
        }

        if (phase == mouvementPerso)
        {
            if (obtientBonus==1)
            {
                 obtientBonus=2;
                 son_Bonus.play();
                 score+=1;
            }
            else if (obtientBonus==2)
                obtientBonus=0;

            if (positX < (positFinale-5))
                positX+=5;
            else
                {
                    positX = (positFinale);
                    phase = enumEtatVictoire;
                    if (etatVictoire!=perdu)
                        son_Score.play();
                }
        }

        if (phase == enumEtatVictoire)
        {
            if (etatVictoire==passed)
                {
                    phase = deplacementLevel;
                    pont_Ecart=0;
                    positFinale=0;
                    score+=1;

                }
            else
                phase = persoTombe;
        }

        if (phase == persoTombe)
        {
            if (positY<700)
                positY+=8;
            else
            {
                son_Mort.play();
                if (score>meilleurScore[modeDeJeu])
                {
                    meilleurScore[modeDeJeu]=score;
                    fichierScore=fopen("files/scores.txt", "w+");
                    std::cout<<meilleurScore[modeDeJeu];
                    fprintf(fichierScore, "%d %d %d %d", meilleurScore[0], meilleurScore[1], meilleurScore[2], meilleurScore[3]);

                    fclose(fichierScore);
                }

                phase = tremblement;
                //decalage=0;
                somme=1;
                //startGame (positX,positY,obstacleX,obstacleSize,obstacleX_Prec,obstacleSize_Prec,pont_Taille, pont_Posit,pont_Taille_Anc,pont_Posit_Anc,phase, checkDeplacement);
            }
            if (pont_Rotation<180)
            {
                pont_Rotation+=5;
                if (pont_Rotation>180)
                    pont_Rotation=180;
            }
        }
        if (phase == tremblement)
        {

            decalage.decalageEnCours=10*sin(decalage.avancement)/decalage.avancement;
            decalage.avancement+=0.5;
            if (decalage.avancement>15)
            {
                phase=attenteAppuie;
                //startGame (positX,positY,obstacleX,obstacleSize,obstacleX_Prec,obstacleSize_Prec,pont_Taille, pont_Posit,pont_Taille_Anc,pont_Posit_Anc,phase, checkDeplacement, decalage);
                phase=tableauScore;
            }
        }
        if (phase == deplacementLevel)
        {

            somme=0;
            for (int a=0; a<3; a++)
            {
                somme+=checkDeplacement[a];
            }
            //0: Gauche a dépacé l'écran
            //1: Bonhomme est bien placé au départ
            //2: Le sol de droite est en place
            if (somme!=3) //positX>56)
            {
                if (!checkDeplacement[1])
                {
                    positX-=vitesseDeplacementLVL;
                    pont_Posit_Anc-=vitesseDeplacementLVL;
                    pont_Posit-=vitesseDeplacementLVL;
                    obstacleX_Prec-=vitesseDeplacementLVL;
                    obstacleX-=vitesseDeplacementLVL;
                    if (positX<=(50+vitesseDeplacementLVL))
                    {
                        pont_Ecart=positX-50;
                        positX=50;
                        //pont_Posit_Anc=200;
                        checkDeplacement[1]=1;
                        //checkDeplacement[0]=1;
                        obstacleX_Prec = positX - (obstacleSize_Prec-24);
                    }
                }
                if ((obstacleX_Prec+obstacleSize_Prec)<0)
                {
                    checkDeplacement[0]=1;
                    obstacleX_Prec = obstacleX;
                    obstacleSize_Prec = obstacleSize;
///
                    positFinale = 61 + (rand()%30)*7;// + 24;
                    //positFinale=61+29*7;// + 30*6 ;
                    obstacleSize = 20 + (rand()%22)*4;
///
                    //obstacleSize = 24 + 21*4; //+ 5*16;
                    obstacleX = 500;
                }
                if (checkDeplacement[2]==0 && checkDeplacement[0])
                {
                    obstacleX -= (obstacleX>(positFinale+obstacleX_Prec+obstacleSize_Prec))*10;
                    if (obstacleX<=(positFinale+obstacleX_Prec+obstacleSize_Prec))
                    {
                        checkDeplacement[2]=1;
                        obstacleX=positFinale+obstacleX_Prec+obstacleSize_Prec;
                    }
                }
            }
            else
            {

                positX=50;
                //obstacleSize_Prec = obstacleSize;
                //obstacleX_Prec = positX - (obstacleSize_Prec-24);
                //obstacleX=400;
                pont_Taille_Anc=pont_Taille;
                pont_Taille=0;
                pont_Posit -= pont_Ecart;
                pont_Posit_Anc = pont_Posit;
                phase=attenteAppuie;
                for (int a=0; a<3; a++)
                    checkDeplacement[a]=0;
                obstacleX=positFinale+obstacleX_Prec+obstacleSize_Prec;
            }

        }

        fenetre.clear(Color::Blue);
        graph.afficherDecors(phase);


        if ((modeDeJeu == 2 || modeDeJeu==3) && ((phase==enAppuie) || (phase == attenteAppuie) || phase==deplacementLevel))
        {
            if (var_Etat==0 && phase != deplacementLevel)
            {
                var_Etat=1;
                clock.restart();
                graph.afficherSol(obstacleX, obstacleSize, obstacleX_Prec, obstacleSize_Prec, (int)decalage.decalageEnCours, obtientBonus);
            }
            else if (var_Etat==1)
            {
                if (clock.getElapsedTime().asMilliseconds()<200)
                    graph.afficherSol(obstacleX, obstacleSize, obstacleX_Prec, obstacleSize_Prec, (int)decalage.decalageEnCours, obtientBonus);
                else
                    graph.afficherSol(700, obstacleSize, obstacleX_Prec, obstacleSize_Prec, 0, obtientBonus);

            }
            if (phase==deplacementLevel && checkDeplacement[0])
                graph.afficherSol(700, obstacleSize, obstacleX_Prec, obstacleSize_Prec, 0, obtientBonus);
            else if (phase == deplacementLevel)
                graph.afficherSol(obstacleX, obstacleSize, obstacleX_Prec, obstacleSize_Prec, (int)decalage.decalageEnCours, obtientBonus);
        }
        else
            graph.afficherSol(obstacleX, obstacleSize, obstacleX_Prec, obstacleSize_Prec, (int)decalage.decalageEnCours, obtientBonus);



        graph.afficherPerso(positX,positY, phase);
        if (!((modeDeJeu==1 || modeDeJeu==3)&& phase == enAppuie))
            graph.afficherPont(pont_Posit, pont_Taille, pont_Rotation, (int)decalage.decalageEnCours);
        graph.afficherPont(pont_Posit_Anc, pont_Taille_Anc, 90, (int)decalage.decalageEnCours);
        //graph.afficherMode(modeDeJeu, phase);


        if (phase == tableauScore)
        {
            //system("pause");

            graph.afficherTableauScore(score, meilleurScore[graph.getModeDeJeu()], modeDeJeu);
            //startGame (positX,positY,obstacleX,obstacleSize,obstacleX_Prec,obstacleSize_Prec,pont_Taille, pont_Posit,pont_Taille_Anc,pont_Posit_Anc,phase, checkDeplacement, decalage, score);
            //system("pause");
            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                startGame (positX,positY,obstacleX,obstacleSize,obstacleX_Prec,obstacleSize_Prec,pont_Taille, pont_Posit,pont_Taille_Anc,pont_Posit_Anc,phase, checkDeplacement, decalage, score);
                phase=attenteAppuie;
                modeDeJeu=graph.getModeDeJeu();
                graph.resetAnimationTableauScore();
                while (Keyboard::isKeyPressed(Keyboard::Space))
                {

                }
            }
        }
        else
        {
             texte.affTexteMiddle(score, 228, 20, fenetre, 35);
        }
        texte.affTexteMiddle("Baptystes", 32, -1, fenetre, 12);

        fenetre.display();


    }

    fenetre.close();


}

void startGame (int &positX, int &positY, int &obstacleX, int &obstacleSize, int &obstacleX_Prec, int &obstacleSize_Prec, int &pont_Taille, int &pont_Posit, int &pont_Taille_Anc, int &pont_Posit_Anc, int &phase, int checkDeplacement[3], Decalage &decalage, int &score)
{
    score=0;
    positX=50, positY=441;
    decalage={0.1,0};
    obstacleX = 150 + (rand()%30)*7;
    //obstacleX= 140 + (rand()%30)*6;
                    //obstacleSize = 25 + 5*16;
    obstacleSize = 24 + (rand()%22)*4;

    //obstacleX=200, obstacleSize=50;
    obstacleX_Prec=0, obstacleSize_Prec=200;

    pont_Taille=0;
    pont_Posit=0;

    pont_Taille_Anc=0;
    pont_Posit_Anc=0;
    phase=0;
    obstacleX_Prec = positX - (obstacleSize_Prec-24);

    for (int a=0; a<3; a++)
        checkDeplacement[a]=0;
}

