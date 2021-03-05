// C++
#include <string>
#include <iostream>
// GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

std::string programName = "GLFW window";
int windowWidth = 1200,
windowHeight = 800;
float backgroundR = 0.1f,
backgroundG = 0.3f,
backgroundB = 0.2f;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void teardown(GLFWwindow* window)
{
    if (window != NULL) { glfwDestroyWindow(window); }
    glfwTerminate();
}

int main(int argc, char* argv[])
{
    if (!glfwInit())
    {
        std::cerr << "[ERROR] Couldn't initialize GLFW\n";
        return -1;
    }
    else
    {
        std::cout << "[INFO] GLFW initialized\n";
    }

    // setup GLFW window

    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

    std::string glsl_version = "";
#ifdef __APPLE__
    // GL 3.3 + GLSL 330
    glsl_version = "#version 330";
    glfwWindowHint( // required on Mac OS
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
    );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
    // GL 3.3 + GLSL 330
    glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
    // GL 3.3 + GLSL 330
    glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    float highDPIscaleFactor = 1.0;
#ifdef _WIN32
    // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    if (xscale > 1 || yscale > 1)
    {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
    // to prevent 1200x800 from becoming 2400x1600
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        programName.c_str(),
        NULL,
        NULL
    );
    if (!window)
    {
        std::cerr << "[ERROR] Couldn't create a GLFW window\n";
        teardown(NULL);
        return -1;
    }
    // watch window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    // VSync
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "[ERROR] Couldn't initialize GLEW" << std::endl;
        teardown(window);
        return -1;
    }
    else
    {
        std::cout << "[INFO] GLEW initialized\n";
    }

    std::cout << "[INFO] OpenGL from glew "
        << GLEW_VERSION_MAJOR << "." << GLEW_VERSION_MINOR
        << std::endl;

    int actualWindowWidth, actualWindowHeight;
    glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
    glViewport(0, 0, actualWindowWidth, actualWindowHeight);

    glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);

    // --- rendering loop
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    teardown(window);

    return 0;
}
