#ifndef LV_PIPELINE
#define LV_PIPELINE

#include <vector>
#include <string>

namespace LV {
	class LV_pipeline {
	public:
		LV_pipeline(const std::string vertexPath, const std::string fragmentPath);

	private:
		void initPipeline(const std::string vertexPath, const std::string fragmentPath);
		static std::vector<char> readFile(const std::string filePath);
	};
}

#endif