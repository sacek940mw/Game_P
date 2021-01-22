#include "Renderer.h"

void Renderer::mainLoop() {
    usedCommandPool = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    eng.getPlayer()->setCordChanged(true);
    eng.run(0);
    manageObjects();
    recreateBuffers();

    while (!(glfwWindowShouldClose(window) && modelsAdding == false)) {
        
        glfwPollEvents();        

        if (deletionPrepare == true) {
            if (doDeletion == true) {
                std::thread delMD(&Renderer::deleteModelsData, this);
                delMD.detach();
            }
            else {
                doDeletion = true;
            }
        }       

        drawFrame(usedCommandPool);

        /*
        std::thread tr1(&Api::runEngine, this, time);
        std::thread tr2(&Api::checkModelsToAdd, this, eng.getModels(), eng.getPlayer()->getPlPos().xx, eng.getPlayer()->getPlPos().yy, eng.getPlayer()->getPlPos().zz);
        std::thread tr3(&Api::manageObjects, this);
        tr1.join();
        tr2.join();
        tr3.join();
       */
        
        runEngine(time);
        checkModelsToAdd(eng.getModels(), eng.getPlayer()->getPlPos().xx, eng.getPlayer()->getPlPos().yy, eng.getPlayer()->getPlPos().zz);
        manageObjects();

        if (toRecreateBufers == true) {
            recreateBuffers();
            toRecreateBufers = false;
        }       

        currentTime = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        if (frameLimit > 0) {
            if (time < (float)(1 / frameLimit)) {
                int timeToSleep = (float)((1 / frameLimit) - time) * 1000;
                std::chrono::milliseconds timespan(timeToSleep);
                std::this_thread::sleep_for(timespan);
            }
        }        
    }
    vkQueueWaitIdle(graphicsQueue);
    vkDeviceWaitIdle(device);
}

void Renderer::runEngine(float time) {
    eng.run(time);
}

void Renderer::checkModelsToAdd(std::vector<ModelN> models, int32_t xx, int32_t yy, int32_t zz) {
    eng.checkModelsToAdd(models, xx, yy, zz);
}

void Renderer::manageObjects() {
    if (modelsAdding == false) {
        //delete:
        if (eng.getModelsP()->size() > 1 && deletionPrepare == false) {
            if (eng.modelsTD.size() > 0) {
                for (int32_t i = 0; i < eng.modelsTD.size(); i++) {
                    if (eng.modelsTD.at(0) > 0) {
                        deleteModel(eng.modelsTD.at(i));
                        //deleteModel(0);
                    }
                }
                eng.modelsTD.clear();
                deletionPrepare = true;
                //recreateBuffers();
                toRecreateBufers = true;
            }
        }
        //add:
        if (toJoin == true) {
            if (joined == true) {
                vkResetCommandPool(device, buffsPool, NULL);                

                MultiModelManager* mmm = eng.getMultiModelManagerP();
                for (ModelN& m : *eng.getModelsbP()) {
                    mmm->translateModel(&m.mPos, 20.0f * (m.xx - eng.getPlayer()->getPlPos().xx) - eng.getPlayer()->getPlPosVec().x,
                        20.0f * (m.yy - eng.getPlayer()->getPlPos().yy) - eng.getPlayer()->getPlPosVec().y,
                        20.0f * (m.zz - eng.getPlayer()->getPlPos().zz) - eng.getPlayer()->getPlPosVec().z);
                }

                eng.getModelsP()->insert(eng.getModelsP()->end(), eng.getModelsbP()->begin(), eng.getModelsbP()->end());
                eng.getModelsbP()->clear();
                //recreateBuffers();
                toRecreateBufers = true;
                joined = false;
                toJoin = false;
            }
            else {
                joined = true;
            }            
        }
        else {
            int32_t j = eng.getMultiModelManagerP()->getModelsToAddSize();
             if (j > 0) {
                modelsAdding = true;
                std::thread tr1(&Renderer::addModels, this);
                tr1.detach();
             }
        }        
    }
}

void Renderer::deleteModelsData() {
    mtx.lock();
    if (!eng.getModelsDP()->empty()) {
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            for (ModelN j : eng.getModelsD()) {
                vkDestroyBuffer(device, j.uniformBuffers[i], nullptr);
                vkFreeMemory(device, j.uniformBuffersMemory[i], nullptr);
            }
        }
        for (ModelN j : eng.getModelsD()) {
            vkDestroyDescriptorPool(device, j.descriptorPool, nullptr);
            vkDestroyImageView(device, j.textureImageView, nullptr);
            vkDestroyImage(device, j.textureImage, nullptr);
            vkFreeMemory(device, j.textureImageMemory, nullptr);
            vkDestroyBuffer(device, j.vertexBuffer, nullptr);
            vkFreeMemory(device, j.vertexBufferMemory, nullptr);
        }
        eng.getModelsDP()->clear();
    }
    mtx.unlock();
    doDeletion = false;
    deletionPrepare = false;
}