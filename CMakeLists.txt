# CMake entry point
cmake_minimum_required (VERSION 3.0)
# this line suppresses the ZERO_CHECK project that is usually generated
set(CMAKE_SUPPRESS_REGENERATION true)	
project (AdvancedGraphics-Labs)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-4.1.0/include/
	external/soil2/src/
	build/external/assimp-4.1.0/include
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	SOIL2
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!



set (LABNAME Lab1)
# Lab 1 Starting point for lab 1
add_executable(${LABNAME} 
	${LABNAME}/Lab1.cpp
	common/shader.cpp
	common/shader.hpp
	${LABNAME}/Lab1VertexShader.vertexshader
	${LABNAME}/Lab1FragmentShader.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
)
# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK 

####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab2)
# Lab 2
add_executable(${LABNAME} 
	${LABNAME}/Lab2.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
	common/GLError.h
	common/GLError.cpp

	${LABNAME}/TransformVertexShader.vertexshader
	${LABNAME}/TextureFragmentShader.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
)
# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB2



####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab3)
# Lab 3
add_executable(${LABNAME} 
	${LABNAME}/Lab3.cpp
	common/controls.hpp
	common/controls.cpp
	common/quaternion_utils.hpp
	common/quaternion_utils.cpp
	common/model.h
	common/model.cpp
	common/shader.h
	common/GLError.h
	common/GLError.cpp

	${LABNAME}/StandardShading.vertexshader
	${LABNAME}/StandardShading.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
	assimp
)

set_target_properties(${LABNAME} PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")

# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB3


####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab4)
# Lab 4
add_executable(${LABNAME}
	${LABNAME}/Lab4.cpp
	common/controls.hpp
	common/controls.cpp
	common/quaternion_utils.hpp
	common/quaternion_utils.cpp
	common/model.h
	common/model.cpp
	common/shader.h
	common/GLError.h
	common/GLError.cpp
	common/skybox.h
	common/skybox.cpp

	${LABNAME}/StandardShading.vertexshader
	${LABNAME}/StandardShading.fragmentshader
	${LABNAME}/StandardShadingWithAOSpecmaps.vertexshader
	${LABNAME}/StandardShadingWithAOSpecmaps.fragmentshader
	${LABNAME}/LightShading.vertexshader
	${LABNAME}/LightShading.fragmentshader
	${LABNAME}/skybox.vertexshader
	${LABNAME}/skybox.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
	assimp
)

set_target_properties(${LABNAME} PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")

# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB4



####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab5)
# Lab 5
add_executable(${LABNAME}
	${LABNAME}/Lab5.cpp
	common/controls.hpp
	common/controls.cpp
	common/quaternion_utils.hpp
	common/quaternion_utils.cpp
	common/model.h
	common/model.cpp
	common/shader.h
	common/GLError.h
	common/GLError.cpp

	${LABNAME}/StandardShadingWithVertexBlend.vertexshader
	${LABNAME}/StandardShadingWithVertexBlend.fragmentshader
	${LABNAME}/LightShading.vertexshader
	${LABNAME}/LightShading.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
	assimp
)

set_target_properties(${LABNAME} PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")

# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB5

####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab6)
# Lab 6
add_executable(${LABNAME}
	${LABNAME}/Lab6.cpp
	common/controls.hpp
	common/controls.cpp
	common/quaternion_utils.hpp
	common/quaternion_utils.cpp
	common/model.h
	common/model.cpp
	common/shader.h
	common/GLError.h
	common/GLError.cpp
	common/skybox.h
	common/skybox.cpp

	${LABNAME}/StandardShading.vertexshader
	${LABNAME}/StandardShading.fragmentshader
	${LABNAME}/StandardShadingWithAOSpecRimGlossHalfLambert.vertexshader
	${LABNAME}/StandardShadingWithAOSpecRimGlossHalfLambert.fragmentshader
	${LABNAME}/LightShading.vertexshader
	${LABNAME}/LightShading.fragmentshader
	${LABNAME}/skybox.vertexshader
	${LABNAME}/skybox.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
	assimp
)

set_target_properties(${LABNAME} PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")

# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB6

####################################### STANDARD BLOCK FOR A LAB PROGRAM IN THE .SLN FILE - REPLACE LAB<number> WITH YOUR OWN NUMBER!

set (LABNAME Lab7)
# Lab 7
add_executable(${LABNAME}
	${LABNAME}/Lab7.cpp
	common/controls.hpp
	common/controls.cpp
	common/quaternion_utils.hpp
	common/quaternion_utils.cpp
	common/model.h
	common/model.cpp
	common/shader.h
	common/GLError.h
	common/GLError.cpp
	common/skybox.h
	common/skybox.cpp

	${LABNAME}/StandardShading.vertexshader
	${LABNAME}/StandardShading.fragmentshader
	${LABNAME}/StandardShadingWithReflect.fragmentshader
	${LABNAME}/StandardShadingWithReflect.vertexshader
	${LABNAME}/StandardShadingWithRefract.fragmentshader
	${LABNAME}/StandardShadingWithRefract.vertexshader
	${LABNAME}/LightShading.vertexshader
	${LABNAME}/LightShading.fragmentshader
	${LABNAME}/skybox.vertexshader
	${LABNAME}/skybox.fragmentshader
)
target_link_libraries(${LABNAME}
	${ALL_LIBS}
	assimp
)

set_target_properties(${LABNAME} PROPERTIES COMPILE_DEFINITIONS "USE_ASSIMP")

# Visual studio working directories (tells Visual Studio where you can debug from etc)
create_target_launcher(${LABNAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/")

# Visual studio custom build command (copies .exe (debug or release) file from build dir back into the folder where the source is)
add_custom_command(
   TARGET ${LABNAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${LABNAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${LABNAME}/"
)

###################################### END STANDARD BLOCK FOR LAB7


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

