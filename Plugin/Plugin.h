// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the PLUGIN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// PLUGIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif


# include <iostream>

std::string g_name = "Plugin";

enum class Status {
	Success,
	Error
};

extern "C" struct PLUGIN_API Version {
	unsigned int major;
	unsigned int minor;
};

extern "C" PLUGIN_API constexpr Version getVersion();

extern "C" PLUGIN_API const std::string& getName();

extern "C" PLUGIN_API Status initialize();

extern "C" PLUGIN_API Status execute(void* args);

extern "C" PLUGIN_API void destroy();
