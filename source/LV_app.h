#ifndef LV_APP
#define LV_APP

#include "LV_window.h"
#include "LV_device.h"
#include "LV_pipeline.h"
#include "LV_swapchain.h"
#include <memory>

namespace LV {
	class App {
	public:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;

		App();
		~App();

		App(const App&) = delete;
		App& operator=(const App&) = delete;

		void run();
		void setupPipeline();
		void initPipelineLayout();
		void initCommandBuffers();
		void drawFrame();
	private:
		LV_window window{ WIDTH, HEIGHT, "LearnVulkan" };
		LVDevice device{ window };
		LVSwapChain swapchain{ device, window.windowExtent()};
		std::unique_ptr<LV_pipeline> pipeline;	
		VkPipelineLayout layout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}

#endif
