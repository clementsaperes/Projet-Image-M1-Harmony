#include "interface.hpp"
#include "imgui.h"
#include <filesystem>

static const char *FORMAT_NAMES[] = {"i", "V", "L", "I", "T", "Y", "X", "t", "q"};
static constexpr int FORMAT_COUNT = 9;

Interface::Interface() {}

// ---------------------------------------------------------------------------
// Image loading
// ---------------------------------------------------------------------------
void Interface::load_images(const std::string &folder) {
    images.clear();
    for (const auto &entry : std::filesystem::directory_iterator(folder)) {
        if (!entry.is_regular_file())
            continue;
        const std::string path = entry.path().string();
        if (path.size() < 4)
            continue;
        const std::string ext = path.substr(path.size() - 4);
        if (ext == ".ppm" || ext == ".jpg" || ext == ".png")
            images.push_back(path);
    }
}

// ---------------------------------------------------------------------------
// ImGui rendering
// ---------------------------------------------------------------------------
void Interface::render() {
    ImGui::Begin("Interface Harmony");

    // --- Image picker ---
    ImGui::Text("Choisir une image :");
    if (!images.empty()) {
        if (ImGui::BeginCombo("Images", images[selected_img].c_str())) {
            for (int i = 0; i < (int)images.size(); i++) {
                const bool selected = (selected_img == i);
                if (ImGui::Selectable(images[i].c_str(), selected))
                    selected_img = i;
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // --- Algorithm picker ---
    ImGui::Separator();
    ImGui::Text("Choisir l'algorithme :");
    ImGui::RadioButton("Aucun algo",          &selected_algo, 0);
    ImGui::RadioButton("Color harmonization", &selected_algo, 1);

    ImGui::Indent();
    if (ImGui::CollapsingHeader("Parametres##harmo")) {
        ImGui::InputFloat("Angle", &angle, 0.01f, 100.0f, "%.3f");

        for (int i = 0; i < FORMAT_COUNT; i++) {
            if (ImGui::RadioButton(FORMAT_NAMES[i], static_cast<int>(fmt) == i))
                fmt = static_cast<Template_format>(i);
            if (i < FORMAT_COUNT - 1)
                ImGui::SameLine();
        }

        ImGui::SliderFloat("Width",  &width,  0.1f,   1.0f,   "%.1f");
        ImGui::SliderFloat("Lambda", &lambda, 0.1f, 100.0f,   "%.1f");
        ImGui::SliderFloat("Sigma",  &sigma,  0.1f,   1.0f,   "%.2f");
    }
    ImGui::Unindent();

    ImGui::RadioButton("Mosaïque", &selected_algo, 2);

    ImGui::Indent();
    if (ImGui::CollapsingHeader("Parametres##mosa")) {
        ImGui::Text("Bloc size");
        ImGui::SameLine();
        if (ImGui::RadioButton("4",  bloc_size == 4))  bloc_size = 4;
        ImGui::SameLine();
        if (ImGui::RadioButton("8",  bloc_size == 8))  bloc_size = 8;
        ImGui::SameLine();
        if (ImGui::RadioButton("16", bloc_size == 16)) bloc_size = 16;

        ImGui::SliderFloat("Lambda##mosa", &lambda_2, 0.1f, 100.0f, "%.1f");
        ImGui::SliderFloat("Sigma##mosa",  &sigma_2,  0.1f,   1.0f, "%.2f");
    }
    ImGui::Unindent();

    ImGui::RadioButton("Benchmark", &selected_algo, 3);

    ImGui::End();
}

// ---------------------------------------------------------------------------
// Getters / setters
// ---------------------------------------------------------------------------
std::string     Interface::get_img()       const { return images.empty() ? "" : images[selected_img]; }
int             Interface::get_algo()      const { return selected_algo; }

double          Interface::get_lambda()    const { return static_cast<double>(lambda);   }
double          Interface::get_sigma()     const { return static_cast<double>(sigma);    }
double          Interface::get_angle()     const { return static_cast<double>(angle);    }
double          Interface::get_width()     const { return static_cast<double>(width);    }
Template_format Interface::get_fmt()       const { return fmt;                           }

double          Interface::get_lambda_2()  const { return static_cast<double>(lambda_2); }
double          Interface::get_sigma_2()   const { return static_cast<double>(sigma_2);  }
int             Interface::get_bloc_size() const { return bloc_size;                     }

void Interface::set_algo(int algo)              { selected_algo = algo;              }
void Interface::set_angle(double a)             { angle = static_cast<float>(a);     }
void Interface::set_fmt(Template_format f)      { fmt   = f;                         }
void Interface::set_width()                     { width = 1.0f;                      }