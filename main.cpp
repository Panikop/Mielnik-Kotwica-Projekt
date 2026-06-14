#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "include/Ship.h"
#include "include/UIBar.h"
#include "include/UIUpgradeMenu.h"
#include "include/UIFractionsMenu.h"
#include "include/UIInventoryMenu.h"
#include "include/UISaveMenu.h"
#include "include/UIInventoryBar.h"
#include "include/Projectile.h"
#include "include/Mine.h"
#include "include/Explosion.h"
#include "include/Enemy.h"
#include "include/fraction.h"
#include "include/LocalMap.h"
#include "include/LocalResource.h"
#include "include/mainmenu.h"
#include "include/player.h"
#include "include/SaveSystem.h"
#include "include/Textures.h"
#include "include/QuestSystem.h"
#include "include/UIQuestMenu.h"
#include "include/AppearingText.h"
#include "include/AppearingText.h"
#include "include/GameObject.h"
#include <vector>

#define _USE_MATH_DEFINES

#include "include/Textures.h"
using namespace std;

sf::Texture Textures::shipTex;
sf::Texture Textures::shipAnimTex;
sf::Texture Textures::bulletTex;
sf::Texture Textures::npcTex;
sf::Texture Textures::playerTex;
sf::Texture Textures::playerAnimTex;
sf::Texture Textures::ufoTex;
sf::Texture Textures::explosionTex;
sf::Texture Textures::mineTex;
sf::Texture Textures::stationTex;
sf::Texture Textures::wallTex;
sf::Texture Textures::floorTex;

// bufor na nowe obiekty
std::vector<std::unique_ptr<GameObject>> pendingObjects;

int main() {
  srand(time(NULL));
  Textures::load();

  float skala = 30;

  // ustawienia glownego okna i kamer
  sf::Vector2f rozdzielczosc(1620, 820);

  sf::RenderWindow window(sf::VideoMode(rozdzielczosc.x, rozdzielczosc.y),
                          "PANIC ANCHOR");
  sf::View camera;
  camera.setSize(rozdzielczosc);
  sf::View characterCamera;
  characterCamera.setSize(rozdzielczosc);

  sf::Clock clock;

  // ladowanie tla kosmosu
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

  // ustawienia paskow UI

  sf::Vector2f barSize(230.f, 28.f);
  float marginX = 12.f;
  float marginY = 12.f;
  float spacingY = 38.f;

  UIBar healthBar(sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                               rozdzielczosc.y - barSize.y - marginY),
                  barSize, sf::Color(220, 50, 50),
                  "HP", font);

  UIBar nitroBar(sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                              rozdzielczosc.y - barSize.y - marginY - spacingY),
                 barSize, sf::Color(0, 190, 220),
                 "NITRO", font);

  UIBar shieldBar(
      sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                   rozdzielczosc.y - barSize.y - marginY - 2.f * spacingY),
      barSize, sf::Color(50, 100, 255),
      "SHIELD", font);

  UIInventoryBar inventoryBar(
      sf::Vector2f(rozdzielczosc.x - barSize.x - marginX,
                   rozdzielczosc.y - barSize.y - marginY - 3.f * spacingY),
      barSize, font);

    //zarzadzanie aktywnym menu
  bool upgradeMode = false;
  UIUpgradeMenu upgradeMenu(rozdzielczosc, font);

  bool fractionsMode = false;
  UIFractionsMenu fractionsMenu(rozdzielczosc, font);

  bool inventoryMode = false;
  UIInventoryMenu inventoryMenu(rozdzielczosc, font);

  bool saveMode = false;
  UISaveMenu saveMenu(rozdzielczosc, font);

  bool questMode = false;
  UIQuestMenu questMenu(rozdzielczosc, font);

  sf::Vector2i mousePosition(0, 0);
  sf::Vector2f mouseWorldPosition(0, 0);

  //////////////////////////////
  // glowna stacja orbitalna
  sf::CircleShape station(256.f);
  station.setTexture(&Textures::stationTex);
  station.setFillColor(sf::Color::White);
  sf::Sprite mapSprite;
  mapSprite.setTexture(Textures::stationTex);

  AppearingText notificationText;
  notificationText.setFont(font);
  int previousHoveredID = -1;

  float lastSpawnTime = 0.0f;
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

  // tlo i ekrany koncowe
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

  sf::RectangleShape winBackgroundShape(rozdzielczosc);
  winBackgroundShape.setTexture(&deathBackgroundTex);
  winBackgroundShape.setFillColor(sf::Color(0, 255, 0, 130));

  sf::Text winText;
  winText.setFont(font);
  winText.setString("Osiagnieto pokoj w galaktyce");
  winText.setCharacterSize(60);
  winText.setFillColor(sf::Color::White);
  winText.setStyle(sf::Text::Bold);
  sf::FloatRect wBounds = winText.getLocalBounds();
  winText.setOrigin(wBounds.left + wBounds.width / 2.f,
                      wBounds.top + wBounds.height / 2.f);
  winText.setPosition(rozdzielczosc.x / 2.f, rozdzielczosc.y * 0.4f);

  sf::Text winQuitText;
  winQuitText.setFont(font);
  winQuitText.setString("Wyjscie z gry");
  winQuitText.setCharacterSize(40);
  winQuitText.setFillColor(sf::Color::Yellow);
  sf::FloatRect wqBounds = winQuitText.getLocalBounds();
  winQuitText.setOrigin(wqBounds.left + wqBounds.width / 2.f,
                           wqBounds.top + wqBounds.height / 2.f);
  winQuitText.setPosition(rozdzielczosc.x / 2.f, rozdzielczosc.y * 0.75f);

  sf::Text keybindsText;
  keybindsText.setFont(font);
  keybindsText.setString("W/A/S/D + mouse - movement\nF - fraction status\nM - Map\nJ - Current quest\nK - inventory");
  keybindsText.setCharacterSize(16);
  keybindsText.setFillColor(sf::Color::White);
  keybindsText.setOutlineColor(sf::Color::Black);
  keybindsText.setOutlineThickness(1.f);
  keybindsText.setPosition(15.f, rozdzielczosc.y / 2.f - 60.f);

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

  float ambushTimer = 15.f;

  //frakcje
  std::vector<Fraction> fractions;
  fractions.push_back(Fraction("Centari Alliance", -600.f, -400.f, 20));
  fractions.push_back(Fraction("Vectron Mining", 1500.f, -800.f, 20));
  fractions.push_back(Fraction("Orion Republic", -1200.f, 1200.f, 20));
  fractions.push_back(Fraction("Sol Empire", 2200.f, 800.f, 20));

  fractions[0].createPlanet(font, 80.f, 160.f,
                            sf::Color(220, 100, 100));
  fractions[1].createPlanet(font, 60.f, 130.f,
                            sf::Color(100, 220, 100));
  fractions[2].createPlanet(font, 100.f, 200.f,
                            sf::Color(100, 100, 220));
  fractions[3].createPlanet(font, 50.f, 110.f,
                            sf::Color(220, 180, 50));

  ////////////////////////////////////////////// Walka
  std::vector<std::unique_ptr<GameObject>> gameObjects;

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

  ////////////////////////////////////////

  LocalMap asteroidaMap;
  asteroidaMap.generateAsteroidMap(font);

  QuestSystem questSystem;
  questSystem.generateAvailableQuests(fractions);

  notificationText.addText("Zadokuj do Stacji Orbitalnej Alpha po misje", sf::Color::Cyan, 1.0f);
  clock.restart();

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    if (!questSystem.newlyAcceptedQuestTitle.empty()) {
        notificationText.addText("Nowe Zadanie: " + questSystem.newlyAcceptedQuestTitle, sf::Color::Green);
        questSystem.newlyAcceptedQuestTitle = "";
    }

    notificationText.update(dt);

    //update fizyki
    if (activeState == state::STATEK) {
      playerShip.update(dt, mouseWorldPosition, (mapMode || upgradeMode || fractionsMode || inventoryMode) ? state::ANIMACJA : activeState);
      camera.setCenter(playerShip.getPosition());

      if (!mapMode && !upgradeMode && !fractionsMode && !inventoryMode) {
          ambushTimer -= dt;
          if (ambushTimer <= 0.f) {
              if ((rand() % 100) < 30) {
                  int totalTrust = 0;
                  for (const auto& f : fractions) {
                      totalTrust += f.getTrustLevel();
                  }

                  // Bazowo 1 piratow, dodatkowy za kazde 40 punktow zaufania we wszystkich frakcjach lacznie
                  int numPirates = 1 + (totalTrust / 40);

                  for (int p = 0; p < numPirates; ++p) {
                      float angle = (rand() % 360) * M_PI / 180.f;
                      sf::Vector2f pos = playerShip.getPosition() + sf::Vector2f(cos(angle) * 850.f, sin(angle) * 850.f);
                      gameObjects.push_back(std::make_unique<Enemy>(EnemyType::CHASER, pos));
                  }
                  notificationText.addText("UWAGA! ZASADZKA PIRATOW!", sf::Color::Red, 1.0f);
                  ambushTimer = 30.f;
              } else {
                  ambushTimer = 20.f;
              }
          }
      }
    } else if (activeState == state::LUDZIK) {
      camera.setCenter(playerCharacter.sprite.getPosition());
      playerCharacter.update(dt, mouseWorldPosition);

      float aktualneCargo = playerShip.getCurrentScrap();

      if (aktywneWnetrzeID == -1) {
        stacjaMap.updateCollisions(playerCharacter.sprite, playerShip, questSystem);
      }
      else if (aktywneWnetrzeID == -2) {
        asteroidaMap.updateCollisions(playerCharacter.sprite, playerShip, questSystem);
      }
       else {
        planetyMaps[aktywneWnetrzeID].updateCollisions(playerCharacter.sprite, playerShip, questSystem);
      }
    }

    mousePosition = sf::Mouse::getPosition(window);
    mouseWorldPosition = window.mapPixelToCoords(mousePosition);

    //obsluga tla i kamery - ruch
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

    // obsluga wcisnietych klawiszy
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
            // wchodzenie / wychodzenie ze statku
        if (event.key.code == sf::Keyboard::Space) {
          if (!mapMode && !upgradeMode && !fractionsMode && !inventoryMode) {
            if (activeState == state::STATEK) {
              float dystansStacja = sqrt(
                  pow((prawdziwySrodek.x - playerShip.getPosition().x), 2) +
                  pow((prawdziwySrodek.y - playerShip.getPosition().y), 2));
              if (dystansStacja < 85) {
                    activeState = state::LUDZIK;
                    aktywneWnetrzeID = -1;
                    playerCharacter.sprite.setPosition(400.f, 280.f);
                }
                else if (questSystem.isAsteroidSpawned && sqrt(pow(questSystem.asteroidShape.getPosition().x - playerShip.getPosition().x, 2) + pow(questSystem.asteroidShape.getPosition().y - playerShip.getPosition().y, 2)) < 85) {
                    activeState = state::LUDZIK;
                    aktywneWnetrzeID = -2;
                    playerCharacter.sprite.setPosition(500.f, 500.f);
              }
              else if (questSystem.isWreckSpawned && sqrt(pow(questSystem.wreckShape.getPosition().x - playerShip.getPosition().x, 2) + pow(questSystem.wreckShape.getPosition().y - playerShip.getPosition().y, 2)) < 85) {
                    questSystem.currentQuest.objectiveComplete = true;
              }
              else
              {
                for (size_t i = 0; i < fractions.size(); ++i) {
                    sf::Vector2f diff =
                        playerShip.getPosition() - fractions[i].getLocation();
                    float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
                    if (dist < fractions[i].interactionArea.getRadius()) {
                    activeState = state::LUDZIK;
                    aktywneWnetrzeID = i;
                    playerCharacter.sprite.setPosition(500.f, 380.f);
                    break;
                    }
                }
              }
            }

            else if (activeState == state::LUDZIK) {
              bool checwyjscia = false;
              if (aktywneWnetrzeID == -1)
                checwyjscia = stacjaMap.checkExit(playerCharacter.sprite);
              else if (aktywneWnetrzeID == -2)
                checwyjscia = asteroidaMap.checkExit(playerCharacter.sprite);
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
        if (event.key.code == sf::Keyboard::J) {
          if (!mapMode && !upgradeMode && !fractionsMode && !inventoryMode && !saveMode) {
            questMode = !questMode;
          }
        }
        if (event.key.code == sf::Keyboard::Escape) {
            if (saveMode) saveMode = false;
            else if (upgradeMode) upgradeMode = false;
            else if (fractionsMode) fractionsMode = false;
            else if (inventoryMode) inventoryMode = false;
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
          if (questMode) {
            questMode = false;
          }
        }
        if (upgradeMode) {
          if (event.key.code == sf::Keyboard::Num1 ||
              event.key.code == sf::Keyboard::Numpad1) {
            playerShip.upgradeMaxShield();
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
          if (event.key.code == sf::Keyboard::Num5 ||
              event.key.code == sf::Keyboard::Numpad5) {
            playerShip.healShipWithScrap();
          }
        }
      }
    }

    // sprawdzenie konca gry
    if ((activeState == state::STATEK || activeState == state::LUDZIK) &&
        playerShip.getCurrentHealth() <= 0) {
      activeState = state::DEATH;
    }
    else if (activeState == state::STATEK || activeState == state::LUDZIK) {
      bool all100 = true;
      for (const auto& frac : fractions) {
          if (frac.getTrustLevel() < 100) {
              all100 = false;
              break;
          }
      }
      if (all100 && !fractions.empty()) {
          activeState = state::WIN;
      }
    }

    questSystem.update(dt);

    if (activeState == state::MENU) {
      sf::Vector2f mousePosScreen =
          window.mapPixelToCoords(mousePosition, window.getDefaultView());
      int slot = mainMenu.update(mousePosScreen, mouse_clicked, activeState);
      if (slot > 0) {
          std::string filename = "savegame_slot" + std::to_string(slot) + ".txt";
          bool loaded = SaveSystem::loadGame(filename, playerShip, fractions);
          if (!loaded) {
              std::cout << "Blad wczytywania slota: " << slot << std::endl;

              activeState = state::MENU;
          }
      }
    } else if (activeState == state::DEATH || activeState == state::WIN) {
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
        gameObjects.clear();
          pendingObjects.clear();
          aktywneWnetrzeID = -1;
          mapMode = false;
          upgradeMode = false;
          fractionsMode = false;
          inventoryMode = false;
        }
      } else {
        returnMenuText.setFillColor(sf::Color::Yellow);
      }

      if (activeState == state::WIN) {
          if (winQuitText.getGlobalBounds().contains(mousePosScreen)) {
              winQuitText.setFillColor(sf::Color::White);
              if (mouse_clicked) window.close();
          } else {
              winQuitText.setFillColor(sf::Color::Yellow);
          }
      }
    }

    // aktualizacja paskow
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
    if (questMode) {
      questMenu.update(questSystem, fractions);
    }
    if (saveMode) {
        sf::Vector2f mousePosScreen = window.mapPixelToCoords(mousePosition, window.getDefaultView());
        int slot = saveMenu.update(mousePosScreen, mouse_clicked);
        if (slot > 0) {
            std::string filename = "savegame_slot" + std::to_string(slot) + ".txt";
            SaveSystem::saveGame(filename, playerShip, fractions);
            saveMode = false; // zamknij po zapisaniu
        }
    }

    // Sprawdzanie, czy gracz jest w obszarze interakcji jakiejkolwiek planety/stacji
    int currentHoveredID = -100; // -100 means nothing
    for (size_t i = 0; i < fractions.size(); ++i) {
      sf::Vector2f diff = playerShip.getPosition() - fractions[i].getLocation();
      float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
      if (dist < fractions[i].interactionArea.getRadius()) {
          currentHoveredID = i;
      }
    }

    float distToStation = sqrt(pow((prawdziwySrodek.x - playerShip.getPosition().x), 2) + pow((prawdziwySrodek.y - playerShip.getPosition().y), 2));
    if (distToStation < 85.f) currentHoveredID = -1;

    if (questSystem.isAsteroidSpawned && sqrt(pow(questSystem.asteroidShape.getPosition().x - playerShip.getPosition().x, 2) + pow(questSystem.asteroidShape.getPosition().y - playerShip.getPosition().y, 2)) < 85.f) {
        currentHoveredID = -2;
    }
    if (questSystem.isWreckSpawned && sqrt(pow(questSystem.wreckShape.getPosition().x - playerShip.getPosition().x, 2) + pow(questSystem.wreckShape.getPosition().y - playerShip.getPosition().y, 2)) < 85.f) {
        currentHoveredID = -3;
    }

    if (currentHoveredID != previousHoveredID) {
        if (currentHoveredID != -100 && activeState == state::STATEK) {
            std::string nameStr = "";
            if (currentHoveredID == -1) nameStr = "Stacja Orbitalna Alpha";
            else if (currentHoveredID == -2) nameStr = "Zbadana Asteroida";
            else if (currentHoveredID == -3) nameStr = "Zbadany Wrak";
            else if (currentHoveredID >= 0) nameStr = fractions[currentHoveredID].getName();

            if (currentHoveredID == -3) {
                notificationText.addText("Wcisnij [SPACJA] aby zbadac wrak", sf::Color::Yellow, 0.5f);
            } else {
                notificationText.addText("Wcisnij [SPACJA] aby wyladowac: " + nameStr, sf::Color::Cyan, 0.5f);
            }
        }
        previousHoveredID = currentHoveredID;
    }


    // odbijanie gracza od stacji
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

    for (auto& obj : gameObjects) {
        obj->update(dt, playerShip.getPosition());
    }

    for (auto &proj : playerShip.projectiles) {
        if (!proj.active) continue;

        for (auto &obj : gameObjects) {

            if (Enemy* enemy = dynamic_cast<Enemy*>(obj.get())) {
                if (proj.shape.getGlobalBounds().intersects(enemy->getBounds())) {
                    if (enemy->takeDamage(proj.damage)) {
                        pendingObjects.push_back(std::make_unique<Explosion>(enemy->getPosition()));
                    }
                    proj.active = false;
                    break;
                }
            }
        }
    }

    for (auto &obj : gameObjects) {
        if (!obj->active) continue;

        if (Projectile* p = dynamic_cast<Projectile*>(obj.get())) {
            if (p->getBounds().intersects(playerShip.sprite.getGlobalBounds())) {
                if (playerShip.takeDamage(p->damage)) {
                    pendingObjects.push_back(std::make_unique<Explosion>(playerShip.getPosition()));
                }
                p->active = false;
            }
        }
        else if (Mine* m = dynamic_cast<Mine*>(obj.get())) {
            if (m->exploded && m->explosionLinger == 0.5f) {
                sf::Vector2f diff = playerShip.getPosition() - m->getPosition();
                if (sqrt(diff.x * diff.x + diff.y * diff.y) <= m->blastRadius + 15.f) {
                    if (playerShip.takeDamage(m->damage)) pendingObjects.push_back(std::make_unique<Explosion>(playerShip.getPosition()));
                }
            }
        }
        else if (Enemy* e = dynamic_cast<Enemy*>(obj.get())) {
            if (e->hp <= 0 && e->isQuestTarget) {
                questSystem.registerKill();
                e->isQuestTarget = false;
            }
        }
    }

    for (auto& newObj : pendingObjects) {
        gameObjects.push_back(std::move(newObj));
    }
    pendingObjects.clear();

    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
        [](const std::unique_ptr<GameObject>& obj) { return !obj->active; }),
        gameObjects.end());





        //rysowanie
    window.clear();
    window.setView(camera);

    if (activeState == state::STATEK) {
      window.draw(background);

      window.draw(station);
      window.draw(dockingmodule_area);

      for (auto &fraction : fractions) {
        window.draw(fraction.planetShape);
        window.draw(fraction.interactionArea);
        window.draw(fraction.nameText);
      }

      questSystem.drawSpaceObjects(window, playerShip.getPosition(), font, fractions);


      for (auto &m : playerShip.mines) {
        if (!m.exploded) window.draw(m.shape);
        else window.draw(m.blastShape);
      }
      for (auto &p : playerShip.projectiles) window.draw(p.shape);


      window.draw(playerShip.sprite);
      if (playerShip.getCurrentShield() > 0) {
        window.draw(playerShip.shieldShape);
      }

    for (auto& obj : gameObjects) {
        obj->draw(window);
    }

      questSystem.drawNavigationHUD(window, playerShip.getPosition(), font, fractions);
      if (mapMode) {
          questSystem.drawMapMarkers(window, font, fractions);
      }
    }
    else if (activeState == state::LUDZIK) {
      window.clear(sf::Color(25, 27, 33));

      if (aktywneWnetrzeID == -1) {
        stacjaMap.draw(window);
        stacjaMap.checkNPCInteractions(playerCharacter.sprite, window, font, questSystem, aktywneWnetrzeID, playerShip, fractions, gameObjects, upgradeMode);
      }
      else if (aktywneWnetrzeID == -2) {
        asteroidaMap.draw(window);
      }
      else {
        planetyMaps[aktywneWnetrzeID].draw(window);
        planetyMaps[aktywneWnetrzeID].checkNPCInteractions(playerCharacter.sprite, window, font, questSystem, aktywneWnetrzeID, playerShip, fractions, gameObjects, upgradeMode);
      }
      window.draw(playerCharacter.sprite);
    }

    window.setView(window.getDefaultView());

    if (activeState == state::STATEK) {
        if (mapMode) {
            window.draw(mapBorder);
            window.draw(mapText);
        } else {
            healthBar.draw(window);
            nitroBar.draw(window);
            shieldBar.draw(window);
            inventoryBar.draw(window);

            if (upgradeMode) upgradeMenu.draw(window);
            if (fractionsMode) fractionsMenu.draw(window);
            if (inventoryMode) inventoryMenu.draw(window);
            if (saveMode) saveMenu.draw(window);
            if (questMode) questMenu.draw(window);

            window.draw(keybindsText);
        }
    }
    else if (activeState == state::LUDZIK) {
        inventoryBar.draw(window);
        if (inventoryMode) inventoryMenu.draw(window);
        if (saveMode) saveMenu.draw(window);
        if (questMode) questMenu.draw(window);
        if (upgradeMode) upgradeMenu.draw(window);

        window.draw(keybindsText);
    }
    else if (activeState == state::MENU) {
        window.clear(sf::Color(15, 15, 20));
        mainMenu.draw(window);
    }
    else if (activeState == state::DEATH) {
        window.clear();
        window.draw(deathBackgroundShape);
        window.draw(deathText);
        window.draw(returnMenuText);
    }
    else if (activeState == state::WIN) {
        window.clear();
        window.draw(winBackgroundShape);
        window.draw(winText);
        window.draw(returnMenuText);
        window.draw(winQuitText);
    }

    if (activeState == state::STATEK) {
        window.setView(camera);
        notificationText.draw(window);
    } else if (activeState == state::LUDZIK) {
        window.setView(characterCamera);
        notificationText.draw(window);
    }

    window.display();
  }

  return 0;
}
