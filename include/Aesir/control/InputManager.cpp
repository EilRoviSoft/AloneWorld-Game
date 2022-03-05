//alone
#include <Aesir/control/InputManager.hpp>

namespace aesir {
	//InputStatus

	bool InputStatus::isKeyPressed() const {
		return !this->pre && this->now;
	}
	bool InputStatus::isKeyUpped() const {
		return this->pre && !this->now;
	}

	//InputManager

	void InputManager::load(const toml::value& toml) {
		for (const auto& it : toml.at("content").as_array()) {
			uint64_t type = it.at("type").as_integer(), statusId = it.at("id").as_integer();
			
			if (type == 0) {
				sf::Mouse::Button button = (sf::Mouse::Button)it.at("button").as_integer();
				this->m_buttons.emplace(button, statusId);
			} else if (type == 1) {
				sf::Keyboard::Key key = (sf::Keyboard::Key)it.at("key").as_integer();
				this->m_keys.emplace(key, statusId);
			} else continue;

			this->m_statuses.emplace(statusId, InputStatus());
		}
	}

	void InputManager::update() {
		for (auto& it : this->m_keys) {
			auto& status = this->m_statuses.at(it.second);
			status.pre = status.now;
			status.now = sf::Keyboard::isKeyPressed(it.first);
		}
		for (auto& it : this->m_buttons) {
			auto& status = this->m_statuses.at(it.second);
			status.pre = status.now;
			status.now = sf::Mouse::isButtonPressed(it.first);
		}
	}

	const InputStatus& InputManager::at(uint64_t id) const {
		return this->m_statuses.at(id);
	}
}