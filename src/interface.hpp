#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>
#include "template.hpp"

class Interface {
private:
    std::vector<std::string> images;
    int selected_img;
    int selected_algo = 0;
    float lambda;
    float sigma;
    float angle;
    Template_format fmt = Template_format::I;

public:
    Interface();

    void load_images(const std::string& folder);
    std::string get_img() const; // chemin de l'image sélectionnée
    

    void render(); // la partie ImGui

    double get_lambda() const;
    double get_sigma() const;
    double get_angle() const;
    Template_format get_fmt() const;
    void set_angle(double angle_);
    void set_fmt(Template_format fmt_);

    int get_algo() const;
    void set_algo(int algo);
};

#endif