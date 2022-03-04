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
}

void generate(std::string filename, size_t width, size_t height) {
	std::ofstream file(filename);
	file << width << " " << height << '\n';

	file << "0 0 11 ";
	for (size_t i = 1; i != width - 1; i++)
		file << "0 0 6 ";
	file << "0 0 8\n";

	for (size_t i = 1; i != height - 1; i++) {
		file << "0 0 7 ";
		for (size_t j = 1; j != width - 1; j++) {
			file << "0 3 1 ";
		}
		file << "0 0 7\n";
	}

	file << "0 0 10 ";
	for (size_t i = 1; i != width - 1; i++)
		file << "0 0 6 ";
	file << "0 0 9 ";

	file.close();
}

int main() {
	//generate("source/settings/test.map", 100, 100);

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

	std::function render = [&]() {
		tilemap.update(position);

		//rendering
		tilemap.render();

		//drawing on window
		core.batch.render(core.window, states);
		core.batch.flush();
		core.window.display();
	};

	while (core.window.isOpen()) {
		//event processing
		sf::Event event;
		while (core.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				core.window.close();
		}

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

		render();
	}

	core.window.close();
	return 0;
}