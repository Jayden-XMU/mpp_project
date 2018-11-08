#vim: syntax=cmake
include_directories(.)

#hal rmvb decoder api
set(HAL_RMVBD_API
   ../../inc/hal_rmvbd_api.h
   ../../../common/rmvbd_syntax.h
   )

#hal rmvb header
set(HAL_RMVBD_APID_HDR
   hal_rmvbd_reg.h
   )

#hal rmvb decoder sourse
set(HAL_RMVBD_SRC
   hal_rmvbd_reg.c
   )

add_library(${HAL_RMVBD} STATIC
   ${HAL_RMVBD_API}
   ${HAL_RMVBD_HDR}
   ${HAL_RMVBD_SRC}
   )

target_link_libraries(${HAL_RMVBD} mpp_base)
set_target_properties(${HAL_RMVBD} PROPERTIES FOLDER "mpp/hal")

