#include "Image.h"
#include <cstdio>
#include <cstdlib>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../thirdparty/stb_image_write.h"

#define TINYEXR_IMPLEMENTATION
#include "../thirdparty/tinyexr.h"

#define PNG 0
#define JPG 1

Image::Image(int width, int height)
	:width(width), height(height)
{
	data = new Color*[height];

	for (int y = 0; y < height; ++y)
	{
		data[y] = new Color[width];
	}
}

Image::Image(const char *filePath)
{
	if (IsEXR(filePath) == TINYEXR_SUCCESS)
		Load_EXR(filePath);
	else
		Load_PNGorJPG(filePath);
}

Image::~Image()
{
	for (int y = 0; y < height; y++)
	{
		delete[] data[y];
	}

	delete[] data;
}

void Image::SetPixelValue(int col, int row, const Vector3& color)
{
	data[row][col].red = color.x;
	data[row][col].grn = color.y;
	data[row][col].blu = color.z;
}

void Image::SaveImage(const char *imageName, const TMO * tmo)
{
	if (tmo)
	{
		SaveAsEXR(imageName);
		ToneMapping(tmo);

		std::string s = imageName;
		s.replace(s.size() - 3, 3, "png");
		SaveAsPNG(s.c_str());
	}
	else
	{
		SaveAsPNG(imageName);
	}
}

void Image::ToneMapping(const TMO* tmo)
{

	float log_average_luminance = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float l = 0.27f * data[i][j].red + 0.67f * data[i][j].grn + 0.06f * data[i][j].blu;

			log_average_luminance += log(l + 0.00001);
		}
	}

	log_average_luminance = exp(log_average_luminance / (width * height));
	float scale_factor = tmo->keyValue / log_average_luminance;
	std::vector<float> world_luminances;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float l = 0.27f * data[i][j].red + 0.67f * data[i][j].grn + 0.06f * data[i][j].blu;

			float scaled_luminance = l * scale_factor;

			world_luminances.push_back(scaled_luminance);
		}
	}

	std::sort(world_luminances.begin(), world_luminances.end());

	float index = round(world_luminances.size() * ((100.f - tmo->burn_percent) / 100.f));
	float white_luminance = world_luminances[index - 1];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float l = 0.27f * data[i][j].red + 0.67f * data[i][j].grn + 0.06f * data[i][j].blu;

			float scaled_luminance = l * scale_factor;

			float display_luminance = (scaled_luminance * (1 + scaled_luminance / (white_luminance * white_luminance))) / (1 + scaled_luminance);

			float r = pow(data[i][j].red / l, tmo->saturation) * display_luminance;
			float g = pow(data[i][j].grn / l, tmo->saturation) * display_luminance;
			float b = pow(data[i][j].blu / l, tmo->saturation) * display_luminance;

			r = clamp(r, 0.0f, 1.0f);
			g = clamp(g, 0.0f, 1.0f);
			b = clamp(b, 0.0f, 1.0f);

			data[i][j].red = pow(r, 1 / tmo->gamma) * 255;
			data[i][j].grn = pow(g, 1 / tmo->gamma) * 255;
			data[i][j].blu = pow(b, 1 / tmo->gamma) * 255;
		}
	}

}

void Image::SaveAsEXR(const char *filename) const
{
	float * img = (float*)malloc(width * height * 3 * sizeof(float));

	int temp = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			img[temp++] = data[i][j].red;
			img[temp++] = data[i][j].grn;
			img[temp++] = data[i][j].blu;
		}
	}

	int ret = SaveEXR(img, width, height, 3, 1, filename, 0);

	if (ret != TINYEXR_SUCCESS)
	{
		printf("Error while saving exr file");
		exit(-1);
	}

	free(img);
}

void Image::SaveAsPNG(const char *filename) const
{
	unsigned char * img = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));
	int temp = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			img[temp++] = data[i][j].red > 255 ? 255 : data[i][j].red;
			img[temp++] = data[i][j].grn > 255 ? 255 : data[i][j].grn;
			img[temp++] = data[i][j].blu > 255 ? 255 : data[i][j].blu;
		}
	}

	stbi_write_png(filename, width, height, 3, img, width * 3);
	free(img);
}

void Image::SaveAsJPG(const char *filename) const
{
	unsigned char * img = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));
	int temp = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			img[temp++] = data[i][j].red > 255 ? 255 : data[i][j].red;
			img[temp++] = data[i][j].grn > 255 ? 255 : data[i][j].grn;
			img[temp++] = data[i][j].blu > 255 ? 255 : data[i][j].blu;
		}
	}

	stbi_write_jpg(filename, width, height, 3, img, 100);
	free(img);
}

void Image::Load_PNGorJPG(const char *filename)
{
	int w, h, c;
	unsigned char * image = stbi_load(filename, &w, &h, &c, 0);

	if (image == NULL)
	{
		printf("Error in loading PNG or JPG Image\n");
		exit(-1);
	}

	width = w;
	height = h;

	if (!(c == 1 || c == 3 || c == 4))
	{
		printf("Error, c is %d in loading PNG or JPG Image\n", c);
		exit(-1);
	}

	data = new Color*[height];

	int temp = 0;

	for (int i = 0; i < height; i++)
	{
		data[i] = new Color[width];

		for (int j = 0; j < width; j++)
		{
			if (c == 1)
			{
				data[i][j].red = (float)image[temp];
				data[i][j].grn = (float)image[temp];
				data[i][j].blu = (float)image[temp++];
				continue;
			}

			data[i][j].red = (float)image[temp++];
			data[i][j].grn = (float)image[temp++];
			data[i][j].blu = (float)image[temp++];
			if (c == 4) temp++;

		}
	}

	stbi_image_free(image);
}

void Image::Load_EXR(const char *filename)
{
	float* image;
	int w, h;
	int ret = LoadEXR(&image, &w, &h, filename, 0);

	if (ret != TINYEXR_SUCCESS)
	{
		printf("Error while loading EXR file\n");
		exit(-1);
	}

	width = w;
	height = h;

	data = new Color*[height];

	int temp = 0;

	for (int i = 0; i < height; i++)
	{
		data[i] = new Color[width];

		for (int j = 0; j < width; j++)
		{
			data[i][j].red = (float)image[temp++];
			data[i][j].grn = (float)image[temp++];
			data[i][j].blu = (float)image[temp++];
			image[temp++];
		}
	}

	free(image);
}