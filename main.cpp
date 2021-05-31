#include <iostream>
#include "funkcje.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iomanip>
#include <math.h>
#include <cstdlib>








int main() {

    sf::RenderWindow window(sf::VideoMode(1500, 700), "Pingwini skok");

    sf::Clock clock;

    std::string name = "Miki", path_="D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/pingwiny.png";
    int hp = 100;
    float x0 = (window.getSize().x-218*0.65)/2;
    float y0 = window.getSize().y-228*0.65;
    sf::Vector2f startPos = {x0,y0};
    sf::Vector2f startPos1 = {100,y0};

    Character hero(name, hp, startPos, path_);
    Character hero1(name, hp, startPos1, path_);
    hero.createBullet();

    //zmienne skoku bohatera
    bool skok = false, skok2 = false, shot = false, shot1=false, bull=false;
    float gathered=0, gathered_shot = 0, gathered_shot1 = 0;

    //zmienne skoku wroga
    bool skok_enemy= false, skok_enemy2=false, now=false;
    float gat=0;

    //automatyka wroga
    bool plat2[2]={false,false};
    sf::Vector2f dir1={0.0f, 0.0f};

    Money money("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/moneta.png"), money1("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/moneta.png");

    Platform platform("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/podloze.png", 0, 400, false);
    Platform platform1("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/podloze.png", 800, 250, true);
    Platform platform2("D:/AA_POLIBUDA/semestr 2/infa/grafika zaliczenie/podloze.png", 1400, 500, false);

    Info info;


    while (window.isOpen()) {

        auto event = sf::Event();

        sf::Time elapsed = clock.getElapsedTime();
        sf::Vector2f dir = {0.0f, 0.0f};
        float movementSpeed = 2.0f;




        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

//        std::cout<<hero.getPosition().y<<"\n"<<window.getSize().y-228*0.65<<"\n\n";
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !skok && !skok2){
            skok=true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            dir.y +=movementSpeed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            dir.x -=movementSpeed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            dir.x +=movementSpeed;
        }if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !shot1){
            shot=true;
            shot1=true;
            bull=true;
            hero.setBulletPosition();
        }

        if(shot){
            gathered_shot +=elapsed.asSeconds();
            if(gathered_shot>=0.1f){
                shot=false;
                gathered_shot=0;
            }
        }

        if(shot1){
            gathered_shot1 +=elapsed.asSeconds();
            if(gathered_shot1>=0.5f){
                shot1=false;
                gathered_shot1=0;
            }
        }

        if(skok){
            gathered+=elapsed.asSeconds();
            if(gathered<=0.25f){
                dir.y=-4.0f;
            }else{
                gathered=0;
                skok2=true;
                skok=false;
            }
        }



        if(skok2){
            if(hero.getPos().y<=startPos.y){
                dir.y=2.0f;
            }else{
                hero.setPosition(startPos.y);
                gathered=0;
                skok2=false;
                skok=false;
            }
        }


        //std::cout<<hero.getPosition().y<<"\n";

        hero.SetDirection(dir, shot);
        hero.colide(dir, window, platform.getPosition(), platform.getSize(), skok2, skok, startPos);
        hero.colide(dir, window, platform1.getPosition(), platform1.getSize(), skok2, skok, startPos);
        hero.colide(dir, window, platform2.getPosition(), platform2.getSize(), skok2, skok, startPos);
        hero.Update(elapsed.asSeconds());




        //czy hero jest na najwyzszej platformie, a enemy jest pod spodem (góra, prawo
        if(hero.getPos().y<=platform1.getPosition().y-platform1.getSize().y && hero.getPos().x+hero.getWidth()<=platform1.getPosition().x+platform1.getSize().x && hero.getPos().x>=platform1.getPosition().x
                && !plat2[0] && hero1.getPos().x>platform.getPosition().x+platform.getSize().x && hero1.getPos().y==startPos.y){
            plat2[0]=true;
            std::cout<<"pierwszy\n";

        }


        if(plat2[0]){
            dir1.x=-1.0f;
            if(hero1.getPos().x+hero1.getWidth()<platform.getPosition().x+platform.getSize().x && !skok_enemy && !skok_enemy2 ){
                dir1.x=0.0f;
                skok_enemy=true;
                plat2[0]=false;
                plat2[1]=true;
                //std::cout<<"drugi\n";
            }

        }else if(plat2[1] && !skok_enemy && !skok_enemy2){

            skok_enemy=true;
            plat2[1]=false;
            std::cout<<"trzeci\n\n";

        }else{
            if(hero1.getPos().x<hero.getPos().x-5){
                dir1.x=1.0f;
            }else if(hero1.getPos().x>hero.getPos().x+5){
                dir1.x=-1.0f;
            }else{
                dir1.x=0.0f;
            }




            if(hero.getPos().y<=platform2.getPosition().y && !skok_enemy && !skok_enemy2 && !now){
                now=true;
            }else{
                now=false;
            }

            if(now){
                gat=gat+=elapsed.asSeconds();
                if(gat>=0.5f){
                    now=false;
                    skok_enemy=true;
                    gat=0;
                }
            }

            //skok enemy
            if(skok_enemy){
                gat+=elapsed.asSeconds();
                if(gat<=0.4f){
                    dir1.y=-2.0f;
                }else{
                    gat=0;
                    skok_enemy2=true;
                    skok_enemy=false;
                }
            }



            if(skok_enemy2){
                if(hero1.getPos().y<=startPos.y){
                    dir1.y=2.0f;
                }else{
                    hero1.setPosition(startPos.y);
                    gat=0;
                    skok_enemy2=false;
                    skok_enemy=false;
                }
            }

        }

        hero1.SetDirection(dir1, shot);
        hero1.colide(dir1, window, platform.getPosition(), platform.getSize(), skok_enemy2, skok_enemy, startPos);
        hero1.colide(dir1, window, platform1.getPosition(), platform1.getSize(), skok_enemy2, skok_enemy, startPos);
        hero1.colide(dir1, window, platform2.getPosition(), platform2.getSize(), skok_enemy2, skok_enemy, startPos);
        hero1.Update(elapsed.asSeconds());


        window.clear(sf::Color::White);

        //rysowanie platformy
        platform.drawPlatform(window);
        platform1.drawPlatform(window);
        platform2.drawPlatform(window);
        //rysowanie bohatera
        hero.Draw(window, true);
        hero1.Draw(window, false);
        //rysowanie strzalu
        if(bull){
            hero.doShot(window, elapsed.asSeconds());
        }


        int score = money.getScore() + money1.getScore();

        info.draw(window, score);
        money.drawMoney(window, hero.getPos(), hero.getWidth(), hero.getHeight());
        money1.drawMoney(window, hero.getPos(), hero.getWidth(), hero.getHeight());
        elapsed = clock.restart();
        window.display();

    }




    return 0;
}
