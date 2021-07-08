#pragma once
#include "Renderer_Structs.h"
#include "ModelManager.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Engine.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <map>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Renderer {
public:
    void run();

private:    
    std::mutex mtx;
    std::mutex mtx1;
    std::mutex mtxSh;
    bool joined = false;
    bool toJoin = false;
    //bool modelsAdding = false;
    bool toRecreateBufers = false;
    bool deletionPrepare = false;
    bool doDeletion = false;    

    glm::vec3 m_direction = glm::vec3(0.0001, 4.0000, 0.0);

    float pomoc = 0.0f;
    bool pom = true;
    float frameLimit = 120;

    Camera cam;
    Engine eng;

    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorSetLayout textDescriptorSetLayout;    
    VkPipelineLayout textPipelineLayout;    
    VkPipeline textGraphicsPipeline;

    VkCommandPool commandPool;
    VkCommandPool commandPool1;
    VkCommandPool buffsPool;
    int32_t usedCommandPool = 0;
    int32_t unUsedCommandPool = 1;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkSampler textureSampler;

    std::vector<Vertex> vertices;
    //std::vector<uint32_t> indices;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkCommandBuffer> commandBuffers1;
    std::vector<VkCommandBuffer*> buffsBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    uint32_t imageIndex;    

    float time = 0.0f;
    bool recreate = false;

    bool framebufferResized = false;    

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void initWindow();
    void initVulkan();
    void prepare();
    void mainLoop();
    void cleanup();

    void cleanupSwapChain();
    void recreateSwapChain();
    void createInstance();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice device);
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createTextDescriptorSetLayout();
    void createGraphicsPipeline();
    void createTextGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    //bool hasStencilComponent(VkFormat format);
    void createTextureImage(std::string texturePath, ModelN* n);
    VkImageView createTextureImageView(VkImage image);
    void createTextureSampler();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool whichPool);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool whichPool);    
    void loadModel(std::string modelPath, ModelN* n);
    void createVertexBuffer(std::vector<Vertex>& verti, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
    void createIndexBuffer();
    void createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory);
    void createDescriptorPool(VkDescriptorPool& descriptorPool);
    void createDescriptorSets(VkDescriptorPool& descriptorPool, std::vector<VkDescriptorSet>& descriptorSets, std::vector<VkBuffer>& uniformBuffers, VkImageView& textureImageView);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool whichPool);
    void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool whichPool);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool whichPool);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createCommandBuffers(int8_t whichPool);
    void createSyncObjects();
    void drawFrame(int8_t whichPool);    
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    static std::vector<char> readFile(const std::string& filename);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void updateUniformBuffer(uint32_t currentImage, ModelN* n);
    void updateUniformBufferPlayer(uint32_t currentImage, ModelN* n);
    void updateUniformBufferCharacter(uint32_t currentImage, Character* c);
    ModelN addModelI(ModelInfo mi);
    void deleteModel(int j);
    void deleteModelM(ModelN* t);
    void deleteModelsData();
    void addModels();
    void recreateBuffers();
    void runEngine(float time);
    void checkModelsToAdd(std::vector<ModelN> models, int32_t xx, int32_t yy, int32_t zz);
    void manageObjects();

    void deleteWords();
    void addWords();
    void addWord(Word* w);
    void addLetters();
    void addLetter(Character* c, float fontSize, glm::vec4 color);
    void createTextureImageLetter(Character* charact);
};