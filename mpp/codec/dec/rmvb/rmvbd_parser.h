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

/*
 * @file        rmvbd_parser.h
 * @brief
 * @author      dzx(princejay.dai@rock-chips.com)

 * @version     1.0.0
 * @history
 *   2016.10.15 : Create
 */

#ifndef __RMVBD_PARSER_H__
#define __RMVBD_PARSER_H__

#include "mpp_common.h"
#include "mpp_mem.h"
#include "mpp_bitread.h"
#include "mpp_buf_slot.h"
#include "mpp_frame.h"
#include "mpp_packet.h"
#include "parser_api.h"
#include "mpp_dec.h"
#include "mpp_err.h"
#include "rmvbd_syntax.h"




#define RMVBD_DBG_FUNCTION          (0x00000001)
#define RMVBD_DBG_STARTCODE         (0x00000002)
#define RMVBD_DBG_BITS              (0x00000004)
#define RMVBD_DBG_STATUS            (0x00000008)
#define RMVBD_DBG_TIME              (0x00000100)
#define RMVBD_STREAM_BUFF_SIZE (512*1024)
//#define   Get32bit(buff)      ((buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|buff[3])
//#define   Get16bit(buff)      ((buff[0]<<8)|buff[1])

#define VPU_BITSTREAM_START_CODE (0x42564b52)


/* Audio codec defines */
#define RA_FORMAT_ID        0x2E7261FD  /* RealAudio Stream */
#define RA_NO_INTERLEAVER   0x496E7430  /* 'Int0' (no interleaver) */
#define RA_INTERLEAVER_SIPR 0x73697072  /* interleaver used for SIPRO codec ("sipr") */
#define RA_INTERLEAVER_GENR 0x67656E72  /* interleaver used for ra8lbr and ra8hbr codecs ("genr") */
#define RA_INTERLEAVER_VBRS 0x76627273  /* Simple VBR interleaver ("vbrs") */
#define RA_INTERLEAVER_VBRF 0x76627266  /* Simple VBR interleaver (with possibly fragmenting) ("vbrf") */

/* Video codec defines */
#define HX_MEDIA_AUDIO    0x4155444FL /* 'AUDO' */
#define HX_MEDIA_VIDEO    0x5649444FL /* 'VIDO' */

#define HX_RVTRVIDEO_ID   0x52565452  /* 'RVTR' (for rv20 codec) */
#define HX_RVTR_RV30_ID   0x52565432  /* 'RVT2' (for rv30 codec) */
#define HX_RV20VIDEO_ID   0x52563230  /* 'RV20' */
#define HX_RV30VIDEO_ID   0x52563330  /* 'RV30' */
#define HX_RV40VIDEO_ID   0x52563430  /* 'RV40' */
#define HX_RVG2VIDEO_ID   0x52564732  /* 'RVG2' (raw TCK format) */
#define HX_RV89COMBO_ID   0x54524F4D  /* 'TROM' (raw TCK format) */

#define RV20_MAJOR_BITSTREAM_VERSION  2

#define RV30_MAJOR_BITSTREAM_VERSION  3
#define RV30_BITSTREAM_VERSION        2
#define RV30_PRODUCT_RELEASE          0
#define RV30_FRONTEND_VERSION         2

#define RV40_MAJOR_BITSTREAM_VERSION  4
#define RV40_BITSTREAM_VERSION        0
#define RV40_PRODUCT_RELEASE          2
#define RV40_FRONTEND_VERSION         0

#define RAW_BITSTREAM_MINOR_VERSION   128

#define RAW_RVG2_MAJOR_VERSION        2
#define RAW_RVG2_BITSTREAM_VERSION    RAW_BITSTREAM_MINOR_VERSION
#define RAW_RVG2_PRODUCT_RELEASE      0
#define RAW_RVG2_FRONTEND_VERSION     0

#define RAW_RV8_MAJOR_VERSION         3
#define RAW_RV8_BITSTREAM_VERSION     RAW_BITSTREAM_MINOR_VERSION
#define RAW_RV8_PRODUCT_RELEASE       0
#define RAW_RV8_FRONTEND_VERSION      0

#define RAW_RV9_MAJOR_VERSION         4
#define RAW_RV9_BITSTREAM_VERSION     RAW_BITSTREAM_MINOR_VERSION
#define RAW_RV9_PRODUCT_RELEASE       0
#define RAW_RV9_FRONTEND_VERSION      0

#define RV40_SPO_BITS_NUMRESAMPLE_IMAGES    0x00070000  /* max of 8 RPR images sizes */
#define RV40_SPO_BITS_NUMRESAMPLE_IMAGES_SHIFT 16

#define HX_GET_MAJOR_VERSION(prodVer)   ((prodVer >> 28) & 0xF)
#define HX_GET_MINOR_VERSION(prodVer)   ((prodVer >> 20) & 0xFF)

#define END_OF_STREAM               0xFFFFFFFF




typedef void* rmvbdParser;

/*
enum
{
    RV_I_PIC = 0,
    RV_FI_PIC = 1,
    RV_P_PIC = 2,
    RV_B_PIC = 3
};

*/

typedef struct rmvbdContext {


    RK_S32      frame_slot_index; /* slot index for output */

    RK_U32 streamLength;   /* input stream length or buffer size */
    RK_U32 bufferSize; /* input stream buffer size */
    RK_U32 decImageType;   /* Full image or Thumbnail to be decoded */
    MppFrameFormat output_fmt;//mpp color format define
    //RVFrameHead rmFrmhd;
    MppPacket input_packet;
    MppFrame output_frame;


    //MppPacket       task_pkt;
    RK_S32  frameWidth, frameheight;
    RK_S64 pts;
    RK_U32 eos;
    RK_U32 parser_debug_enable;
    // runtime parameter

    RK_U32          frame_count;
    RK_U32          internal_pts;
    rmvbdParser     parser;
} rm_Ctx;

typedef struct rmvbdParser_Contex {
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

//  RK_S32          pos_frm_start;      // negtive - not found; non-negtive - position of frame start
//  RK_S32          pos_frm_end;        // negtive - not found; non-negtive - position of frame end

    RK_S32          output;
    RK_S64          pts;
    RK_U32          decode_width_no_alignment;
    RK_U32          decode_height_no_alignment;
    RK_U32               getFromhd;
    FILE    *fp_dbg_file;
    RK_U32   frame_no;
} rmvbdParserContex;

MPP_RET  rmvbd_parser_init   (void *ctx, ParserCfg *cfg);
MPP_RET  rmvbd_parser_deinit(void *ctx);
MPP_RET  rmvbd_parser_reset(void *ctx);
MPP_RET  rmvbd_parser_flush(void *ctx);
MPP_RET  rmvbd_parser_control(void *ctx, RK_S32 cmd_type, void *param);
MPP_RET  rmvbd_parser_prepare(void *ctx, MppPacket pkt, HalDecTask *task);
MPP_RET  rmvbd_parser_parse(void *ctx, HalDecTask *task);
MPP_RET  rmvbd_parser_callback(void *ctx, void *err_info);



MPP_RET rmvbd_parser_split_frame(RK_U8 *src, RK_U32 src_size, RK_U8 *dst, RK_U32 *dst_size);




#endif

