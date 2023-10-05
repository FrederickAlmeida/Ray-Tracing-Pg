#ifndef IMAGE_HPP
#define IMAGE_HPP

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "point and vector.hpp"
#include <iostream>
#include <vector>

class Image {
public:
    Image(const char* image_path) {
        int width, height, channels;
        image_data = stbi_load(image_path, &width, &height, &channels, STBI_rgb);

        if (!image_data) {
            std::cerr << "Failed to load image." << std::endl;
            return;
        }

        this->width = width;
        this->height = height;
    }

    ~Image() {
        if (image_data) {
            stbi_image_free(image_data);
        }
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    std::vector<unsigned char> getPixel(int x, int y) const {
        std::vector<unsigned char> pixel(3);
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int pixel_index = (y * width + x) * 3;
            pixel[0] = image_data[pixel_index];
            pixel[1] = image_data[pixel_index + 1];
            pixel[2] = image_data[pixel_index + 2];
        }
        return pixel;
    }

    vec3 getPixelColor(int x, int y) const {
        std::vector<unsigned char> pixel = getPixel(x, y);
        return vec3(pixel[0], pixel[1], pixel[2]);
    }

private:
    unsigned char* image_data = nullptr;
    int width = 0;
    int height = 0;
};

#endif