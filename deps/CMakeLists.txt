# filament
add_library(filament STATIC IMPORTED GLOBAL)
set_target_properties(filament PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libfilament-full.a)
set_target_properties(filament PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/filament/include)

# SDL2
add_subdirectory(libsdl2/tnt)

# imgui
add_subdirectory(imgui/tnt)

# implot
add_library(implot STATIC implot/implot.cpp implot/implot_demo.cpp implot/implot_items.cpp)
target_include_directories(implot PUBLIC implot)
target_link_libraries(implot PUBLIC imgui)

# stb
add_subdirectory(stb/tnt)

# assimp
add_subdirectory(libassimp/tnt)