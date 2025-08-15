#include "LV_pipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace LV {
	LV_pipeline::LV_pipeline(LVDevice& d,
		const std::string vertexPath,
		const std::string fragmentPath,
		const PipelineConfigurationInfo& config) : device{ d } {
		initPipeline(vertexPath, fragmentPath, config);
	}

	LV_pipeline::~LV_pipeline() {
		vkDestroyShaderModule(device.device(), vertexShaderModule, nullptr);
		vkDestroyShaderModule(device.device(), fragmentShaderModule, nullptr);
		vkDestroyPipeline(device.device(), pipeline, nullptr);
	}

	void LV_pipeline::bind(VkCommandBuffer commandBuffer) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void LV_pipeline::defaultPipelineConfigurationInfo(PipelineConfigurationInfo& info, uint32_t w, const uint32_t h) {
		info.inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		info.inputAssembly.primitiveRestartEnable = VK_FALSE;

		info.viewport.x = 0.0f;
		info.viewport.y = 0.0f;
		info.viewport.width = static_cast<float>(w);
		info.viewport.height = static_cast<float>(h);
		info.viewport.minDepth = 0.0f;
		info.viewport.maxDepth = 1.0f;

		info.scissor.offset = { 0, 0 };
		info.scissor.extent = { w, h };

		info.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		info.viewportInfo.viewportCount = 1;
		info.viewportInfo.pViewports = &info.viewport;
		info.viewportInfo.scissorCount = 1;
		info.viewportInfo.pScissors = &info.scissor;

		info.rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.rasterization.depthClampEnable = VK_FALSE;
		info.rasterization.rasterizerDiscardEnable = VK_FALSE;
		info.rasterization.polygonMode = VK_POLYGON_MODE_FILL;
		info.rasterization.lineWidth = 1.0f;
		info.rasterization.cullMode = VK_CULL_MODE_NONE;
		info.rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.rasterization.depthBiasEnable = VK_FALSE;
		// Excluded optional shizz

		info.multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.multisample.sampleShadingEnable = VK_FALSE;
		info.multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		// Excluded optional shizz

		info.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		info.colorBlendAttachment.blendEnable = VK_FALSE;
		info.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		info.colorBlendInfo.logicOpEnable = VK_FALSE;
		info.colorBlendInfo.attachmentCount = 1;
		info.colorBlendInfo.pAttachments = &info.colorBlendAttachment;

		info.depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		info.depthStencil.depthTestEnable = VK_TRUE;
		info.depthStencil.depthWriteEnable = VK_TRUE;
		info.depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		info.depthStencil.depthBoundsTestEnable = VK_FALSE;
	}

	void LV_pipeline::initPipeline(const std::string vertexPath,
		const std::string fragmentPath,
		const PipelineConfigurationInfo& config) {
		std::vector<char> vertexShaderCode = readFile(vertexPath);
		std::vector<char> fragmentShaderCode = readFile(fragmentPath);
		initShaderModule(vertexShaderCode, &vertexShaderModule);
		initShaderModule(fragmentShaderCode, &fragmentShaderModule);

		VkPipelineShaderStageCreateInfo shaderStages[2]{};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertexShaderModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;
		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragmentShaderModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &config.inputAssembly;
		pipelineInfo.pViewportState = &config.viewportInfo;
		pipelineInfo.pRasterizationState = &config.rasterization;
		pipelineInfo.pMultisampleState = &config.multisample;
		pipelineInfo.pColorBlendState = &config.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &config.depthStencil;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = config.pipelineLayout;
		pipelineInfo.renderPass = config.renderPass;
		pipelineInfo.subpass = config.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if(vkCreateGraphicsPipelines(device.device(), nullptr, 1, &pipelineInfo, nullptr, &pipeline))
			throw std::runtime_error("Failed to create graphics pipeline");
	}

	void LV_pipeline::initShaderModule(const std::vector<char> code, VkShaderModule* module) {
		VkShaderModuleCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.device(), &info, nullptr, module) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module");
	}

	std::vector<char> LV_pipeline::readFile(const std::string filePath) {
		std::ifstream file{ filePath, std::ios::ate | std::ios::binary };
		if (!file.is_open()) throw std::runtime_error("Unable to open " + filePath);

		const uint32_t fileSize = file.tellg();
		std::vector<char> fileData(fileSize);
		file.seekg(0);
		file.read(fileData.data(), fileSize);

		file.close();
		return fileData;
	}
}