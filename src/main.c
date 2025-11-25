#include <stdio.h>
#include <system/window.h>
#include <vulkan/vulkan_core.h>
#include "graphics/graphics_instance.h"

int main(int argc, char* argv[]){
    EvWindow window = evCreateWindow(0, 0, 800, 600, "Eclipse Vulkan");
    EvGraphicsIntance instance = evCreateGraphicsInstance("Eclipse Vulkan", VK_MAKE_VERSION(1, 0, 0), false);

    while(!evWindowClose(window)){

    }

    evDestroyGraphicsInstance(instance);
    evDestroyWindow(window);

    printf("Program Success!!.\n");
    return 0;

}