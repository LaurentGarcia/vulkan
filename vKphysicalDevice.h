/*
 * vKphysicalDevice.h
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */

#ifndef VKPHYSICALDEVICE_H_
#define VKPHYSICALDEVICE_H_

#ifdef __linux__
	#define GLFW_INCLUDE_VULKAN
	#include <glfw3.h>
	#include "vKdevice.h"
	#include "vKwindow.h"
	#include <set>
	#include <vector>
	#include <algorithm>
#endif

const int WIDTH  = 800;
const int HEIGHT = 600;

class vKphysicalDevice : public vKdevice{
public:

	vKphysicalDevice();
	virtual ~vKphysicalDevice();

	struct SwapChainSupportDetails {
	    VkSurfaceCapabilitiesKHR capabilities;
	    std::vector<VkSurfaceFormatKHR> formats;
	    std::vector<VkPresentModeKHR> presentModes;
	};

	void pickPhysicalDevice(const VkInstance* vkDevice,vKwindow* window);

	vKdevice::QueueFamilyIndices   findQueueFamilies(VkPhysicalDevice device,vKwindow* window);
	VkPhysicalDevice   			   getPhysicalDevice();
	const std::vector<const char*> getDeviceExtensions();
	void createSwapChain(VkPhysicalDevice device,vKwindow* window,const VkSurfaceKHR* surface);

private:

	const std::vector<const char*>       deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	VkPhysicalDevice                     physicalDevice = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice>        availablePhysicalDevices = {};

	void                    enumerateDevices           (const VkInstance* vkDevice);
	bool                    isDeviceSuitable           (VkPhysicalDevice device,vKwindow* window);
	bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport      (VkPhysicalDevice device, vKwindow* window);

	VkSwapchainCreateInfoKHR createChainInfo = {};


};

#endif /* VKPHYSICALDEVICE_H_ */
