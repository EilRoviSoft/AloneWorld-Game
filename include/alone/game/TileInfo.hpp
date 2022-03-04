#pragma once
//std
#include <cstdint>
#include <memory>
#include <unordered_map>

//sfml
#include <SFML/Graphics.hpp>

//toml
#include <toml.hpp>

//alone
#include <alone/primitive/TomlIO.hpp>
#include <alone/primitive/util.hpp>

namespace alone {
	struct TileInfo {
		friend class TileInfoContainer;
		friend class Tilemap;
	public:
		TileInfo(const sf::Texture& texture, sf::Vector2i texPos, uint8_t variations, uint8_t shapes);

		const sf::Texture& texture() const;
		sf::Vector2i texPos() const;

		uint8_t variations() const;
		uint8_t shapes() const;

	protected:
		const sf::Texture& m_texture;
		sf::Vector2i m_texPos;
		uint8_t m_variations, m_shapes;
	};

	class TileInfoContainer : public ITomlLoadable {
	public:
		void load(const toml::value& toml) final override;

		const TileInfo& at(const std::string& id) const;
		const sf::Texture& texture() const;

	protected:
		std::unordered_map <std::string, TileInfo> m_content;
		const sf::Texture* m_texture;
	};
}