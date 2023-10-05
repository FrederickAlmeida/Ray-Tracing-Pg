#include "stb_image.h"
#include "image.hpp"
#include <iostream>


int main() {
    const char* image_path = "image.jpg"; 
    Image image(image_path);

    if (image.getWidth() == 0 || image.getHeight() == 0) {
        return 1;
    }

   

    std::cout << "Pixel at (100, 100): " << image.getPixelColor(40, 24).x() << std::endl;

    return 0;
}
