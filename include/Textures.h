#pragma once
#include <SFML/Graphics.hpp>

class Textures {
public:
    static sf::Texture shipTex;
    static sf::Texture shipAnimTex;
    static sf::Texture bulletTex;
    static sf::Texture npcTex;
    static sf::Texture playerTex;
    static sf::Texture playerAnimTex;
    static sf::Texture ufoTex;
    static sf::Texture explosionTex;
    static sf::Texture mineTex;
    static sf::Texture stationTex;
    static sf::Texture wallTex;
    static sf::Texture floorTex;

    static void load() {
        shipTex.loadFromFile("textures/ship_spritesheet.png");
        shipAnimTex.loadFromFile("textures/ship_anim.png");
        bulletTex.loadFromFile("textures/bullet_sprite.png");
        npcTex.loadFromFile("textures/npc_sprite.png");
        playerTex.loadFromFile("textures/player_sprite.png");
        playerAnimTex.loadFromFile("textures/player_anim_trimmed.png");
        ufoTex.loadFromFile("textures/ufo_sprite.png");
        explosionTex.loadFromFile("textures/explosion_sprite.png");
        mineTex.loadFromFile("textures/mine_sprite.png");
        stationTex.loadFromFile("textures/station_sprite.png");
        wallTex.loadFromFile("textures/wall_texture.png");
        wallTex.setRepeated(true);
        floorTex.loadFromFile("textures/floor_texture.png");
        floorTex.setRepeated(true);
    }
};
