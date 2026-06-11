#pragma once
#include <SFML/Graphics.hpp>

class Textures {
public:
    static sf::Texture shipTex;
    static sf::Texture bulletTex;
    static sf::Texture npcTex;
    static sf::Texture playerTex;
    static sf::Texture ufoTex;
    static sf::Texture explosionTex;
    static sf::Texture mineTex;
    static sf::Texture stationTex;

    static void load() {
        shipTex.loadFromFile("src/ship_spritesheet.png");
        bulletTex.loadFromFile("src/bullet_sprite.png");
        npcTex.loadFromFile("src/npc_sprite.png");
        playerTex.loadFromFile("src/player_sprite.png");
        ufoTex.loadFromFile("src/ufo_sprite.png");
        explosionTex.loadFromFile("src/explosion_sprite.png");
        mineTex.loadFromFile("src/mine_sprite.png");
        stationTex.loadFromFile("src/station_sprite.png");
    }
};
