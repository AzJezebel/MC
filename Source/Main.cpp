#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Trigo.h"
#include "InputManager.h"
#include "Inventory.h"
#include "Minecraft.h"
#include "UI.h"
#include <irrKlang.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"

GLFWwindow* CreateWindow(int windowWidth, int windowHeight);
void Exit(int exitCode);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void windowFocusCallback(GLFWwindow* window, int focused);

int main()
{
	int windowWidth = 1280;
	int windowHeight = 960;
	GLFWwindow* window = CreateWindow(windowWidth, windowHeight);

	const char* glsl_version = "#version 450";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//GUI gui(window);
	Minecraft::InputManager inputManager{ window, windowWidth, windowHeight };

	Minecraft::World world{ Trigo::DEGREE_TO_RADIAN * 45.f, (float)windowWidth / (float)windowHeight, 250.f };
	glfwSetWindowUserPointer(window, &world); // for FramebufferSizeCallback
	Minecraft::TextureManager textureManager;
	Minecraft::UI UI{ textureManager };
	Minecraft::Minecraft minecraft{ inputManager, world, UI };

	float last = (float)glfwGetTime();
	float current;
	float delta = 0.01666667f; // 60 fps

	world.GetSoundEngine()->play2D("assets/music/Wet_Hands.ogg", true);
	world.GetSoundEngine()->setSoundVolume(0.01f);
	while (!glfwWindowShouldClose(window)
		&& !minecraft.ShouldClose())
	{
		UI.UpdateGLFWWindowSize(window);

		current = (float)glfwGetTime();
#ifdef _DEBUG
		float newDelta = current - last;
		delta = (newDelta < 0.2) ? newDelta : delta;
		last = current;
#else // in release
		delta = current - last;
		last = current;
#endif // _DEBUG

		glfwPollEvents();
		inputManager.Update();

		minecraft.Update(delta);

		UI.Update(world);
		glfwSwapBuffers(window);
	}
	world.GetSoundEngine()->drop(); // delete engine
	Exit(EXIT_SUCCESS);
}

GLFWwindow* CreateWindow(int windowWidth, int windowHeight)
{
	if (!glfwInit()) // TODO: log and all log in this function
	{
		std::cout << "Fail to init glfw" << std::endl;
		Exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // _DEBUG

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Minecraft", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		Exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		Exit(EXIT_FAILURE);
	}

	return window;
}

void windowFocusCallback(GLFWwindow* window, int focused)
{
	if (focused == GLFW_FALSE)
	{
		((Minecraft::World*)glfwGetWindowUserPointer(window))->m_forcePause = true;
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Minecraft::InputManager::WindowResize(window, width, height);

	((Minecraft::World*)glfwGetWindowUserPointer(window))->GetCamera().UpdateAspectRatio((float)width / (float)height);
}

void Exit(int exitCode)
{
	glfwTerminate();
	exit(exitCode);
}