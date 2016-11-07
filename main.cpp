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
	#include "VDeleter.h"
#endif

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
VkApplicationInfo appinfo = {};
VkInstanceCreateInfo createinfo = {};
unsigned int glfwExtensionCount = 0;
const char** glfwExtensions;
VDeleter<VkInstance> instance{vkDestroyInstance};
GLFWwindow* window;

const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_core_validation"};


#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


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


void createInstance()
{

		auto extensions = getRequiredExtensions();
		createinfo.enabledExtensionCount = extensions.size();
		createinfo.ppEnabledExtensionNames = extensions.data();
		appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appinfo.pApplicationName = "Hello Triangle";
		appinfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
		appinfo.pEngineName = "No engine";
		appinfo.engineVersion = VK_MAKE_VERSION(1,0,0);
		appinfo.apiVersion = VK_API_VERSION_1_0;
		createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createinfo.pApplicationInfo = &appinfo;
		createinfo.enabledLayerCount = 0;
		createinfo.enabledLayerCount = validationLayers.size();
		createinfo.ppEnabledLayerNames = validationLayers.data();
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createinfo.enabledExtensionCount = glfwExtensionCount;
		createinfo.ppEnabledExtensionNames = glfwExtensions;

		if (vkCreateInstance(&createinfo,nullptr,instance.replace())!=VK_SUCCESS)
		{
			printf("Vulkan instance NOT created properly");
			throw std::runtime_error("failed to create instance!");
		}
		VkResult result = vkCreateInstance(&createinfo,nullptr,instance.replace());
		if (result == VK_SUCCESS){
			printf("Vulkan instance created properly, %S", createinfo.ppEnabledLayerNames);

		}

}



int main(void){

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(512,512,"Vulkan",nullptr, nullptr);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
	    std::cout << "\t" << extension.extensionName << std::endl;
	}
	createInstance();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

	};

	return 0;
}
