#include "LV_pipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace LV {
	LV_pipeline::LV_pipeline(const std::string vertexPath, const std::string fragmentPath) {
		initPipeline(vertexPath, fragmentPath);
	}

	void LV_pipeline::initPipeline(const std::string vertexPath, const std::string fragmentPath) {
		std::cout << readFile(vertexPath).size() << '\n';
		std::cout << readFile(fragmentPath).size();
	}

	std::vector<char> LV_pipeline::readFile(const std::string filePath) {
		std::ifstream file{ filePath, std::ios::ate | std::ios::binary };
		if (!file.is_open()) throw std::runtime_error("Unable to open " + filePath);

		const unsigned fileSize = file.tellg();
		std::vector<char> fileData(fileSize);
		file.seekg(0);
		file.read(fileData.data(), fileSize);

		file.close();
		return fileData;
	}
}