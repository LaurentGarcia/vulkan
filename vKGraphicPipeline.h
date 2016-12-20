/*
 * vKGraphicPipeline.h
 *
 *  Created on: Dec 20, 2016
 *      Author: lcarro
 */

#ifndef VKGRAPHICPIPELINE_H_
#define VKGRAPHICPIPELINE_H_

#include "VDeleter.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

class vKGraphicPipeline {


public:
	vKGraphicPipeline();
	virtual ~vKGraphicPipeline();

	void                     createGraphicPipeline();
	const std::vector<char>  loadShaders          (const std::string& filename);
	void					 createShaderModule   (const std::vector<char>& code,VDeleter<VkShaderModule>& shaderModule);
private:
};

#endif /* VKGRAPHICPIPELINE_H_ */
