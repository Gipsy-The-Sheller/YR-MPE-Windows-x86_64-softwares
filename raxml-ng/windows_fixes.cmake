# fix_windows.cmake - Windows 编译修复脚本
message(STATUS "Applying Windows fixes...")

if(MSVC)
    # 移除所有 GCC 特有的编译选项
    macro(remove_gcc_flags)
        set(CMAKE_C_FLAGS "")
        set(CMAKE_CXX_FLAGS "")
        
        # 设置 MSVC 编译选项
        add_compile_options(/W4)
        add_compile_options(/wd4244 /wd4267 /wd4996 /wd4018 /wd4101)
        add_compile_options(/D_CRT_SECURE_NO_WARNINGS)
        add_compile_options(/EHsc)  # 修复 C4530 警告
        
        # 移除 GCC 特有选项的正则表达式
        set(gcc_flags "-Wsign-compare;-Wextra;-Wall;-Wconversion;-Wsign-conversion;-Werror;-Wpedantic;/Wsign-compare;/Wextra")
        
        # 处理所有目标
        get_property(targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
        foreach(target ${targets})
            get_target_property(target_flags ${target} COMPILE_OPTIONS)
            if(target_flags)
                foreach(flag ${gcc_flags})
                    list(REMOVE_ITEM target_flags ${flag})
                endforeach()
                set_target_properties(${target} PROPERTIES COMPILE_OPTIONS "${target_flags}")
            endif()
        endforeach()
    endmacro()
    
    # 调用宏
    remove_gcc_flags()
    
    # 设置 Windows 定义
    add_definitions(-D_WIN32 -D_CRT_SECURE_NO_WARNINGS)
    
    # 添加 Windows 兼容头文件路径
    include_directories(${CMAKE_SOURCE_DIR})
endif()