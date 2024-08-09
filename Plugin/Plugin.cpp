// Plugin.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Plugin.h"

extern "C" PLUGIN_API constexpr Version getVersion() {
	return Version{ 0, 1 };
}

extern "C" PLUGIN_API const std::string& getName() {
	return g_name;
}

extern "C" PLUGIN_API Status initialize() {
	std::cout << "Initializing ..." << std::endl;
	return Status::Success;
}

extern "C" PLUGIN_API Status execute(void* args) {
	std::cout << "Executing ..." << std::endl;
	return Status::Success;
}

extern "C" PLUGIN_API void destroy() {
	std::cout << "Destroying ..." << std::endl;
}