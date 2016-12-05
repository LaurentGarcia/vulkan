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

const VkInstance* vKdevice::getInstance(){
	return instance.operator &();
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
	//this->fillExtensionsProperties();
}
void vKdevice::fillAppVkInfo(vKlayers* vklayersInfo){

	this->createVkInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	this->createVkInfo.pApplicationInfo        = &this->appVkInfo;

	this->vulkanExtensions       = vklayersInfo->getRequiredExtensions();
	this->vulkanLayersAvailable  = vklayersInfo->getValidationLayers();

	//this->fillExtensionsProperties();

	this->createVkInfo.enabledExtensionCount   = this->vulkanExtensions.size();
	this->createVkInfo.ppEnabledExtensionNames = this->vulkanExtensions.data();

	if (vklayersInfo->ValidationLayersActivated()){
		this->createVkInfo.enabledLayerCount =       vulkanLayersAvailable.size();
		this->createVkInfo.ppEnabledLayerNames =     vulkanLayersAvailable.data();
	}
}

void vKdevice::fillExtensionsProperties(){

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,nullptr);
	this->instanceExtensionsProperties.resize(extensionCount);
	this->vulkanExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,this->instanceExtensionsProperties.data());
	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : instanceExtensionsProperties) {
	    std::cout << "\t" << extension.extensionName << std::endl;
	    this->vulkanExtensions.push_back(extension.extensionName);
	}
}

void vKdevice::setupDebugCallback(){

	this->createCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	this->createCallbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	this->createCallbackInfo.pfnCallback = vKlayers::debugCallback;

	if (vKlayers::CreateDebugReportCallbackEXT(this->instance,&this->createCallbackInfo,nullptr,this->callback.replace())!=VK_SUCCESS){
		throw std::runtime_error("failed to set up debug callback!");
	}
}

VkResult vKdevice::initVulkan(){

	this->fillVkInfo();
	this->fillAppVkInfo();

	return vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());

}

VkResult vKdevice::initVulkan(vKlayers* vklayersInfo){
	VkResult result;
	if (vklayersInfo->getLayersEnable()){
		printf("Init Vulkan with layers.");
		fflush(stdout);
		this->fillVkInfo();
		this->fillAppVkInfo(vklayersInfo);
		result= vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());
		if (result != VK_SUCCESS) {
		            throw std::runtime_error("failed to create instance!");
		};
		this->setupDebugCallback();
		return result;
	}
	else
	{
		printf("Init Vulkan without Layers");
		fflush(stdout);
		this->fillVkInfo();
		this->fillAppVkInfo();
		return vkCreateInstance(&this->createVkInfo,nullptr,this->instance.replace());
	}
}

