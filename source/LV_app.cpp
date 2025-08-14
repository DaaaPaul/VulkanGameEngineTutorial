#include "LV_app.h"

namespace LV {
	void App::run() {
		while (!window.shouldClose()) {
			glfwPollEvents();
			window.processInput();
		}
	}
}