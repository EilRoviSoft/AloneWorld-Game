#pragma once
//std
#include <unordered_map>

//sfml
#include <SFML/Graphics.hpp>

//toml
#include <toml.hpp>

//alone
#include <alone/primitive/TomlIO.hpp>
#include <alone/primitive/util.hpp>

namespace alone {
	using TextureInfo = std::pair <std::string, std::shared_ptr <sf::Texture>>;

	class TextureManager : public ITomlLoadable {
	public:
		//clears content and loads to empty container
		//doesn't check specified poles in file, may throw errors
		void load(const toml::value& toml) final override;

		const std::shared_ptr <sf::Texture>& at(const std::string& id) const;
		//const TextureInfo& info(const std::string& id) const;

	protected:
		std::unordered_map <std::string, std::shared_ptr <sf::Texture>> m_content;
	};
}