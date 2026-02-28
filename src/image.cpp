#include "image.h"

// ereur
#include <stdexcept>

// extention
#include <algorithm>

// lecture & write
#define STB_IMAGE_IMPLEMENTATION
#include "stb.hpp"
#include <fstream>
#include <sstream>

// histogramme
#include <map>
#include <cstdlib> 


// TEMPLATE //

// constructeur
Template::Template(std::vector<float> c = {}, std::vector<float> w = {})
{
    if (c.size() != w.size()) throw std::runtime_error("Il doit y avoir autant de centre de secteur que de largeur de secteur !\n");
    centers.resize(c.size());
    widths.resize(w.size());
    for (int i=0 ; i<c.size() ; i++)
    {
        centers[i] = Template::congru(c[i]);
        widths[i] = Template::congru(w[i]);
    }
}
Template::Template(float c = TEMPLATE_DEFAULT_CENTER, float w = TEMPLATE_DEFAULT_S_WIDTH)
{
    centers = {Template::congru(c)};
    widths = {Template::congru(w)};
}
Template::Template(Template_format format) 
{
    switch (format)
    {
        case i :
        {
            centers = {TEMPLATE_DEFAULT_CENTER};
            widths = {TEMPLATE_DEFAULT_S_WIDTH};
            break;
        }
        case V :
        {
            centers = {TEMPLATE_DEFAULT_CENTER};
            widths = {TEMPLATE_DEFAULT_M_WIDTH};
            break;
        }
        case L :
        {
            centers = {TEMPLATE_DEFAULT_CENTER, TEMPLATE_DEFAULT_CENTER-M_PI/2.0};
            widths = {TEMPLATE_DEFAULT_S_WIDTH, TEMPLATE_DEFAULT_M_WIDTH};
            break;
        }
        case I :
        {
            centers = {TEMPLATE_DEFAULT_CENTER, TEMPLATE_DEFAULT_CENTER-M_PI};
            widths = {TEMPLATE_DEFAULT_S_WIDTH, TEMPLATE_DEFAULT_S_WIDTH};
            break;
        }
        case T :
        {
            centers = {TEMPLATE_DEFAULT_CENTER};
            widths = {TEMPLATE_DEFAULT_L_WIDTH};
            break;
        }
        case Y :
        {
            centers = {TEMPLATE_DEFAULT_CENTER, TEMPLATE_DEFAULT_CENTER-M_PI};
            widths = {TEMPLATE_DEFAULT_M_WIDTH, TEMPLATE_DEFAULT_S_WIDTH};
            break;
        }
        case X :
        {
            centers = {TEMPLATE_DEFAULT_CENTER, TEMPLATE_DEFAULT_CENTER-M_PI};
            widths = {TEMPLATE_DEFAULT_M_WIDTH, TEMPLATE_DEFAULT_M_WIDTH};
            break;
        }
    }
}

// accesseurs
int Template::get_nbSector() {return centers.size();}
float Template::get_center(int n) {return centers[n];}
float Template::get_widths(int n) {return widths[n];}
std::vector<float> Template::get_center() {return centers;}
std::vector<float> Template::get_widths() {return widths;}

float Template::congru(float angle)
{
    float pi2 = 2*M_PI;
    float rest = angle - float(int(angle/pi2))*pi2;
    return (rest>M_PI)*(rest-pi2) + (rest<=-M_PI)*(rest+pi2) + (rest<=M_PI && rest>-M_PI)*rest;
}

void Template::rotate(float angle)
{
    for (int i=0 ; i<centers.size() ; i++)
        centers[i] = Template::congru(centers[i]+angle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// IMAGE //


// constructeur
Image::Image(const std::string path)
{
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


Image::Image(const std::vector<unsigned char> &buffer, int width, int height)
{
    this->nb_pixels = width * height;
        if (buffer.size() != this->nb_pixels * 3)
            throw std::runtime_error("Taille du buffer incorrecte");
    
    this->image_data.resize(this->nb_pixels);
    for (int i = 0; i < nb_pixels; ++i)
    {
        this->image_data[i] = Pixel(buffer[3*i + 0],
                                    buffer[3*i + 1],
                                    buffer[3*i + 2]);
    }
}


// read
void Image::read_ppm(const std::string path)
{
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
    while (file.peek() == '#')
    {
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
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    if (file.gcount() != static_cast<std::streamsize>(buffer.size()))
        throw std::runtime_error("Erreur lecture données PPM");

    for (int i = 0; i < this->nb_pixels; ++i)
        this->image_data[i] = Pixel(buffer[3*i + 0],
                                    buffer[3*i + 1],
                                    buffer[3*i + 2]);
}


void Image::read_stb(const std::string path)
{
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 3);
    if (!data) throw std::runtime_error("Impossible de charger " + path);
    
    this->nb_pixels = this->width * this->height;
    this->image_data.resize(this->nb_pixels);

    for (int p = 0; p < this->nb_pixels; p++)
        this->image_data[p] = Pixel(data[3 * p + 0],
                                    data[3 * p + 1],
                                    data[3 * p + 2]);
    
    stbi_image_free(data);
}


// write
void Image::write_ppm(const std::string& path) const
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Impossible d'ouvrir " + path);

    // P6
    file << "P6\n" << width << " " << height << "\n255\n";

    // Pixels
    for (int p = 0; p < nb_pixels; p++)
    {
        file.put(image_data[p].r);
        file.put(image_data[p].g);
        file.put(image_data[p].b);
    }
}

// psnr
double Image::psnr(const Image& other) const
{
    if (this->width != other.width || this->height != other.height)
        throw std::runtime_error("Images de tailles différentes");

    double mse = 0.0; 
    for (int p = 0; p < nb_pixels; p++)
    {
        double dr = static_cast<double>(image_data[p].r) - other.image_data[p].r;
        double dg = static_cast<double>(image_data[p].g) - other.image_data[p].g;
        double db = static_cast<double>(image_data[p].b) - other.image_data[p].b;
        mse += (dr * dr + dg * dg + db * db) / 3.0;
    } 
    mse /= nb_pixels;
    if (mse == 0.0)
        return 100.0;
    return 10.0 * log10(255.0*255.0 / mse);
}

// histogramme
void Image::histogram(const std::string name) const
{
    std::map<unsigned char, int> histo_r, histo_g, histo_b;
    for (int i = 0; i < 256; i++)
    {
        histo_r[i] = 0;
        histo_g[i] = 0;
        histo_b[i] = 0;
    }

    for (const Pixel& p : image_data)
    {
        histo_r[p.r]++;
        histo_g[p.g]++;
        histo_b[p.b]++;
    }

    // csv
    std::string csv_file = "../assets/data/csv/" + name + ".csv";
    std::ofstream csv(csv_file);
    if (!csv) throw std::runtime_error("Impossible d'ouvrir " + csv_file);

    for (int i = 0; i < 256; ++i) {
        csv << i << " "
            << histo_r[i] << " "
            << histo_g[i] << " "
            << histo_b[i] << "\n";
    }
    csv.close();


    std::ofstream csv_r("../assets/data/csv/" + name + "_R.csv");
    std::ofstream csv_g("../assets/data/csv/" + name + "_G.csv");
    std::ofstream csv_b("../assets/data/csv/" + name + "_B.csv");
    if (!csv_r || !csv_g || !csv_b)
        throw std::runtime_error("Impossible d'ouvrir CSV séparés");
    for (int i = 0; i < 256; ++i) {
        csv_r << i << " " << histo_r[i] << "\n";
        csv_g << i << " " << histo_g[i] << "\n";
        csv_b << i << " " << histo_b[i] << "\n";
    }
    csv_r.close(); csv_g.close(); csv_b.close();

    // png
    std::string png_global = "../assets/data/histo/" + name + ".png";
    std::string png_r = "../assets/data/histo/" + name + "_R.png";
    std::string png_g = "../assets/data/histo/" + name + "_G.png";
    std::string png_b = "../assets/data/histo/" + name + "_B.png";

    // PNG global
    std::string cmd_global =
        "gnuplot -e \""
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'; "
        "set output '" + png_global + "'; "
        "set title 'Histogramme'; "
        "set xlabel 'Intensité'; "
        "set ylabel 'Nombre de pixels'; "
        "plot '" + csv_file + "' using 1:2 with lines lc rgb 'red' title 'R', "
        "'" + csv_file + "' using 1:3 with lines lc rgb 'green' title 'G', "
        "'" + csv_file + "' using 1:4 with lines lc rgb 'blue' title 'B'\"";

    if(std::system(cmd_global.c_str()) != 0)
        printf("Erreur lors de la génération de l'image avec Gnuplot\n");

    // PNG R
    std::string cmd_r =
        "gnuplot -e \""
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'; "
        "set output '" + png_r + "'; "
        "set title 'Histogramme R'; "
        "set xlabel 'Intensité'; "
        "set ylabel 'Nombre de pixels'; "
        "plot '" + csv_file + "' using 1:2 with lines lc rgb 'red' title 'R'\"";
    std::system(cmd_r.c_str());

    // PNG G
    std::string cmd_g =
        "gnuplot -e \""
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'; "
        "set output '" + png_g + "'; "
        "set title 'Histogramme G'; "
        "set xlabel 'Intensité'; "
        "set ylabel 'Nombre de pixels'; "
        "plot '" + csv_file + "' using 1:3 with lines lc rgb 'green' title 'G'\"";
    std::system(cmd_g.c_str());

    // PNG B
    std::string cmd_b =
        "gnuplot -e \""
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'; "
        "set output '" + png_b + "'; "
        "set title 'Histogramme B'; "
        "set xlabel 'Intensité'; "
        "set ylabel 'Nombre de pixels'; "
        "plot '" + csv_file + "' using 1:4 with lines lc rgb 'blue' title 'B'\"";
    std::system(cmd_b.c_str());
}


// getter
int Image::get_width() const
{
    return this->width;
}


int Image::get_height() const
{
    return this->height;
}


std::vector<Pixel> Image::get_img() const
{
    return this->image_data;
}
