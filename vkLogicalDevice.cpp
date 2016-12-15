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

	VkPhysicalDevice        actualPhysicalDevice = physicalDevice.getPhysicalDevice();
	vKdevice::QueueFamilyIndices indices = physicalDevice.findQueueFamilies(actualPhysicalDevice,window);

	std::vector<const char*> validationLayers = layers.getValidationLayers();

	this->queueLogicalDeviceCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	this->queueLogicalDeviceCreateInfo.queueFamilyIndex = indices.graphicFamily;
	this->queueLogicalDeviceCreateInfo.queueCount       = 1;
	this->queueLogicalDeviceCreateInfo.pQueuePriorities = &this->queuePriority;

	this->createLogicalDeviceInfo.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	this->createLogicalDeviceInfo.pQueueCreateInfos     = &this->queueLogicalDeviceCreateInfo;
	this->createLogicalDeviceInfo.queueCreateInfoCount  = 1;
	this->createLogicalDeviceInfo.pEnabledFeatures      = &this->logicalDeviceFeatures;
	this->createLogicalDeviceInfo.enabledExtensionCount = 0;

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
};
