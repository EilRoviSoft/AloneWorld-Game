#pragma once
//alone
#include <alone/game/TileInfo.hpp>

namespace alone {
	struct Tile {
		friend class Tilemap;
	public:
		static const float size;

		Tile();
		Tile(const TileInfo* info, uint8_t variation, uint8_t shape);

		void setInfo(const TileInfo& info);
		const TileInfo& getInfo() const;

		void setVariation(uint8_t variation);
		uint8_t getVariation() const;

		void setShape(uint8_t shape);
		uint8_t getShape() const;

	protected:
		const TileInfo* m_info;
		uint8_t m_variation, m_shape;
	};
}