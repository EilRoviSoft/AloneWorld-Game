//alone
#include <alone/game/TileInfo.hpp>
#include <alone/control/Core.hpp>

namespace alone {
	extern Core core;

	//TileInfo

	TileInfo::TileInfo(const sf::Texture& texture, sf::Vector2i texPos, uint8_t variations, uint8_t shapes) : m_texture(texture) {
		this->m_texPos = texPos;
		this->m_variations = variations;
		this->m_shapes = shapes;
	}

	const sf::Texture& TileInfo::texture() const { return this->m_texture; }
	sf::Vector2i TileInfo::texPos() const { return this->m_texPos; }

	uint8_t TileInfo::variations() const { return this->m_variations; }
	uint8_t TileInfo::shapes() const { return this->m_shapes; }

	//TileInfoManager

	void TileInfoContainer::load(const toml::value& toml) {
		const auto& array = toml.at("content").as_array();

		const std::string& texture_id = toml.at("atlas").as_string();
		this->m_texture = core.textures.at(texture_id).get();

		for (const auto& it : array) {
			std::string name = it.at("name").as_string();

			int x = it.at("x").as_integer(), y = it.at("y").as_integer();

			uint8_t variations = it.at("variations").as_integer();
			uint8_t shapes = it.at("shapes").as_integer();

			this->m_content.emplace(name, TileInfo(*this->m_texture, sf::Vector2i(x, y), variations, shapes));
		}
	}

	const TileInfo& TileInfoContainer::at(const std::string& id) const {
		return this->m_content.at(id);
	}
	const sf::Texture& TileInfoContainer::texture() const {
		return *this->m_texture;
	}
}