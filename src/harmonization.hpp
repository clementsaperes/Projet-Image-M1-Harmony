#ifndef HARMONIZATION_HPP
#define HARMONIZATION_HPP

#include "image.hpp"
#include "template.hpp"
#include <string>
#include <vector>

class Harmonization {
public:
    Harmonization(double lambda = 1.0, double sigma = 0.5);

    void set_image(const std::string &path);

    void compute_best_template(double &angle, Template_format &format);
    void new_template(double angle, Template_format fmt, double width);

    void build_graph();
    void solve_graph();
    std::vector<Pixel> shift_hues();

    double get_lambda() const;
    double get_sigma() const;

    void set_lambda(double l);
    void set_sigma(double s);

private:
    double lambda;
    double sigma;
    std::string img_path;
    Template tmpl;
};

#endif