#pragma once
//sfml
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

//aesir
#include <alone/primitive/Util.hpp>

namespace aesir {
	class BoxCollider {
	public:
		const sf::FloatRegion& box() const;
		sf::FloatRegion& box();

		const sf::Vector2f& speed() const;
		sf::Vector2f& speed();

		virtual void update(const sf::Clock& dt);

	protected:
		sf::FloatRegion m_box;
		sf::Vector2f m_speed;
	};
}