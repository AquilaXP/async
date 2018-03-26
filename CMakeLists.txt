cmake_minimum_required(VERSION 3.2)

set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
set( DOC_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/Doc )
project(async VERSION 0.0.${PROJECT_VERSION_PATCH})

configure_file(version.h.in version.h @ONLY)

set(SRC
    async.cpp
    bulkmt/IState.cpp
    bulkmt/ISubject.cpp
    )

set(HDR
    async.h
    service.h
    SubjectRawData.h
    bulkmt/IState.h
    bulkmt/IObserver.h
    bulkmt/ISubject.h
    bulkmt/threadsafe_queue.h
    bulkmt/threadsafe_queue_const.h
    bulkmt/ConsoleObserver.h
    bulkmt/FileObserver.h
    bulkmt/Statistic.h
    bulkmt/ThreadObserver.h
    bulkmt/FileObserverMT.h
    bulkmt/IStreamSubject.h
    bulkmt/pack_cmd.h
    )

#library async
add_library(async SHARED ${SRC} ${HDR})

target_include_directories(async PRIVATE ${CMAKE_CURRENT_BINARY_DIR} bulkmt )

set_target_properties( async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall
    COMPILE_DEFINITIONS BUILDING_DLL
)
#program test_async
add_executable(test_async main.cpp)

target_include_directories(test_async PRIVATE ${CMAKE_CURRENT_BINARY_DIR} bulkmt )

target_link_libraries(test_async 
    async
)

set_target_properties( test_async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall
)

if(UNIX)

set_target_properties( async PROPERTIES
    COMPILE_OPTIONS -pthread
)
target_link_libraries( async pthread )

set_target_properties( test_async PROPERTIES
    COMPILE_OPTIONS -pthread
)
target_link_libraries( test_async pthread )

endif(UNIX)

if(WIN32)
  install(TARGETS async
    RUNTIME DESTINATION bin)
else()
  install(TARGETS async
    LIBRARY DESTINATION lib)
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT aquila-2004@yandex.ru)

include(CPack)
