#ifndef LV_PIPELINE
#define LV_PIPELINE

#include <vector>
#include <string>
#include "LV_device.h"

namespace LV {
	struct PipelineConfigurationInfo {
		PipelineConfigurationInfo() = default;
		PipelineConfigurationInfo(const PipelineConfigurationInfo&) = delete;
		PipelineConfigurationInfo& operator=(const PipelineConfigurationInfo&) = delete;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssembly;
		VkPipelineRasterizationStateCreateInfo rasterization;
		VkPipelineMultisampleStateCreateInfo multisample;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencil;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LV_pipeline {
	public:
		LV_pipeline(LVDevice& d, 
			const std::string vertexPath, 
			const std::string fragmentPath, 
			const PipelineConfigurationInfo& config);
		~LV_pipeline();
		LV_pipeline(const LV_pipeline&) = delete;
		LV_pipeline& operator=(const LV_pipeline&) = delete;
		void bind(VkCommandBuffer commandBuffer);

		static void defaultPipelineConfigurationInfo(PipelineConfigurationInfo& info, uint32_t w, const uint32_t h);
	private:
		void initPipeline(const std::string vertexPath, 
			const std::string fragmentPath, 
			const PipelineConfigurationInfo& config);
		void initShaderModule(const std::vector<char> code, VkShaderModule* module);

		static std::vector<char> readFile(const std::string filePath);

		LVDevice& device;
		VkPipeline pipeline;
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}

#endif