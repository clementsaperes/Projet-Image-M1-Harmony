#include "harmonization.hpp"

Harmonization::Harmonization(double lambda, double sigma)
    : lambda(lambda), sigma(sigma), tmpl(std::vector<double>{}, std::vector<double>{}) {}

void Harmonization::set_image(const std::string &path) {
    img_path = path;
    tmpl = Template(std::vector<double>{}, std::vector<double>{});
    tmpl.set_image(path);
}

void Harmonization::compute_best_template(double &angle, Template_format &format) {
    auto [fmt, a] = tmpl.bestTemplate();
    format = fmt;
    angle  = a;

    tmpl = Template(format);
    tmpl.set_image(img_path);
    tmpl.rotate(angle);
}

void Harmonization::new_template(double angle, Template_format fmt, double width) {
    Template t(fmt);
    t.set_image(img_path);
    t.rotate(angle);

    std::vector<double> scaled;
    scaled.reserve(t.get_nbSector());
    for (int i = 0; i < t.get_nbSector(); i++)
        scaled.push_back(t.get_widths(i) * width);
    t.setWidths(scaled);

    t.build_graph();
    tmpl = std::move(t);
}

void Harmonization::build_graph() { tmpl.build_graph(); }
void Harmonization::solve_graph() { tmpl.solve_graph(lambda); }

std::vector<Pixel> Harmonization::shift_hues() {
    return tmpl.shift_hues(sigma);
}

double Harmonization::get_lambda() const { return lambda; }
double Harmonization::get_sigma()  const { return sigma;  }

void Harmonization::set_lambda(double l) { lambda = l; }
void Harmonization::set_sigma(double s)  { sigma  = s; }