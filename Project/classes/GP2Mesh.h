#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <vulkan/vulkan_core.h>

class GP2Mesh
{
public:
	struct Vertex final
	{
	public:
		glm::vec2 pos;
		glm::vec3 color;

		static const VkVertexInputBindingDescription BINDING_DESC;
		static const std::array<VkVertexInputAttributeDescription, 2> ATTRIBUTE_DESCS;
	};

	GP2Mesh(VkDevice device, std::vector<Vertex> verts, VkQueue graphicsQueue);
	~GP2Mesh();
	GP2Mesh(const GP2Mesh&) = delete;
	GP2Mesh(GP2Mesh&&) = delete;
	GP2Mesh& operator=(const GP2Mesh&) = delete;
	GP2Mesh& operator=(GP2Mesh&&) = delete;

	void Draw(const VkCommandBuffer& commandBuffer) const;

private:
	VkBuffer m_StagingBuffer;
	VkDeviceMemory m_StagingBufferMemory;
	
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;

	uint32_t m_NumVerts;

	VkDevice m_Device;
};