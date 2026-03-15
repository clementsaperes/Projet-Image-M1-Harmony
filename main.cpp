#include "interface.hpp"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "interface.hpp"
#include "template.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void test_graphcut(const std::string& img_path, Template_format fmt, double lambda)
{
    printf("\n=== TEST GRAPH CUT ===\n");
    printf("Image  : %s\n", img_path.c_str());
    printf("Format : %d\n", (int)fmt);
    printf("Lambda : %.2f\n", lambda);

    Template tmpl(fmt);
    tmpl.set_image(img_path);
    const std::vector<Pixel> pixels = tmpl.get_img();

    int n = pixels.size();
    std::vector<double> theta1, theta2;
    std::vector<bool> is_fixed;
    std::vector<int> v;
    tmpl.compute_thetas(pixels, theta1, theta2, is_fixed, v);

    int n_fixed = 0;
    for (bool f : is_fixed) if (f) n_fixed++;
    printf("Pixels fixes (dans secteur) : %d / %d (%.1f%%)\n",
           n_fixed, n, 100.0 * n_fixed / n);

    double e_before = tmpl.compute_energie(lambda, v);
    printf("Energie avant graph cut : %.6f\n", e_before);

    tmpl.run_graphcut(pixels, theta1, theta2, is_fixed, lambda, v);

    int n_plus = 0, n_minus = 0;
    for (int idx = 0; idx < n; idx++)
    {
        if (v[idx] ==  1)
            n_plus++;
        if (v[idx] == -1)
            n_minus++;
    }
    printf("Labels apres graph cut : +1=%d  -1=%d\n", n_plus, n_minus);

    double e_after = tmpl.compute_energie(lambda, v);
    printf("Energie apres  graph cut : %.6f\n", e_after);

    if (e_after <= e_before)
        printf("Energie reduite de %.6f\n", e_before - e_after);
    else
        printf("Energie n'a pas diminue (diff=%.6f)\n", e_after - e_before);
}

GLuint load_texture(const std::string& path, int& width, int& height)
{
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 4);
    if (!data)
        return 0;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

int main()
{
    // glfw
    if (!glfwInit())
        return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Image Harmony", NULL, NULL);
    if (!window)
    { 
        glfwTerminate(); 
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // glew
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    { 
        printf("Failed GLEW\n"); 
        return -1;
    }

    // imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    Interface iface;
    iface.load_images("../assets/img");
    test_graphcut("../assets/img/Gnar.png", Template_format::V, 2.0);

    GLuint current_tex = 0;
    int tex_w = 0, tex_h = 0;
    std::string last_image;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        iface.render();

        std::string img_path = iface.get_img();
        if (img_path != last_image)
        {
            if (current_tex)
                glDeleteTextures(1, &current_tex);
            current_tex = load_texture(img_path, tex_w, tex_h);
            last_image = img_path;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (current_tex)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, current_tex);

            glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(-1, -1);
            glTexCoord2f(1, 1); glVertex2f(1, -1);
            glTexCoord2f(1, 0); glVertex2f(1, 1);
            glTexCoord2f(0, 0); glVertex2f(-1, 1);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    if (current_tex)
        glDeleteTextures(1, &current_tex);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}