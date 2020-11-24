#include <DualSenseWindows/Helpers.h>

DS5W_API DS5W::Color DS5W::color_R32G32B32_FLOAT(float r, float g, float b) {
	return {(unsigned char)(255.0F * r), (unsigned char)(255.0F * g), (unsigned char)(255.0F * b)};
}

DS5W_API DS5W::Color DS5W::color_R32G32B32A32_FLOAT(float r, float g, float b, float a) {
	return { (unsigned char)(255.0F * r * a), (unsigned char)(255.0F * g * a), (unsigned char)(255.0F * b * a) };
}

DS5W_API DS5W::Color DS5W::color_R8G8B8A8_UCHAR(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { 
	return { (unsigned char)(r * (a / 255.0f)), (unsigned char)(g * (a / 255.0f)), (unsigned char)(b * (a / 255.0f)) };
}

DS5W_API DS5W::Color DS5W::color_R8G8B8_UCHAR_A32_FLOAT(unsigned char r, unsigned char g, unsigned char b, float a) {
	return { (unsigned char)(r * a), (unsigned char)(g * a), (unsigned char)(b * a) };
}
