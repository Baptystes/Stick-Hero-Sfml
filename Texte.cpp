//Par Baptystes
//Youtube : https://www.youtube.com/user/Baptystes
//© Baptystes 2014

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "Texte.h"
#include <string>
using namespace std;
using namespace sf;

TexteObj :: TexteObj()
{
    if (!font.loadFromFile("files/ClearSans-Bold.ttf"))
        exit(1);

    m_texte.setFont(font);
    m_texte.setCharacterSize(20);
    m_texte.setColor(Color::Black);
}



void TexteObj :: affTexte(string &texte, int positionX, int positionY, RenderWindow &fenetre,int taille, int rotation)
{
    m_texte.setString(texte);
    m_texte.setRotation(rotation);
    m_texte.setCharacterSize(taille);
    m_texte.setPosition(Vector2f(positionX, positionY));
    fenetre.draw(m_texte);
   // m_texte.setPosition()
}

void TexteObj :: affTexteo(char texte[], int positionX, int positionY, RenderWindow &fenetre,int taille, int rotation)
{
    string texteAff = texte;
    m_texte.setString(texte);
    m_texte.setRotation(rotation);
    m_texte.setCharacterSize(taille);
    m_texte.setPosition(Vector2f(positionX, positionY));
    fenetre.draw(m_texte);
   // m_texte.setPosition()
}

void TexteObj :: affTexte(int chiffre, int positionX, int positionY, RenderWindow &fenetre,int taille, int rotation)
{
    ostringstream texte_Converti;

    texte_Converti.seekp(0);
    texte_Converti<<chiffre;
    Text texteAff;
    m_texte.setString(texte_Converti.str());

    m_texte.setRotation(rotation);
    m_texte.setCharacterSize(taille);
    m_texte.setPosition(Vector2f(positionX, positionY));
    fenetre.draw(m_texte);
   // texteAff.setPosition()
}

void TexteObj :: affTexteMiddle(string texte, int positionX, int positionY, RenderWindow &fenetre,int taille, int rotation)
{
    m_texte.setString(texte);
    m_texte.setRotation(rotation);
    m_texte.setCharacterSize(taille);
    m_texte.setPosition(Vector2f(positionX - (int)(m_texte.getGlobalBounds().width/2), positionY));
    fenetre.draw(m_texte);
   // m_texte.setPosition()
}

void TexteObj :: affTexteMiddle(int chiffre, int positionX, int positionY, RenderWindow &fenetre,int taille, int rotation)
{
    ostringstream texte_Converti;

    texte_Converti.seekp(0);
    texte_Converti<<chiffre;
    m_texte.setString(texte_Converti.str());
   // m_texte.setString(texte);
    m_texte.setRotation(rotation);
    m_texte.setCharacterSize(taille);
    m_texte.setPosition(Vector2f(positionX - (int)(m_texte.getGlobalBounds().width/2), positionY));
    fenetre.draw(m_texte);
   // m_texte.setPosition()
}
