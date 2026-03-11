if (NOT EXISTS "C:/Users/usuari/Documents/GitHub/PlatformGame/vcpkg_installed/x64-windows/vcpkg/blds/sdl3-image/x64-windows-dbg/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"C:/Users/usuari/Documents/GitHub/PlatformGame/vcpkg_installed/x64-windows/vcpkg/blds/sdl3-image/x64-windows-dbg/install_manifest.txt\"")
endif()

file(READ "C:/Users/usuari/Documents/GitHub/PlatformGame/vcpkg_installed/x64-windows/vcpkg/blds/sdl3-image/x64-windows-dbg/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND C:/Users/usuari/AppData/Local/vcpkg/downloads/tools/cmake-3.30.1-windows/cmake-3.30.1-windows-i386/bin/cmake.exe -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach()
