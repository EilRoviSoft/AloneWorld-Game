#pragma once
//std
#include <string>

//sf
#include <SFML/Graphics.hpp>

//alone
#include <alone/graphics/Batch.hpp>
#include <alone/control/TextureManager.hpp>
#include <alone/control/InputManager.hpp>

namespace alone {
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
	};
}