#include "Renderer.h"

void Renderer::run() {
    initWindow();
    initVulkan();
    prepare();
    mainLoop();
    cleanup();
}

void Renderer::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetKeyCallback(window, key_callback);
}

void Renderer::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createTextDescriptorSetLayout();
    createGraphicsPipeline();
    createTextGraphicsPipeline();
    createCommandPool();
    createDepthResources();
    createFramebuffers();
    createTextureSampler();
}

void Renderer::prepare() {
    eng.getTextClass()->loadCharacters();
    eng.getTextClass()->addWord("TEST", 400, 1.0f, 0.0f, 'L');
    addWord(&eng.getTextClass()->getWordsP()->back());
    addLetters();

    ModelInfo miTmp;
    miTmp.z = 2000;
    miTmp.modelPath = "models/qube.obj";
    miTmp.texturePath = "textures/texture.jpg";
    eng.getModelsbP()->push_back(addModelI(miTmp));

    ModelN m = addModelI(*eng.getPlayer()->getModelInfo());
    eng.getPlayer()->setModelN(m);

    toJoin = true;
    {
        createIndexBuffer(); createCommandBuffers(0); createCommandBuffers(1);
        createSyncObjects();
    }
    recreateSwapChain();
}

void Renderer::cleanupSwapChain() {
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);

    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    if (usedCommandPool == 0) {
        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }
    else {
        vkFreeCommandBuffers(device, commandPool1, static_cast<uint32_t>(commandBuffers1.size()), commandBuffers1.data());
    }

    vkDestroyPipeline(device, textGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, textPipelineLayout, nullptr);
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);

    //DESTROY UNIFORM BUFFERS:
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        for (ModelN j : eng.getModels()) {
            if (j.uniformBuffers.size() > 0) {
                vkDestroyBuffer(device, j.uniformBuffers[i], nullptr);
                vkFreeMemory(device, j.uniformBuffersMemory[i], nullptr);
            }            
        }
        if (eng.getPlayer()->getModelN()->uniformBuffers.size() > 0) {
            vkDestroyBuffer(device, eng.getPlayer()->getModelN()->uniformBuffers[i], nullptr);
            vkFreeMemory(device, eng.getPlayer()->getModelN()->uniformBuffersMemory[i], nullptr);
        }        
        for (Character& c : eng.getTextClass()->getCharacters()) {
            if (c.txtData.size > 0) {
                if (c.uniformBuffers.size() > 0) {
                    vkDestroyBuffer(device, c.uniformBuffers[i], nullptr);
                    vkFreeMemory(device, c.uniformBuffersMemory[i], nullptr);
                }
            }            
        }
        for (Word& w : eng.getTextClass()->getWords()) {
            for (Character& c : w.characters) {
                if (c.txtData.size > 0) {
                    if (c.uniformBuffers.size() > 0) {
                        vkDestroyBuffer(device, c.uniformBuffers[i], nullptr);
                        vkFreeMemory(device, c.uniformBuffersMemory[i], nullptr); 
                    }                                      
                }
            }
        }
    }

    //DESTROY DESCRIPTOR POOLS:
    for (ModelN j : eng.getModels()) {
        vkDestroyDescriptorPool(device, j.descriptorPool, nullptr);
    }
    vkDestroyDescriptorPool(device, eng.getPlayer()->getModelN()->descriptorPool, nullptr);
    for (Character& c : eng.getTextClass()->getCharacters()) {
        if (c.txtData.size > 0) {
            vkDestroyDescriptorPool(device, c.descriptorPool, nullptr);
        }        
    }
    for (Word& w : eng.getTextClass()->getWords()) {        
        for (Character& c : w.characters) {            
            if (c.txtData.size > 0) {
                vkDestroyDescriptorPool(device, c.descriptorPool, nullptr);                
            }
        }
    }
}

void Renderer::cleanup() {
    eng.getModelsP()->insert(eng.getModelsP()->end(), eng.getModelsbP()->begin(), eng.getModelsbP()->end());
    eng.getModelsbP()->clear();
    eng.getModelsP()->insert(eng.getModelsP()->end(), eng.getModelsDP()->begin(), eng.getModelsDP()->end());
    eng.getModelsDP()->clear();
    //eng.getModelsP()->push_back(*eng.getPlayer()->getModelN());
    cleanupSwapChain();

    vkDestroySampler(device, textureSampler, nullptr);

    for (ModelN j : eng.getModels()) {
        vkDestroyImageView(device, j.textureImageView, nullptr);
        vkDestroyImage(device, j.textureImage, nullptr);
        vkFreeMemory(device, j.textureImageMemory, nullptr);
        vkDestroyBuffer(device, j.vertexBuffer, nullptr);
        vkFreeMemory(device, j.vertexBufferMemory, nullptr);
    }
    vkDestroyImageView(device, eng.getPlayer()->getModelN()->textureImageView, nullptr);
    vkDestroyImage(device, eng.getPlayer()->getModelN()->textureImage, nullptr);
    vkFreeMemory(device, eng.getPlayer()->getModelN()->textureImageMemory, nullptr);
    vkDestroyBuffer(device, eng.getPlayer()->getModelN()->vertexBuffer, nullptr);
    vkFreeMemory(device, eng.getPlayer()->getModelN()->vertexBufferMemory, nullptr);
    for (Character& c : eng.getTextClass()->getCharacters()) {
        vkDestroyImage(device, c.txtData.textureImage, nullptr);
        vkDestroyImageView(device, c.txtData.textureImageView, nullptr);        
        vkFreeMemory(device, c.txtData.textureImageMemory, nullptr);
        vkDestroyBuffer(device, c.vertexBuffer, nullptr);
        vkFreeMemory(device, c.vertexBufferMemory, nullptr);
    }
    for (Word& w : eng.getTextClass()->getWords()) {
        for (Character& c : w.characters) {
            if (c.txtData.size > 0) {
                vkDestroyImage(device, c.txtData.textureImage, nullptr);
                vkDestroyImageView(device, c.txtData.textureImageView, nullptr);
                vkFreeMemory(device, c.txtData.textureImageMemory, nullptr);
                vkDestroyBuffer(device, c.vertexBuffer, nullptr);
                vkFreeMemory(device, c.vertexBufferMemory, nullptr);
            }
        }
    }

    vkDestroyDescriptorSetLayout(device, textDescriptorSetLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);
    vkDestroyCommandPool(device, commandPool1, nullptr);
    vkDestroyCommandPool(device, buffsPool, nullptr);

    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Renderer::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createTextGraphicsPipeline();
    createDepthResources();
    createFramebuffers();

    for (uint32_t i = 0; i < eng.getModels().size(); i++) {
        ModelN* n = &eng.getModelsP()->at(i);
        createUniformBuffers(n->uniformBuffers, n->uniformBuffersMemory);
        createDescriptorPool(n->descriptorPool);
        createDescriptorSets(n->descriptorPool, n->descriptorSets, n->uniformBuffers, n->textureImageView);
        for (int i = 0; i < n->uniformBuffers.size(); i++) {
            updateUniformBuffer(i, n);
        }
    }
    ModelN* n = eng.getPlayer()->getModelN();
    createUniformBuffers(n->uniformBuffers, n->uniformBuffersMemory);
    createDescriptorPool(n->descriptorPool);
    createDescriptorSets(n->descriptorPool, n->descriptorSets, n->uniformBuffers, n->textureImageView);
    for (int i = 0; i < n->uniformBuffers.size(); i++) {
        updateUniformBufferPlayer(i, n);
    }
    for (uint32_t i = 0; i < eng.getTextClass()->getCharactersP()->size(); i++) {
        Character* c = &eng.getTextClass()->getCharactersP()->at(i);
        if (c->txtData.size > 0) {
            createUniformBuffers(c->uniformBuffers, c->uniformBuffersMemory);
            createDescriptorPool(c->descriptorPool);
            createDescriptorSets(c->descriptorPool, c->descriptorSets, c->uniformBuffers, c->txtData.textureImageView);
            for (int i = 0; i < c->uniformBuffers.size(); i++) {
                updateUniformBufferCharacter(i, c);
            }
        }        
    }
    for(uint32_t i = 0; i < eng.getTextClass()->getWordsP()->size(); i++){
        Word* w = &eng.getTextClass()->getWordsP()->at(i);
        for (Character& c : w->characters) {
            if (c.txtData.size > 0) {
                createUniformBuffers(c.uniformBuffers, c.uniformBuffersMemory);
                createDescriptorPool(c.descriptorPool);
                createDescriptorSets(c.descriptorPool, c.descriptorSets, c.uniformBuffers, c.txtData.textureImageView);
                for (int i = 0; i < c.uniformBuffers.size(); i++) {
                    updateUniformBufferCharacter(i, &c);
                }
            }
        }
    }

    createCommandBuffers(0);
    createCommandBuffers(1);
}

void Renderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

/*
bool Api::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
*/

void Renderer::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Renderer::drawFrame(int8_t whichPool) {
    mtx.lock();
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    for (ModelN& j : *eng.getModelsP()) {
        updateUniformBuffer(imageIndex, &j);
    }
    updateUniformBufferPlayer(imageIndex, eng.getPlayer()->getModelN());
    for (Character& c : eng.getTextClass()->getCharacters()) {
        if (c.txtData.size > 0) {
            updateUniformBufferCharacter(imageIndex, &c);
        }        
    }

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    if (whichPool == 0) {
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
    }
    else {
        submitInfo.pCommandBuffers = &commandBuffers1[imageIndex];
    }

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }    

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    mtx.unlock();
}

void Renderer::addWord(Word* w) {
    float offset = w->posX / 100000.0f;
    for (int16_t i = 0; i < w->characters.size(); i++) {
        Character* c = &w->characters.at(i);
        c->posX += offset;
        c->posY -= (c->txtData.texHeight - c->Bearing.y) / (200000.0f);
        if (c->txtData.size > 0) {
            addLetter(c, w->fontSize);
        }
        offset += c->Advance / 11000000.0f;
    }
}

void Renderer::addLetters() {
    for (int16_t i = 0; i < eng.getTextClass()->getCharactersP()->size(); i++) {
        Character* c = &eng.getTextClass()->getCharactersP()->at(i);
        if (c->txtData.size > 0) {
            addLetter(c, 20);
        }
    }
}

void Renderer::addLetter(Character* c, float fontSize) {
    //std::cout << "Dane: " << (float)((float)c->txtData.texWidth/30) << "x" << (float)((float)c->txtData.texHeight/30) << std::endl;
    createTextureImageLetter(c);
    c->txtData.textureImageView = createTextureImageView(c->txtData.textureImage);
    c->vertices.clear();

    float size = (float)(200000.0f);
    float height = 9.8f;

    c->vertices.push_back({ { 0.0f, 0.0f, height}, eng.getCurrentColor() , { 0.0f, 1.0f } });
    c->vertices.push_back({ { (float)((float)c->txtData.texWidth / size), 0.0f, height}, eng.getCurrentColor() , { 1.0f, 1.0f } });
    c->vertices.push_back({ { (float)((float)c->txtData.texWidth / size), (float)((float)c->txtData.texHeight / size), height}, eng.getCurrentColor() , { 1.0f, 0.0f } });
    c->vertices.push_back({ { (float)((float)c->txtData.texWidth / size), (float)((float)c->txtData.texHeight / size), height}, eng.getCurrentColor() , { 1.0f, 0.0f } });
    c->vertices.push_back({ { 0.0f, (float)((float)c->txtData.texHeight / size), height}, eng.getCurrentColor() , { 0.0f, 0.0f } });
    c->vertices.push_back({ { 0.0f, 0.0f, height}, eng.getCurrentColor() , { 0.0f, 1.0f } });

    createVertexBuffer(c->vertices, c->vertexBuffer, c->vertexBufferMemory);
    createUniformBuffers(c->uniformBuffers, c->uniformBuffersMemory);
    createDescriptorPool(c->descriptorPool);
    createDescriptorSets(c->descriptorPool, c->descriptorSets, c->uniformBuffers, c->txtData.textureImageView);
    updateUniformBufferCharacter(0, c);
}

ModelN Renderer::addModelI(ModelInfo mi) {
    ModelN m;
    MultiModelManager* mmm = eng.getMultiModelManagerP();

    m.id = mi.id;
    m.xx = mi.xx;
    m.yy = mi.yy;
    m.zz = mi.zz;
      
    mmm->translateModel(&m.mPos, mi.x, mi.y, mi.z);
    mmm->rotateModel(&m.mRot, mi.xR, mi.yR, mi.zR);

    createTextureImage(mi.texturePath, &m);
    m.textureImageView = createTextureImageView(m.textureImage);

    loadModel(mi.modelPath, &m);
    createVertexBuffer(vertices, m.vertexBuffer, m.vertexBufferMemory);
    createUniformBuffers(m.uniformBuffers, m.uniformBuffersMemory);
    createDescriptorPool(m.descriptorPool);
    createDescriptorSets(m.descriptorPool, m.descriptorSets, m.uniformBuffers, m.textureImageView);
    for (int i = 0; i < m.uniformBuffers.size(); i++) {
        updateUniformBuffer(i, &m);
    }

    return m;
}

void Renderer::deleteModel(int j) {
    mtx1.lock();
    ModelN m = eng.getModelsP()->at(j);
    eng.getModelsP()->erase(eng.getModelsP()->begin() + j);
    eng.getModelsDP()->push_back(m);
    mtx1.unlock();    
}

void Renderer::deleteModelM(ModelN* t) {
    int i = 0;
    for (ModelN n : eng.getModels()) {
        if (t == &n) {
            deleteModel(i);
            break;
        }
        i++;
    }
}

void Renderer::addModels() {
        int32_t j = eng.getMultiModelManagerP()->getModelsToAddSize();
            for (int32_t i = 0; i < j; i++) {
                eng.getModelsbP()->push_back( addModelI(eng.getMultiModelManagerP()->getFirstModelInfo()) );
            }
        toJoin = true;
        modelsAdding = false;
        eng.getMultiModelManagerP()->clearModelsToAdd();
}

 VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

 void Renderer::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
     auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
     app->framebufferResized = true;
 }

 bool QueueFamilyIndices::isComplete() {
     return graphicsFamily.has_value() && presentFamily.has_value();
 }