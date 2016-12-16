/*
 * vKphysicalDevice.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */

#include "vKphysicalDevice.h"



vKphysicalDevice::vKphysicalDevice() {
	// TODO Auto-generated constructor stub
}

vKphysicalDevice::~vKphysicalDevice() {
	// TODO Auto-generated destructor stub
}


void vKphysicalDevice::enumerateDevices(const VkInstance* vkDevice){

	uint32_t deviceCount = 0;\
	vkEnumeratePhysicalDevices(*vkDevice,&deviceCount,nullptr);

	if (deviceCount==0){
		throw std::runtime_error ("failed to find GPU with Vulkan Support");
	}
	this->availablePhysicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(*vkDevice,&deviceCount,this->availablePhysicalDevices.data());
}

bool vKphysicalDevice::isDeviceSuitable(VkPhysicalDevice device,vKwindow* window) {

	vKdevice::QueueFamilyIndices indices = findQueueFamilies(device,window);

	bool extensionsSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate   = false;

	if(extensionsSupported){
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device,window);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;

}

void vKphysicalDevice::pickPhysicalDevice(const VkInstance* vkDevice,vKwindow* window){

	this->enumerateDevices(vkDevice);

	for (const auto& device: availablePhysicalDevices ){
		if(isDeviceSuitable(device,window))
		{
			printf("Physical GPU: OK \n");
			fflush(stdout);
			this->physicalDevice = device;
			break;
		}
	}

	if (this->physicalDevice == VK_NULL_HANDLE){
		throw std::runtime_error("failed to find a Suitable GPU");
	}

}


vKdevice::QueueFamilyIndices vKphysicalDevice::findQueueFamilies(VkPhysicalDevice device,vKwindow* window){


	vKdevice::QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());

	int i = 0;

	for(const auto& queueFamily : queueFamilies){
		if (queueFamily.queueCount>0 && queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT){
			indices.graphicFamily = i;
		}

		VkBool32 presentSupport = false;

		vkGetPhysicalDeviceSurfaceSupportKHR(device,i,*window->getSurface(),&presentSupport);

		printf("Device Surface present Support KHR: %d \n", presentSupport);fflush(stdout);

		if(queueFamily.queueCount > 0 && presentSupport){
			indices.presentFamily = i;
		}

		if (indices.isComplete()){
			printf("Indices completed: OK \n");fflush(stdout);
			break;
		}
		i++;
	}
	return indices;
}

VkPhysicalDevice vKphysicalDevice::getPhysicalDevice(){
	return this->physicalDevice;

};

bool vKphysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device){

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);

	vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,availableExtensions.data());

	std::set<std::string> requiredExtension(deviceExtensions.begin(),deviceExtensions.end());

	for (const auto& extension : availableExtensions){
		requiredExtension.erase(extension.extensionName);
	}

	return requiredExtension.empty(); //ToDo
};

const std::vector<const char*> vKphysicalDevice::getDeviceExtensions(){
	return this->deviceExtensions;
};


vKphysicalDevice::SwapChainSupportDetails vKphysicalDevice::querySwapChainSupport(VkPhysicalDevice device,vKwindow* window){

	const VkSurfaceKHR* surface = window->getSurface();
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,*surface,&details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device,*surface,&formatCount,nullptr);
	if(formatCount!=0){
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device,*surface,&formatCount,details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device,*surface,&presentModeCount,nullptr);
	if(presentModeCount!=0){
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device,*surface,&presentModeCount,details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){

	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED){
		return {VK_FORMAT_B8G8R8A8_UNORM,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}

	for(const auto& availableFormat : availableFormats){
		if(availableFormat.format== VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
			return availableFormat;
		}
	}

	return availableFormats[0];
};

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes){

	for (const auto& availablePresentMode : availablePresentModes){
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
		return capabilities.currentExtent;
	}else{
		VkExtent2D actualExtent = {WIDTH,HEIGHT};
		actualExtent.width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}

void vKphysicalDevice::createSwapChain(VkPhysicalDevice device,vKwindow* window,const VkSurfaceKHR* surface){

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, window);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR   presentMode   = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D         extent		 = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount +1;
	if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	this->createChainInfo.sType    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	this->createChainInfo.surface  = *surface;

	this->createChainInfo.minImageCount    = imageCount;
	this->createChainInfo.imageFormat      = surfaceFormat.format;
	this->createChainInfo.imageColorSpace  = surfaceFormat.colorSpace;
	this->createChainInfo.imageExtent      = extent;
	this->createChainInfo.imageArrayLayers = 1;
	this->createChainInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	vKdevice::QueueFamilyIndices indices = findQueueFamilies(device,window);

	uint32_t queueFamilyIndices[] = {(uint32_t)indices.graphicFamily,(uint32_t)indices.presentFamily};
	if (indices.graphicFamily != indices.presentFamily){
		this->createChainInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		this->createChainInfo.queueFamilyIndexCount = 2;
		this->createChainInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}else{
		this->createChainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		this->createChainInfo.queueFamilyIndexCount = 0;
		this->createChainInfo.pQueueFamilyIndices   = nullptr;
	}

	this->createChainInfo.preTransform    = swapChainSupport.capabilities.currentTransform;
	this->createChainInfo.compositeAlpha  = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	this->createChainInfo.presentMode     = presentMode;
	this->createChainInfo.clipped         = VK_TRUE;
	this->createChainInfo.oldSwapchain    = VK_NULL_HANDLE;





};


//ToDo Rating for selected the most valuable GPU
//	   Based in GPU features



