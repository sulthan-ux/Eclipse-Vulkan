#include "graphics/graphics_device.h"
#include "graphics/graphics_instance.h"
#include "utilities/error.h"
#include "stdlib.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <vulkan/vulkan_core.h>

void evGraphicsDeviceSelectPhysicalDevice(EvGraphicsIntance instance, EvGraphicsDevice device);
void evGraphicsDeviceEnumerateQueueFamilyIndices(EvGraphicsDevice device);
void evGraphicsDeviceCreateLogicalDevice(EvGraphicsDevice device);

EvGraphicsDevice evCreateGraphicsDevice(EvGraphicsIntance instance){
    assert(instance);

    EvGraphicsDevice device = (EvGraphicsDevice)malloc(sizeof(EvGraphicsDevice_T));
    if(!device){
        EV_ERROR("Failed to allocate EvGraphicsDevice device!!.\n");
    }

    evGraphicsDeviceSelectPhysicalDevice(instance, device);
    evGraphicsDeviceEnumerateQueueFamilyIndices(device);
    evGraphicsDeviceCreateLogicalDevice(device);

    return device;
}
void evDestroyGraphicsDevice(EvGraphicsDevice device){
    vkDestroyDevice(device->logical, NULL);
    free(device);
}
void evGraphicsDeviceSelectPhysicalDevice(EvGraphicsIntance instance, EvGraphicsDevice device){
    VkResult result;
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(instance->instance, &deviceCount, NULL);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkEnumeratePhysicalDevices!!.\n");
    }
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);
    if(!devices){
        EV_ERROR("Failed to allocate VkPhysicalDevice* devices!!.\n");
    }
    result = vkEnumeratePhysicalDevices(instance->instance, &deviceCount, devices);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkEnumeratePhysicalDevices!!.\n");
    }

    for(uint32_t i = 0; i < deviceCount; ++i){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);
        if((props.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) || (props.deviceType & VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)){
            fprintf(stdout, "GPU: %s\n", props.deviceName);
            device->physical = devices[i];
            break;
        }
    }

    if(device->physical == NULL){
        EV_ERROR("Cannot get GPU with type integrated or discreted!!.\n");
    }
    free(devices);
}
void evGraphicsDeviceEnumerateQueueFamilyIndices(EvGraphicsDevice device){
    VkResult result;
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device->physical, &queueCount, NULL);

    VkQueueFamilyProperties* queueProperties = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueCount);
    if(!queueProperties){
        EV_ERROR("Failed to allocate VkQueueFamilyProperties* queueProperties!!.\n");
    }
    vkGetPhysicalDeviceQueueFamilyProperties(device->physical, &queueCount, queueProperties);

    device->graphicsFamily = UINT32_MAX;
    device->transferFamily = UINT32_MAX;
    device->computeFamily = UINT32_MAX;

    for(uint32_t i = 0; i < queueCount; ++i){
        if(queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) device->graphicsFamily = i;
        if(queueProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) device->transferFamily = i;
        if(queueProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) device->computeFamily = i;
    }


}
void evGraphicsDeviceCreateLogicalDevice(EvGraphicsDevice device){
    VkResult result;
    uint32_t uniqueQueue = 1;
    uint32_t* uniqueFamilies = (uint32_t*)malloc(sizeof(uint32_t) * uniqueQueue);
    if(!uniqueFamilies){
        EV_ERROR("Failed to allocate uint32_t* uniqueFamilies!!.\n");
    }
    uniqueFamilies[0] = device->graphicsFamily;

    uint32_t queueFamilies[] = {device->transferFamily, device->computeFamily};

    for(uint32_t i = 0; i < 2; ++i){
        if(uniqueFamilies[uniqueQueue - 1] != queueFamilies[i]){
            void* temp = realloc(uniqueFamilies, sizeof(uint32_t) * (uniqueQueue + 1));
            if(!temp){
                EV_ERROR("Failed to allocate void* temp!!.\n");
            }
            uniqueFamilies = temp;
            uniqueFamilies[uniqueQueue++] = queueFamilies[i];
        }
    }

    VkDeviceQueueCreateInfo queueInfos[3];
    float queuePriority = 1.0f;

    for(uint32_t i = 0; i < uniqueQueue; ++i){
        queueInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfos[i].queueCount = 1;
        queueInfos[i].queueFamilyIndex = uniqueFamilies[i];
        queueInfos[i].flags = 0;
        queueInfos[i].pQueuePriorities = &queuePriority;
        queueInfos[i].pNext = NULL;
    }

    const char* extension = "VK_KHR_swapchain";
    VkPhysicalDeviceFeatures features = {0};

    VkDeviceCreateInfo deviceInfo = {0};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.flags = 0;
    deviceInfo.queueCreateInfoCount = uniqueQueue;
    deviceInfo.pQueueCreateInfos = queueInfos;
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = &extension;
    deviceInfo.pEnabledFeatures = &features;
    deviceInfo.pNext = NULL;

    result = vkCreateDevice(device->physical, &deviceInfo, NULL, &device->logical);
    if(result != VK_SUCCESS){
        EV_ERROR("Failed to vkCreateDevice!!.\n");
    }

    free(uniqueFamilies);

    VkDeviceQueueInfo2 queues[3];
    queues[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queues[0].flags = 0;
    queues[0].queueFamilyIndex = device->graphicsFamily;
    queues[0].queueIndex = 0;
    queues[0].pNext = NULL;

    queues[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queues[1].flags = 0;
    queues[1].queueFamilyIndex = device->transferFamily;
    queues[1].queueIndex = 0;
    queues[1].pNext = NULL;

    queues[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queues[2].flags = 0;
    queues[2].queueFamilyIndex = device->computeFamily;
    queues[2].queueIndex = 0;
    queues[2].pNext = NULL;

    vkGetDeviceQueue2(device->logical, &queues[0], &device->graphicsQueue);
    vkGetDeviceQueue2(device->logical, &queues[1], &device->transferQueue);
    vkGetDeviceQueue2(device->logical, &queues[2], &device->computeQueue);
}