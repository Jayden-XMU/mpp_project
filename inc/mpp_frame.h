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

#ifndef __MPP_FRAME_H__
#define __MPP_FRAME_H__

#include "mpp_buffer.h"

typedef void* MppFrame;

/*
 * bit definition for mode flag in MppFrame
 */
/* progressive frame */
#define MPP_FRAME_FLAG_FRAME            (0x00000000)
/* top field only */
#define MPP_FRAME_FLAG_TOP_FIELD        (0x00000001)
/* bottom field only */
#define MPP_FRAME_FLAG_BOT_FIELD        (0x00000002)
/* paired field */
#define MPP_FRAME_FLAG_PAIRED_FIELD     (MPP_FRAME_FLAG_TOP_FIELD|MPP_FRAME_FLAG_BOT_FIELD)
/* paired field with field order of top first */
#define MPP_FRAME_FLAG_TOP_FIRST        (0x00000004)
/* paired field with field order of bottom first */
#define MPP_FRAME_FLAG_BOT_FIRST        (0x00000008)
/* paired field with unknown field order (MBAFF) */
#define MPP_FRAME_FLAG_DEINTERLACED     (MPP_FRAME_FLAG_TOP_FIRST|MPP_FRAME_FLAG_BOT_FIRST)
#define MPP_FRAME_FLAG_FIELD_ORDER_MASK (0x0000000C)
// for multiview stream
#define MPP_FRAME_FLAG_VIEW_ID_MASK     (0x000000f0)


/*
 * MPEG vs JPEG YUV range.
 */
typedef enum {
    MPP_FRAME_RANGE_UNSPECIFIED = 0,
    MPP_FRAME_RANGE_MPEG        = 1,    ///< the normal 219*2^(n-8) "MPEG" YUV ranges
    MPP_FRAME_RANGE_JPEG        = 2,    ///< the normal     2^n-1   "JPEG" YUV ranges
    MPP_FRAME_RANGE_NB,                 ///< Not part of ABI
} MppFrameColorRange;

/*
 * Chromaticity coordinates of the source primaries.
 */
typedef enum {
    MPP_FRAME_PRI_RESERVED0   = 0,
    MPP_FRAME_PRI_BT709       = 1,      ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
    MPP_FRAME_PRI_UNSPECIFIED = 2,
    MPP_FRAME_PRI_RESERVED    = 3,
    MPP_FRAME_PRI_BT470M      = 4,      ///< also FCC Title 47 Code of Federal Regulations 73.682 (a)(20)

    MPP_FRAME_PRI_BT470BG     = 5,      ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM
    MPP_FRAME_PRI_SMPTE170M   = 6,      ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
    MPP_FRAME_PRI_SMPTE240M   = 7,      ///< functionally identical to above
    MPP_FRAME_PRI_FILM        = 8,      ///< colour filters using Illuminant C
    MPP_FRAME_PRI_BT2020      = 9,      ///< ITU-R BT2020
    MPP_FRAME_PRI_NB,                   ///< Not part of ABI
} MppFrameColorPrimaries;

/*
 * Color Transfer Characteristic.
 */
typedef enum {
    MPP_FRAME_TRC_RESERVED0    = 0,
    MPP_FRAME_TRC_BT709        = 1,     ///< also ITU-R BT1361
    MPP_FRAME_TRC_UNSPECIFIED  = 2,
    MPP_FRAME_TRC_RESERVED     = 3,
    MPP_FRAME_TRC_GAMMA22      = 4,     ///< also ITU-R BT470M / ITU-R BT1700 625 PAL & SECAM
    MPP_FRAME_TRC_GAMMA28      = 5,     ///< also ITU-R BT470BG
    MPP_FRAME_TRC_SMPTE170M    = 6,     ///< also ITU-R BT601-6 525 or 625 / ITU-R BT1358 525 or 625 / ITU-R BT1700 NTSC
    MPP_FRAME_TRC_SMPTE240M    = 7,
    MPP_FRAME_TRC_LINEAR       = 8,     ///< "Linear transfer characteristics"
    MPP_FRAME_TRC_LOG          = 9,     ///< "Logarithmic transfer characteristic (100:1 range)"
    MPP_FRAME_TRC_LOG_SQRT     = 10,    ///< "Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range)"
    MPP_FRAME_TRC_IEC61966_2_4 = 11,    ///< IEC 61966-2-4
    MPP_FRAME_TRC_BT1361_ECG   = 12,    ///< ITU-R BT1361 Extended Colour Gamut
    MPP_FRAME_TRC_IEC61966_2_1 = 13,    ///< IEC 61966-2-1 (sRGB or sYCC)
    MPP_FRAME_TRC_BT2020_10    = 14,    ///< ITU-R BT2020 for 10 bit system
    MPP_FRAME_TRC_BT2020_12    = 15,    ///< ITU-R BT2020 for 12 bit system
    MPP_FRAME_TRC_NB,                   ///< Not part of ABI
} MppFrameColorTransferCharacteristic;

/*
 * YUV colorspace type.
 */
typedef enum {
    MPP_FRAME_SPC_RGB         = 0,      ///< order of coefficients is actually GBR, also IEC 61966-2-1 (sRGB)
    MPP_FRAME_SPC_BT709       = 1,      ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    MPP_FRAME_SPC_UNSPECIFIED = 2,
    MPP_FRAME_SPC_RESERVED    = 3,
    MPP_FRAME_SPC_FCC         = 4,      ///< FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    MPP_FRAME_SPC_BT470BG     = 5,      ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
    MPP_FRAME_SPC_SMPTE170M   = 6,      ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC / functionally identical to above
    MPP_FRAME_SPC_SMPTE240M   = 7,
    MPP_FRAME_SPC_YCOCG       = 8,      ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    MPP_FRAME_SPC_BT2020_NCL  = 9,      ///< ITU-R BT2020 non-constant luminance system
    MPP_FRAME_SPC_BT2020_CL   = 10,     ///< ITU-R BT2020 constant luminance system
    MPP_FRAME_SPC_NB,                   ///< Not part of ABI
} MppFrameColorSpace;

/*
 * Location of chroma samples.
 *
 * Illustration showing the location of the first (top left) chroma sample of the
 * image, the left shows only luma, the right
 * shows the location of the chroma sample, the 2 could be imagined to overlay
 * each other but are drawn separately due to limitations of ASCII
 *
 *                1st 2nd       1st 2nd horizontal luma sample positions
 *                 v   v         v   v
 *                 ______        ______
 *1st luma line > |X   X ...    |3 4 X ...     X are luma samples,
 *                |             |1 2           1-6 are possible chroma positions
 *2nd luma line > |X   X ...    |5 6 X ...     0 is undefined/unknown position
 */
typedef enum {
    MPP_CHROMA_LOC_UNSPECIFIED = 0,
    MPP_CHROMA_LOC_LEFT        = 1,     ///< mpeg2/4 4:2:0, h264 default for 4:2:0
    MPP_CHROMA_LOC_CENTER      = 2,     ///< mpeg1 4:2:0, jpeg 4:2:0, h263 4:2:0
    MPP_CHROMA_LOC_TOPLEFT     = 3,     ///< ITU-R 601, SMPTE 274M 296M S314M(DV 4:1:1), mpeg2 4:2:2
    MPP_CHROMA_LOC_TOP         = 4,
    MPP_CHROMA_LOC_BOTTOMLEFT  = 5,
    MPP_CHROMA_LOC_BOTTOM      = 6,
    MPP_CHROMA_LOC_NB,                  ///< Not part of ABI
} MppFrameChromaLocation;

#define MPP_FRAME_FMT_MASK    0x000f0000
#define MPP_FRAME_FMT_YUV     0x00000000
#define MPP_FRAME_FMT_RGB     0x00010000

/*
 *mpp color format define
 */
typedef enum {
    MPP_FMT_YUV420SP        = MPP_FRAME_FMT_YUV,        /* YYYY... UVUVUV...        */
    MPP_FMT_YUV420SP_10BIT,
    MPP_FMT_YUV422SP,                                   /* YYYY... UVUVUV...        */
    MPP_FMT_YUV422SP_10BIT,                             ///< Not part of ABI
    MPP_FMT_YUV420P,                                    /* YYYY... UUUU... VVVV     */
    MPP_FMT_YUV420SP_VU,                                /* YYYY... VUVUVU...        */
    MPP_FMT_YUV422P,                                    /* YYYY... UUUU... VVVV     */
    MPP_FMT_YUV422SP_VU,                                /* YYYY... VUVUVU...        */
    MPP_FMT_YUV422_YUYV,                                /* YUYVYUYV...              */
    MPP_FMT_YUV422_UYVY,                                /* UYVYUYVY...              */
    MPP_FMT_YUV_BUTT,
    MPP_FMT_RGB565          = MPP_FRAME_FMT_RGB,        /* 16-bit RGB               */
    MPP_FMT_BGR565,                                     /* 16-bit RGB               */
    MPP_FMT_RGB555,                                     /* 15-bit RGB               */
    MPP_FMT_BGR555,                                     /* 15-bit RGB               */
    MPP_FMT_RGB444,                                     /* 12-bit RGB               */
    MPP_FMT_BGR444,                                     /* 12-bit RGB               */
    MPP_FMT_RGB888,                                     /* 24-bit RGB               */
    MPP_FMT_BGR888,                                     /* 24-bit RGB               */
    MPP_FMT_RGB101010,                                  /* 30-bit RGB               */
    MPP_FMT_BGR101010,                                  /* 30-bit RGB               */
    MPP_FMT_ARGB8888,                                   /* 32-bit RGB               */
    MPP_FMT_ABGR8888,                                   /* 32-bit RGB               */
    MPP_FMT_RGB_BUTT,
    MPP_FMT_BUTT            = MPP_FMT_RGB_BUTT,
} MppFrameFormat;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MppFrame interface
 */
MPP_RET mpp_frame_init(MppFrame *frame);
MPP_RET mpp_frame_deinit(MppFrame *frame);
MppFrame mpp_frame_get_next(MppFrame frame);

/*
 * normal parameter
 */
RK_U32  mpp_frame_get_width(const MppFrame frame);
void    mpp_frame_set_width(MppFrame frame, RK_U32 width);
RK_U32  mpp_frame_get_height(const MppFrame frame);
void    mpp_frame_set_height(MppFrame frame, RK_U32 height);
RK_U32  mpp_frame_get_hor_stride(const MppFrame frame);
void    mpp_frame_set_hor_stride(MppFrame frame, RK_U32 hor_stride);
RK_U32  mpp_frame_get_ver_stride(const MppFrame frame);
void    mpp_frame_set_ver_stride(MppFrame frame, RK_U32 ver_stride);
RK_U32  mpp_frame_get_mode(const MppFrame frame);
void    mpp_frame_set_mode(MppFrame frame, RK_U32 mode);
RK_U32  mpp_frame_get_discard(const MppFrame frame);
void    mpp_frame_set_discard(MppFrame frame, RK_U32 discard);
RK_U32  mpp_frame_get_viewid(const MppFrame frame);
void    mpp_frame_set_viewid(MppFrame frame, RK_U32 viewid);
RK_U32  mpp_frame_get_poc(const MppFrame frame);
void    mpp_frame_set_poc(MppFrame frame, RK_U32 poc);
RK_S64  mpp_frame_get_pts(const MppFrame frame);
void    mpp_frame_set_pts(MppFrame frame, RK_S64 pts);
RK_S64  mpp_frame_get_dts(const MppFrame frame);
void    mpp_frame_set_dts(MppFrame frame, RK_S64 dts);
RK_U32  mpp_frame_get_errinfo(const MppFrame frame);
void    mpp_frame_set_errinfo(MppFrame frame, RK_U32 errinfo);
/*
 * flow control parmeter
 */
RK_U32  mpp_frame_get_eos(const MppFrame frame);
void    mpp_frame_set_eos(MppFrame frame, RK_U32 eos);
RK_U32  mpp_frame_get_info_change(const MppFrame frame);
void    mpp_frame_set_info_change(MppFrame frame, RK_U32 info_change);

/*
 * buffer parameter
 */
MppBuffer mpp_frame_get_buffer(const MppFrame frame);
void    mpp_frame_set_buffer(MppFrame frame, MppBuffer buffer);

/*
 * color related parameter
 */
MppFrameColorRange mpp_frame_get_color_range(const MppFrame frame);
void    mpp_frame_set_color_range(MppFrame frame, MppFrameColorRange color_range);
MppFrameColorPrimaries mpp_frame_get_color_primaries(const MppFrame frame);
void    mpp_frame_set_color_primaries(MppFrame frame, MppFrameColorPrimaries color_primaries);
MppFrameColorTransferCharacteristic mpp_frame_get_color_trc(const MppFrame frame);
void    mpp_frame_set_color_trc(MppFrame frame, MppFrameColorTransferCharacteristic color_trc);
MppFrameColorSpace mpp_frame_get_colorspace(const MppFrame frame);
void    mpp_frame_set_colorspace(MppFrame frame, MppFrameColorSpace colorspace);
MppFrameChromaLocation mpp_frame_get_chroma_location(const MppFrame frame);
void    mpp_frame_set_chroma_location(MppFrame frame, MppFrameChromaLocation chroma_location);
MppFrameFormat mpp_frame_get_fmt(MppFrame frame);
void    mpp_frame_set_fmt(MppFrame frame, MppFrameFormat fmt);


/*
 * HDR parameter
 */

#ifdef __cplusplus
}
#endif

#endif /*__MPP_FRAME_H__*/
