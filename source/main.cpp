#include "LV_app.h"
#include "LV_pipeline.h"
#include <iostream>
#include <stdexcept>

int main() {
	try {
		LV::App app{};
		app.run();
	} catch (const std::exception e) {
		std::cerr << e.what();
	}

	return EXIT_SUCCESS;
}