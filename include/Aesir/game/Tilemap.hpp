#pragma once
//std
#include <memory>
#include <string>

//sfml
#include <SFML/Graphics.hpp>

//toml
#include <toml.hpp>

//aesir
#include <Aesir/primitive/Util.hpp>
#include <Aesir/primitive/TomlIO.hpp>
#include <Aesir/primitive/Matrix.hpp>
#include <Aesir/game/Tile.hpp>
#include <Aesir/game/TileInfo.hpp>

namespace aesir {
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