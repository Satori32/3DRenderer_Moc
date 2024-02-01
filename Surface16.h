#pragma once
#include <vector>
#include <cstdint>

class Surface16 {
public:
	struct RGBA {
		union {
			struct {
				std::uint16_t R : 4;
				std::uint16_t G : 4;
				std::uint16_t B : 4;
				std::uint16_t A : 4;
			};
			struct {
				std::uint16_t r : 4;
				std::uint16_t g : 4;
				std::uint16_t b : 4;
				std::uint16_t a : 4;
			};
			std::uint16_t rgba = 0;
		};
	};

	bool Resize(std::uintmax_t Width, std::uintmax_t Height) {
		P = Width % 2;
		S.clear();
		S.resize(Height * (Width + P));
		W = Width;
		H = Height;
		return true;
	}

	bool InRange(std::uintmax_t X, std::uintmax_t Y) {
		if (X >= W) { return false; }
		if (Y >= H) { return false; }

		return true;
	}

	RGBA Index(std::uintmax_t X, std::uintmax_t Y) {
		if (InRange(X, Y) == false) { throw std::out_of_range("Surface16::Index();"); }
		return S[(H * (W + P)) + X];


	}

protected:
	std::uintmax_t P = 0;
	std::uintmax_t W = 0;
	std::uintmax_t H = 0;
	std::vector<RGBA> S;
};