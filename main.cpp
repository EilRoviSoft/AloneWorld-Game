#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include <alone/control/Core.hpp>
#include <alone/game/Tilemap.hpp>

namespace alone {
	Core core;
}

alone::Core& core = alone::core;

void init() {
	core.textures.loadFromFile("source/settings/texture.toml");
	core.input.loadFromFile("source/settings/input.toml");
	core.font.loadFromFile("source/settings/arial.ttf");
}

int main() {
	init();

	const auto& moveUp = core.input.at(1);
	const auto& moveLeft = core.input.at(2);
	const auto& moveDown = core.input.at(3);
	const auto& moveRight = core.input.at(4);

	core.window.create(sf::VideoMode(1600, 900), "Alone World");
	core.window.setVerticalSyncEnabled(true);

	sf::Vector2f position = { 400.f, 300.f };

	alone::Tilemap tilemap;
	tilemap.loadFromFile("source/settings/tilemap.toml");

	sf::RenderStates states = sf::RenderStates::Default;
	
	sf::Text fps("", core.font);

	while (core.window.isOpen()) {
		//event processing
		sf::Event event;
		while (core.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				core.window.close();
		}

		core.clock.restart();
		auto start = core.clock.getElapsedTime().asMicroseconds();

		//preparing
		core.window.clear(sf::Color::Black);

		//updating
		core.input.update();

		if (moveLeft.now) 
			position.x -= 1;
		if (moveRight.now) 
			position.x += 1;
		if (moveUp.now)
			position.y -= 1;
		if (moveDown.now)
			position.y += 1;

		tilemap.update(position);

		//rendering
		tilemap.render();

		//drawing on window
		core.batch.render(core.window, states);
		core.batch.flush();

		auto end = core.clock.getElapsedTime().asMicroseconds();
		fps.setString(std::to_string(end - start));
		core.window.draw(fps);

		core.window.display();
	}

	core.window.close();
	return 0;
}