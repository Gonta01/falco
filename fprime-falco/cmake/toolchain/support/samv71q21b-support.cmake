function(finalize_sbc_executable)
    # Add link dependency on
    if (APP_MODE STREQUAL "BOOT")
        target_link_libraries(
            "${FPRIME_CURRENT_MODULE}"
            PUBLIC HAL_MMCU_BOOT
        )
    else()
        target_link_libraries(
            "${FPRIME_CURRENT_MODULE}"
            PUBLIC HAL_MMCU
        )
    endif()

    set_target_properties(
        ${FPRIME_CURRENT_MODULE}
            PROPERTIES
            SUFFIX ".elf"
    )

    # Generate *.map file for the *.elf file
    target_link_options(${FPRIME_CURRENT_MODULE} PRIVATE
            -Wl,-Map=${FPRIME_CURRENT_MODULE}.map
    )

    
    add_custom_command(TARGET "${FPRIME_CURRENT_MODULE}" 
    POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${FPRIME_CURRENT_MODULE}>" "${FPRIME_CURRENT_MODULE}.bin"
    COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${FPRIME_CURRENT_MODULE}>" "${FPRIME_CURRENT_MODULE}.hex"
    COMMAND "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} -t install
    )
    
    install(FILES ${CMAKE_BINARY_DIR}/${FPRIME_CURRENT_MODULE}/${FPRIME_CURRENT_MODULE}.map DESTINATION ${TOOLCHAIN_NAME}/${FPRIME_CURRENT_MODULE}/bin)
    install(FILES ${CMAKE_BINARY_DIR}/${FPRIME_CURRENT_MODULE}/${FPRIME_CURRENT_MODULE}.bin DESTINATION ${TOOLCHAIN_NAME}/${FPRIME_CURRENT_MODULE}/bin)
    install(FILES ${CMAKE_BINARY_DIR}/${FPRIME_CURRENT_MODULE}/${FPRIME_CURRENT_MODULE}.hex DESTINATION ${TOOLCHAIN_NAME}/${FPRIME_CURRENT_MODULE}/bin)
    
endfunction(finalize_sbc_executable)