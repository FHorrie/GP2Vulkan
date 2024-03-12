#include "vulkanbase/VulkanBase.h"


void VulkanBase::recordCommandBuffer(uint32_t imageIndex) {
	//VkCommandBufferBeginInfo beginInfo{};
	//beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags = 0; // Optional
	//beginInfo.pInheritanceInfo = nullptr; // Optional
	//
	//if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to begin recording command buffer!");
	//}

	m_pCommandBuffer->BeginRecord(device);
	DrawFrame(imageIndex);
	m_pCommandBuffer->EndRecord();
	
	//if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to record command buffer!");
	//}
}

void VulkanBase::DrawFrame(uint32_t imageIndex) {
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(m_pCommandBuffer->GetVkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(m_pCommandBuffer->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(m_pCommandBuffer->GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(m_pCommandBuffer->GetVkCommandBuffer(), 0, 1, &scissor);

	drawScene();
	vkCmdEndRenderPass(m_pCommandBuffer->GetVkCommandBuffer());
}