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