cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Luvu")

add_subdirectory(thirdparty/luau)

add_executable (Luvu "main.cpp"  "LuauVM/LuauVM.cpp" "LuauVM/LuauVM.h" "LuauVM/Userdata.h"  "LuauVM/UserdataTest.h" "LuauVM/UserdataTest.cpp" "LuauVM/Userdata.cpp" "Rendering/Shader.h" "Rendering/Shader.cpp")

add_library (glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/thirdparty/glfw/lib-vc2022/glfw3.lib)
target_include_directories(glfw INTERFACE ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include)

add_library(glad ${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty/glad/include)

add_library(cglm INTERFACE)
target_include_directories(glad INTERFACE ${PROJECT_SOURCE_DIR}/thirdparty/cglm/include)

target_link_libraries(Luvu PRIVATE Luau.VM Luau.Compiler glfw glad cglm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Luvu PROPERTY CXX_STANDARD 20)
endif()