cmake_minimum_required(VERSION 3.10)

# Build a DLL with stdcall exports for Delphi
add_library(delphi_dll SHARED
    delphi.cpp
    delphi.def
)

# The library project must define DELPHI_DLL_EXPORTS so that symbols are
# exported instead of imported.
target_compile_definitions(delphi_dll PRIVATE DELPHI_DLL_EXPORTS)

# Add dependency on minifmod so we can play XM/MOD files
# minifmod library is built earlier in the same workspace, so just link to it

target_include_directories(delphi_dll PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/minifmod/include
)

# Link against in-tree minifmod and its dependencies
target_link_libraries(delphi_dll PRIVATE minifmod)

# On Windows, add linker flag to use definition file (already included above as source)
if(MSVC)
    # def file is automatically honored when listed as source
    # optionally you can set LINK_FLAGS
endif()

# Optionally set output name
set_target_properties(delphi_dll PROPERTIES
    VERSION 1.0
    SOVERSION 1
    OUTPUT_NAME "minifmod_delphi"
)
