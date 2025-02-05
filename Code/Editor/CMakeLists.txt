cmake_minimum_required(VERSION 3.10)

project(EditorStatic)

get_filename_component(EDITOR_LIB_SRC_DIR
                       ${CMAKE_CURRENT_LIST_DIR}/../Maple/lib
                       ABSOLUTE)

set(CMAKE_CXX_STANDARD 17)


file(GLOB EDITOR_STATIC_SRC
	src/*.cpp
	src/*.h
	src/*.inl
	src/Plugin/*.cpp
	src/Plugin/*.h
	src/UI/*.cpp
	src/UI/*.h
)

add_definitions(-DEDITOR_STATIC -DMAPLE_DYNAMIC -DPLATFORM_WINDOWS)

add_library(EditorStatic 
	${EDITOR_STATIC_SRC}
)

target_include_directories(EditorStatic PUBLIC
	src
	../Maple/src
	${EDITOR_LIB_SRC_DIR}/opengl/include
	${EDITOR_LIB_SRC_DIR}/imgui/src
	${EDITOR_LIB_SRC_DIR}/spdlog/include
	${EDITOR_LIB_SRC_DIR}/stb_image
	${EDITOR_LIB_SRC_DIR}/vulkan/include
	${EDITOR_LIB_SRC_DIR}/tinyobjloader
	${EDITOR_LIB_SRC_DIR}/glm
	${EDITOR_LIB_SRC_DIR}/entt
	${EDITOR_LIB_SRC_DIR}/SPIRV-Cross
	${EDITOR_LIB_SRC_DIR}/ktx/include
	${EDITOR_LIB_SRC_DIR}/ktx/other_include
	${EDITOR_LIB_SRC_DIR}/cereal/include
	${EDITOR_LIB_SRC_DIR}/libmono/6.13.0/include
	${EDITOR_LIB_SRC_DIR}/imGuIZMOquat/
	${EDITOR_LIB_SRC_DIR}/ecs
	${EDITOR_LIB_SRC_DIR}/imgui-node-editor
)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${EDITOR_STATIC_SRC})