#pragma once
//std
#include <string>

//sf
#include <SFML/Graphics.hpp>

//aesir
#include <Aesir/graphics/Batch.hpp>
#include <Aesir/control/TextureManager.hpp>
#include <Aesir/control/InputManager.hpp>

namespace aesir {
	class Core {
	public:
		//util
		
		const std::hash <std::string> hasher;

		//controllers

		TextureManager textures;
		InputManager input;

		//graphics

		Batch batch;

		//sfml

		sf::RenderWindow window;
		sf::Clock clock;
		sf::Font font;
	};
}