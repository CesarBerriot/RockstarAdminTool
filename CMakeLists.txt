cmake_minimum_required(VERSION 3.31)
project(rockstar_admin_tool C CXX)

if(NOT MSVC)
  message(FATAL_ERROR "RockstarAdminTool requires MSVC to be built.")
endif()

include(FetchContent)
FetchContent_Declare(fetch_github_content GIT_REPOSITORY https://github.com/CesarBerriot/fetch_github_content.git GIT_TAG 1.0.0)
FetchContent_MakeAvailable(fetch_github_content)
include(${fetch_github_content_SOURCE_DIR}/fetch_github_content.cmake)

fetch_github_content(
  hard_assert CesarBerriot 1.2.1
  make_string CesarBerriot 2.2.1
  raylayout CesarBerriot 1.0.0
  discover_sources CesarBerriot 1.0.0 INCLUDE_DEFAULT
  Internal2 CesarBerriot 1.0.2 INCLUDE_DEFAULT
)

add_executable(rockstar_admin_tool)
discover_sources(rockstar_admin_tool)
internal2_setup_target(rockstar_admin_tool)
target_include_directories(rockstar_admin_tool PRIVATE source)
target_compile_definitions(rockstar_admin_tool PRIVATE MODULE_NAME="R* Admin Tool")
target_link_libraries(rockstar_admin_tool PRIVATE hard_assert fwpuclnt Ws2_32 raylayout make_string)
target_compile_options(rockstar_admin_tool PRIVATE /W4 /wd4047 /wd4204 /wd4221 /wd4244)
target_link_options(rockstar_admin_tool PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /MANIFESTUAC:level='requireAdministrator')
set_property(TARGET rockstar_admin_tool PROPERTY OUTPUT_NAME RockstarAdminTool)