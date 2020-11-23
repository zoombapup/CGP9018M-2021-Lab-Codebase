if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###

add_subdirectory (glfw-3.3.2)

include_directories(
	glfw-3.3.2/include/GLFW/
	glew-1.13.0/include/
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLEW ###

set(GLEW_SOURCE
	glew-1.13.0/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_1130 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_1130
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

### ASSIMP ###
# AssImp already has a CMakeLists.txt so let's use these

set(BUILD_STATIC_LIB ON)
#set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib)
add_subdirectory( assimp-5.0.1 )

include_directories(
	assimp-5.0.1/code
	assimp-5.0.1/include
)


### SOIL2 ###

file (GLOB SOIL2_HEADERS soil2/src/*.h)
file (GLOB SOIL2_SOURCE soil2/src/*.c)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

include_directories(
	soil2/src/
)


add_library( SOIL2 STATIC
	${SOIL2_SOURCE}
	${SOIL2_HEADERS}
	soil2/common/common.cpp
	soil2/common/common.hpp
)

target_link_libraries(SOIL2
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)


