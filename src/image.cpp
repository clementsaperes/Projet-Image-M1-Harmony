#include "image.hpp"

// ereur
#include <stdexcept>

// extention
#include <algorithm>

// lecture & write
#include "../common/stb_image.h"
#include <fstream>

// histogramme
#include <cmath>
#include <cstdlib>

Image::Image(const std::string path) {
    auto dot = path.find_last_of('.');
    if (dot == std::string::npos)
        throw std::runtime_error("Pas d'extension");

    std::string ext = path.substr(dot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "ppm")
        read_ppm(path);
    else if (ext == "png" || ext == "jpg" || ext == "jpeg")
        read_stb(path);
    else
        throw std::runtime_error("Format non supporté");
}

Image::Image(const std::vector<unsigned char> &buffer, int width, int height) {
    this->width = width;
    this->height = height;
    this->nb_pixels = width * height;
    if (buffer.size() != this->nb_pixels * 3)
        throw std::runtime_error("Taille du buffer incorrecte");

    this->image_data.resize(this->nb_pixels);
    for (int i = 0; i < nb_pixels; ++i)
        this->image_data[i] =
            Pixel(buffer[3 * i + 0], buffer[3 * i + 1], buffer[3 * i + 2]);
}

// read
void Image::read_ppm(const std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Impossible d'ouvrir " + path);
    // format
    std::string p;
    file >> p;
    if (p != "P6")
        throw std::runtime_error("Format PPM invalide");
    // com
    char c;
    file >> std::ws;
    while (file.peek() == '#') {
        std::string comment;
        std::getline(file, comment);
    }
    // val
    int max_val;
    file >> this->width >> this->height >> max_val;
    file.get();

    if (max_val != 255)
        throw std::runtime_error("PPM non supporté (max_val != 255)");

    this->nb_pixels = this->width * this->height;
    this->image_data.resize(this->nb_pixels);

    // lecture
    std::vector<unsigned char> buffer(this->nb_pixels * 3);
    file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
    if (file.gcount() != static_cast<std::streamsize>(buffer.size()))
        throw std::runtime_error("Erreur lecture données PPM");

    for (int i = 0; i < this->nb_pixels; ++i)
        this->image_data[i] =
            Pixel(buffer[3 * i + 0], buffer[3 * i + 1], buffer[3 * i + 2]);
}

void Image::read_stb(const std::string path) {
    int n;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, 3);
    if (!data)
        throw std::runtime_error("Impossible de charger " + path);

    this->nb_pixels = this->width * this->height;
    this->image_data.resize(this->nb_pixels);

    for (int p = 0; p < this->nb_pixels; p++)
        this->image_data[p] =
            Pixel(data[3 * p + 0], data[3 * p + 1], data[3 * p + 2]);

    stbi_image_free(data);
}

// write
void Image::write_ppm(const std::string &path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Impossible d'ouvrir " + path);

    // P6
    file << "P6\n" << width << " " << height << "\n255\n";

    // Pixels
    for (int p = 0; p < nb_pixels; p++) {
        file.put(image_data[p].r);
        file.put(image_data[p].g);
        file.put(image_data[p].b);
    }
}

// getter
const int Image::get_width() const { return this->width; }
const int Image::get_height() const { return this->height; }
const int Image::get_nb_pixels() const { return this->nb_pixels; }
const std::vector<Pixel> &Image::get_img() const { return this->image_data; }

std::vector<Pixel>::const_iterator Image::begin() const {
    return this->image_data.begin();
}
std::vector<Pixel>::const_iterator Image::end() const {
    return this->image_data.end();
}

void Image::set_path(const std::string &path) {
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".ppm")
        read_ppm(path);
    else
        read_stb(path);
}