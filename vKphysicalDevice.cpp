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

bool vKphysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {

	vKphysicalDevice::QueueFamilyIndices indices = findQueueFamilies(device);
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures   deviceFeatures;

	vkGetPhysicalDeviceProperties(device,&deviceProperties);
	vkGetPhysicalDeviceFeatures(device,&deviceFeatures);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		   deviceFeatures.geometryShader && indices.isComplete();

}

void vKphysicalDevice::pickPhysicalDevice(const VkInstance* vkDevice){

	this->enumerateDevices(vkDevice);

	for (const auto& device: availablePhysicalDevices ){
		if(isDeviceSuitable(device))
		{
			this->physicalDevice = device;
			break;
		}
	}

	if (this->physicalDevice == VK_NULL_HANDLE){
		throw std::runtime_error("failed to find a Suitable GPU");
	}

}


vKphysicalDevice::QueueFamilyIndices vKphysicalDevice::findQueueFamilies(VkPhysicalDevice device){

	vKphysicalDevice::QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());

	int i = 0;
	for(const auto& queueFamily : queueFamilies){
		if (queueFamily.queueCount>0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
			indices.graphicFamily = i;
	}
	if (indices.isComplete()){
		break;
	}
	i++;
	}
	return indices;
}

VkPhysicalDevice vKphysicalDevice::getPhysicalDevice(){
	return this->physicalDevice;

};
//ToDo Rating for selected the most valuable GPU
//	   Based in GPU features



