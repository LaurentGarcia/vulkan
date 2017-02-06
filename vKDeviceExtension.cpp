/*
 * vKphysicalDevice.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: lcarro
 */


#include "vKDeviceExtension.h"



vKDeviceExtension::vKDeviceExtension(vKwindow* window) {

	this->window = window;
	//glfwSetWindowSizeCallback(window->getWindow(),vKDeviceExtension::onWindowResized);
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

	for (uint32_t i = 0; i < this->swapChainImages.size();i++){
		VkImageViewCreateInfo createInfo = {};
		  	  	  	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		            createInfo.image = swapChainImages[i];
		            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		            createInfo.format = swapChainImageFormat;
		            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		            createInfo.subresourceRange.baseMipLevel = 0;
		            createInfo.subresourceRange.levelCount = 1;
		            createInfo.subresourceRange.baseArrayLayer = 0;
		            createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(this->logicalDevice,&createInfo,nullptr,swapChainImageViews[i].replace()) != VK_SUCCESS ){
		    throw std::runtime_error("failed to create image views!");
		}
		else{
			printf("Image Views: %d, Ok \n",i);fflush(stdout);
		}
	};
}

void vKDeviceExtension::createRenderPass(){

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format  = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	VkSubpassDependency dependency    = {};
	dependency.srcSubpass      		  = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass			  = 0;
	dependency.srcStageMask			  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask		  = 0;
	dependency.dstStageMask     	  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask		  = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassInfo.dependencyCount	  = 1;
	renderPassInfo.pDependencies	  = &dependency;

	if (vkCreateRenderPass(logicalDevice,&renderPassInfo,nullptr,renderPass.replace())!= VK_SUCCESS){
		throw std::runtime_error("Unable to create the render Pass");
	}
	else{
		printf("Vulkan Created Render Pass : OK\n");fflush(stdout);
	}

};

void vKDeviceExtension::createGraphicPipeline(){

	auto vertShaderCode = this->loadShaders("Shaders/vert.spv");
	auto fragShaderCode = this->loadShaders("Shaders/frag.spv");

	VDeleter<VkShaderModule> vertShaderModule{logicalDevice, vkDestroyShaderModule};
	VDeleter<VkShaderModule> fragShaderModule{logicalDevice, vkDestroyShaderModule};

	this->createShaderModule(vertShaderCode,vertShaderModule);
	this->createShaderModule(fragShaderCode,fragShaderModule);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName  = "main";
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount   = 0;
	vertexInputInfo.pVertexBindingDescriptions      = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions    = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset   = {0,0};
	scissor.extent   = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode  = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading    = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = 0;

	if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo,nullptr,pipelineLayout.replace())!= VK_SUCCESS){
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount         = 2;
	pipelineInfo.pStages            = shaderStages;
	pipelineInfo.pVertexInputState  = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState= &inputAssembly;
	pipelineInfo.pViewportState     = &viewportState;
	pipelineInfo.pRasterizationState= &rasterizer;
	pipelineInfo.pMultisampleState  = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState   = &colorBlending;
	pipelineInfo.pDynamicState      = nullptr;
	pipelineInfo.layout             = pipelineLayout;
	pipelineInfo.renderPass         = renderPass;
	pipelineInfo.subpass            = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex  = -1;

	if (vkCreateGraphicsPipelines(logicalDevice,VK_NULL_HANDLE,1,&pipelineInfo,nullptr,graphicsPipeline.replace()) != VK_SUCCESS){
	    throw std::runtime_error("failed to create graphics pipeline!");
	}else{
		printf("Vulkan Created Graphic Pipeline: OK\n");fflush(stdout);
	}
};


const std::vector<char> vKDeviceExtension::loadShaders(const std::string& filename){

	std::ifstream file(filename,std::ios::ate | std::ios::binary);
	if (!file.is_open()){
		throw std::runtime_error("Failed to open the Shaders code");
	}else{
		std::cout << "Loaded Shader: " << filename <<" \n";fflush(stdout);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(),fileSize);
	file.close();

	return buffer;

};

void vKDeviceExtension::createShaderModule(const std::vector<char>& code,VDeleter<VkShaderModule>& shaderModule){

	VkShaderModuleCreateInfo createShaderInfo = {};
	createShaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;


	createShaderInfo.codeSize = code.size();
	createShaderInfo.pCode    = (uint32_t*) code.data();

	if (vkCreateShaderModule(logicalDevice,&createShaderInfo,nullptr,shaderModule.replace()) != VK_SUCCESS){
		throw std::runtime_error("Failure creating Shader Module");
	}


};


void vKDeviceExtension::createFramebuffers(){
	swapChainFramebuffers.resize(swapChainImageViews.size(),VDeleter<VkFramebuffer>{logicalDevice, vkDestroyFramebuffer});

	for (size_t i = 0; i < swapChainImageViews.size(); i++){
		VkImageView attachments[] = {swapChainImageViews[i]};

		VkFramebufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass       = renderPass;
		frameBufferInfo.attachmentCount  = 1;
		frameBufferInfo.pAttachments     = attachments;
		frameBufferInfo.width            = swapChainExtent.width;
		frameBufferInfo.height           = swapChainExtent.height;
		frameBufferInfo.layers           = 1;

		if(vkCreateFramebuffer(logicalDevice,&frameBufferInfo,nullptr,swapChainFramebuffers[i].replace())!=VK_SUCCESS){
			throw std::runtime_error("failed to create framebuffer!");
		}
		else{
			printf("Creating FrameBuffers : %d OK \n", i);fflush(stdout);
		}
	};



};

void vKDeviceExtension::createCommandPool(VkPhysicalDevice device,vKwindow* window){

	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device,window);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicFamily;
	poolInfo.flags            = 0;

	if (vkCreateCommandPool(logicalDevice,&poolInfo,nullptr,commandPool.replace())!= VK_SUCCESS){
	    throw std::runtime_error("failed to create command pool!");
	}else{
		printf("Creating CommandPool : OK \n");fflush(stdout);
	}
};


void vKDeviceExtension::createCommandBuffers(){

	commandBuffers.resize(swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool        = commandPool;
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (u_int32_t) commandBuffers.size();

	if (vkAllocateCommandBuffers(logicalDevice,&allocInfo,commandBuffers.data())!=VK_SUCCESS){
	    throw std::runtime_error("failed to allocate command buffers!");
	}else{
		printf("Allocating Command Buffers : OK \n");fflush(stdout);
	}

	for (size_t i = 0; i < commandBuffers.size(); i++){
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType 			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags 			= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo  = nullptr;

		vkBeginCommandBuffer(commandBuffers[i],&beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType       			= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass  			= renderPass;
		renderPassInfo.framebuffer 			= swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset	= {0,0};
		renderPassInfo.renderArea.extent    = swapChainExtent;

		VkClearValue clearColor 		    = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount      = 1;
		renderPassInfo.pClearValues         = &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i],&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i],VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipeline);
		vkCmdDraw(commandBuffers[i],3,1,0,0);
		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i])!=VK_SUCCESS){
		    throw std::runtime_error("failed to record command buffer!");
		}
	}
};

void vKDeviceExtension::createSemaphores(){
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if ( vkCreateSemaphore(logicalDevice,&semaphoreInfo,nullptr,this->imageAvailableSemaphore.replace()) ||
		 vkCreateSemaphore(logicalDevice,&semaphoreInfo,nullptr,this->renderFinishedSemaphore.replace()) != VK_SUCCESS){
		throw std::runtime_error("Unable to Create Vulkan Semaphores \n");
	}else{
		printf("Initializing Vulkan Semaphores: OK\n");fflush(stdout);
	}
};
void vKDeviceExtension::drawFrame(){
	u_int32_t imageIndex;
	vkAcquireNextImageKHR(this->logicalDevice, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo           = {};
	submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[]      = {imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	submitInfo.waitSemaphoreCount     = 1;
	submitInfo.pWaitSemaphores        = waitSemaphores;
	submitInfo.pWaitDstStageMask      = waitStages;
	submitInfo.commandBufferCount     = 1;
	submitInfo.pCommandBuffers        = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[]    = {renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount   = 1;
	submitInfo.pSignalSemaphores      = signalSemaphores;

	if (vkQueueSubmit(graphicQueue,1,&submitInfo,VK_NULL_HANDLE) != VK_SUCCESS  ){
	    throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo 	  = {};
	presentInfo.sType 				  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount    = 1;
	presentInfo.pWaitSemaphores		  = signalSemaphores;

	VkSwapchainKHR swapChains[]		  = {swapChain};

	presentInfo.swapchainCount        = 1;
	presentInfo.pSwapchains			  = swapChains;
	presentInfo.pImageIndices         = &imageIndex;
	presentInfo.pResults		      = nullptr;

	VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result != VK_SUCCESS) {
	    throw std::runtime_error("failed to present swap chain image!");
	}
};


void vKDeviceExtension::deviceWaitIdle(){
	vkDeviceWaitIdle(logicalDevice);
};




vKwindow* vKDeviceExtension::getWindow(){
	return this->window;
};

//ToDo Rating for selected the most valuable GPU
//	   Based in GPU features










