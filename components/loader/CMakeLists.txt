idf_component_register(
    SRCS
        "loader.c"
        "loader_menu.c"
        "loader_applications.c"
        "menu_custom.c"
        "apps/about_app.c"
    INCLUDE_DIRS "."
    REQUIRES furi gui toolbox assets storage archive dialogs flipper_application
)

# Localization affects only the Cardputer-Adv display labels.
if(FLIPPER_BOARD STREQUAL "m5stack_cardputer_adv")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE FLIPPER_ZH_CN=1)
endif()
