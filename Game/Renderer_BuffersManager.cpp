#include "Renderer.h"

void Renderer::updateUniformBuffer(uint32_t currentImage, ModelN* n) {
    UniformBufferObject ubo{};

    ubo.model = n->mRot;
    if (n->scale != 1.0f)
        ubo.model = glm::scale(ubo.model, glm::vec3(n->scale));
    ubo.view = glm::lookAt(eng.getPlayer()->getPlCam()->getCameraFast() + n->mPos, n->mPos, glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(89.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 60.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(device, n->uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, n->uniformBuffersMemory[currentImage]);
}

void Renderer::updateUniformBufferPlayer(uint32_t currentImage, ModelN* n){
    UniformBufferObject ubo{};

    glm::vec3 camVec = eng.getPlayer()->getPlCam()->getBasicCam() * eng.getPlayer()->getPlCam()->getScale();
    camVec = glm::rotate(camVec, glm::radians(eng.getPlayer()->getPlCam()->getVerAngle()), glm::vec3(1.0f, 0.0f, 0.0f));

    ubo.model = n->mRot;
    ubo.view = glm::lookAt(camVec + n->mPos, n->mPos, glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(89.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 60.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(device, n->uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, n->uniformBuffersMemory[currentImage]);
}

void Renderer::updateUniformBufferCharacter(uint32_t currentImage, Character* c) {
    UniformBufferObject ubo{};

    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.00000f, -0.000001f, 10.0f) + glm::vec3(-c->posX, -c->posY, 0.0f), glm::vec3(-c->posX, -c->posY, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(5.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 20.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(device, c->uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, c->uniformBuffersMemory[currentImage]);
}

void Renderer::recreateBuffers() {
    vkQueueWaitIdle(graphicsQueue);
    //vkDeviceWaitIdle(device);
    createIndexBuffer();
    createCommandBuffers(unUsedCommandPool);
    usedCommandPool = (usedCommandPool + 1) % 2;
    unUsedCommandPool = (unUsedCommandPool + 1) % 2;
    if (usedCommandPool == 0) {
        vkFreeCommandBuffers(device, commandPool1, static_cast<uint32_t>(commandBuffers1.size()), commandBuffers1.data());
        vkResetCommandPool(device, commandPool1, NULL);
    }
    else {
        vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        vkResetCommandPool(device, commandPool, NULL);
    }
    toJoin = false;
}

void Renderer::createIndexBuffer() {
    std::vector<uint32_t> indices;
    /*
    ModelN* n = eng.getPlayer()->getModelN();
    n->indicesOffset = indices.size();
    indices.insert(indices.end(), n->indices.begin(), n->indices.end());

    for (auto& n : eng.getModels()) {
        n.indicesOffset = indices.size();
        indices.insert(indices.end(), n.indices.begin(), n.indices.end());
    }
    */
    for (int32_t i = 0; i < eng.getTrianglesDatas().size(); i++) {
        TrianglesData* td = &eng.getTrianglesDatasP()->at(i);
        td->indicesOffset = indices.size();
        indices.insert(indices.end(), td->indices.begin(), td->indices.end());
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, indexBufferMemory, nullptr);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize, buffsPool);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    if (vkBindBufferMemory(device, buffer, bufferMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind buffer memory!");
    }
}

VkCommandBuffer Renderer::beginSingleTimeCommands(VkCommandPool whichPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = whichPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool whichPool) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    //buffsBuffers.push_back(&commandBuffer);

    mtxSh.lock();
    //vkQueueWaitIdle(graphicsQueue);
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
    vkFreeCommandBuffers(device, whichPool, 1, &commandBuffer);
    mtxSh.unlock();
}

void Renderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool whichPool) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(whichPool);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer, whichPool);
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Renderer::createCommandBuffers(int8_t whichPool) {
    if (whichPool == 0) {
        commandBuffers.resize(swapChainFramebuffers.size());
    }
    else {
        commandBuffers1.resize(swapChainFramebuffers.size());
    }


    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    if (whichPool == 0) {
        allocInfo.commandPool = commandPool;
    }
    else {
        allocInfo.commandPool = commandPool1;
    }
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (whichPool == 0) {
        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChainExtent;

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkDeviceSize offsets[] = { 0 };
            
            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
            for (ModelN j : eng.getModels()) {                
                vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &eng.getTrianglesDatasP()->at(j.trianglesData).vertexBuffer, offsets);
                vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &j.descriptorSets[i], 0, nullptr);
                vkCmdDrawIndexed(commandBuffers[i], eng.getTrianglesDatasP()->at(j.trianglesData).indicesSize, 1, eng.getTrianglesDatasP()->at(j.trianglesData).indicesOffset, 0, 0);
            }

            if (eng.getDrawPlayer() == true) {
                vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).vertexBuffer, offsets);
                vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &eng.getPlayer()->getModelN()->descriptorSets[i], 0, nullptr);
                vkCmdDrawIndexed(commandBuffers[i], eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).indicesSize, 1, eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).indicesOffset, 0, 0);
            }

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, textGraphicsPipeline);
            for (Word& w : eng.getTextClass()->getWords()) {
                if (w.added == true) {
                    for (Character& c : w.characters) {
                        if (c.txtData.size > 0) {
                            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &c.vertexBuffer, offsets);
                            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, textPipelineLayout, 0, 1, &c.descriptorSets[i], 0, nullptr);
                            vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(c.vertices.size()), 1, 0, 0);
                        }
                    }
                }
            }

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }
    else {
        if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers1.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers1.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers1[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChainExtent;

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers1[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffers1[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

            VkDeviceSize offsets[] = { 0 };            
            
            vkCmdBindIndexBuffer(commandBuffers1[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
            for (ModelN j : eng.getModels()) {                
                vkCmdBindVertexBuffers(commandBuffers1[i], 0, 1, &eng.getTrianglesDatasP()->at(j.trianglesData).vertexBuffer, offsets);
                vkCmdBindDescriptorSets(commandBuffers1[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &j.descriptorSets[i], 0, nullptr);
                vkCmdDrawIndexed(commandBuffers1[i], eng.getTrianglesDatasP()->at(j.trianglesData).indicesSize, 1, eng.getTrianglesDatasP()->at(j.trianglesData).indicesOffset, 0, 0);
            }

            if (eng.getDrawPlayer() == true) {
                vkCmdBindVertexBuffers(commandBuffers1[i], 0, 1, &eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).vertexBuffer, offsets);
                vkCmdBindDescriptorSets(commandBuffers1[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &eng.getPlayer()->getModelN()->descriptorSets[i], 0, nullptr);
                vkCmdDrawIndexed(commandBuffers1[i], eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).indicesSize, 1, eng.getTrianglesDatasP()->at(eng.getPlayer()->getModelN()->trianglesData).indicesOffset, 0, 0);
            }

            vkCmdBindPipeline(commandBuffers1[i], VK_PIPELINE_BIND_POINT_GRAPHICS, textGraphicsPipeline);            
            for (Word& w : eng.getTextClass()->getWords()) {
                if (w.added == true) {
                    for (Character& c : w.characters) {
                        if (c.txtData.size > 0) {
                            vkCmdBindVertexBuffers(commandBuffers1[i], 0, 1, &c.vertexBuffer, offsets);
                            vkCmdBindDescriptorSets(commandBuffers1[i], VK_PIPELINE_BIND_POINT_GRAPHICS, textPipelineLayout, 0, 1, &c.descriptorSets[i], 0, nullptr);
                            vkCmdDraw(commandBuffers1[i], static_cast<uint32_t>(c.vertices.size()), 1, 0, 0);
                        }
                    }
                }                
            }

            vkCmdEndRenderPass(commandBuffers1[i]);

            if (vkEndCommandBuffer(commandBuffers1[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }
}

