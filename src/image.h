#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <string>
#include <cmath>

const float TEMPLATE_DEFAULT_S_WIDTH = M_PI/6.0;
const float TEMPLATE_DEFAULT_M_WIDTH = M_PI/2.0;
const float TEMPLATE_DEFAULT_L_WIDTH = M_PI;
const float TEMPLATE_DEFAULT_CENTER = 0.0;

enum Template_format{i = 0,
                     V = 1,
                     L = 2,
                     I = 3,
                     T = 4,
                     Y = 5,
                     X = 6};

struct Pixel
{
    unsigned char r = 0, g = 0, b = 0;

    Pixel() = default;
    Pixel(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
};

class Template
{
    private :
    std::vector<float> centers;
    std::vector<float> widths;

    void autoCongru();

    public :
    Template(std::vector<float> c = {}, std::vector<float> w = {});
    Template(float c = 0, float w = 0);
    Template(Template_format format);

    int get_nbSector();
    float get_center(int n);
    float get_widths(int n);
    std::vector<float> get_center();
    std::vector<float> get_widths();

    void rotate(float angle);

    static float congru(float angle);
};

class Image 
{
    private :
        std::vector<Pixel> image_data;
        int width = 0;
        int height = 0;
        int nb_pixels = 0;
    
    public :
        Image(const std::string path);
        Image(const std::vector<unsigned char> &buffer, int width, int height);
        ~Image() = default;
        void read_ppm(const std::string path);
        void read_stb(const std::string path);
        void write_ppm(const std::string& path) const;

        double psnr(const Image& other) const;
        void histogram(const std::string name) const;

        int get_width() const; 
        int get_height() const;
        std::vector<Pixel> get_img() const;
};

#endif