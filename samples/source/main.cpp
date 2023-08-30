//
//  main.cpp
//
//  Created by Grigory Shabaganov on 23.07.2017.
//

#include <memory>
#include <utility>
#include <iostream>
#include <chrono>

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Config.h"
#include "GlfwContext.h"
#include "Scenes/PostprocessScene.h"
#include "Scenes/InstancingScene.h"

static std::unique_ptr<GlfwContext> context;

void onFramebufferSize(GLFWwindow *window, int width, int height)
{
    gmt::utils::logf("Windows resized: {}, {}", width, height);
    context->resize(width, height);
}

int main(int argc, const char * argv[])
{
    if (!glfwInit()) {
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    int width = mode->width / 2;
    int height = mode->height / 2;
    auto window = glfwCreateWindow(width, height, "Sandbox", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwGetFramebufferSize(window, &width, &height);
    context.reset(new GlfwContext{ width, height });

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(0);
    
    glfwSetFramebufferSizeCallback(window, onFramebufferSize);

#if !defined(__APPLE__)
    glewExperimental = GL_TRUE;

    auto err = glewInit();
    if (GLEW_OK != err) {
        gmt::utils::log("glewInit failed, error = {}", (const char *)glewGetErrorString(err));
        return EXIT_FAILURE;
    }
#endif

    {
        PostprocessScene scene{ context.get() };
        //InstancingScene scene{ context.get() };

        auto t = std::chrono::high_resolution_clock::now();
        int fps = 0;

        while (!glfwWindowShouldClose(window))
        {
            scene.update();

            scene.render();

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            fps += 1;
            
            const auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> dt = now - t;
            if (dt.count() > 1.0f) {
                t = now;
                std::cout << fps << std::endl;
                fps = 0;
            }
        }
    }

    glfwTerminate();
    return 0;
}
