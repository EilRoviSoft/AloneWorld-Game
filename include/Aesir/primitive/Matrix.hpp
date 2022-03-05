#pragma once
//std
#include <vector>

namespace aesir {
	template <class _T>
	//using _T = int;
	class Matrix {
	public:
		Matrix() : m_width(0), m_height(0), m_capacity(0) {}
		Matrix(size_t width, size_t height) {
			this->resize(width, height);
		}
		~Matrix() {
			this->_Free();
		}

		_T& operator[](size_t i) {
			return this->m_content[i];
		}
		const _T& operator[](size_t i) const {
			return this->m_content[i];
		}

		_T* getSafe(size_t x, size_t y) const {			
			if (x < 0 || y < 0 || x >= this->m_width || y >= this->m_height)
				return nullptr;
			return &this->m_content[x + y * this->m_width];
		}

		_T& operator()(size_t x, size_t y) {
			return this->m_content[x + y * this->m_width];
		}
		const _T& operator()(size_t x, size_t y) const {
			return this->m_content[x + y * this->m_width];
		}

		void resize(size_t width, size_t height) {
			if (this->m_content)
				this->_Free();
			
			this->m_width = width;
			this->m_height = height;
			this->m_capacity = this->m_width * this->m_height;

			this->_Alloc();
		}

		size_t width() const {
			return this->m_width;
		}
		size_t height() const {
			return this->m_height;
		}
		size_t capacity() const {
			return this->m_capacity;
		}

		void clear() {
			this->m_width = 0;
			this->m_height = 0;
			this->m_capacity = 0;
			this->_Free();
		}

	protected:
		_T* m_content = nullptr;
		size_t m_width, m_height, m_capacity;

	private:
		void _Free() {
			delete[] this->m_content;
		}
		void _Alloc() {
			this->m_content = new _T[this->m_capacity];
		}
	};
}