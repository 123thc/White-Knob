file(GLOB_RECURSE RESOURCES "./*.c")
list(APPEND RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui_manager/time_bg.c")
list(APPEND RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/app_core/led/light_custom.c")
list(REMOVE_DUPLICATES RESOURCES)

set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/app_core
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui/custom
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui/generated/guider_customer_fonts
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui/generated/images
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/lcd/ui/generated/guider_fonts
)

idf_component_register(SRCS ${RESOURCES}
                    INCLUDE_DIRS ${INCLUDES})
#ESP32特供，非Cmake所有
spiffs_create_partition_image(html ./app_core/html FLASH_IN_PROJECT)#spiffs分区名称 html所在文件夹

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_LVGL_H_INCLUDE_SIMPLE)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-const-variable) #禁用'未使用的常量变量'警告"
