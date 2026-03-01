#include "template.h"
#include <stdexcept>
Template::Template(std::vector<float> c, std::vector<float> w)
{
    if (c.size() != w.size())
        throw std::runtime_error("Il doit y avoir autant de centre de secteur que de largeur de secteur !\n");

    centers.resize(c.size());
    widths.resize(w.size());

    for (int i=0 ; i<c.size() ; i++)
    {
        centers[i] = Template::congru(c[i]);
        widths[i] = Template::congru(w[i]);
    }
}


Template::Template(float c, float w)
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

// getters
const int Template::get_nbSector() const {return centers.size();}
const float Template::get_center(int n) const {return centers[n];}
const float Template::get_widths(int n) const {return widths[n];}
const std::vector<float> Template::get_center() const {return centers;}
const std::vector<float> Template::get_widths() const {return widths;}


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