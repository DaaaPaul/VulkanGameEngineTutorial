#include "LV_window.h"
#include <stdexcept>

namespace LV {
	LV_window::LV_window(unsigned w, unsigned h, const char* n) : width{ w }, height{ h }, name{ n } {
		initWindow();
	}

	LV_window::~LV_window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LV_window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) throw std::runtime_error("Window surface creation failed");
	}

	void LV_window::processInput() const {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void LV_window::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	}
}