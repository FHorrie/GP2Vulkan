#pragma once
#include <vulkan/vulkan_core.h>

class CommandBuffer
{
public:
	void BeginRecord(const VkDevice& vkDevice) const;
	void EndRecord() const;

	CommandBuffer(const VkDevice& vkDevice, const VkCommandPool& commandPool);
	~CommandBuffer() = default;
	CommandBuffer(const CommandBuffer&) = delete;
	CommandBuffer(CommandBuffer&&) = delete;
	CommandBuffer& operator=(const CommandBuffer&) = delete;
	CommandBuffer& operator=(CommandBuffer&&) = delete;

	VkCommandBuffer GetVkCommandBuffer() { return m_CommandBuffer; }
	VkCommandBuffer* GetVkCommandBufferPtr() { return &m_CommandBuffer; }

private:
	VkCommandBuffer m_CommandBuffer{};

};
