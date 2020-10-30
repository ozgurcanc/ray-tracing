#pragma once
#include "../core/Vector3.h"

struct TMO
{
	real keyValue = 0.18f;
	real burn_percent = 1.f;
	real saturation = 1.f;
	real gamma = 2.2f;
};

typedef union Color
{
	struct
	{
		float red;
		float grn;
		float blu;
	};

	float channel[3];
} Color;


class Image
{
public:
	Color** data;                  
	unsigned width;						
	unsigned height;

	Image(int width, int height);
	Image(const char *filePath);
	~Image();
	void SetPixelValue(int col, int row, const Vector3& color);          
	void SaveImage(const char *imageName, const TMO * tmo);
	void ToneMapping(const TMO * tmo);
	
	void SaveAsEXR(const char *filename) const;
	void SaveAsPNG(const char *filename) const;
	void SaveAsJPG(const char *filename) const;

	void Load_PNGorJPG(const char *filename);
	void Load_EXR(const char *filename);
};



