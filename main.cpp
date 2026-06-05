
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "Ship.h"
#include "player.h"

#define _USE_MATH_DEFINES

using namespace std;





int main(){



    float skala = 30;

    sf::Vector2f rozdzielczosc(1620,820);


    sf::RenderWindow window(sf::VideoMode(rozdzielczosc.x,rozdzielczosc.y), "SFML works!");
    sf::View camera;
    camera.setSize(rozdzielczosc);

    sf::Clock clock;

    sf::Texture space_background;
    space_background.loadFromFile("src/kosmos_tlo.png");
    space_background.setRepeated(true);

    sf::RectangleShape background(rozdzielczosc);
    background.setOrigin(rozdzielczosc.x/2,rozdzielczosc.y/2);
    background.setTexture(&space_background);


    ///////////////////////////////////

    sf::RectangleShape healtbarbackgroud(sf::Vector2f(228,36));
    healtbarbackgroud.setOrigin(228,36);
    healtbarbackgroud.setPosition(rozdzielczosc.x-6,rozdzielczosc.y-6);
    healtbarbackgroud.setFillColor(sf::Color::White);
    healtbarbackgroud.setOutlineThickness(6);
    healtbarbackgroud.setOutlineColor(sf::Color::Black);


    sf::RectangleShape healthbar(sf::Vector2f(228,36));
    healthbar.setOrigin(0, 36);
    healthbar.setPosition(rozdzielczosc.x - 6 - 228, rozdzielczosc.y - 6);
    healthbar.setFillColor(sf::Color::Red);


    /////////////////////////////////////////



    sf::RectangleShape nitrobarbackground(sf::Vector2f(228,36));
    nitrobarbackground.setOrigin(228,36);
    nitrobarbackground.setPosition(rozdzielczosc.x-6,rozdzielczosc.y-6-48);
    nitrobarbackground.setFillColor(sf::Color::White);
    nitrobarbackground.setOutlineThickness(6);
    nitrobarbackground.setOutlineColor(sf::Color::Black);

    sf::RectangleShape nitrobar(sf::Vector2f(228,36));
    nitrobar.setOrigin(0, 36);
    nitrobar.setPosition(rozdzielczosc.x - 6 - 228, rozdzielczosc.y - 6-48);
    nitrobar.setFillColor(sf::Color::Cyan);



    /////////////////////////////////////////

    sf::RectangleShape storagebarbackground(sf::Vector2f(228,36));
    storagebarbackground.setOrigin(228,36);
    storagebarbackground.setPosition(rozdzielczosc.x-6,rozdzielczosc.y-6-96);
    storagebarbackground.setFillColor(sf::Color::White);
    storagebarbackground.setOutlineThickness(6);
    storagebarbackground.setOutlineColor(sf::Color::Black);

    sf::RectangleShape storagebar(sf::Vector2f(228,36));
    storagebar.setOrigin(0, 36);
    storagebar.setPosition(rozdzielczosc.x - 6 - 228, rozdzielczosc.y - 6-96);
    storagebar.setFillColor(sf::Color(218, 165, 32)); // Goldenrod color for metal scraps



    /////////////////////////////////////////


    sf::Vector2i mousePosition(0,0);
    sf::Vector2f mouseWorldPosition(0,0);


    ///////////////////////////////////

    sf::RectangleShape station(sf::Vector2f(100,100));
    station.setFillColor(sf::Color::Green);
    station.setPosition(1000,100);


    sf::Vector2f station_center(station.getGlobalBounds().left+(station.getSize().x/2),station.getGlobalBounds().top+(station.getSize().y/2) );

    sf::RectangleShape dockingmodule(sf::Vector2f(30,30));
    dockingmodule.setFillColor(sf::Color::Magenta);
    dockingmodule.setPosition(station.getPosition().x-dockingmodule.getSize().x, station.getPosition().y+station.getSize().y/2-dockingmodule.getSize().y/2);


    sf::FloatRect granice = dockingmodule.getGlobalBounds();

    float srodekX = granice.left + (granice.width / 2.0f);
    float srodekY = granice.top + (granice.height / 2.0f);

    sf::Vector2f prawdziwySrodek(srodekX, srodekY);


    sf::CircleShape dockingmodule_area(85);
    dockingmodule_area.setOrigin(85,85);
    dockingmodule_area.setOutlineColor(sf::Color::Yellow);
    dockingmodule_area.setOutlineThickness(3);
    dockingmodule_area.setFillColor(sf::Color::Transparent);
    dockingmodule_area.setPosition(prawdziwySrodek.x-dockingmodule.getSize().x,prawdziwySrodek.y);





    ////////////////

    Ship playerShip;
    playerShip.createShip();
    state activeState = state::STATEK;
    bool space_clicked;
    bool mapMode = false;

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "BĹ‚Ä…d Ĺ‚adowania czcionki arial.ttf" << endl;
    }

    sf::Text mapText;
    mapText.setFont(font);
    mapText.setString("MAP");
    mapText.setCharacterSize(50);
    mapText.setFillColor(sf::Color::White);
    mapText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = mapText.getLocalBounds();
    mapText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    mapText.setPosition(sf::Vector2f(rozdzielczosc.x / 2.0f, rozdzielczosc.y * 0.07f));

    sf::RectangleShape mapBorder(sf::Vector2f(rozdzielczosc.x * 0.8f, rozdzielczosc.y * 0.8f));
    mapBorder.setPosition(rozdzielczosc.x * 0.1f, rozdzielczosc.y * 0.15f);
    mapBorder.setFillColor(sf::Color::Transparent);
    mapBorder.setOutlineColor(sf::Color::White);
    mapBorder.setOutlineThickness(3.0f);

    player playerCharacter;
    playerCharacter.createPlayer();



    while (window.isOpen()){
        float dt = clock.restart().asSeconds();

        camera.setCenter(playerShip.getPosition());


        mousePosition = sf::Mouse::getPosition(window);
        mouseWorldPosition = window.mapPixelToCoords(mousePosition);

        playerShip.update(dt,mouseWorldPosition,mapMode ? state::ANIMACJA : activeState);

        sf::Vector2f cameraCenter = camera.getCenter();
        background.setPosition(cameraCenter);

        sf::Vector2f currentViewSize = camera.getSize();
        background.setSize(currentViewSize);
        background.setOrigin(currentViewSize.x / 2.f, currentViewSize.y / 2.f);

        sf::IntRect background_slice((int)(cameraCenter.x - currentViewSize.x/2),(int)(cameraCenter.y - currentViewSize.y/2),currentViewSize.x,currentViewSize.y);
        background.setTextureRect(background_slice);


        sf::Event event;

        space_clicked = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    if (!mapMode)
                    {
                        cout << "klikniety" << endl;
                        space_clicked = true;
                        activeState = state::LUDZIK;
                    }
                }
                if(event.key.code == sf::Keyboard::M)
                {
                    mapMode = !mapMode;
                    if (mapMode)
                    {
                        camera.setSize(rozdzielczosc * 3.f);
                        camera.setViewport(sf::FloatRect(0.1f, 0.15f, 0.8f, 0.8f));
                    }
                    else
                    {
                        camera.setSize(rozdzielczosc);
                        camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                    }
                }
            }
        }

        float health = playerShip.getCurrentHealth();
        float max_health = playerShip.getMaxHealth();

        float nitro = playerShip.getCurrentNitro();
        float max_nitro = playerShip.getMaxNitro();

        if(health/max_health >= 0)
        {
            healthbar.setScale(sf::Vector2f((health/max_health),1));
        }
        if(nitro/max_nitro >= 0)
        {
            if(playerShip.getNitroOnCooldown())
            {
                nitrobar.setFillColor(sf::Color::Blue);
            }
            else
            {
                nitrobar.setFillColor(sf::Color::Cyan);
            }
            nitrobar.setScale(sf::Vector2f((nitro/max_nitro),1));
        }

        float storage = playerShip.getCurrentStorage();
        float max_storage = playerShip.getMaxStorage();

        if (max_storage > 0 && storage/max_storage >= 0)
        {
            storagebar.setScale(sf::Vector2f((storage/max_storage),1));
        }



        float dystans = sqrt(pow(((prawdziwySrodek.x-dockingmodule.getSize().x) - playerShip.getPosition().x),2) + pow((prawdziwySrodek.y - playerShip.getPosition().y),2));
        if(dystans < 85)
        {
            cout << "Gracz w zasiegu" << endl;
             if(space_clicked)
             {
                if(activeState==state::STATEK)
                {
                    activeState = state::LUDZIK;
                }
                else if(activeState==state::LUDZIK)
                {
                    activeState = state::STATEK;
                }
             }
        }


        sf::FloatRect penetrationDepth;
        if(playerShip.sprite.getGlobalBounds().intersects(station.getGlobalBounds(),penetrationDepth))
        {
            if(penetrationDepth.width<penetrationDepth.height)
            {
                if(playerShip.getPosition().x < station_center.x)
                {
                    playerShip.collisionMove(-penetrationDepth.width,0,1);
                }
                else
                {
                    playerShip.collisionMove(penetrationDepth.width,0,1);
                }
            }
            else
            {
                if(playerShip.getPosition().y < station_center.y)
                {
                    playerShip.collisionMove(0,-penetrationDepth.height,1);
                }
                else
                {
                    playerShip.collisionMove(0,penetrationDepth.height,1);
                }
            }

        }
        sf::FloatRect penetrationDepth2;
        if(playerShip.sprite.getGlobalBounds().intersects(dockingmodule.getGlobalBounds(),penetrationDepth2))
        {
            if(penetrationDepth2.width<penetrationDepth2.height)
            {
                if(playerShip.getPosition().x < prawdziwySrodek.x)
                {
                    playerShip.collisionMove(-penetrationDepth2.width,0,1);
                }
                else
                {
                    playerShip.collisionMove(penetrationDepth2.width,0,1);
                }
            }
            else
            {
                if(playerShip.getPosition().y < prawdziwySrodek.y)
                {
                    playerShip.collisionMove(0,-penetrationDepth2.height,1);
                }
                else
                {
                    playerShip.collisionMove(0,penetrationDepth2.height,1);
                }
            }

        }



        window.clear();

        window.setView(camera);

        window.draw(background);
        if(activeState==state::STATEK)
        {

            window.draw(playerShip.sprite);


            window.draw(station);
            window.draw(dockingmodule);
            window.draw(dockingmodule_area);


            window.setView(window.getDefaultView());

            if (mapMode)
            {
                window.draw(mapBorder);
                window.draw(mapText);
            }
            else
            {
                window.draw(healtbarbackgroud);
                window.draw(healthbar);

                window.draw(nitrobarbackground);
                window.draw(nitrobar);

                window.draw(storagebarbackground);
                window.draw(storagebar);
            }
        }

        else if(activeState==state::LUDZIK)
        {
            window.draw(playerCharacter.sprite);
        }



        window.setView(camera);
        window.display();
    }

    return 0;
}


