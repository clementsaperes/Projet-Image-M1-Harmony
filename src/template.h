#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <vector>
#include <cmath>

const float TEMPLATE_DEFAULT_S_WIDTH = M_PI / 6.0f;
const float TEMPLATE_DEFAULT_M_WIDTH = M_PI / 2.0f;
const float TEMPLATE_DEFAULT_L_WIDTH = M_PI;
const float TEMPLATE_DEFAULT_CENTER = 0.0f;

enum Template_format{i = 0,
                     V = 1,
                     L = 2,
                     I = 3,
                     T = 4,
                     Y = 5,
                     X = 6};


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

        const int get_nbSector() const;
        const float get_center(int n) const;
        const float get_widths(int n) const;
        const std::vector<float> get_center() const;
        const std::vector<float> get_widths() const;

        void rotate(float angle);

        static float congru(float angle);
};

#endif