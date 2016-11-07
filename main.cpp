/*
 * main.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: lauren garcia, www.loganvfx.com
 *      Vulkan APP Resource and series of classes to help using Vulkan API
 *      Based in the tutorials online at: https://vulkan-tutorial.com
 *
 *      Resources to compile the code:
 *
 *      	GLFW       = https://github.com/glfw/glfw
 *      	VULKAN API = https://vulkan.lunarg.com/
 */


#ifdef __linux__
	#define GLFW_INCLUDE_VULKAN
    #include <stdio.h>
    #include <iostream>
	#include <glfw3.h>
	#include <stdexcept>
	#include <functional>
	#include <vector>
	#include <cstring>
	#include "VDeleter.h"
#endif

static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) {

	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}


const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
VkApplicationInfo appinfo = {};
VkInstanceCreateInfo createInstanceInfo = {};
unsigned int glfwExtensionCount = 0;
const char** glfwExtensions;
VDeleter<VkInstance> instance{vkDestroyInstance};
GLFWwindow* window;
VDeleter<VkDebugReportCallbackEXT> callback{instance, DestroyDebugReportCallbackEXT};
VkDebugReportCallbackCreateInfoEXT createCallbackInfo = {};

const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_core_validation"};


#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif






bool checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
	    bool layerFound = false;

	    for (const auto& layerProperties : availableLayers) {
	        if (strcmp(layerName, layerProperties.layerName) == 0) {
	            layerFound = true;
	            break;
	        }
	    }

	    if (!layerFound) {
	        return false;
	    }
	}

	return true;
}

std::vector<const char*> getRequiredExtensions()
{
	std::vector<const char*> extensions;

	unsigned int glfwExtensionsCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

	for (unsigned int i = 0 ; i < glfwExtensionsCount ; i++)
	{
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
	return extensions;
}

void setupDebugCallback() {

	if (!enableValidationLayers) return;
	else{
    createCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createCallbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createCallbackInfo.pfnCallback = debugCallback;

    if (CreateDebugReportCallbackEXT(instance, &createCallbackInfo, nullptr, callback.replace()) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
	}

}


void createInstance()
{

		auto extensions = getRequiredExtensions();
		createInstanceInfo.enabledExtensionCount = extensions.size();
		createInstanceInfo.ppEnabledExtensionNames = extensions.data();
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pApplicationName = "Hello Triangle";
		appinfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
		appinfo.pEngineName = "No engine";
		appinfo.engineVersion = VK_MAKE_VERSION(1,0,0);
		appinfo.apiVersion = VK_API_VERSION_1_0;
		createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInstanceInfo.pApplicationInfo = &appinfo;
		createInstanceInfo.enabledLayerCount = 0;
		createInstanceInfo.enabledLayerCount = validationLayers.size();
		createInstanceInfo.ppEnabledLayerNames = validationLayers.data();
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInstanceInfo.enabledExtensionCount = glfwExtensionCount;
		createInstanceInfo.ppEnabledExtensionNames = glfwExtensions;

		if (vkCreateInstance(&createInstanceInfo,nullptr,instance.replace())!=VK_SUCCESS)
		{
			printf("Vulkan instance NOT created properly");
			throw std::runtime_error("failed to create instance!");
		}
		VkResult result = vkCreateInstance(&createInstanceInfo,nullptr,instance.replace());
		if (result == VK_SUCCESS){
			printf("Vulkan instance created properly, %S", createInstanceInfo.ppEnabledLayerNames);

		}

}


int main(void){

	//test..
	bool validationSupport = false;

	//

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(512,512,"Vulkan",nullptr, nullptr);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	validationSupport = checkValidationLayerSupport();
	printf("Validation Layer support: %d \n", validationSupport);

	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions) {
	    std::cout << "\n" << extension.extensionName << std::endl;
	}


	createInstance();
	setupDebugCallback();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

	};

	return 0;
}
