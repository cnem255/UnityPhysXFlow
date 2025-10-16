cmake_minimum_required(VERSION 3.20)

project(UnityPhysXFlow LANGUAGES C CXX)

if (MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Locate PhysX/Flow SDK root
set(PHYSX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../PhysX" CACHE PATH "Path to PhysX 5.6.1 root (contains flow/, physx/, blast/)")

if (NOT EXISTS "${PHYSX_ROOT}/flow/include")
    message(FATAL_ERROR "PHYSX_ROOT not valid: ${PHYSX_ROOT}. Expected folder 'flow/include' under it.")
endif()
if (NOT EXISTS "${PHYSX_ROOT}/flow/include/nvflow/NvFlow.h")
    message(FATAL_ERROR "NvFlow headers not found at ${PHYSX_ROOT}/flow/include/nvflow. Check PHYSX_ROOT.")
endif()

add_library(unity_physx_flow SHARED
    src/UnityPhysXFlow.cpp
)

target_include_directories(unity_physx_flow
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Flow headers sometimes include without subfolder (e.g., "NvFlowContext.h"),
    # so add both the root include and subfolders to the search path
    PRIVATE ${PHYSX_ROOT}/flow/include
    PRIVATE ${PHYSX_ROOT}/flow/include/nvflow
    PRIVATE ${PHYSX_ROOT}/flow/include/nvflowext
)

target_compile_definitions(unity_physx_flow
    PRIVATE FLOW_UNITY_BRIDGE_EXPORTS
)

# Windows-specific export macro
if (WIN32)
    target_compile_definitions(unity_physx_flow PUBLIC FLOW_UNITY_BRIDGE_WINDOWS)
endif()

# We load nvflow.dll and nvflowext.dll at runtime via NvFlowLoader.h, no link libs required

# Set output name and folder
set_target_properties(unity_physx_flow PROPERTIES
    OUTPUT_NAME "unity_physx_flow"
)

# Install rules (optional)
install(TARGETS unity_physx_flow
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
