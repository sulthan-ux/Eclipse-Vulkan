#include "graphics/graphics_instance.h"
#include "utilities/error.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

void evGraphicsIntanceCreateInstance(const char* pAppName, uint32_t appVersion, bool enableValidation, EvGraphicsIntance instance);
void evGraphicsInstanceCreateMessenger(EvGraphicsIntance instance);

EvGraphicsIntance evCreateGraphicsInstance(const char* pAppName, uint32_t appVersion, bool enableValidation){
    assert(pAppName);
    assert(appVersion);

    EvGraphicsIntance instance = (EvGraphicsIntance)malloc(sizeof(EvGraphicsIntance_T));
    if(!instance){
        EV_ERROR("Failed to allocate EvGraphicsIntance instance!!.\n");
    }

    evGraphicsIntanceCreateInstance(pAppName, appVersion, enableValidation, instance);
    evGraphicsInstanceCreateMessenger(instance);

    return instance;
}
void evDestroyGraphicsInstance(EvGraphicsIntance instance){
    vkDestroyInstance(instance->instance, NULL);
    free(instance);
}
void evGraphicsIntanceCreateInstance(const char* pAppName, uint32_t appVersion, bool enableValidation, EvGraphicsIntance instance){
    VkResult result;
    uint32_t extensionCount = 0;
    result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkEnumerateInstanceExtensionProperties!!.\n");
    }

    VkExtensionProperties* extensionProps = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * extensionCount);
    if(!extensionProps){
        EV_ERROR("Failed to allocate VkExtensionProperties* extensionProps!!.\n");
    }
    result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProps);

    uint32_t selectExtensionCount = 0;
    for(uint32_t i = 0; i < extensionCount; ++i){
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_surface") == 0) selectExtensionCount++;
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_xlib_surface") == 0) selectExtensionCount++;
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_xcb_surface") == 0) selectExtensionCount++;
        if((strcmp(extensionProps[i].extensionName, "VK_EXT_debug_utils") == 0) && enableValidation) selectExtensionCount++;
    }

    char** extensions = (char**)malloc(sizeof(char*) * selectExtensionCount);
    if(!selectExtensionCount){
        EV_ERROR("Failed to allocate const char* extensions!!.\n");
    }

    uint32_t extensionsIndex = 0;
    for(uint32_t i = 0; i < extensionCount; ++i){
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_surface") == 0) extensions[extensionsIndex++] = "VK_KHR_surface";
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_xlib_surface") == 0) extensions[extensionsIndex++] = "VK_KHR_xlib_surface";
        if(strcmp(extensionProps[i].extensionName, "VK_KHR_xcb_surface") == 0) extensions[extensionsIndex++] = "VK_KHR_xcb_surface";
        if((strcmp(extensionProps[i].extensionName, "VK_EXT_debug_utils") == 0) && enableValidation) extensions[extensionsIndex++] = "VK_EXT_debug_utils";
    }

    uint32_t layerCount = 0;
    result = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkEnumerateInstanceLayerProperties!!.\n");
    }

    VkLayerProperties* layerProps = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * layerCount);
    if(!layerProps){
        EV_ERROR("Failed to allocate VkLayerProperties* layerProps!!.\n");
    }
    result = vkEnumerateInstanceLayerProperties(&layerCount, layerProps);

    bool validationSupport = false;
    for(uint32_t i = 0; i < layerCount; ++i){
        if(strcmp(layerProps[i].layerName, "VK_LAYER_KHRONOS_validation")) validationSupport = true;
    }

    free(extensionProps); free(layerProps); 

    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = pAppName;
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = "Eclipse Vulkan";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;
    appInfo.pNext = NULL;

    const char* layer = "VK_LAYER_KHRONOS_validation";

    VkInstanceCreateInfo instanceInfo = {0};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.flags = 0;
    instanceInfo.enabledLayerCount = validationSupport ? 1 : 0;
    instanceInfo.ppEnabledLayerNames = &layer;
    instanceInfo.enabledExtensionCount = selectExtensionCount;
    instanceInfo.ppEnabledExtensionNames = (const char**)extensions;
    instanceInfo.pNext = NULL;

    result = vkCreateInstance(&instanceInfo, NULL, &instance->instance);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkCreateInstance!!.");
    }

    for(uint32_t i = 0; i < selectExtensionCount; ++i){
        fprintf(stdout, "Enable Extensions: %s\n", extensions[i]);
    }

    fprintf(stdout, "Enable Layer: %s\n", layer);

    free(extensions);
}
void evGraphicsInstanceCreateMessenger(EvGraphicsIntance instance){

}