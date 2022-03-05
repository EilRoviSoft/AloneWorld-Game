//std
#include <fstream>
#include <cmath>

//aesir
#include <Aesir/control/Core.hpp>
#include <Aesir/game/Tilemap.hpp>
#include <Aesir/graphics/Batch.hpp>

//How old is the auther of this text now?

namespace aesir {
	extern Core core;

	//static

	void Tilemap::generate(std::string filename, size_t width, size_t height) {
		std::ofstream file(filename);
		file << width << " " << height << '\n';

		file << "0 0 11 ";
		for (size_t i = 1; i != width - 1; i++)
			file << "0 0 6 ";
		file << "0 0 8\n";

		for (size_t i = 1; i != height - 1; i++) {
			file << "0 0 7 ";
			for (size_t j = 1; j != width - 1; j++) {
				file << "0 3 1 ";
			}
			file << "0 0 7\n";
		}

		file << "0 0 10 ";
		for (size_t i = 1; i != width - 1; i++)
			file << "0 0 6 ";
		file << "0 0 9 ";

		file.close();
	}

	//Tilemap

	void Tilemap::load(const toml::value& toml) {
		//loading tileinfo
		this->m_tileinfo.load(toml.at("TileInfo"));

		//loading content of map
		const toml::value& map = toml.at("Map");
		std::unordered_map <size_t, std::string> dict;
		for (const auto& it : map.at("dict").as_array()) {
			size_t id = it.at("id").as_integer();
			const std::string& name = it.at("name").as_string();
			dict.emplace(id, name);
		}

		std::string filename = map.at("path").as_string();
		std::ifstream file(filename);
		if (!file.is_open()) return void();

		size_t width, height;
		file >> width >> height;
		this->m_matrix.resize(width, height);

		for (size_t i = 0; i != this->m_matrix.capacity(); i++) {
			Tile& tile = this->m_matrix[i];
			size_t id, variation, shape;

			file >> id >> variation >> shape;
			tile = Tile(&this->m_tileinfo.at(dict.at(id)), variation, shape);
		}

		file.close();
	}
	toml::value Tilemap::save() const {
		return toml::value();
	}

	const TileInfoContainer& Tilemap::tileinfo() const {
		return this->m_tileinfo;
	}

	Tile& Tilemap::at(size_t x, size_t y) {
		return this->m_matrix(x, y);
	}
	const Tile& Tilemap::at(size_t x, size_t y) const {
		return this->m_matrix(x, y);
	}

	void Tilemap::update(const sf::Vector2f& center) {
		sf::Vector2u windowSize = core.window.getSize();

		this->m_center = center;

		this->m_drawRect = sf::FloatRegion(
			this->m_center.x - (float)windowSize.x / 2,
			this->m_center.y - (float)windowSize.y / 2,
			this->m_center.x - (float)windowSize.x / 2 + windowSize.x,
			this->m_center.y - (float)windowSize.y / 2 + windowSize.y
		);
		this->m_mapRect = sf::LongRegion(
			std::floorf(this->m_drawRect.x1 / Tile::size),
			std::floorf(this->m_drawRect.y1 / Tile::size),
			std::ceilf(this->m_drawRect.x2 / Tile::size),
			std::ceilf(this->m_drawRect.y2 / Tile::size)
		);

		sf::Vector2ul actualDrawSize(
			this->m_mapRect.x2 - (this->m_mapRect.x1 < 0 ? 1 : -1) * this->m_mapRect.x1,
			this->m_mapRect.y2 - (this->m_mapRect.y1 < 0 ? 1 : -1) * this->m_mapRect.y1
		);
		this->m_vertexSize = 4ull * actualDrawSize.x * actualDrawSize.y;
	}
	void Tilemap::render() const {
		//init

		aesir::QuadRequestBuilder builder;
		sf::Vector2u windowSize = core.window.getSize();

		//prepare

		builder.createRequest(this->m_vertexSize, sf::PrimitiveType::Quads, this->m_tileinfo.texture());

		//rendering

		for (int64_t i = this->m_mapRect.y1; i != this->m_mapRect.y2; i++) {
			for (int64_t j = this->m_mapRect.x1; j != this->m_mapRect.x2; j++) {
				const Tile* tile = this->m_matrix.getSafe(j, i);
				if (tile == nullptr) 
					continue;

				sf::Vector2f textureOffset(
					tile->getInfo().texPos().x + tile->getShape() * (Tile::size + 2) + 2,
					tile->getInfo().texPos().y + tile->getVariation() * (Tile::size + 2) + 2
				);
				sf::Vector2f position(j * Tile::size - this->m_drawRect.x1, i * Tile::size - this->m_drawRect.y1);
				sf::Vertex vertices[4];

				vertices[0].position = position;
				vertices[1].position = sf::Vector2f(position.x + Tile::size, position.y);
				vertices[2].position = sf::Vector2f(position.x + Tile::size, position.y + Tile::size);
				vertices[3].position = sf::Vector2f(position.x, position.y + Tile::size);

				vertices[0].texCoords = textureOffset;
				vertices[1].texCoords = sf::Vector2f(textureOffset.x + Tile::size, textureOffset.y);
				vertices[2].texCoords = sf::Vector2f(textureOffset.x + Tile::size, textureOffset.y + Tile::size);
				vertices[3].texCoords = sf::Vector2f(textureOffset.x, textureOffset.y + Tile::size);

				builder.add(vertices);
			}
		}

		core.batch.send(builder.releaseRequest());
	}
}