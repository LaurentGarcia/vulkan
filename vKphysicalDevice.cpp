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
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures   deviceFeatures;

	vkGetPhysicalDeviceProperties(device,&deviceProperties);
	vkGetPhysicalDeviceFeatures(device,&deviceFeatures);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
		printf("VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: OK \n");
		fflush(stdout);
	};
	if(deviceFeatures.geometryShader == 1){
		printf("Geometry Shader: OK \n");
		fflush(stdout);
	};

	if(indices.isComplete())
	{
		printf("Indices: OK \n");
		fflush(stdout);
	}else
	{
		printf("Indices not completed, Graphics family: %d, Present Family: %d \n", indices.graphicFamily,indices.presentFamily);
		fflush(stdout);
	};


	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		   deviceFeatures.geometryShader && indices.isComplete();

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
//ToDo Rating for selected the most valuable GPU
//	   Based in GPU features



