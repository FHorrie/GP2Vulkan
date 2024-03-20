#include "VulkanBase.h"

void VulkanBase::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanBase::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(VkUtils::SCREENWIDTH, VkUtils::SCREENHEIGHT, "2DAE09 - Horrie Finian - BULKAN", nullptr, nullptr);
}

void VulkanBase::InitVulkan()
{
	// week 06
	createInstance();
	setupDebugMessenger();
	CreateSurface();

	// week 04 
	createSwapChain();
	createImageViews();
	
	// week 05
	pickPhysicalDevice();
	createLogicalDevice();

	// week 03
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFrameBuffers();

	// week 02
	auto queueFamilyIndices{ VkUtils::FindQueueFamilies(VkUtils::physicalDevice, m_Surface) };
	m_pCommandBuffer = std::make_unique<CommandBuffer>(device, queueFamilyIndices.graphicsFamily.value());
	CreateVertexBuffer();

	m_pMesh = std::make_unique<GP2Mesh>(device, vertices, graphicsQueue);

	// week 06
	createSyncObjects();
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();
		// week 06
		DrawFrame();
	}
	vkDeviceWaitIdle(device);
}


void VulkanBase::Cleanup()
{
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, inFlightFence, nullptr);

	m_pMesh.reset();
	m_pCommandBuffer.reset();
	m_pShader.reset();

	vkDestroyCommandPool(device, m_pCommandBuffer->GetVkCommandPool(), nullptr);
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (VkUtils::VALIDATIONLAYERS) {
		VkUtils::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroyDevice(device, nullptr);

	vkDestroySurfaceKHR(instance, m_Surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(m_Window);
	glfwTerminate();

	//cleanupSwapChain();

	vkDestroyBuffer(device, vertexBuffer, nullptr);
}

void VulkanBase::DrawScene()
{
	vkCmdDraw(m_pCommandBuffer->GetVkCommandBuffer(), 6, 1, 0, 0);
}

void VulkanBase::RecordCommandBuffer(uint32_t imageIndex)
{
	m_pCommandBuffer->BeginRecord();
	DrawFrame(imageIndex);
	m_pCommandBuffer->EndRecord();
}

void VulkanBase::DrawFrame(uint32_t imageIndex) 
{
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
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	vkCmdSetViewport(m_pCommandBuffer->GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(m_pCommandBuffer->GetVkCommandBuffer(), 0, 1, &scissor);

	//Draw the Scene
	DrawScene();////
	////////////////

	vkCmdEndRenderPass(m_pCommandBuffer->GetVkCommandBuffer());
}
