//std
#include <list>
#include <memory>
#include <string>

//alone
#include <alone/control/TextureManager.hpp>
#include <alone/control/Core.hpp>

namespace alone {
	extern Core core;

	void TextureManager::load(const toml::value& toml) {
		std::string folder;
		std::list <std::pair <std::string, std::string>> elems;

		this->m_content.clear();

		folder = toml.at("folder").as_string();

		auto array = toml.at("content").as_array();

		for (const auto& it : array) {
			auto& elem = it.as_table();
			auto& name = elem.at("name").as_string();
			auto& path = elem.at("path").as_string();

			elems.emplace_back(name, path);
		}

		for (const auto& it : elems) {
			std::shared_ptr <sf::Texture> texture(new sf::Texture());
			if (!texture->loadFromFile(folder + it.second))
				continue;

			this->m_content.emplace(it.first, texture);
		}
	}

	const std::shared_ptr <sf::Texture>& TextureManager::at(const std::string& id) const {
		return this->m_content.at(id);
	}
}