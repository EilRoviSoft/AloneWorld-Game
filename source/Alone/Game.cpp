//std
#include <cmath>

//alone
#include "Game.hpp"

namespace aesir {
	extern Core core;
}

namespace alone {
	const float accel = 1.f;
	const float delay = 1000.f / 144.f;
	const float maxSpeed = 10.f;

	//Player

	void Player::init() {
		this->_moveLeft = &aesir::core.input.at(Actions::MoveLeft);
		this->_moveRight = &aesir::core.input.at(Actions::MoveRight);
		this->_moveUp = &aesir::core.input.at(Actions::MoveUp);
		this->_moveDown = &aesir::core.input.at(Actions::MoveDown);
	}

	void Player::update(sf::Time dt) {
		float daccel = accel / ((float)dt.asMilliseconds());

		sf::Vector2 <bool> moved = { false, false };
		if (this->_moveLeft->now) {
			this->m_speed.x -= daccel;
			moved.x = true;
		}
		if (this->_moveRight->now) {
			this->m_speed.x += daccel;
			moved.x = true;
		}
		if (this->_moveUp->now) {
			this->m_speed.y += daccel;
			moved.y = true;
		}
		if (this->_moveDown->now) {
			this->m_speed.y -= daccel;
			moved.y = true;
		}

		const auto& editSpeed = [](const float& speed, const float& accel, bool moved) {
			if (std::fabsf(speed) < accel) {
				return 0.f;
			} else if (!moved) {
				if (speed < 0) {
					return speed + accel;
				} else {
					return speed - accel;
				}
			} else {
				return aesir::clamp(speed, -maxSpeed, maxSpeed);
			}
		};

		this->m_speed.x = editSpeed(this->m_speed.x, daccel, moved.x);
		this->m_speed.y = editSpeed(this->m_speed.y, daccel, moved.y);

		this->m_box += { this->m_speed.x, -this->m_speed.y };
	}

	//Game

	void Game::init() {
		//init uitl
		aesir::core.threads.resize(1);

		//init sfml
		aesir::core.window.create(sf::VideoMode(1600, 900), "Alone World");
		aesir::core.window.setVerticalSyncEnabled(true);

		//init aesir
		aesir::core.textures.loadFromFile("assets/settings/texture.toml");
		aesir::core.input.loadFromFile("assets/settings/input.toml");
		aesir::core.font.loadFromFile("assets/settings/arial.ttf");

		aesir::core.font.loadFromFile("assets/settings/arial.ttf");

		//init game
		this->_fps.setFont(aesir::core.font);
		this->_fps.setFillColor(sf::Color::Black);

		this->_tilemap.loadFromFile("assets/settings/tilemap.toml");

		this->_player.init();
	}

	void Game::process() {
		//update clock
		aesir::core.clock.restart();

		//update input
		aesir::core.input.update();

		//update game
		this->_player.update(this->_start - aesir::core.clock.getElapsedTime());

		auto center = this->_player.box().center();
		this->_tilemap.update(center);

		//clear
		aesir::core.window.clear(sf::Color::White);

		//drawing game
		this->_tilemap.render();

		aesir::core.batch.render(aesir::core.window);

		//fps
		auto dt = this->_start - aesir::core.clock.getElapsedTime();
		uint32_t fps = 1000000.f / dt.asMicroseconds();
		this->_fps.setString(std::to_string(fps));
		aesir::core.window.draw(this->_fps);

		//display
		aesir::core.window.display();
		aesir::core.batch.flush();

		//postupdate
		this->_start = aesir::core.clock.getElapsedTime();
	}

}