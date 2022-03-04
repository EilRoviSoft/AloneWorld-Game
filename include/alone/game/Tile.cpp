//aesir
#include <alone/game/Tile.hpp>

namespace aesir {
	const float Tile::size = 32.f;

	Tile::Tile() : m_info(nullptr), m_variation(0), m_shape(0) {}
	Tile::Tile(const TileInfo* info, uint8_t variation, uint8_t shape) {
		this->m_info = info;
		this->m_variation = variation;
		this->m_shape = shape;
	}

	void Tile::setInfo(const TileInfo& info) { this->m_info = &info; }
	const TileInfo& Tile::getInfo() const { return *this->m_info; }
	
	void Tile::setVariation(uint8_t variation) { this->m_variation = variation; }
	uint8_t Tile::getVariation() const { return this->m_variation; }
	
	void Tile::setShape(uint8_t shape) { this->m_shape = shape; }
	uint8_t Tile::getShape() const { return this->m_shape; }
}