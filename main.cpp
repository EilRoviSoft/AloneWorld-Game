//alone
#include "source/Alone/Game.hpp"

namespace aesir {
	Core core;
}

int main() {
	aesir::Core& core = aesir::core;
	alone::Game game;
	game.init();

	while (core.window.isOpen()) {
		//event processing
		sf::Event event;
		while (core.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				core.window.close();
		}

		game.process();
	}

	core.window.close();

	return 0;
}