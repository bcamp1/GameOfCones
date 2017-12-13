#include "SFML/Graphics.hpp"
#include <iostream>
#include "Entity.h"

Entity::Entity(sf::Vector2f size, sf::Vector2f pos, std::string filename) : fname(filename){
	setImage(fname);
	width = 64;
	height = 64;

	x = pos.x;
	y = pos.y;

}

void Entity::show(sf::RenderWindow &window) {
	sprite.setPosition(x, y);
	x += velX;
	y += velY;
	velX += accX;
	velY += accY;

	window.draw(sprite);
	setImage(fname);
}

void Entity::setImage(string filename) {
    tx.loadFromFile(filename);
    sprite.setTexture(tx);
}

sf::FloatRect Entity::getBounds() { return sprite.getGlobalBounds(); }

sf::Vector2f Entity::getPos() { return sprite.getPosition(); }
