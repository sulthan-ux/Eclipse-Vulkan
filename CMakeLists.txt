cmake_minimum_required(VERSION 3.20)
project(EclipseVulkan)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB EV_LIBRARIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/*.c
)
set(EV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)

add_executable(EclipseVulkan ${EV_LIBRARIES})

target_include_directories(
    EclipseVulkan 
    PRIVATE
    ${EV_INCLUDE_DIRS}
)

target_link_libraries(
    EclipseVulkan
    PRIVATE
    Vulkan::Vulkan
    SDL3::SDL3
)