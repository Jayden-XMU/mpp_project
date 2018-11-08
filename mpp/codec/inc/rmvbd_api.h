/*
 * Copyright 2015 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RMVBD_API_H__
#define __RMVBD_API_H__

#include "parser_api.h"


enum {
    RV_I_PIC = 0,
    RV_FI_PIC = 1,
    RV_P_PIC = 2,
    RV_B_PIC = 3
};


extern RK_U32 rmvb_debug;

#define RMVB_DBG_FUNCTION          (0x00000001)
#define RMVB_DBG_REG               (0x00000002)
#define RMVB_INF_LOG          (0x00000004)
#define RMVB_ERR_LOG               (0x00000008)

#define RMVB_DBG_ASSERT               (1)

#define DEC_X170_LITTLE_ENDIAN      1
#define DEC_X170_BIG_ENDIAN     0

#define DEC_X170_BUS_BURST_LENGTH_UNDEFINED        0
#define DEC_X170_BUS_BURST_LENGTH_4                4
#define DEC_X170_BUS_BURST_LENGTH_8                8
#define DEC_X170_BUS_BURST_LENGTH_16               16


#define CODEC_HEADER_DATA       0x10
#define CODEC_FRAME_DATA            0x20

#define RV_SKIP_TO_KEY_FRAME    1




#define RMFUN_TEST(tag) \
do {\
if (RMVB_DBG_FUNCTION & rmvb_debug)\
{ mpp_log("%s: line(%d), func(%s)", tag, __LINE__, __FUNCTION__); }\
} while (0)




#define RMVB_ASSERT(val)\
do {\
if (RMVB_DBG_ASSERT)\
{ mpp_assert(val); }\
} while (0)


#define RMVB_INFO_LOG(fmt, ...) \
do {\
if (RMVB_INF_LOG & rmvb_debug)\
{ mpp_log("[Info] func(%s), line(%d), "fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__); }\
} while (0)



#define RMVB_ERROR_LOG(fmt, ...) \
do {\
if (RMVB_ERR_LOG & rmvb_debug)\
{ mpp_log("[Error] func(%s), line(%d), "fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__); }\
} while (0)



/*

typedef struct rmvbdParser_Contex{
    rmvb_Syntax* rmvbsyn;
    RK_U32          framesize;
    RK_U32          slicenum;
    MppPacket       task_pkt;
    RK_U32          isRV8;
    RK_U8           *stream;
    size_t          stream_size;
    RK_U32          pic_rpr_num;
    RK_U32          pic_rpr_size[2 * 9];
    RK_U32          display_width;
    RK_U32          display_height;
    RK_U32          decode_width;
    RK_U32          decode_height;
    RK_U32          need_split;
    RK_U32          frame_width;
    RK_U32          frame_height;
    IOInterruptCB   notify_cb;
    RK_U32          mb_width;
    RK_U32          mb_height;
    RK_U32          ref_frame_cnt;
    RK_U32          max_stream_size;
    RK_U32              TRWrap;
    MppBufSlots packet_slots;
    MppBufSlots     frame_slots;
    RVFrameHead     Framehead[3];
    RVFrameHead     *frame_ref0;
    RVFrameHead     *frame_ref1;
    RVFrameHead     *frame_cur;
    RK_U32 eos;
    BitReadCtx_t    *bit_ctx;
    RK_U32         needSkipToKeyFrm;
    RK_U32          internal_pts;
    RK_S32          pos_frm_start;      // negtive - not found; non-negtive - position of frame start
    RK_S32          pos_frm_end;        // negtive - not found; non-negtive - position of frame end
    RK_S32          output;
    RK_S64          pts;
    RK_U32          decode_width_no_alignment;
    RK_U32          decode_height_no_alignment;
    RK_U32               getFromhd;
    rmvb_Syntax*syntax;
} rmvbdParserContex;



*/





#ifdef  __cplusplus
extern "C" {
#endif

extern const ParserApi api_rmvbd_parser;


#ifdef  __cplusplus
}
#endif

#endif /*__RMVBD_API_H__*/