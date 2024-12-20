cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# Имя поекта
project(Test_task LANGUAGES CXX)

# Стандарт языка С++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Указываем путь к заголовочным и исходным файлам
set(HEADERS

)

set(SOURCES
    src/main.cpp
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Включаем путь к заголовочным файлам для сборки
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Подключаем Qt5 к проекту
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# Устанавливаем пути для установки исполняемых файлов и заголовков
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include/${PROJECT_NAME}
)
