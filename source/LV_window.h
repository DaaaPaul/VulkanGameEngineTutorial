#ifndef LV_WINDOW
#define LV_WINDOW

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace LV {
	class LV_window {
	public:
		LV_window(unsigned w, unsigned h, const char* n);
		~LV_window();
		LV_window(const LV_window& w) = delete;
		LV_window& operator=(const LV_window& w) = delete;
		bool shouldClose() const { return glfwWindowShouldClose(window); }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;
		void processInput() const;

	private:
		void initWindow();
		const unsigned width;
		const unsigned height;
		const char* name;
		GLFWwindow* window;
	};
}

#endif