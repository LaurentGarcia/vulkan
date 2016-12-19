/*
 * vKphysicalDevice.h
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */

#ifndef VKDEVICEEXTENSION_H_
#define VKDEVICEEXTENSION_H_

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

class vKDeviceExtension : public vKdevice{
public:

	vKDeviceExtension();
	virtual ~vKDeviceExtension();

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
	void createLogicalDevice(vKDeviceExtension physicalDevice,vKlayers layers,vKwindow* window);
	void createImageViews();

private:

	const std::vector<const char*>       deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	VkPhysicalDevice                     physicalDevice = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice>        availablePhysicalDevices = {};

	void                    enumerateDevices           (const VkInstance* vkDevice);
	bool                    isDeviceSuitable           (VkPhysicalDevice device,vKwindow* window);
	bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport      (VkPhysicalDevice device, vKwindow* window);

	VkSwapchainCreateInfoKHR createChainInfo = {};

	//logicalDevice Features
	VkPhysicalDeviceFeatures logicalDeviceFeatures        = {};
	VkDeviceCreateInfo       createLogicalDeviceInfo      = {};
	float queuePriority = 1.0f;

	VDeleter<VkDevice>       logicalDevice{vkDestroyDevice};
	VDeleter<VkSwapchainKHR> swapChain    {logicalDevice, vkDestroySwapchainKHR};

	VkQueue            graphicQueue;
	VkQueue			   presentQueue;

	std::vector<VkImage> swapChainImages;
	VkFormat             swapChainImageFormat;
	VkExtent2D           swapChainExtent;

	std::vector<VDeleter<VkImageView>> swapChainImageViews;

};

#endif /* VKDEVICEEXTENSION_H_ */
