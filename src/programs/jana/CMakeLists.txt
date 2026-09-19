

add_executable(jana jana.cc )

find_package(Threads REQUIRED)
target_link_libraries(jana jana2_shared_lib)
target_link_options(jana PRIVATE -rdynamic)
install(TARGETS jana DESTINATION bin)


if(EMSCRIPTEN)
    install(FILES $<TARGET_FILE_DIR:jana>/jana.wasm DESTINATION bin)
    install(FILES $<TARGET_FILE_DIR:jana>/jana.worker.js DESTINATION bin OPTIONAL)
endif()
