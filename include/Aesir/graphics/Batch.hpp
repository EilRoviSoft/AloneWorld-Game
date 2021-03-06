#pragma once
//std
#include <list>
#include <memory>

//sfml
#include <SFML/Graphics.hpp>

namespace aesir {
	class Batch {
	public:
		struct request_t {
			request_t(size_t size, sf::PrimitiveType type, const sf::Texture& texture);

			sf::VertexArray vertices;
			const sf::Texture& texture;
		};

		void send(request_t&& request);

		void render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
		//call after `render`
		void flush();

	protected:
		std::list <request_t> m_requests;
	};

	class RequestBuilder {
	public:
		void begin(size_t size, sf::PrimitiveType type, const sf::Texture& texture);
		virtual void add(sf::Vertex* vertices) = 0;
		Batch::request_t&& end();

	protected:
		std::unique_ptr <Batch::request_t> m_request = nullptr;
		size_t m_offset = 0;
	};

	class QuadRequestBuilder : public RequestBuilder {
	public:
		void add(sf::Vertex* vertices) final override;
	};
}