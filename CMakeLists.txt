cmake_minimum_required(VERSION 3.15)
project(KafkaCpplaygroundQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# Librdkafka++
# -------------------------------
set(RDKAFKA_INCLUDE_DIR "/opt/homebrew/include")
set(RDKAFKA_LIB "/opt/homebrew/lib/librdkafka++.dylib")
include_directories(${RDKAFKA_INCLUDE_DIR})

# -------------------------------
# Qt6
# -------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -------------------------------
# Fuentes comunes
# -------------------------------
set(SOURCES_COMMON
        ../src/kafka_producer.cpp
        ../src/kafka_consumer.cpp
        ../src/kafka_utils.cpp
)

# -------------------------------
# Producer UI
# -------------------------------
add_executable(producer_ui
        ../main_producer_ui.cpp
        ${SOURCES_COMMON}
)

target_link_libraries(producer_ui
        Qt6::Widgets
        ${RDKAFKA_LIB}
)

# -------------------------------
# Consumer UI
# -------------------------------
add_executable(consumer_ui
        ../main_consumer_ui.cpp
        ${SOURCES_COMMON}
)

target_link_libraries(consumer_ui
        Qt6::Widgets
        ${RDKAFKA_LIB}
)

# -------------------------------
# Includes
# -------------------------------
target_include_directories(producer_ui PRIVATE ../src)
target_include_directories(consumer_ui PRIVATE ../src)
