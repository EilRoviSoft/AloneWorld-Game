//aesir
#include <alone/game/Physics.hpp>
#include <alone/control/Core.hpp>

namespace aesir {
	const sf::FloatRegion& BoxCollider::box() const { return this->m_box; }
	sf::FloatRegion& BoxCollider::box() { return this->m_box; }
	
	const sf::Vector2f& BoxCollider::speed() const { return this->m_speed; }
	sf::Vector2f& BoxCollider::speed() { return this->m_speed; }
	
	void BoxCollider::update(const sf::Clock& dt) {

	}
}
