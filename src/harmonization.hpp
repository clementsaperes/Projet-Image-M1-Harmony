#ifndef HARMONIZATION_HPP
#define HARMONIZATION_HPP

#include "image.hpp"
#include "template.hpp"
#include <string>
#include <vector>

class Harmonization {
  private:
    Template_format format;
    double angle;
    double lambda;
    double sigma;
    std::string img_path;
    Template tmpl;

  public:
    Harmonization(double lambda = 50.0, double sigma = 0.5);

    void compute_best_template(double &angle, Template_format &format);
    double compute_best_angle(Template_format fmt);
    void compute_labels();
    std::vector<Pixel> shift_hues();
    std::vector<Pixel> shift_hues2();
    void new_template(double angle, Template_format fmt, double width);

    double get_lambda() const;
    double get_sigma() const;
    int get_format() const;
    double get_angle() const;

    void set_lambda(double l);
    void set_sigma(double s);
    void set_image(const std::string &path);

    void build_graph();
    void solve_graph();
};

#endif