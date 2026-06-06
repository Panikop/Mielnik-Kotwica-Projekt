
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "Ship.h"
#include "player.h"
#include "UIBar.h"
#include "fraction.h"
#include <vector>

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


    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Blad ladowania czcionki arial.ttf" << endl;
    }

    // Nowoczesna stylizacja i układ pasków
    sf::Vector2f barSize(230.f, 28.f);
    float marginX = 12.f;
    float marginY = 12.f;
    float spacingY = 38.f;

    UIBar healthBar(
        sf::Vector2f(rozdzielczosc.x - barSize.x - marginX, rozdzielczosc.y - barSize.y - marginY),
        barSize,
        sf::Color(220, 50, 50), // Karmazynowa czerwień
        "HP",
        font
    );

    UIBar nitroBar(
        sf::Vector2f(rozdzielczosc.x - barSize.x - marginX, rozdzielczosc.y - barSize.y - marginY - spacingY),
        barSize,
        sf::Color(0, 190, 220), // Neonowy błękit
        "NITRO",
        font
    );

    UIBar storageBar(
        sf::Vector2f(rozdzielczosc.x - barSize.x - marginX, rozdzielczosc.y - barSize.y - marginY - 2.f * spacingY),
        barSize,
        sf::Color(220, 160, 30), // Złocisty/Brązowy
        "CARGO",
        font
    );

    bool upgradeMode = false;
    UIUpgradeMenu upgradeMenu(rozdzielczosc, font);

    bool fractionsMode = false;
    UIFractionsMenu fractionsMenu(rozdzielczosc, font);


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

    // Dodanie 4 przykładowych frakcji z planetami
    std::vector<Fraction> fractions;
    fractions.push_back(Fraction("Centari Alliance", -600.f, -400.f, 20));
    fractions.push_back(Fraction("Vectron Mining", 1500.f, -800.f, 20));
    fractions.push_back(Fraction("Orion Republic", -1200.f, 1200.f, 20));
    fractions.push_back(Fraction("Sol Empire", 2200.f, 800.f, 20));

    // Inicjalizacja planet dla frakcji (czcionka, promien planety, promien interakcji, kolor)
    fractions[0].createPlanet(font, 80.f, 160.f, sf::Color(220, 100, 100)); // Czerwona planeta
    fractions[1].createPlanet(font, 60.f, 130.f, sf::Color(100, 220, 100)); // Zielona planeta
    fractions[2].createPlanet(font, 100.f, 200.f, sf::Color(100, 100, 220)); // Niebieska planeta
    fractions[3].createPlanet(font, 50.f, 110.f, sf::Color(220, 180, 50));  // Zolta planeta

    while (window.isOpen()){
        float dt = clock.restart().asSeconds();

        camera.setCenter(playerShip.getPosition());


        mousePosition = sf::Mouse::getPosition(window);
        mouseWorldPosition = window.mapPixelToCoords(mousePosition);

        playerShip.update(dt,mouseWorldPosition,(mapMode || upgradeMode || fractionsMode) ? state::ANIMACJA : activeState);

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
                    if (!mapMode && !upgradeMode && !fractionsMode)
                    {
                        cout << "klikniety" << endl;
                        space_clicked = true;
                        activeState = state::LUDZIK;
                    }
                }
                if(event.key.code == sf::Keyboard::M)
                {
                    if (!upgradeMode && !fractionsMode)
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
                if(event.key.code == sf::Keyboard::U)
                {
                    if (!mapMode && !fractionsMode)
                    {
                        upgradeMode = !upgradeMode;
                    }
                }
                if(event.key.code == sf::Keyboard::F)
                {
                    if (!mapMode && !upgradeMode)
                    {
                        fractionsMode = !fractionsMode;
                    }
                }
                if(event.key.code == sf::Keyboard::Escape)
                {
                    if (upgradeMode)
                    {
                        upgradeMode = false;
                    }
                    if (fractionsMode)
                    {
                        fractionsMode = false;
                    }
                }
                if(upgradeMode)
                {
                    if(event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
                    {
                        playerShip.upgradeMaxHealth();
                    }
                    if(event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
                    {
                        playerShip.upgradeMaxNitro();
                    }
                    if(event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3)
                    {
                        playerShip.upgradeMaxStorage();
                    }
                    if(event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4)
                    {
                        playerShip.upgradeShipSpeed();
                    }
                }
            }
        }

        healthBar.update(dt, playerShip.getCurrentHealth(), playerShip.getMaxHealth());
        nitroBar.update(dt, playerShip.getCurrentNitro(), playerShip.getMaxNitro(), playerShip.getNitroOnCooldown());
        storageBar.update(dt, playerShip.getCurrentStorage(), playerShip.getMaxStorage());

        if (upgradeMode)
        {
            upgradeMenu.update(playerShip);
        }
        if (fractionsMode)
        {
            fractionsMenu.update(fractions);
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

        // Sprawdzanie, czy gracz jest w obszarze interakcji jakiejkolwiek planety frakcji
        for (auto& fraction : fractions) {
            sf::Vector2f shipPos = playerShip.getPosition();
            sf::Vector2f planetPos = fraction.getLocation();
            sf::Vector2f diff = shipPos - planetPos;
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y);

            if (dist < fraction.interactionArea.getRadius()) {
                cout << "W obszarze" << endl;
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

        // Kolizje z planetami frakcji
        for (auto& fraction : fractions) {
            sf::Vector2f shipPos = playerShip.getPosition();
            sf::Vector2f planetPos = fraction.getLocation();
            sf::Vector2f diff = shipPos - planetPos;
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
            float minDistance = fraction.planetShape.getRadius() + 15.f; // 15.f to przyblizony promien statku

            if (dist < minDistance) {
                sf::Vector2f pushDir(1.f, 0.f);
                if (dist > 0.001f) {
                    pushDir = diff / dist;
                }
                float overlap = minDistance - dist;
                playerShip.collisionMove(pushDir.x * overlap, pushDir.y * overlap, 1.f);
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

            // Rysowanie planet, obszarow interakcji oraz nazwy frakcji
            for (auto& fraction : fractions) {
                window.draw(fraction.planetShape);
                window.draw(fraction.interactionArea);
                window.draw(fraction.nameText);
            }


            window.setView(window.getDefaultView());

            if (mapMode)
            {
                window.draw(mapBorder);
                window.draw(mapText);
            }
            else
            {
                healthBar.draw(window);
                nitroBar.draw(window);
                storageBar.draw(window);

                if (upgradeMode)
                {
                    upgradeMenu.draw(window);
                }
                if (fractionsMode)
                {
                    fractionsMenu.draw(window);
                }
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


