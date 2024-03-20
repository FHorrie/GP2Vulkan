#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

#include "classes/GP2Mesh.h"

using std::string;

class GP2Shader
{
public:
	GP2Shader(const VkDevice& vkDevice, const string& vertexShaderFile, const string& fragmentShaderFile);
	~GP2Shader();
	GP2Shader(const GP2Shader&)	= delete;
	GP2Shader(GP2Shader&&) = delete;
	GP2Shader& operator=(const GP2Shader&) = delete;
	GP2Shader& operator=(GP2Shader&&) = delete;

	void DestroyShaderModules(const VkDevice& vkDevice);
	VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();


	const inline VkPipelineShaderStageCreateInfo& GetVertexInfo() const		{ return m_VertexInfo; }
	const inline VkPipelineShaderStageCreateInfo& GetFragmentInfo() const	{ return m_FragmentInfo; }

private:
	VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo(const VkDevice& vkDevice);
	VkPipelineShaderStageCreateInfo CreateVertexShaderInfo(const VkDevice& vkDevice);
	VkShaderModule CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code);
	
	//!Initialize before Info vars!//
	string m_VertexShaderFile;
	string m_FragmentShaderFile;
	/////////////////////////////////

	VkPipelineShaderStageCreateInfo m_VertexInfo, m_FragmentInfo;
	VkDevice m_Device;

};
