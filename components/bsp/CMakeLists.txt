idf_component_register(
        SRCS
        "display/st7789/ST7789.c"
        "display/gc9a01/GC9A01.c"

        INCLUDE_DIRS
        "display"
        "display/gc9a01"
        "display/st7789"
        "."
        REQUIRES
        esp_lcd
        esp_timer
        main
)