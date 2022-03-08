//aesir
#include <Aesir/graphics/Batch.hpp>

namespace aesir {
	//Batch

	void Batch::send(request_t&& request) {
		this->m_requests.push_back(std::move(request));
	}

	void Batch::render(sf::RenderTarget& target, sf::RenderStates states) {
		for (const auto& it : this->m_requests) {
			states.texture = &it.texture;
			target.draw(it.vertices, states);
		}
	}

	void Batch::flush() {
		this->m_requests.clear();
	}

	//Batch::request_t
	
	Batch::request_t::request_t(size_t size, sf::PrimitiveType type, const sf::Texture& texture) : texture(texture) {
		this->vertices.resize(size);
		this->vertices.setPrimitiveType(type);
	}

	//RequestBuilder

	void RequestBuilder::begin(size_t size, sf::PrimitiveType type, const sf::Texture& texture) {
		this->m_request.reset(new Batch::request_t(size, type, texture));
	}

	Batch::request_t&& RequestBuilder::end() {
		this->m_offset = 0;
		return std::move(*this->m_request.release());
	}

	//QuadRequestBuilder

	void QuadRequestBuilder::add(sf::Vertex* vertices) {
		for (size_t i = 0; i != 4; i++)
			this->m_request->vertices[this->m_offset + i] = vertices[i];
		this->m_offset += 4;
	}
}