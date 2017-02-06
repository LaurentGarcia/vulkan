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
	#include "vKDeviceExtension.h"
#endif


char* windowName = {"Vulkan Window"};

int main(void){

	//1. Init Layers
	vKlayers vkLayers;
	vkLayers.initLayerSupport();
	vkLayers.printAvailableLayers();

	//2. Setup a Window
	vKwindow window;
	window.initWindow(WIDTH,HEIGHT,windowName);

	//3. Create a logical Vulkan Device
	vKdevice vkDevice;
	VkResult resultInitVulkan;
	resultInitVulkan = vkDevice.initVulkan(&vkLayers);
	printf("Init Vulkan: %d, 0 = Succesfull\n", resultInitVulkan);
	fflush(stdout);
	//4. Create SurfACE
	window.createSurface(*vkDevice.getInstance());
	//4. Found the most suitable GPU in our computer
	vKDeviceExtension computerDevice{&window};
	computerDevice.pickPhysicalDevice(vkDevice.getInstance(),&window);
	//5. Create Logical Device (Interface for our Physical Device and init Queues!
	computerDevice.createLogicalDevice(computerDevice,vkLayers,&window);
	//Creatubg Swap Chain
	computerDevice.createSwapChain(computerDevice.getPhysicalDevice(),&window,window.getSurface());
	computerDevice.createImageViews();

	//Graphic Pipeline
	computerDevice.createRenderPass();
	computerDevice.createGraphicPipeline();
	computerDevice.createFramebuffers();
	computerDevice.createCommandPool(computerDevice.getPhysicalDevice(),&window);
	computerDevice.createCommandBuffers();
	computerDevice.createSemaphores();

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		glfwPollEvents();
		computerDevice.drawFrame();
	};

	computerDevice.deviceWaitIdle();
	glfwDestroyWindow(window.getWindow());
	return 0;
}
