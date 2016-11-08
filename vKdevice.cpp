/*
 * vKdevice.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: lcarro
 */

#include "vKdevice.h"

vKdevice::vKdevice() {
	// TODO Auto-generated constructor stub

}

vKdevice::~vKdevice() {
	// TODO Auto-generated destructor stub
}

void vKdevice::fillVkInfo(){

	this->appVkInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	this->appVkInfo.pApplicationName   = "Hello Triangle";
	this->appVkInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	this->appVkInfo.pEngineName        = "No Engine";
	this->appVkInfo.engineVersion      = VK_MAKE_VERSION(1,0,0);
	this->appVkInfo.apiVersion		   = VK_API_VERSION_1_0;
}

void vKdevice::fillAppVkInfo(){

	this->createVkInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	this->createVkInfo.pApplicationInfo = &this->appVkInfo;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	this->createVkInfo.enabledExtensionCount   = glfwExtensionCount;
	this->createVkInfo.ppEnabledExtensionNames = glfwExtensions;
	this->createVkInfo.enabledLayerCount = 0;
	this->fillExtensionsProperties();
}
void vKdevice::fillAppVkInfo(vKlayers vklayersInfo){

	this->createVkInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	this->createVkInfo.pApplicationInfo = &this->appVkInfo;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	this->createVkInfo.enabledExtensionCount   = glfwExtensionCount;
	this->createVkInfo.ppEnabledExtensionNames = glfwExtensions;
	this->createVkInfo.enabledLayerCount =       vklayersInfo.getValidationLayers().size();
	this->createVkInfo.ppEnabledLayerNames =     vklayersInfo.getValidationLayers().data();
	this->fillExtensionsProperties();
}

void vKdevice::fillExtensionsProperties(){

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,nullptr);
	this->extensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,this->extensions.data());
	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions) {
	    std::cout << "\t" << extension.extensionName << std::endl;
	}
}



VkResult vKdevice::initVulkan(){

	this->fillVkInfo();
	this->fillAppVkInfo();
	return vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());

}

VkResult vKdevice::initVulkan(vKlayers vklayersInfo){

	if (vklayersInfo.getLayersEnable()){
		printf("Enabled layers.");
		fflush(stdout);
		this->fillVkInfo();
		this->fillAppVkInfo(vklayersInfo);
		return vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());
	}
	else
	{
		this->fillVkInfo();
		this->fillAppVkInfo();
		return vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());
	}
}

