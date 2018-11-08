# vim: syntax=cmake
set(RMVBD_PARSER_HDR
    rmvbd_parser.h
    )

set(RMVBD_PARSER_SRC
    rmvbd_parser.c

    )

add_library(${CODEC_RMVBD} STATIC
    ${RMVBD_PARSER_SRC}
    ${RMVBD_PARSER_HDR}
    )

set_target_properties(${CODEC_RMVBD} PROPERTIES FOLDER "mpp/codec")
target_link_libraries(${CODEC_RMVBD} mpp_base)

#add_subdirectory(test)
