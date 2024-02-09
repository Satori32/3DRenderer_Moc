#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

//logical address is if need width to need "W*3" to phisical address?? because Indexer is byte access to it.
//i try to data align to 4.

//please kill the catmal method.

class Surface24 {
public:
	struct refRGB{
		std::uint8_t& B;
		std::uint8_t& G;
		std::uint8_t& R;
		
	};

	bool Resize(std::uintmax_t Width, std::uintmax_t Height) {
		std::uintmax_t V =(4-((Width*3)%4))%4;
		P = V ? V : 0;
		S.clear();
		S.resize(Height*((Width*3)+P));
		W = Width;
		H = Height;
		return true;
	}

	bool InRange(std::uintmax_t X, std::uintmax_t Y) {
		if (X >= W) { return false; }
		if (Y >= H) { return false; }

		return true;
	}

	refRGB Index(std::uintmax_t X, std::uintmax_t Y) {
		if (InRange(X, Y) == false) { throw std::out_of_range("Surface24::Index();"); }
		std::uintmax_t PP = (Y * ((W * 3)+P)) + X*3;
		
		return { S[PP],S[PP+1],S[PP+2]};
	}
	std::uintmax_t BitCount() {
		return 24;
	}
	std::uintmax_t Pod() {
		return P;
	}
	std::uintmax_t Width() {
		return W;
	}
	std::uintmax_t Height() {
		return H;
	}
	std::size_t Size() {
		return S.size();
	}

	bool Clear() {
		S.clear();
		return true;
	}
	
	typename std::vector<std::uint8_t>::iterator begin() {
		return S.begin();
	}
	typename std::vector<std::uint8_t>::iterator end() {
		return S.end();
	}
protected:
	std::uintmax_t P = 0;
	std::uintmax_t W = 0;
	std::uintmax_t H = 0;
	std::vector<std::uint8_t> S;
};

class BITMAP24 {
	//__declspec(align(#)) is maybe vc only.
public:
	struct FileHeader {
		std::uint16_t Type = ('M' << 8) | 'B';
		std::uint32_t FileSize = 0;
		std::uint16_t ResurvedA = 0;
		std::uint16_t ResurvedB = 0;
		std::uint32_t PixelPosition = sizeof(FileHeader)+sizeof(InfoHeader)-2;
	};
	struct InfoHeader {
		std::uint32_t Size = 40;
		std::uint32_t Width = 0;
		std::uint32_t Height = 0;
		std::uint16_t Plain = 1;
		std::uint16_t BitCount = 24;
		std::uint32_t Compression = 0;//BI_RGB.
		std::uint32_t ImageSize = 0;
		std::uint32_t PixelParMeterX = 0;
		std::uint32_t PixelParMeterY = 0;
		std::uint32_t UsingPaletteColor = 0;
		std::uint32_t ImpotantColorPalleteNumber = 0;
	};

	bool Resize(std::size_t Widrh, std::size_t Height) {
		return S.Resize(Widrh, Height);
		
	}

	bool SetPixel(const std::uint32_t& X, const std::uint32_t& Y, const std::uint8_t& R, const std::uint8_t& G, const std::uint8_t& B) {

		Surface24::refRGB Re{S.Index(X, Y)};
		Re.R = R;
		Re.G = G;
		Re.B = B;

		return true;
	}

	Surface24::refRGB Index(std::size_t X, std::size_t Y) {
		return S.Index(X, Y);
	}
	std::size_t Pod() {
		return S.Pod();
	}

	std::size_t Width() {
		return S.Width();
	}
	std::size_t Height() {
		return S.Height();
	}

	bool Clear() {
		S.Clear();
	}

	std::vector<std::uint8_t> CreateStructuer() {
		FileHeader FH;
		InfoHeader IH;

		FH.FileSize = sizeof(FH) + sizeof(IH) + S.Size()-2;//it -2 is memory alignment ploblem
		FH.PixelPosition = sizeof(FH) + sizeof(IH)-2;
		

		IH.Size = sizeof(IH);
		IH.Width = S.Width();
		IH.Height = S.Height();
		IH.BitCount = S.BitCount();

		std::vector<std::uint8_t>R;
		R.insert(R.end(),(std::uint8_t*) &FH,(std::uint8_t*) &FH + 2);
		R.insert(R.end(),(std::uint8_t*) &FH+4,(std::uint8_t*) &FH + sizeof(FH));
		R.insert(R.end(), (std::uint8_t*)&IH, (std::uint8_t*)&IH + sizeof(IH));
		R.insert(R.end(), S.begin(), S.end());

		return R;
	}

protected:
	Surface24 S;

};

int main(){
	Surface24 S;

	S.Resize(16, 16);
	S.Index(0, 0).R = 1;
	S.Resize(7, 3);
	S.Index(2, 2).G = 1;

	BITMAP24 B;
	B.Resize(16, 16);
	B.SetPixel(0, 0, 255, 255, 255);
	B.SetPixel(1, 0, 255, 0, 255);
	auto M = B.CreateStructuer();

	std::ofstream of("Hoge.bmp", std::ios::binary); 
	of.write((const char*)&M[0], M.size());

	return 0;

}