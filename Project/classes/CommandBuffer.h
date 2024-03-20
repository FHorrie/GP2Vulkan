#pragma once
#include <vulkan/vulkan_core.h>

class CommandBuffer
{
public:
	CommandBuffer(const VkDevice& vkDevice, uint32_t queueFamilyIdx);
	CommandBuffer(const VkDevice& vkDevice);
	~CommandBuffer();
	CommandBuffer(const CommandBuffer& other) = delete;
	CommandBuffer(CommandBuffer&& other) = delete;
	CommandBuffer& operator=(const CommandBuffer& other) = delete;
	CommandBuffer& operator=(CommandBuffer&& other) = delete;
	
	void BeginRecord() const;
	void EndRecord() const;

	VkCommandBuffer GetVkCommandBuffer() const	{ return m_CommandBuffer; }
	VkCommandPool GetVkCommandPool() const		{ return COMMANDPOOL; }
	VkCommandBuffer* GetVkCommandBufferPtr()	{ return &m_CommandBuffer; }

private:
	static inline VkCommandPool COMMANDPOOL{};
	static inline int NUMBUFFERS{};

	VkCommandBuffer m_CommandBuffer{};

	VkDevice m_Device{};
};
