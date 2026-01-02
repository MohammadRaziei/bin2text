# Optimize.cmake
#
# Aggressive optimization flags for maximum runtime performance.
# Works with GCC, Clang, and MSVC.
#
# Usage:
#   include(PROJECTOptimize)
#   project_set_default_optimizations()
#   project_enable_optimizations(<target>)

option(PROJECT_ARCH_NATIVE "Enable -march=native (tune to local CPU)" ON)
option(PROJECT_FAST_MATH  "Enable fast-math (unsafe for strict IEEE compliance)" ON)

# --- Helper: enable IPO/LTO if available ---
function(_project_enable_ipo target)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT _ipo_ok OUTPUT _ipo_msg)
    if(_ipo_ok)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO not supported for ${target}: ${_ipo_msg}")
    endif()
endfunction()

# --- Apply optimization flags per target ---
function(project_enable_optimizations target)
    target_compile_definitions(${target} PRIVATE
            $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:NDEBUG>
    )

    if(MSVC)
        target_compile_options(${target} PRIVATE
                $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:/O2 /GL /Oi /Ot /Ob3 /DNDEBUG>
                $<$<BOOL:${PROJECT_FAST_MATH}>:/fp:fast>
        )
        target_link_options(${target} PRIVATE
                $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:/LTCG>
        )
    else()
        target_compile_options(${target} PRIVATE
                $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:-O3 -fno-plt -funroll-loops -finline-functions -fstrict-aliasing>
                $<$<BOOL:${PROJECT_ARCH_NATIVE}>:-march=native>
                $<$<BOOL:${PROJECT_FAST_MATH}>:-ffast-math>
        )
        target_link_options(${target} PRIVATE
                $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:-flto>
        )
    endif()

    _project_enable_ipo(${target})
endfunction()

# --- Apply global defaults ---
function(project_set_default_optimizations)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()

    include(CheckIPOSupported)
    check_ipo_supported(RESULT _ipo_ok OUTPUT _ipo_msg)
    if(_ipo_ok)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    if(MSVC AND NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endfunction()
