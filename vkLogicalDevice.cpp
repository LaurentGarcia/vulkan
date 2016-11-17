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

bool vkLogicalDevice::createLogicalDevice(vKphysicalDevice psDevice,vKlayers layers){

	vKphysicalDevice::QueueFamilyIndices indices = psDevice.findQueueFamilies(psDevice.getPhysicalDevice());


	this->queueCreateInfo.sType 		   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	this->queueCreateInfo.queueFamilyIndex = indices.graphicFamily;
	this->queueCreateInfo.queueCount       = 1;

	// Queue priority, you must setup it and it defines the influence of every Queue in the scheduling of command buffer
	const float queuePriority = 1.0f;
	this->queueCreateInfo.pQueuePriorities = &queuePriority;

	this->createInfo.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	this->createInfo.pQueueCreateInfos     = &this->queueCreateInfo;
	this->createInfo.queueCreateInfoCount  = 1;
	this->createInfo.pEnabledFeatures      = &this->deviceFeatures;
	this->createInfo.enabledExtensionCount = 0;

	if (enableValidationLayers){
		this->createInfo.enabledLayerCount       = layers.getValidationLayers().size();
		this->createInfo.ppEnabledLayerNames = layers.getValidationLayers().data();
	}

	else{
		this->createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(psDevice.getPhysicalDevice(),&this->createInfo,nullptr,this->device.replace())!= VK_SUCCESS){
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(this->device,indices.graphicFamily,0,&this->graphicsQueue);

	return 1;
};

const VkDevice* vkLogicalDevice::getLogicalDevice(){
	return this->device.operator &();
}
