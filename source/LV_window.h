#ifndef LV_WINDOW
#define LV_WINDOW

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace LV {
	class LV_window {
	public:
		LV_window(uint32_t w, uint32_t h, const char* n);
		~LV_window();
		LV_window(const LV_window&) = delete;
		LV_window& operator=(const LV_window&) = delete;
		bool shouldClose() const { return glfwWindowShouldClose(window); }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;
		void processInput() const;
		VkExtent2D windowExtent() const { return { width, height }; };

	private:
		void initWindow();
		const uint32_t width;
		const uint32_t height;
		const char* name;
		GLFWwindow* window;
	};
}

#endif