/*
 * vKphysicalDevice.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */


#include "vKDeviceExtension.h"



vKDeviceExtension::vKDeviceExtension() {
	// TODO Auto-generated constructor stub
}

vKDeviceExtension::~vKDeviceExtension() {
	// TODO Auto-generated destructor stub
}


void vKDeviceExtension::enumerateDevices(const VkInstance* vkDevice){

	uint32_t deviceCount = 0;\
	vkEnumeratePhysicalDevices(*vkDevice,&deviceCount,nullptr);

	if (deviceCount==0){
		throw std::runtime_error ("failed to find GPU with Vulkan Support");
	}
	this->availablePhysicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(*vkDevice,&deviceCount,this->availablePhysicalDevices.data());
}

bool vKDeviceExtension::isDeviceSuitable(VkPhysicalDevice device,vKwindow* window) {

	vKdevice::QueueFamilyIndices indices = findQueueFamilies(device,window);

	bool extensionsSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate   = false;

	if(extensionsSupported){
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device,window);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;

}

void vKDeviceExtension::pickPhysicalDevice(const VkInstance* vkDevice,vKwindow* window){

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


vKdevice::QueueFamilyIndices vKDeviceExtension::findQueueFamilies(VkPhysicalDevice device,vKwindow* window){


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

VkPhysicalDevice vKDeviceExtension::getPhysicalDevice(){
	return this->physicalDevice;

};

bool vKDeviceExtension::checkDeviceExtensionSupport(VkPhysicalDevice device){

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

const std::vector<const char*> vKDeviceExtension::getDeviceExtensions(){
	return this->deviceExtensions;
};


vKDeviceExtension::SwapChainSupportDetails vKDeviceExtension::querySwapChainSupport(VkPhysicalDevice device,vKwindow* window){

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

void vKDeviceExtension::createSwapChain(VkPhysicalDevice device,vKwindow* window,const VkSurfaceKHR* surface){

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

	if (vkCreateSwapchainKHR(logicalDevice, &this->createChainInfo, nullptr, swapChain.replace()) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create swap chain!");
	}else{
		printf("Init SwapChain for GPU: OK \n");fflush(stdout);
	}

	// now Init the swapChainImages to render

	vkGetSwapchainImagesKHR(logicalDevice,swapChain,&imageCount,nullptr);
	this->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice,swapChain,&imageCount,swapChainImages.data());

	this->swapChainImageFormat = surfaceFormat.format;
	this->swapChainExtent      = extent;


};

void vKDeviceExtension::createLogicalDevice(vKDeviceExtension physicalDevice,vKlayers layers,vKwindow* window){

	VkPhysicalDevice               actualPhysicalDevice  = physicalDevice.getPhysicalDevice();
	vKdevice::QueueFamilyIndices   indices               = physicalDevice.findQueueFamilies(actualPhysicalDevice,window);
	const std::vector<const char*> physicalDevExtensions = physicalDevice.getDeviceExtensions();

	std::vector<const char*> validationLayers = layers.getValidationLayers();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> 						 uniqueQueueFamilies = {indices.graphicFamily,indices.presentFamily};

	for (int queueFamily : uniqueQueueFamilies){
		VkDeviceQueueCreateInfo  queueCreateInfo = {};
		queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount       = 1;
		queueCreateInfo.pQueuePriorities = &this->queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	this->createLogicalDeviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	this->createLogicalDeviceInfo.pQueueCreateInfos       = queueCreateInfos.data();
	this->createLogicalDeviceInfo.queueCreateInfoCount    = (uint32_t)queueCreateInfos.size();
	this->createLogicalDeviceInfo.pEnabledFeatures        = &this->logicalDeviceFeatures;
	this->createLogicalDeviceInfo.enabledExtensionCount   = physicalDevExtensions.size();
	this->createLogicalDeviceInfo.ppEnabledExtensionNames = physicalDevExtensions.data();

	if (enableValidationLayers){
		this->createLogicalDeviceInfo.enabledLayerCount   = validationLayers.size();
		this->createLogicalDeviceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else{
		this->createLogicalDeviceInfo.enabledLayerCount = 0;
	};

	if (vkCreateDevice(actualPhysicalDevice,&this->createLogicalDeviceInfo,nullptr,this->logicalDevice.replace())!=VK_SUCCESS){
	    throw std::runtime_error("failed to create logical device!");
	}
	else{
		printf("Vulkan Logical Device:  0K \n");
	}

	vkGetDeviceQueue(this->logicalDevice,indices.graphicFamily,0,&this->graphicQueue);
	vkGetDeviceQueue(this->logicalDevice,indices.presentFamily,0,&this->presentQueue);
};


void vKDeviceExtension::createImageViews(){

	this->swapChainImageViews.resize(swapChainImages.size(),VDeleter<VkImageView>{this->logicalDevice,vkDestroyImageView});

	for (uint32_t i = 0;( i < this->swapChainImages.size());i++){
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image        = this->swapChainImages[i];
		imageViewInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format       = this->swapChainImageFormat;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel   = 0;
		imageViewInfo.subresourceRange.levelCount     = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount     = 1;

		if (vkCreateImageView(this->logicalDevice,&imageViewInfo,nullptr,swapChainImageViews[i].replace()) != VK_SUCCESS ){
		    throw std::runtime_error("failed to create image views!");
		}
		else{
			printf("Image Views: %d, Ok \n",i);fflush(stdout);
		}
	};
}
//ToDo Rating for selected the most valuable GPU
//	   Based in GPU features



