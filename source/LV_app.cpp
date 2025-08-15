#include "LV_app.h"
#include <stdexcept>

namespace LV {
	void App::run() {
		while (!window.shouldClose()) {
			glfwPollEvents();
			window.processInput();
			drawFrame();
		}

		vkDeviceWaitIdle(device.device());
	}

	App::App() {
		initPipelineLayout();
		setupPipeline();
		initCommandBuffers();
	}

	App::~App() {
		vkDestroyPipelineLayout(device.device(), layout, nullptr);
	}

	void App::setupPipeline() {
		PipelineConfigurationInfo configuration{};
		LV_pipeline::defaultPipelineConfigurationInfo(configuration, swapchain.width(), swapchain.height());
		configuration.renderPass = swapchain.getRenderPass();
		configuration.pipelineLayout = layout;
		pipeline = std::make_unique<LV_pipeline>(device, "source/shaders/simple.vert.spv", "source/shaders/simple.frag.spv", configuration);
	}

	void App::initPipelineLayout() {
		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 0;
		layoutInfo.pSetLayouts = nullptr;
		layoutInfo.pushConstantRangeCount = 0;
		layoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device.device(), &layoutInfo, nullptr, &layout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline layout");
	}

	void App::initCommandBuffers() {
		commandBuffers.resize(swapchain.imageCount());
		
		VkCommandBufferAllocateInfo commandBufferInfo{};
		commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferInfo.commandBufferCount = commandBuffers.size();
		commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferInfo.commandPool = device.getCommandPool();

		if(vkAllocateCommandBuffers(device.device(), &commandBufferInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to create command buffers");

		for (int i = 0; i < commandBuffers.size(); ++i) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			
			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) 
				throw std::runtime_error("Failed to create command buffer begin info");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = swapchain.getRenderPass();
			renderPassInfo.framebuffer = swapchain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapchain.getSwapChainExtent();

			VkClearValue clearValues[2]{};
			clearValues[0].color = { 0.1f, 0.3f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = 2;
			renderPassInfo.pClearValues = clearValues;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			pipeline->bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to end command buffer");
		}
	}

	void App::drawFrame() {
		uint32_t imageIndex;
		VkResult result = swapchain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire next swapchain image");

		result = swapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present swapchain image");
	}
}