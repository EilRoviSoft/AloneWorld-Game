#pragma once
//std
#include <unordered_map>

//sfml
#include <SFML/Window.hpp>

//toml
#include <toml.hpp>

//aesir
#include <Aesir/primitive/TomlIO.hpp>

namespace aesir {
	struct InputStatus {
		bool now = false, pre = false;

		bool isKeyPressed() const;
		bool isKeyUpped() const;
	};

	class InputManager : public ITomlLoadable {
	public:
		void load(const toml::value& toml);

		void update();

		const InputStatus& at(uint64_t id) const;

	protected:
		std::unordered_multimap <sf::Keyboard::Key, uint64_t> m_keys;
		std::unordered_multimap <sf::Mouse::Button, uint64_t> m_buttons;
		std::unordered_map <uint64_t, InputStatus> m_statuses;
	};
}