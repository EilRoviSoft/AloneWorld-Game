#pragma once
//sfml
#include <SFML/Graphics.hpp>

//aesir
#include <Aesir/control/Core.hpp>
#include <Aesir/game/Tilemap.hpp>
#include <Aesir/game/Physics.hpp>

namespace alone {
	enum Actions {
		Click = 0,
		MoveUp = 1,
		MoveLeft = 2,
		MoveDown = 3,
		MoveRight = 4
	};

	class Player : public aesir::BoxCollider {
		friend class Game;
	public:
		void init();

		void update(sf::Time dt) final override;
	
	private:
		const aesir::InputStatus* _moveLeft, * _moveRight, * _moveUp, * _moveDown;
	};

	class Game {
	public:
		void init();

		void process();

	private:
		sf::Time _start = sf::Time();
		Player _player;
		aesir::Tilemap _tilemap;
		sf::Text _fps;

	private:
	};
}