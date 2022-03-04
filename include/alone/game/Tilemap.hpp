#pragma once
//std
#include <memory>
#include <string>

//sfml
#include <SFML/Graphics.hpp>

//toml
#include <toml.hpp>

//alone
#include <alone/primitive/Util.hpp>
#include <alone/primitive/TomlIO.hpp>
#include <alone/primitive/Matrix.hpp>
#include <alone/game/Tile.hpp>
#include <alone/game/TileInfo.hpp>

namespace alone {
	class Tilemap : public ITomlIO {
	public:
		static void generate(std::string filename, size_t width, size_t height);

		void load(const toml::value& toml) override;
		toml::value save() const override;

		const TileInfoContainer& tileinfo() const;

		Tile& at(size_t x, size_t y);
		const Tile& at(size_t x, size_t y) const;

		void update(const sf::Vector2f& center);
		void render() const;

	protected:
		sf::Vector2f m_center;
		sf::FloatRegion m_drawRect;
		sf::LongRegion m_mapRect;
		size_t m_vertexSize;

		TileInfoContainer m_tileinfo;
		Matrix <Tile> m_matrix;
	};
}