#ifndef LV_APP
#define LV_APP

#include "LV_window.h"

namespace LV {
	class App {
	public:
		static constexpr unsigned WIDTH = 800;
		static constexpr unsigned HEIGHT = 600;

		void run();

	private:
		LV_window window{ WIDTH, HEIGHT, "LearnVulkan" };
	};
}

#endif
