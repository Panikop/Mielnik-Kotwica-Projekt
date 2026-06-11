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
        shipTex.loadFromFile("textures/ship_spritesheet.png");
        bulletTex.loadFromFile("textures/bullet_sprite.png");
        npcTex.loadFromFile("textures/npc_sprite.png");
        playerTex.loadFromFile("textures/player_sprite.png");
        ufoTex.loadFromFile("textures/ufo_sprite.png");
        explosionTex.loadFromFile("textures/explosion_sprite.png");
        mineTex.loadFromFile("textures/mine_sprite.png");
        stationTex.loadFromFile("textures/station_sprite.png");
    }
};
