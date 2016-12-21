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
	#include <fstream>
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



	vKdevice::QueueFamilyIndices   findQueueFamilies(VkPhysicalDevice device,vKwindow* window);
	VkPhysicalDevice   			   getPhysicalDevice();
	const std::vector<const char*> getDeviceExtensions();

	void pickPhysicalDevice    (const VkInstance* vkDevice,vKwindow* window);
	void createSwapChain       (VkPhysicalDevice device,vKwindow* window,const VkSurfaceKHR* surface);
	void createLogicalDevice   (vKDeviceExtension physicalDevice,vKlayers layers,vKwindow* window);
	void createImageViews();
	void createRenderPass();
	void createGraphicPipeline();
	void createFramebuffers();
	void createCommandPool     (VkPhysicalDevice device,vKwindow* window);
	void createCommandBuffers();

protected:

	VDeleter<VkDevice>         			 logicalDevice   {vkDestroyDevice};
	VDeleter<VkSwapchainKHR>   			 swapChain       {logicalDevice, vkDestroySwapchainKHR};
	VDeleter<VkPipelineLayout> 			 pipelineLayout  {logicalDevice, vkDestroyPipelineLayout};
	VDeleter<VkRenderPass>     			 renderPass      {logicalDevice, vkDestroyRenderPass};
	VDeleter<VkPipeline>                 graphicsPipeline{logicalDevice, vkDestroyPipeline};
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool>              commandPool     {logicalDevice, vkDestroyCommandPool};
	std::vector<VkCommandBuffer>         commandBuffers;

private:

	const std::vector<const char*>       deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	VkPhysicalDevice                     physicalDevice = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice>        availablePhysicalDevices = {};


	void                     enumerateDevices           (const VkInstance* vkDevice);
	bool                     isDeviceSuitable           (VkPhysicalDevice device,vKwindow* window);
	bool                     checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails  querySwapChainSupport      (VkPhysicalDevice device, vKwindow* window);
	const std::vector<char>  loadShaders                (const std::string& filename);
	void			         createShaderModule         (const std::vector<char>& code,
												         VDeleter<VkShaderModule>& shaderModule);

	VkQueue              			   graphicQueue;
	VkQueue			     			   presentQueue;
	std::vector<VkImage> 			   swapChainImages;
	VkFormat             			   swapChainImageFormat;
	VkExtent2D           			   swapChainExtent;
	std::vector<VDeleter<VkImageView>> swapChainImageViews;

	VkSwapchainCreateInfoKHR createChainInfo              = {};
	VkPhysicalDeviceFeatures logicalDeviceFeatures        = {};
	VkDeviceCreateInfo       createLogicalDeviceInfo      = {};
	float queuePriority = 1.0f;
};

#endif /* VKDEVICEEXTENSION_H_ */
