#include "Image.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

//std::vector<int> bucketCount;

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Pixel(uint8_t _r, uint8_t _g, uint8_t _b) {
        r = _r;
        g = _g;
        b = _b;
    }
};

struct PixelStack {
    int r;
    int g;
    int b;

    int total = 0;

    PixelStack(int _r, int _g, int _b) {
        r = _r;
        g = _g;
        b = _b;
        total++;
    }

    void add(int _r, int _g, int _b) {
        r += _r;
        g += _g;
        b += _b;
        total++;
    }

    Pixel getav() {
        return Pixel(r / total, g / total, b / total);
    }
};

std::vector<PixelStack> buckets;

float distancesqr(int x1, int y1, int z1, int x2, int y2, int z2) {
    float dist = (pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
    return dist;
}

void group(const Image &image, const std::vector<Pixel> &means, int paletteSize) {
    for (int i = 0; i < image.size; i += image.channels) {
        float dist = FLT_MAX;
        int index = -1;
        for (int j = 0; j < paletteSize; j++) {
            float newDist = distancesqr(image.data[i], image.data[i + 1], image.data[i + 2], means[j].r, means[j].g, means[j].b);
            if (newDist < dist) {
                dist = newDist;
                index = j;
            }
        }
        buckets[index].add(image.data[i], image.data[i + 1], image.data[i + 2]);
        //std::cout << index << std::endl;
    }
}

Pixel average_pixel_value(const std::vector<Pixel>& pixels) {
    int r = 0;
    int g = 0;
    int b = 0;
    for (auto pixel : pixels) {
        r += pixel.r;
        g += pixel.g;
        b += pixel.b;
    }
    if (pixels.size() != 0) {
        r /= pixels.size();
        g /= pixels.size();
        b /= pixels.size();
    }
    return Pixel(r, g, b);
}

int main()
{

    char filename[200];

    std::cout << "Enter Filename : ";
    std::cin.getline(filename, sizeof(filename));
    Image workImage(filename);

    if (!workImage.created) {
        std::cout << "Press enter key to end..." << std::endl;
        std::getchar();
        return 0;
    }

    int paletteSize = 10;
    std::cout << "Enter palette size : ";
    std::cin >> paletteSize;

    std::vector<Pixel> means;
    

    // Pick first means
    for (int i = 0; i < paletteSize; i++) {
        int index = (rand() % (workImage.w * workImage.h)) * 3;
        //std::cout << (int)workImage.data[index] << ", " << (int)workImage.data[index+1] << ", " << (int)workImage.data[index + 2] << std::endl;
        means.push_back(Pixel((int)workImage.data[index], (int)workImage.data[index + 1], (int)workImage.data[index + 2]));
    
        buckets.push_back(PixelStack((int)workImage.data[index], (int)workImage.data[index + 1], (int)workImage.data[index + 2]));
        //bucketCount.push_back(0);
    }

    for (int i = 0; i < 10; i++) {
        std::cout << (i+1) * 10 << "%" << std::endl;
        group(workImage, means, paletteSize);
        for (int j = 0; j < paletteSize; j++) {
            means[j] = buckets[j].getav();
            //buckets[j].clear();
            buckets[j] = PixelStack(means[j].r, means[j].g, means[j].b);
        }
    }

    for (int i = 0; i < paletteSize; i++) {
        std::cout << (int)means[i].r << ", " << (int)means[i].g << ", " << (int)means[i].b << std::endl;
    }

    Image outputImage(workImage.w, workImage.h + 30, workImage.channels);
    for(int i = 0; i < workImage.size; i++){
        outputImage.data[i] = workImage.data[i];
    }
    
    
    for (int y = 0; y < 30; y++) {
        int linestart = workImage.size + (y * workImage.w * workImage.channels);
        for (int x = linestart; x < linestart + outputImage.w * outputImage.channels; x += outputImage.channels) {
            outputImage.data[x] = means[means.size()-1].r;
            outputImage.data[x + 1] = means[means.size() - 1].g;
            outputImage.data[x + 2] = means[means.size() - 1].b;
        }
    }

    int width = ((outputImage.w ) / paletteSize);
    for (int y = 0; y < 30; y++) {
        int linestart = workImage.size + (y * workImage.w * workImage.channels);
        for (int meanIndex = 0; meanIndex < paletteSize; meanIndex++) {
            int offset = meanIndex * width;
            for (int j = linestart + offset * outputImage.channels; j < linestart + (offset + width) * outputImage.channels; j += outputImage.channels) {
                outputImage.data[j] = means[meanIndex].r;
                outputImage.data[j + 1] = means[meanIndex].g;
                outputImage.data[j + 2] = means[meanIndex].b;
            }
        }
        
    }
    

    outputImage.write("output.jpg");

    return 0;
}
