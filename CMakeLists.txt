cmake_minimum_required(VERSION 3.16)
project(clientTCP LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp

    include/tcpclient.h
    src/tcpclient.cpp
    resources/Anonymous_Pro.ttf
    include/gui.h
    src/gui.cpp
    src/inetgui.cpp
    include/inetgui.h
    include/pckg.h
)
target_link_libraries(main PRIVATE sfml-graphics wsock32 ws2_32 fwpuclnt ntdsapi)
target_compile_features(main PRIVATE cxx_std_17)

#TODO: make that ttf file can be appload there
if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()

#UDP - SOCK_DGRAM
#TCP - SOCK_STREAM
#??? - SOCK_RAW
#binding - giving a name (address) to socket
#bind() - set socket address, getsockname - get socket address
#AF_ - address family
    #AF_LOCAL, AF_UNIX, AF_FILE - synonims, local namespace
    #AF_INET - IPv4
    #AF_INET6 - IPv6
    #AF_UNSPEC -
#PF_ - protocol family
#It`s for LINUX...


#Server
#Initialize Winsock.
#Create a socket.
#Bind the socket.
#Listen on the socket for a client.
#Accept a connection from a client.
#Receive and send data.
#Disconnect.
#
#Client
#Initialize Winsock.
#Create a socket.
#Connect to the server.
#Send and receive data.
#Disconnect.

#33 символа - длина пароля
#33 символа - длина имени
