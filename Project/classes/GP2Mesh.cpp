#include <stdexcept>
#include "GP2Mesh.h"
#include "vulkanbase/VulkanUtil.h"

const VkVertexInputBindingDescription GP2Mesh::Vertex::BINDING_DESC
{
	.binding = 0,
	.stride = sizeof(Vertex),
	.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
};

const std::array<VkVertexInputAttributeDescription, 2> GP2Mesh::Vertex::ATTRIBUTE_DESCS
{
	VkVertexInputAttributeDescription
	{
		.location = 0,
		.binding = 0,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = offsetof(Vertex, pos),
	},
	VkVertexInputAttributeDescription
	{
		.location = 1,
		.binding = 0,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = offsetof(Vertex, color),
	}
};

GP2Mesh::GP2Mesh(VkDevice device, std::vector<Vertex> verts, VkQueue graphicsQueue)
	: m_Device{ device }
	, m_NumVerts{ static_cast<uint32_t>(verts.size()) }
{
	if (verts.size() < 0)
	{
		throw std::runtime_error("No vertexes passed when constructing mesh");
	}

	VkDeviceSize deviceBufferSize = sizeof(verts[0]) * verts.size();

	std::tie(m_StagingBuffer, m_StagingBufferMemory) = VkUtils::CreateBuffer(
		device,
		deviceBufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		
	void* data;
	vkMapMemory(device, m_StagingBufferMemory, 0, deviceBufferSize, 0, &data);

	std::memcpy(data, verts.data(), static_cast<size_t>(deviceBufferSize));

	vkUnmapMemory(device, m_StagingBufferMemory);

	std::tie(m_VertexBuffer, m_VertexBufferMemory) = VkUtils::CreateBuffer(
		device,
		deviceBufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkUtils::CopyBuffer(m_Device, m_StagingBuffer, m_VertexBuffer, deviceBufferSize, graphicsQueue);
}

GP2Mesh::~GP2Mesh()
{
	vkDestroyBuffer(m_Device, m_VertexBuffer, nullptr);
	vkFreeMemory(m_Device, m_VertexBufferMemory, nullptr);

	vkDestroyBuffer(m_Device, m_StagingBuffer, nullptr);
	vkFreeMemory(m_Device, m_StagingBufferMemory, nullptr);
}

void GP2Mesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	VkBuffer vertexBuffers[] { m_VertexBuffer };
	VkDeviceSize offsets[] { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_NumVerts), 1, 0, 0);
}