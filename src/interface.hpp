#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "template.hpp"
#include <string>
#include <vector>

class Interface {
public:
    Interface();

    void load_images(const std::string &folder);
    void render();

    std::string get_img() const;
    int get_algo() const;

    // Color harmonization
    double get_lambda() const;
    double get_sigma() const;
    double get_angle() const;
    double get_width() const;
    Template_format get_fmt() const;

    void set_algo(int algo);
    void set_angle(double angle);
    void set_fmt(Template_format fmt);
    void set_width();   // remet width à 1.0

    // Mosaïque
    double get_lambda_2() const;
    double get_sigma_2() const;
    int    get_bloc_size() const;

private:
    std::vector<std::string> images;
    int selected_img  = 0;
    int selected_algo = 0;

    // Color harmonization
    float angle  = 0.0f;
    float width  = 1.0f;
    float lambda = 1.0f;
    float sigma  = 0.5f;
    Template_format fmt = Template_format::I;

    // Mosaïque
    float lambda_2  = 1.0f;
    float sigma_2   = 0.5f;
    int   bloc_size = 8;
};

#endif