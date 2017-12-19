//Par Baptystes
//Youtube : https://www.youtube.com/user/Baptystes
//© Baptystes 2014

#ifndef TEXTE
#define TEXTE

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

class TexteObj
    {
    public:
    TexteObj();
    void affTexte(std::string &texte, int positionX, int positionY, sf::RenderWindow &fenetre,int taille = 20, int rotation = 0);
    void affTexteo(char texte[], int positionX, int positionY, sf::RenderWindow &fenetre,int taille = 20, int rotation = 0);
    void affTexte(int chiffre, int positionX, int positionY, sf::RenderWindow &fenetre,int taille = 20, int rotation = 0);
    void affTexteMiddle(std::string texte, int positionX, int positionY, sf::RenderWindow &fenetre,int taille=20, int rotation=0);
    void affTexteMiddle(int chiffre, int positionX, int positionY, sf::RenderWindow &fenetre,int taille=20, int rotation=0);


    private:
        sf::Text m_texte;
        std::ostringstream m_scoreTexte_converti;
        sf::Font font;
    };

#endif
