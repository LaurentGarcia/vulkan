/*
 * vkLogicalDevice.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: lcarro
 */

#include "vkLogicalDevice.h"

vkLogicalDevice::vkLogicalDevice() {
	// TODO Auto-generated constructor stub

}

vkLogicalDevice::~vkLogicalDevice() {
	// TODO Auto-generated destructor stub
}

void vkLogicalDevice::createLogicalDevice(vKphysicalDevice physicalDevice,vKlayers layers,vKwindow* window){

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


