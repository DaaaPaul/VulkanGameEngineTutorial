#include "LV_app.h"
#include "LV_pipeline.h"
#include <iostream>
#include <stdexcept>

int main() {
	try {
		LV::App app{};
		app.run();
		LV::LV_pipeline pipeline{ "source/shaders/simple.vert.spv", "source/shaders/simple.frag.spv" };
	} catch (const std::exception e) {
		std::cerr << e.what();
	}

	return EXIT_SUCCESS;
}