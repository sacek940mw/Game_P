#pragma once
#include "vulkan/vulkan.h"
#include <queue>
#include <cstring>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct ModelInfo {
	std::string modelPath;
	std::string texturePath;
	float x = 0.0f, y = 0.0f, z = 0.0f;
    int32_t xx = 0, yy = 0, zz = 0;
    float xR = 0.0f, yR = 0.0f, zR = 0.0f;
    uint32_t id = 0;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct ModelN {
    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
    VkImageView textureImageView = VK_NULL_HANDLE;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    std::vector<uint32_t> indices;
    uint32_t indicesOffset = 0;
    uint32_t indicesSize = 0;
    std::vector<VkBuffer> uniformBuffers = std::vector<VkBuffer>(VK_NULL_HANDLE);
    std::vector<VkDeviceMemory> uniformBuffersMemory = std::vector<VkDeviceMemory>(VK_NULL_HANDLE);
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets = std::vector<VkDescriptorSet>(VK_NULL_HANDLE);

    int32_t xx=0, yy=0, zz=0;
    glm::vec3 mPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 mRot = glm::mat4(1.0f);
    uint32_t id = 0;
};

class ModelManager
{
public:
    void rotateModel(glm::mat4* mRot, float xR, float yR, float zR);
    void rotateModelX(glm::mat4* mRot, float xR);
    void rotateModelY(glm::mat4* mRot, float yR);
    void rotateModelZ(glm::mat4* mRot, float zR);
    void setModelRotation(glm::mat4* mRot, float xR, float yR, float zR);
    void resetModelRotation(glm::mat4* mRot);

    void translateModel(glm::vec3* mPos, float x, float y, float z);
    void translateModel(glm::vec3* mPos, glm::vec3 vec);

};

