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
	#include "vKwindow.h"
	#include "vKdevice.h"
	#include "vKlayers.h"
	#include "vKphysicalDevice.h"
#endif

const int WIDTH  = 800;
const int HEIGHT = 600;
char* windowName = {"Vulkan Window"};

int main(void){

	vKlayers vkLayers;
	vkLayers.initLayerSupport();
	vkLayers.printAvailableLayers();

	printf("Init Layers: %d, 1 = Succesfull\n", vkLayers.getLayersEnable());
	fflush(stdout);

	vKwindow window;
	window.initWindow(WIDTH,HEIGHT,windowName);

	vKdevice vkDevice;
	VkResult resultInitVulkan;

	resultInitVulkan = vkDevice.initVulkan(&vkLayers);
	printf("Init Vulkan: %d, 0 = Succesfull\n", resultInitVulkan);
	fflush(stdout);


	vKphysicalDevice physicalDevice;
	physicalDevice.pickPhysicalDevice(vkDevice.getInstance());

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		glfwPollEvents();

	};

	return 0;
}
