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

#ifndef __RMVBD_SYNTAX_H__
#define __RMVBD_SYNTAX_H__

#include "rk_type.h"
#include "vpu_api.h"
#include "mpp_frame.h"
/*
typedef struct {
    RK_U32   TimeLow;
    RK_U32   TimeHigh;
} TIME_STAMP;

typedef struct tVPU_FRAME {
    RK_U32          FrameBusAddr[2];       //0: Y address; 1: UV address;
    RK_U32          FrameWidth;         //16X对齐宽度
    RK_U32          FrameHeight;        //16X对齐高度
    RK_U32          OutputWidth;        //非16X必须
    RK_U32          OutputHeight;       //非16X必须
    RK_U32          DisplayWidth;       //显示宽度
    RK_U32          DisplayHeight;      //显示高度
    RK_U32          CodingType;
    RK_U32          FrameType;          //frame;top_field_first;bot_field_first
    RK_U32          ColorType;
    RK_U32          DecodeFrmNum;
    TIME_STAMP      ShowTime;
    RK_U32          ErrorInfo;          //该帧的错误信息，返回给系统方便调试
    RK_U32          employ_cnt;
    struct tVPU_FRAME *    next_frame;
    RK_U32          Res[4];
} VPU_FRAME;

*/

typedef struct _DXVA_PicEntry_rmv {
    union {
        struct {
            RK_U8 Index7Bits : 7;
            RK_U8 AssociatedFlag : 1;
        };
        RK_U8 bPicEntry;
    };
} DXVA_PicEntry_rmv;




typedef struct RV_FrameHead {
    RK_U32          frameNumber;
    RK_U32          tr;
    RK_U32          picCodingType;
    RK_U32          totalMbInFrame;
    RK_U32          frameWidth; /* in macro blocks */
    RK_U32          frameHeight;    /* in macro blocks */
    RK_U32          mb_width;
    RK_U32          mb_height;
    RK_U32          vlcSet;
    RK_U32          qp;
    MppFrame        f;

    RK_S32          slot_index;
    RK_U32          flags;
    RK_U32          frame_codelen;
    VPU_FRAME       *frame_space;
} RVFrameHead;

typedef struct RmvbdSyntaxParam {
    RK_U32          qp;
    RK_U32          framesize;
    RK_U32          slicenum;
    RK_U32          mb_width;
    RK_U32          mb_height;
    RK_U32          TRWrap;
    //VPUMemLinear_t    bitstr;
    DXVA_PicEntry_rmv   CurrPic;
    DXVA_PicEntry_rmv   frame_refs[4];
    RK_U32   decode_width_no_alignment;
    RK_U32   decode_height_no_alignment;
    RK_U32   display_width;
    RK_U32   display_height;
    RK_U32   decode_width;
    RK_U32   decode_height;
    RK_U32   pic_rpr_size[2 * 9];
    RK_U32   pic_rpr_num;
    RK_U32   frame_width;
    RK_U32   frame_height;
    RK_U32  isRV8;
    RVFrameHead Framehead[3];
    RVFrameHead *frame_ref0;
    RVFrameHead *frame_ref1;
    RVFrameHead *frame_cur;
} rmvb_Syntax;


#endif
