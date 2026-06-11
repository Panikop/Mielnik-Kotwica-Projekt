#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Ship.h"
#include "UIBar.h"
#include "UIUpgradeMenu.h"
#include "UIFractionsMenu.h"
#include "UIInventoryMenu.h"
#include "UISaveMenu.h"
#include "UIInventoryBar.h"
#include "Projectile.h"
#include "Mine.h"
#include "Explosion.h"
#include "Enemy.h"
#include "fraction.h"
#include "LocalMap.h"
#include "LocalResource.h"
#include "mainmenu.h"
#include "player.h"
#include "SaveSystem.h"
#include "Textures.h"

#include <vector>

#define _USE_MATH_DEFINES

#include "Textures.h"
using namespace std;

sf::Texture Textures::shipTex;
sf::Texture Textures::bulletTex;
sf::Texture Textures::npcTex;
sf::Texture Textures::playerTex;
sf::Texture Textures::ufoTex;
sf::Texture Textures::explosionTex;
sf::Texture Textures::mineTex;
sf::Texture Textures::stationTex;

int main() {
  Textures::load();

  float skala = 30;

  sf::Vector2f rozdzielczosc(1620, 820);

  sf::RenderWindow window(sf::VideoMode(rozdzielczosc.x, rozdzielczosc.y),
                          "PANIC ANCHOR");
  sf::View camera;
  camera.setSize(rozdzielczosc);

  sf::Clock clock;

  sf::Texture space_background;
  space_background.loadFromFile("textures/kosmos_tlo.png");
  space_background.setRepeated(true);

  sf::RectangleShape background(rozdzielczosc);
  background.setOrigin(rozdzielczosc.x / 2, rozdzielczosc.y / 2);
  background.setTexture(&space_background);

  sf::Font font;
  if (!font.loadFromFile("textures/arial.ttf")) {
    cout << "Blad ladowania czcionki arial.ttf" << endl;
  }

  // Nowoczesna stylizacja i układ pasków
  sf::Vector2f barSize(230.f, 28.f);
  float marginX = 12.f;
  float marginY = 12.f;
  float spacingY = 38.f;

  UIBar healthBar(sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                               rozdzielczosc.y - barSize.y - marginY),
                  barSize, sf::Color(220, 50, 50), // Karmazynowa czerwień
                  "HP", font);

  UIBar nitroBar(sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                              rozdzielczosc.y - barSize.y - marginY - spacingY),
                 barSize, sf::Color(0, 190, 220), // Neonowy błękit
                 "NITRO", font);

  UIBar shieldBar(
      sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                   rozdzielczosc.y - barSize.y - marginY - 2.f * spacingY),
      barSize, sf::Color(50, 100, 255), // Niebieski dla tarczy
      "SHIELD", font);

  UIInventoryBar inventoryBar(
      sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                   rozdzielczosc.y - barSize.y - marginY - 3.f * spacingY),
      barSize, font);

  bool upgradeMode = false;
  UIUpgradeMenu upgradeMenu(rozdzielczosc, font);

  bool fractionsMode = false;
  UIFractionsMenu fractionsMenu(rozdzielczosc, font);

  bool inventoryMode = false;
  UIInventoryMenu inventoryMenu(rozdzielczosc, font);

  bool saveMode = false;
  UISaveMenu saveMenu(rozdzielczosc, font);

  sf::Vector2i mousePosition(0, 0);
  sf::Vector2f mouseWorldPosition(0, 0);

  //////////////////////////////
  sf::CircleShape station(256.f);
  station.setTexture(&Textures::stationTex);
  station.setFillColor(sf::Color::White);
  station.setPosition(1000, 100);

  sf::Vector2f station_center(station.getPosition().x + 256.f,
                              station.getPosition().y + 256.f);

  // Strefa dokowania
  sf::Vector2f prawdziwySrodek(station.getPosition().x, station_center.y);

  sf::CircleShape dockingmodule_area(85);
  dockingmodule_area.setOrigin(85, 85);
  dockingmodule_area.setOutlineColor(sf::Color::Yellow);
  dockingmodule_area.setOutlineThickness(3);
  dockingmodule_area.setFillColor(sf::Color::Transparent);
  dockingmodule_area.setPosition(prawdziwySrodek);

  ////////////////

  Ship playerShip;
  playerShip.createShip();
  state activeState = state::MENU;
  MainMenu mainMenu(rozdzielczosc, font);

  sf::Texture deathBackgroundTex;
  if (!deathBackgroundTex.loadFromFile("textures/menu_bg.png")) {
    cout << "Blad ladowania textures/menu_bg.png" << endl;
  }
  sf::RectangleShape deathBackgroundShape(rozdzielczosc);
  deathBackgroundShape.setTexture(&deathBackgroundTex);
  deathBackgroundShape.setFillColor(sf::Color(255, 0, 0, 130));

  sf::Text deathText;
  deathText.setFont(font);
  deathText.setString("ZGINALES");
  deathText.setCharacterSize(80);
  deathText.setFillColor(sf::Color::Red);
  deathText.setStyle(sf::Text::Bold);
  sf::FloatRect dBounds = deathText.getLocalBounds();
  deathText.setOrigin(dBounds.left + dBounds.width / 2.f,
                      dBounds.top + dBounds.height / 2.f);
  deathText.setPosition(rozdzielczosc.x / 2.f, rozdzielczosc.y * 0.4f);

  sf::Text returnMenuText;
  returnMenuText.setFont(font);
  returnMenuText.setString("Powrot do menu");
  returnMenuText.setCharacterSize(40);
  returnMenuText.setFillColor(sf::Color::Yellow);
  sf::FloatRect rmBounds = returnMenuText.getLocalBounds();
  returnMenuText.setOrigin(rmBounds.left + rmBounds.width / 2.f,
                           rmBounds.top + rmBounds.height / 2.f);
  returnMenuText.setPosition(rozdzielczosc.x / 2.f, rozdzielczosc.y * 0.6f);

  bool space_clicked;
  bool mapMode = false;

  sf::Text mapText;
  mapText.setFont(font);
  mapText.setString("MAP");
  mapText.setCharacterSize(50);
  mapText.setFillColor(sf::Color::White);
  mapText.setStyle(sf::Text::Bold);
  sf::FloatRect textRect = mapText.getLocalBounds();
  mapText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
  mapText.setPosition(
      sf::Vector2f(rozdzielczosc.x / 2.0f, rozdzielczosc.y * 0.07f));

  sf::RectangleShape mapBorder(
      sf::Vector2f(rozdzielczosc.x * 0.8f, rozdzielczosc.y * 0.8f));
  mapBorder.setPosition(rozdzielczosc.x * 0.1f, rozdzielczosc.y * 0.15f);
  mapBorder.setFillColor(sf::Color::Transparent);
  mapBorder.setOutlineColor(sf::Color::White);
  mapBorder.setOutlineThickness(3.0f);

  player playerCharacter;
  playerCharacter.createPlayer();

  // Dodanie 4 przykladowych frakcji z planetami
  std::vector<Fraction> fractions;
  fractions.push_back(Fraction("Centari Alliance", -600.f, -400.f, 20));
  fractions.push_back(Fraction("Vectron Mining", 1500.f, -800.f, 20));
  fractions.push_back(Fraction("Orion Republic", -1200.f, 1200.f, 20));
  fractions.push_back(Fraction("Sol Empire", 2200.f, 800.f, 20));

  // Inicjalizacja planet dla frakcji (czcionka, promien planety, promien
  // interakcji, kolor)
  fractions[0].createPlanet(font, 80.f, 160.f,
                            sf::Color(220, 100, 100)); // Czerwona planeta
  fractions[1].createPlanet(font, 60.f, 130.f,
                            sf::Color(100, 220, 100)); // Zielona planeta
  fractions[2].createPlanet(font, 100.f, 200.f,
                            sf::Color(100, 100, 220)); // Niebieska planeta
  fractions[3].createPlanet(font, 50.f, 110.f,
                            sf::Color(220, 180, 50)); // Zolta planeta

  ////////////////////////////////////////////// Walka
  std::vector<Projectile> enemyProjectiles;
  std::vector<Mine> enemyMines;

  std::vector<Enemy> enemies;
  std::vector<Explosion> explosions;

  enemies.push_back(Enemy(EnemyType::BASIC_SHIELDED, sf::Vector2f(500, 500)));
  enemies.push_back(Enemy(EnemyType::SHOOTER, sf::Vector2f(800, 200)));
  enemies.push_back(Enemy(EnemyType::MINER, sf::Vector2f(1200, -200)));

  //////////////////////////////Mapy lokalne
  LocalMap stacjaMap;
  stacjaMap.generateStationMap(font);

  std::vector<LocalMap> planetyMaps;
  for (size_t i = 0; i < fractions.size(); ++i) {
    LocalMap pMap;
    sf::Color pColor = fractions[i].planetShape.getFillColor();
    pMap.generatePlanetMap(fractions[i].getName(),
                           sf::Color(pColor.r / 2, pColor.g / 2, pColor.b / 2),
                           font);
    planetyMaps.push_back(pMap);
  }

  int aktywneWnetrzeID = -1;

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    if (activeState == state::STATEK) {
      camera.setCenter(playerShip.getPosition());
    } else if (activeState == state::LUDZIK) {
      camera.setCenter(playerCharacter.sprite.getPosition());
      playerCharacter.update(dt, mouseWorldPosition);

      float aktualneCargo = playerShip.getCurrentScrap();

      if (aktywneWnetrzeID == -1) {
        stacjaMap.updateCollisions(playerCharacter.sprite, playerShip);
      } else {
        planetyMaps[aktywneWnetrzeID].updateCollisions(playerCharacter.sprite,
                                                       playerShip);
      }
    }

    mousePosition = sf::Mouse::getPosition(window);
    mouseWorldPosition = window.mapPixelToCoords(mousePosition);

    playerShip.update(dt, mouseWorldPosition,
                      (mapMode || upgradeMode || fractionsMode || inventoryMode)
                          ? state::ANIMACJA
                          : activeState);

    sf::Vector2f cameraCenter = camera.getCenter();
    background.setPosition(cameraCenter);

    sf::Vector2f currentViewSize = camera.getSize();
    background.setSize(currentViewSize);
    background.setOrigin(currentViewSize.x / 2.f, currentViewSize.y / 2.f);

    sf::IntRect background_slice((int)(cameraCenter.x - currentViewSize.x / 2),
                                 (int)(cameraCenter.y - currentViewSize.y / 2),
                                 currentViewSize.x, currentViewSize.y);
    background.setTextureRect(background_slice);

    sf::Event event;

    space_clicked = false;
    bool mouse_clicked = false;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        mouse_clicked = true;
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
          if (!mapMode && !upgradeMode && !fractionsMode && !inventoryMode) {
            if (activeState == state::STATEK) {
              float dystansStacja = sqrt(
                  pow((prawdziwySrodek.x - playerShip.getPosition().x), 2) +
                  pow((prawdziwySrodek.y - playerShip.getPosition().y), 2));
              if (dystansStacja < 85) {
                activeState = state::LUDZIK;
                aktywneWnetrzeID = -1;
                playerCharacter.sprite.setPosition(120.f, 400.f);
              }

              for (size_t i = 0; i < fractions.size(); ++i) {
                sf::Vector2f diff =
                    playerShip.getPosition() - fractions[i].getLocation();
                float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
                if (dist < fractions[i].interactionArea.getRadius()) {
                  activeState = state::LUDZIK;
                  aktywneWnetrzeID = i;
                  playerCharacter.sprite.setPosition(530.f, 430.f);
                  break;
                }
              }
            }

            else if (activeState == state::LUDZIK) {
              bool checwyjscia = false;
              if (aktywneWnetrzeID == -1)
                checwyjscia = stacjaMap.checkExit(playerCharacter.sprite);
              else
                checwyjscia = planetyMaps[aktywneWnetrzeID].checkExit(
                    playerCharacter.sprite);

              if (checwyjscia) {
                activeState = state::STATEK;
              }
            }
          }
        }
        if (event.key.code == sf::Keyboard::M) {
          if (!upgradeMode && !fractionsMode && !inventoryMode) {
            mapMode = !mapMode;
            if (mapMode) {
              camera.setSize(rozdzielczosc * 3.f);
              camera.setViewport(sf::FloatRect(0.1f, 0.15f, 0.8f, 0.8f));
            } else {
              camera.setSize(rozdzielczosc);
              camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
            }
          }
        }
        if (event.key.code == sf::Keyboard::F5) {
          if (!mapMode && !upgradeMode && !fractionsMode && !inventoryMode && activeState != state::MENU) {
            saveMode = !saveMode;
          }
        }
        if (event.key.code == sf::Keyboard::Escape) {
            if (saveMode) saveMode = false;
            else if (upgradeMode) upgradeMode = false;
            else if (fractionsMode) fractionsMode = false;
            else if (inventoryMode) inventoryMode = false;
        }
        if (event.key.code == sf::Keyboard::U) {
          if (!mapMode && !fractionsMode && !inventoryMode && !saveMode) {
            upgradeMode = !upgradeMode;
          }
        }
        if (event.key.code == sf::Keyboard::F) {
          if (!mapMode && !upgradeMode && !inventoryMode) {
            fractionsMode = !fractionsMode;
          }
        }
        if (event.key.code == sf::Keyboard::K) {
          if (!mapMode && !upgradeMode && !fractionsMode) {
            inventoryMode = !inventoryMode;
          }
        }
        if (event.key.code == sf::Keyboard::Escape) {
          if (upgradeMode) {
            upgradeMode = false;
          }
          if (fractionsMode) {
            fractionsMode = false;
          }
          if (inventoryMode) {
            inventoryMode = false;
          }
        }
        if (upgradeMode) {
          if (event.key.code == sf::Keyboard::Num1 ||
              event.key.code == sf::Keyboard::Numpad1) {
            playerShip.upgradeMaxHealth();
          }
          if (event.key.code == sf::Keyboard::Num2 ||
              event.key.code == sf::Keyboard::Numpad2) {
            playerShip.upgradeMaxNitro();
          }
          if (event.key.code == sf::Keyboard::Num3 ||
              event.key.code == sf::Keyboard::Numpad3) {
            playerShip.upgradeMaxStorage();
          }
          if (event.key.code == sf::Keyboard::Num4 ||
              event.key.code == sf::Keyboard::Numpad4) {
            playerShip.upgradeShipSpeed();
          }
        }
      }
    }

    if ((activeState == state::STATEK || activeState == state::LUDZIK) &&
        playerShip.getCurrentHealth() <= 0) {
      activeState = state::DEATH;
    }

    if (activeState == state::MENU) {
      sf::Vector2f mousePosScreen =
          window.mapPixelToCoords(mousePosition, window.getDefaultView());
      int slot = mainMenu.update(mousePosScreen, mouse_clicked, activeState);
      if (slot > 0) {
          std::string filename = "savegame_slot" + std::to_string(slot) + ".txt";
          bool loaded = SaveSystem::loadGame(filename, playerShip, fractions, enemies);
          if (!loaded) {
              std::cout << "Blad wczytywania slota: " << slot << std::endl;
              // Revert to menu if failed
              activeState = state::MENU;
          }
      }
    } else if (activeState == state::DEATH) {
      sf::Vector2f mousePosScreen =
          window.mapPixelToCoords(mousePosition, window.getDefaultView());
      if (returnMenuText.getGlobalBounds().contains(mousePosScreen)) {
        returnMenuText.setFillColor(sf::Color::White);
        if (mouse_clicked) {
          activeState = state::MENU;
          playerShip = Ship();
          playerShip.createShip();
          playerCharacter = player();
          playerCharacter.createPlayer();
          enemies.clear();
          enemies.push_back(
              Enemy(EnemyType::BASIC_SHIELDED, sf::Vector2f(500, 500)));
          enemies.push_back(Enemy(EnemyType::SHOOTER, sf::Vector2f(800, 200)));
          enemies.push_back(Enemy(EnemyType::MINER, sf::Vector2f(1200, -200)));
          enemyProjectiles.clear();
          enemyMines.clear();
          explosions.clear();
          aktywneWnetrzeID = -1;
          mapMode = false;
          upgradeMode = false;
          fractionsMode = false;
          inventoryMode = false;
        }
      } else {
        returnMenuText.setFillColor(sf::Color::Yellow);
      }
    }

    healthBar.update(dt, playerShip.getCurrentHealth(),
                     playerShip.getMaxHealth());
    nitroBar.update(dt, playerShip.getCurrentNitro(), playerShip.getMaxNitro(),
                    playerShip.getNitroOnCooldown());
    shieldBar.update(dt, playerShip.getCurrentShield(),
                     playerShip.getMaxShield());
    inventoryBar.update(dt, playerShip);

    if (upgradeMode) {
      upgradeMenu.update(playerShip);
    }
    if (fractionsMode) {
      fractionsMenu.update(fractions);
    }
    if (inventoryMode) {
      inventoryMenu.update(playerShip);
    }
    if (saveMode) {
        sf::Vector2f mousePosScreen = window.mapPixelToCoords(mousePosition, window.getDefaultView());
        int slot = saveMenu.update(mousePosScreen, mouse_clicked);
        if (slot > 0) {
            std::string filename = "savegame_slot" + std::to_string(slot) + ".txt";
            SaveSystem::saveGame(filename, playerShip, fractions, enemies);
            saveMode = false; // zamknij po zapisaniu
        }
    }

    float dystans =
        sqrt(pow((prawdziwySrodek.x - playerShip.getPosition().x), 2) +
             pow((prawdziwySrodek.y - playerShip.getPosition().y), 2));
    if (dystans < 85) {
      cout << "Gracz w zasiegu" << endl;
      if (space_clicked) {
        if (activeState == state::STATEK) {
          activeState = state::LUDZIK;
        } else if (activeState == state::LUDZIK) {
          activeState = state::STATEK;
        }
      }
    }

    // Sprawdzanie, czy gracz jest w obszarze interakcji jakiejkolwiek planety
    // frakcji
    for (auto &fraction : fractions) {
      sf::Vector2f shipPos = playerShip.getPosition();
      sf::Vector2f planetPos = fraction.getLocation();
      sf::Vector2f diff = shipPos - planetPos;
      float dist = sqrt(diff.x * diff.x + diff.y * diff.y);

      if (dist < fraction.interactionArea.getRadius()) {
        cout << "W obszarze" << endl;
      }
    }

    sf::Vector2f shipPos = playerShip.getPosition();
    sf::Vector2f diff = shipPos - station_center;
    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
    float minDistance = station.getRadius() + 15.f; // 15.f dla promienia statku

    if (dist < minDistance) {
      sf::Vector2f pushDir(1.f, 0.f);
      if (dist > 0.001f) {
        pushDir = diff / dist;
      }
      float overlap = minDistance - dist;
      playerShip.collisionMove(pushDir.x * overlap, pushDir.y * overlap, 1.f);
    }

    // COMBAT
    for (auto &fraction : fractions) {
      sf::Vector2f shipPos = playerShip.getPosition();
      sf::Vector2f planetPos = fraction.getLocation();
      sf::Vector2f diff = shipPos - planetPos;
      float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
      float minDistance = fraction.planetShape.getRadius() + 15.f;

      if (dist < minDistance) {
        sf::Vector2f pushDir(1.f, 0.f);
        if (dist > 0.001f) {
          pushDir = diff / dist;
        }
        float overlap = minDistance - dist;
        playerShip.collisionMove(pushDir.x * overlap, pushDir.y * overlap, 1.f);
      }
    }

    for (auto &proj : playerShip.projectiles) {
      if (!proj.active)
        continue;
      for (auto &enemy : enemies) {
        if (!enemy.active)
          continue;
        if (proj.shape.getGlobalBounds().intersects(
                enemy.shape.getGlobalBounds())) {
          if (enemy.takeDamage(proj.damage)) {
            explosions.push_back(Explosion(enemy.shape.getPosition()));
          }
          proj.active = false;
          break;
        }
      }
    }

    for (auto &m : playerShip.mines) {
      if (m.exploded && m.explosionLinger == 0.5f) {
        for (auto &enemy : enemies) {
          if (!enemy.active)
            continue;

          sf::Vector2f diff = enemy.shape.getPosition() - m.shape.getPosition();
          float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
          if (dist <= m.blastRadius + 20.f) {
            if (enemy.takeDamage(m.damage)) {
              explosions.push_back(Explosion(enemy.shape.getPosition()));
            }
          }
        }
      }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const Enemy &e) { return !e.active; }),
                  enemies.end());

    for (auto &enemy : enemies) {
      enemy.update(dt, enemyProjectiles, enemyMines);
    }

    for (auto &p : enemyProjectiles)
      p.update(dt);
    enemyProjectiles.erase(
        std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
                       [](const Projectile &p) { return !p.active; }),
        enemyProjectiles.end());

    for (auto &ex : explosions)
      ex.update(dt);
    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
                       [](const Explosion &ex) { return !ex.active; }),
        explosions.end());

    for (auto &m : enemyMines)
      m.update(dt);
    enemyMines.erase(std::remove_if(enemyMines.begin(), enemyMines.end(),
                                    [](const Mine &m) { return !m.active; }),
                     enemyMines.end());

    for (auto &proj : enemyProjectiles) {
      if (!proj.active)
        continue;
      if (proj.shape.getGlobalBounds().intersects(
              playerShip.sprite.getGlobalBounds())) {
        if (playerShip.takeDamage(proj.damage)) {
          explosions.push_back(Explosion(playerShip.getPosition()));
        }
        proj.active = false;
      }
    }

    for (auto &m : enemyMines) {
      if (m.exploded && m.explosionLinger == 0.5f) {
        sf::Vector2f diff = playerShip.getPosition() - m.shape.getPosition();
        float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist <= m.blastRadius + 15.f) {
          if (playerShip.takeDamage(m.damage)) {
            explosions.push_back(Explosion(playerShip.getPosition()));
          }
        }
      }
    }

    window.clear();

    window.setView(camera);

    if (activeState == state::STATEK) {
      window.draw(background);

      window.draw(playerShip.sprite);

      window.draw(station);
      window.draw(dockingmodule_area);

      // Rysowanie planet, obszarow interakcji oraz nazwy frakcji
      for (auto &fraction : fractions) {
        window.draw(fraction.planetShape);
        window.draw(fraction.interactionArea);
        window.draw(fraction.nameText);
      }

      // Rysowanie wrogów
      for (auto &enemy : enemies)
        enemy.draw(window);

      // Rysowanie min
      for (auto &m : playerShip.mines) {
        if (!m.exploded)
          window.draw(m.shape);
        else
          window.draw(m.blastShape);
      }

      // Rysowanie pocisków
      for (auto &p : playerShip.projectiles) {
        window.draw(p.shape);
      }

      // Rysowanie statku i jego tarczy
      window.draw(playerShip.sprite);
      if (playerShip.getCurrentShield() > 0) {
        window.draw(playerShip.shieldShape);
      }

      ////rysowanie pocisków wrogow
      for (auto &m : enemyMines) {
        if (!m.exploded)
          window.draw(m.shape);
        else
          window.draw(m.blastShape);
      }
      for (auto &p : enemyProjectiles) {
        window.draw(p.shape);
      }

      for (auto &ex : explosions) {
        ex.draw(window);
      }

      window.setView(window.getDefaultView());

      if (mapMode) {
        window.draw(mapBorder);
        window.draw(mapText);
      } else {
        healthBar.draw(window);
        nitroBar.draw(window);
        shieldBar.draw(window);
        inventoryBar.draw(window);

        if (upgradeMode) {
          upgradeMenu.draw(window);
        }
        if (fractionsMode) {
          fractionsMenu.draw(window);
        }
        if (inventoryMode) {
          inventoryMenu.draw(window);
        }
        if (saveMode) {
          saveMenu.draw(window);
        }
      }

    }

    else if (activeState == state::LUDZIK) {
      window.clear(sf::Color(25, 27, 33));

      if (aktywneWnetrzeID == -1) {
        stacjaMap.draw(window);
      } else {
        planetyMaps[aktywneWnetrzeID].draw(window);
      }
      window.draw(playerCharacter.sprite);
    }

    if (activeState == state::MENU) {
      window.clear(sf::Color(15, 15, 20));
      window.setView(window.getDefaultView());
      mainMenu.draw(window);
    } else if (activeState == state::DEATH) {
      window.clear();
      window.setView(window.getDefaultView());
      window.draw(deathBackgroundShape);
      window.draw(deathText);
      window.draw(returnMenuText);
    }

    window.setView(camera);
    window.display();
  }

  return 0;
}
