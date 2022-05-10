cmake_minimum_required(VERSION 3.21)
project(grpc_tailbook_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GRPC_CXX REQUIRED grpc++)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${GRPC_CXX_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    cplusplus/profile_server_main.cpp
    cplusplus/profile_service.cpp
    cplusplus/profile_service.h
)
set(PROTO_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/social_network.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/social_network.pb.h
)
set(GRPC_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/social_network.grpc.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/social_network.grpc.pb.h
)

add_custom_command(
    OUTPUT ${PROTO_SOURCES} ${GRPC_SOURCES}
    COMMAND protoc
    ARGS
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        --go_out=${CMAKE_CURRENT_SOURCE_DIR}/go
        --python_out=${CMAKE_CURRENT_SOURCE_DIR}/python
        --grpc_python_out=${CMAKE_CURRENT_SOURCE_DIR}/python
        --dart_out=grpc:${CMAKE_CURRENT_SOURCE_DIR}/dart/lib/src/generated
        --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
        --go_grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/go

        --plugin=protoc-gen-grpc=$ENV{HOME}/bin/grpc_cpp_plugin
        --plugin=protoc-gen-grpc_python=$ENV{HOME}/bin/grpc_python_plugin
        --plugin=protoc-gen-go=$ENV{GOPATH}/bin/protoc-gen-go
        --plugin=protoc-gen-go_grpc=$ENV{GOPATH}/bin/protoc-gen-go-grpc
        --plugin=protoc-gen-dart=$ENV{HOME}/.pub-cache/bin/protoc-gen-dart

        -I ${CMAKE_CURRENT_SOURCE_DIR}
        social_network.proto
    DEPENDS social_network.proto
)

add_executable(
    profile_service
    ${SOURCES}
    ${PROTO_SOURCES}
    ${GRPC_SOURCES}
)
target_link_libraries(profile_service ${PROTOBUF_LDFLAGS} ${GRPC_CXX_LDFLAGS})
