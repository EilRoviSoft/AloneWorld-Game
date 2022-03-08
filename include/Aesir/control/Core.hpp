#pragma once
//std
#include <string>

//ctpl
#include <CTPL/ctpl_stl.h>

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
		ctpl::thread_pool threads;

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