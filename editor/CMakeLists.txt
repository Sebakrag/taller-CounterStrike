target_sources(taller_editor
    PRIVATE
    # .cpp files
    src/main.cpp
    src/mapEditor.cpp
    src/dragAndDrop.cpp
    src/mapElements.cpp
    src/yamlHandler.cpp
    src/mapValidator.cpp
    src/backgroundItem.cpp
    src/tileItem.cpp
    PUBLIC
    # .h files
    include/mapEditor.h
    include/dragAndDrop.h
    include/mapElements.h
    include/yamlHandler.h
    include/mapValidator.h
    include/backgroundItem.h
    include/tileItem.h
    )

# Incluir directorios de cabeceras
target_include_directories(taller_editor
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
