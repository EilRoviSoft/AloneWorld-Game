#pragma once
//std
#include <cstdint>

//sfml
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf {
	template <class _T>
	struct Region {
		Region() : x1(0), y1(0), x2(0), y2(0) {}
		Region(_T x1, _T y1, _T x2, _T y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

		_T x1, y1, x2, y2;

		_T width() const { return x2 - x1; }
		_T height() const { return y2 - y1; }

		Vector2 <_T> center() const { return { (x1 + x2) / 2, (y1 + y2) / 2 }; }

		Region& operator+=(const Vector2 <_T>& rhs) {
			this->x1 += rhs.x;
			this->x2 += rhs.x;
			this->y1 += rhs.y;
			this->y2 += rhs.y;

			return *this;
		}
	};
	using IntRegion = Region <int32_t>;
	using FloatRegion = Region <float_t>;

	using Vector2l = Vector2 <int64_t>;
	using LongRect = Rect <int64_t>;
	using LongRegion = Region <int64_t>;

	using Vector2ul = Vector2 <uint64_t>;
	using UlongRect = Rect <uint64_t>;
	using UlongRegion = Rect <uint64_t>;
}

namespace aesir {
	template <class _T>
	_T clamp(_T value, _T min, _T max) {
		if (value < min) return min;
		else if (value > max) return max;
		else return value;
	}
}