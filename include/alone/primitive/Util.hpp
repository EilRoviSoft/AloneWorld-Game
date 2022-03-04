#pragma once
//std
#include <cstdint>

//sfml
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf {
	template <class _T>
	struct region_t {
		region_t() : x1(_T(0)), x2(_T(0)), y1(_T(0)), y2(_T(0)) {}
		region_t(_T x1, _T y1, _T x2, _T y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

		_T x1, x2, y1, y2;
	};
	using FloatRegion = region_t <float_t>;
	using LongRegion = region_t <int64_t>;

	using Vector2l = Vector2 <int64_t>;
	using LongRect = Rect <int64_t>;

	using Vector2ul = Vector2 <uint64_t>;
	using UlongRect = Rect <uint64_t>;
}