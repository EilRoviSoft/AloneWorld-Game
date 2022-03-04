#pragma once
//sfml
#include <SFML/Graphics.hpp>

namespace alone {
	class Camera/* : public sf::View*/ {
	public:
		sf::Vector2f& position() { return this->m_position; }
		const sf::Vector2f& position() const { return this->m_position; }

	protected:
		sf::Vector2f m_position;
	};
}