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

#define MODULE_TAG "hal_vp8d_reg"
#include <string.h>

#include "mpp_log.h"
#include "mpp_env.h"
#include "mpp_mem.h"

#include "vpu.h"
#include "hal_vp8d_reg.h"


RK_U32 vp8h_debug = 0;

#define VP8HWD_VP7             1
#define VP8HWD_VP8             2
#define VP8HWD_WEBP            3

#define DEC_MODE_VP7           9
#define DEC_MODE_VP8           10

#define VP8D_PROB_TABLE_SIZE  (1<<16) /* TODO */
#define VP8D_MAX_SEGMAP_SIZE  (2048 + 1024)  //1920*1080 /* TODO */
#define FUN_T(tag) \
    do {\
        if (VP8H_DBG_FUNCTION & vp8h_debug)\
            { mpp_log("%s: line(%d), func(%s)", tag, __LINE__, __FUNCTION__); }\
    } while (0)

/* VP7 QP LUTs */
static const RK_U16 YDcQLookup[128] = {
    4,    4,    5,    6,    6,    7,    8,    8,
    9,   10,   11,   12,   13,   14,   15,   16,
    17,   18,   19,   20,   21,   22,   23,   23,
    24,   25,   26,   27,   28,   29,   30,   31,
    32,   33,   33,   34,   35,   36,   36,   37,
    38,   39,   39,   40,   41,   41,   42,   43,
    43,   44,   45,   45,   46,   47,   48,   48,
    49,   50,   51,   52,   53,   53,   54,   56,
    57,   58,   59,   60,   62,   63,   65,   66,
    68,   70,   72,   74,   76,   79,   81,   84,
    87,   90,   93,   96,  100,  104,  108,  112,
    116,  121,  126,  131,  136,  142,  148,  154,
    160,  167,  174,  182,  189,  198,  206,  215,
    224,  234,  244,  254,  265,  277,  288,  301,
    313,  327,  340,  355,  370,  385,  401,  417,
    434,  452,  470,  489,  509,  529,  550,  572
};

static const RK_U16 YAcQLookup[128] = {
    4,    4,    5,    5,    6,    6,    7,    8,
    9,   10,   11,   12,   13,   15,   16,   17,
    19,   20,   22,   23,   25,   26,   28,   29,
    31,   32,   34,   35,   37,   38,   40,   41,
    42,   44,   45,   46,   48,   49,   50,   51,
    53,   54,   55,   56,   57,   58,   59,   61,
    62,   63,   64,   65,   67,   68,   69,   70,
    72,   73,   75,   76,   78,   80,   82,   84,
    86,   88,   91,   93,   96,   99,  102,  105,
    109,  112,  116,  121,  125,  130,  135,  140,
    146,  152,  158,  165,  172,  180,  188,  196,
    205,  214,  224,  234,  245,  256,  268,  281,
    294,  308,  322,  337,  353,  369,  386,  404,
    423,  443,  463,  484,  506,  529,  553,  578,
    604,  631,  659,  688,  718,  749,  781,  814,
    849,  885,  922,  960, 1000, 1041, 1083, 1127

};

static const RK_U16 Y2DcQLookup[128] = {
    7,    9,   11,   13,   15,   17,   19,   21,
    23,   26,   28,   30,   33,   35,   37,   39,
    42,   44,   46,   48,   51,   53,   55,   57,
    59,   61,   63,   65,   67,   69,   70,   72,
    74,   75,   77,   78,   80,   81,   83,   84,
    85,   87,   88,   89,   90,   92,   93,   94,
    95,   96,   97,   99,  100,  101,  102,  104,
    105,  106,  108,  109,  111,  113,  114,  116,
    118,  120,  123,  125,  128,  131,  134,  137,
    140,  144,  148,  152,  156,  161,  166,  171,
    176,  182,  188,  195,  202,  209,  217,  225,
    234,  243,  253,  263,  274,  285,  297,  309,
    322,  336,  350,  365,  381,  397,  414,  432,
    450,  470,  490,  511,  533,  556,  579,  604,
    630,  656,  684,  713,  742,  773,  805,  838,
    873,  908,  945,  983, 1022, 1063, 1105, 1148
};

static const RK_U16 Y2AcQLookup[128] = {
    7,    9,   11,   13,   16,   18,   21,   24,
    26,   29,   32,   35,   38,   41,   43,   46,
    49,   52,   55,   58,   61,   64,   66,   69,
    72,   74,   77,   79,   82,   84,   86,   88,
    91,   93,   95,   97,   98,  100,  102,  104,
    105,  107,  109,  110,  112,  113,  115,  116,
    117,  119,  120,  122,  123,  125,  127,  128,
    130,  132,  134,  136,  138,  141,  143,  146,
    149,  152,  155,  158,  162,  166,  171,  175,
    180,  185,  191,  197,  204,  210,  218,  226,
    234,  243,  252,  262,  273,  284,  295,  308,
    321,  335,  350,  365,  381,  398,  416,  435,
    455,  476,  497,  520,  544,  569,  595,  622,
    650,  680,  711,  743,  776,  811,  848,  885,
    925,  965, 1008, 1052, 1097, 1144, 1193, 1244,
    1297, 1351, 1407, 1466, 1526, 1588, 1652, 1719
};

static const RK_U16 UvDcQLookup[128] = {
    4,    4,    5,    6,    6,    7,    8,    8,
    9,   10,   11,   12,   13,   14,   15,   16,
    17,   18,   19,   20,   21,   22,   23,   23,
    24,   25,   26,   27,   28,   29,   30,   31,
    32,   33,   33,   34,   35,   36,   36,   37,
    38,   39,   39,   40,   41,   41,   42,   43,
    43,   44,   45,   45,   46,   47,   48,   48,
    49,   50,   51,   52,   53,   53,   54,   56,
    57,   58,   59,   60,   62,   63,   65,   66,
    68,   70,   72,   74,   76,   79,   81,   84,
    87,   90,   93,   96,  100,  104,  108,  112,
    116,  121,  126,  131,  132,  132,  132,  132,
    132,  132,  132,  132,  132,  132,  132,  132,
    132,  132,  132,  132,  132,  132,  132,  132,
    132,  132,  132,  132,  132,  132,  132,  132,
    132,  132,  132,  132,  132,  132,  132,  132
};


static const RK_U16 UvAcQLookup[128] = {
    4,    4,    5,    5,    6,    6,    7,    8,
    9,   10,   11,   12,   13,   15,   16,   17,
    19,   20,   22,   23,   25,   26,   28,   29,
    31,   32,   34,   35,   37,   38,   40,   41,
    42,   44,   45,   46,   48,   49,   50,   51,
    53,   54,   55,   56,   57,   58,   59,   61,
    62,   63,   64,   65,   67,   68,   69,   70,
    72,   73,   75,   76,   78,   80,   82,   84,
    86,   88,   91,   93,   96,   99,  102,  105,
    109,  112,  116,  121,  125,  130,  135,  140,
    146,  152,  158,  165,  172,  180,  188,  196,
    205,  214,  224,  234,  245,  256,  268,  281,
    294,  308,  322,  337,  353,  369,  386,  404,
    423,  443,  463,  484,  506,  529,  553,  578,
    604,  631,  659,  688,  718,  749,  781,  814,
    849,  885,  922,  960, 1000, 1041, 1083, 1127
};

#define CLIP3(l, h, v) ((v) < (l) ? (l) : ((v) > (h) ? (h) : (v)))

static const RK_U32 mcFilter[8][6] = {
    { 0,  0,  128,    0,   0,  0 },
    { 0, -6,  123,   12,  -1,  0 },
    { 2, -11, 108,   36,  -8,  1 },
    { 0, -9,   93,   50,  -6,  0 },
    { 3, -16,  77,   77, -16,  3 },
    { 0, -6,   50,   93,  -9,  0 },
    { 1, -8,   36,  108, -11,  2 },
    { 0, -1,   12,  123,  -6,  0 }
};

MPP_RET hal_vp8d_init(void *hal, MppHalCfg *cfg)
{
    MPP_RET ret = MPP_OK;
    VP8DHalContext_t *ctx = (VP8DHalContext_t *)hal;

    FUN_T("FUN_IN");
    //configure
    ctx->packet_slots = cfg->packet_slots;
    ctx->frame_slots = cfg->frame_slots;

    mpp_env_get_u32("vp8h_debug", &vp8h_debug, 0);
    //get vpu socket
#ifdef RKPLATFORM
    if (ctx->vpu_socket <= 0) {
        ctx->vpu_socket = VPUClientInit(VPU_DEC);
        if (ctx->vpu_socket <= 0) {
            mpp_err("get vpu_socket(%d) <=0, failed. \n", ctx->vpu_socket);

            FUN_T("FUN_OUT");
            return MPP_ERR_UNKNOW;
        } else {
            mpp_log("get vpu_socket(%d), success. \n", ctx->vpu_socket);
        }
    }
#endif
    if (NULL == ctx->regs) {
        ctx->regs = mpp_calloc_size(void, sizeof(VP8DRegSet_t));
        if (NULL == ctx->regs) {
            mpp_err("hal_vp8 reg alloc failed\n");

            FUN_T("FUN_OUT");
            return MPP_ERR_NOMEM;
        }
    }

    if (NULL == ctx->group) {
#ifdef RKPLATFORM
        mpp_err("mpp_buffer_group_get_internal used ion in");
        ret = mpp_buffer_group_get_internal(&ctx->group, MPP_BUFFER_TYPE_ION);
#else
        ret = mpp_buffer_group_get_internal(&ctx->group, MPP_BUFFER_TYPE_NORMAL);
#endif
        if (MPP_OK != ret) {
            mpp_err("hal_vp8 mpp_buffer_group_get failed\n");

            FUN_T("FUN_OUT");
            return ret;
        }
    }
    ret = mpp_buffer_get(ctx->group, &ctx->probe_table, VP8D_PROB_TABLE_SIZE);
    if (MPP_OK != ret) {
        mpp_err("hal_vp8 probe_table get buffer failed\n");

        FUN_T("FUN_OUT");
        return ret;
    }

    ret = mpp_buffer_get(ctx->group, &ctx->seg_map, VP8D_MAX_SEGMAP_SIZE);

    if (MPP_OK != ret) {
        mpp_err("hal_vp8 seg_map get buffer failed\n");
        FUN_T("FUN_OUT");
        return ret;
    }

    return ret;
}

MPP_RET hal_vp8d_deinit(void *hal)
{
    MPP_RET ret = MPP_OK;

    FUN_T("FUN_IN");
    VP8DHalContext_t *ctx = (VP8DHalContext_t *)hal;
#ifdef RKPLATFORM
    if (ctx->vpu_socket >= 0) {
        VPUClientRelease(ctx->vpu_socket);

    }
#endif
    if (ctx->probe_table) {
        ret = mpp_buffer_put(ctx->probe_table);
        if (MPP_OK != ret) {
            mpp_err("hal_vp8 probe table put buffer failed\n");
        }
    }

    if (ctx->seg_map) {
        ret = mpp_buffer_put(ctx->seg_map);
        if (MPP_OK != ret) {
            mpp_err("hal_vp8 seg map put buffer failed\n");
        }
    }

    if (ctx->group) {
        ret = mpp_buffer_group_put(ctx->group);
        if (MPP_OK != ret) {
            mpp_err("hal_vp8 group free buffer failed\n");
        }
    }

    if (ctx->regs) {
        mpp_free(ctx->regs);
        ctx->regs = NULL;
    }

    FUN_T("FUN_OUT");
    return ret;
}

MPP_RET hal_vp8_init_hwcfg(VP8DHalContext_t *ctx)
{

    VP8DRegSet_t *reg = (VP8DRegSet_t *)ctx->regs;

    FUN_T("FUN_IN");
    memset(reg, 0, sizeof(VP8DRegSet_t));
    reg->reg50_dec_ctrl.sw_dec_out_tiled_e = 0;
    reg->reg50_dec_ctrl.sw_dec_scmd_dis    = 0;
    reg->reg50_dec_ctrl.sw_dec_adv_pre_dis = 0;
    reg->reg50_dec_ctrl.sw_dec_latency     = 0;

    reg->reg53_dec_mode =  DEC_MODE_VP8;

    reg->reg54_endian.sw_dec_in_endian     = 1;
    reg->reg54_endian.sw_dec_out_endian    = 1;
    reg->reg54_endian.sw_dec_inswap32_e    = 1;
    reg->reg54_endian.sw_dec_outswap32_e   = 1;
    reg->reg54_endian.sw_dec_strswap32_e   = 1;
    reg->reg54_endian.sw_dec_strendian_e   = 1;

    reg->reg55_Interrupt.sw_dec_irq = 0;

    reg->reg56_axi_ctrl.sw_dec_axi_rn_id     = 0;
    reg->reg56_axi_ctrl.sw_dec_axi_wr_id     = 0;

    reg->reg56_axi_ctrl.sw_dec_data_disc_e   = 0;
    reg->reg56_axi_ctrl.sw_dec_max_burst     = 16;
    reg->reg57_enable_ctrl.sw_dec_timeout_e  = 1;
    reg->reg57_enable_ctrl.sw_dec_clk_gate_e = 1;
    reg->reg57_enable_ctrl.sw_dec_out_dis = 0;

#ifdef RKPLATFORM
    reg->reg149_segment_map_base = mpp_buffer_get_fd(ctx->seg_map);
    reg->reg61_qtable_base = mpp_buffer_get_fd(ctx->probe_table);
#endif

    FUN_T("FUN_OUT");
    return MPP_OK;
}

MPP_RET hal_vp8d_pre_filter_tap_set(VP8DHalContext_t *ctx)
{
    VP8DRegSet_t *regs = (VP8DRegSet_t *)ctx->regs;

    FUN_T("FUN_IN");
    regs->reg59.sw_pred_bc_tap_0_0  = mcFilter[0][1];
    regs->reg59.sw_pred_bc_tap_0_1  = mcFilter[0][2];
    regs->reg59.sw_pred_bc_tap_0_2  = mcFilter[0][3];
    regs->reg153.sw_pred_bc_tap_0_3 = mcFilter[0][4];
    regs->reg153.sw_pred_bc_tap_1_0 = mcFilter[1][1];
    regs->reg153.sw_pred_bc_tap_1_1 = mcFilter[1][2];
    regs->reg154.sw_pred_bc_tap_1_2 = mcFilter[1][3];
    regs->reg154.sw_pred_bc_tap_1_3 = mcFilter[1][4];
    regs->reg154.sw_pred_bc_tap_2_0 = mcFilter[2][1];
    regs->reg155.sw_pred_bc_tap_2_1 = mcFilter[2][2];
    regs->reg155.sw_pred_bc_tap_2_2 = mcFilter[2][3];
    regs->reg155.sw_pred_bc_tap_2_3 = mcFilter[2][4];

    regs->reg156.sw_pred_bc_tap_3_0 = mcFilter[3][1];
    regs->reg156.sw_pred_bc_tap_3_1 = mcFilter[3][2];
    regs->reg156.sw_pred_bc_tap_3_2 = mcFilter[3][3];
    regs->reg157.sw_pred_bc_tap_3_3 = mcFilter[3][4];
    regs->reg157.sw_pred_bc_tap_4_0 = mcFilter[4][1];
    regs->reg157.sw_pred_bc_tap_4_1 = mcFilter[4][2];
    regs->reg158.sw_pred_bc_tap_4_2 = mcFilter[4][3];
    regs->reg158.sw_pred_bc_tap_4_3 = mcFilter[4][4];
    regs->reg158.sw_pred_bc_tap_5_0 = mcFilter[5][1];

    regs->reg125.sw_pred_bc_tap_5_1 = mcFilter[5][2];

    regs->reg125.sw_pred_bc_tap_5_2 = mcFilter[5][3];

    regs->reg125.sw_pred_bc_tap_5_3 = mcFilter[5][4];
    regs->reg126.sw_pred_bc_tap_6_0 = mcFilter[6][1];
    regs->reg126.sw_pred_bc_tap_6_1 = mcFilter[6][2];
    regs->reg126.sw_pred_bc_tap_6_2 = mcFilter[6][3];
    regs->reg127.sw_pred_bc_tap_6_3 = mcFilter[6][4];
    regs->reg127.sw_pred_bc_tap_7_0 = mcFilter[7][1];
    regs->reg127.sw_pred_bc_tap_7_1 = mcFilter[7][2];
    regs->reg128.sw_pred_bc_tap_7_2 = mcFilter[7][3];
    regs->reg128.sw_pred_bc_tap_7_3 = mcFilter[7][4];

    regs->reg128.sw_pred_tap_2_M1 = mcFilter[2][0];
    regs->reg128.sw_pred_tap_2_4  = mcFilter[2][5];
    regs->reg128.sw_pred_tap_4_M1 = mcFilter[4][0];
    regs->reg128.sw_pred_tap_4_4  = mcFilter[4][5];
    regs->reg128.sw_pred_tap_6_M1 = mcFilter[6][0];
    regs->reg128.sw_pred_tap_6_4  = mcFilter[6][5];

    FUN_T("FUN_OUT");
    return MPP_OK;
}

MPP_RET hal_vp8d_dct_partition_cfg(VP8DHalContext_t *ctx, HalTaskInfo *task)
{
    RK_U32 i = 0, len = 0, len1 = 0;
    RK_U32 extraBytesPacked = 0;
    RK_U32 addr = 0, byte_offset = 0;
#ifdef RKPLATFORM
    RK_U32 fd = 0;
    MppBuffer streambuf = NULL;
#endif
    VP8DRegSet_t *regs = (VP8DRegSet_t *)ctx->regs;
    DXVA_PicParams_VP8 *pic_param = (DXVA_PicParams_VP8 *)task->dec.syntax.data;


    FUN_T("FUN_IN");
#ifdef RKPLATFORM
    mpp_buf_slot_get_prop(ctx->packet_slots, task->dec.input, SLOT_BUFFER, &streambuf);
    fd =  mpp_buffer_get_fd(streambuf);
    regs->reg145_bitpl_ctrl_base = fd;
    if (VPUClientGetIOMMUStatus() > 0) {
        regs->reg145_bitpl_ctrl_base |= (pic_param->stream_start_offset << 10);
    } else {
        regs->reg145_bitpl_ctrl_base += pic_param->stream_start_offset;
    }
    regs->reg122.sw_strm1_start_bit = pic_param->stream_start_bit;
#endif
    /* calculate dct partition length here instead */
    if (pic_param->decMode == VP8HWD_VP8 && !pic_param->frame_type)
        extraBytesPacked += 7;
    len = pic_param->streamEndPos + pic_param->frameTagSize - pic_param->dctPartitionOffsets[0];
    len += ( (1 << pic_param->log2_nbr_of_dct_partitions) - 1) * 3;
    len1 = extraBytesPacked + pic_param->dctPartitionOffsets[0];
    len += (len1 & 0x7);
    regs->reg51_stream_info.sw_stream_len = len;

    //mpp_log("offsetToDctParts = %d pic_param->frameTagSize %d pic_param->stream_start_offset = %d extraBytesPacked = %d",
    //        pic_param->offsetToDctParts,pic_param->frameTagSize ,
    //      pic_param->stream_start_offset,extraBytesPacked);

    len = pic_param->offsetToDctParts + pic_param->frameTagSize -
          (pic_param->stream_start_offset - extraBytesPacked);
    if (pic_param->decMode == VP8HWD_VP7) /* give extra byte for VP7 to pass test cases */
        len ++;

    regs->reg124.sw_stream1_len = len;
    regs->reg124.sw_coeffs_part_am = (1 << pic_param->log2_nbr_of_dct_partitions) - 1;
    for (i = 0; i < (RK_U32)(1 << pic_param->log2_nbr_of_dct_partitions); i++) {
        addr = extraBytesPacked + pic_param->dctPartitionOffsets[i];
        byte_offset = addr & 0x7;
        addr = addr & 0xFFFFFFF8;
#ifdef RKPLATFORM
        if ( i == 0) {
            if (VPUClientGetIOMMUStatus() > 0) {
                regs->reg64_input_stream_base = fd | (addr << 10);
            } else {
                regs->reg_dct_strm_base[i] = fd + addr;
            }
        } else if ( i <= 5) {
            if (VPUClientGetIOMMUStatus() > 0) {
                regs->reg_dct_strm_base[i] = fd | (addr << 10);
            } else {
                regs->reg_dct_strm_base[i] = fd + addr;
            }
        } else {
            if (VPUClientGetIOMMUStatus() > 0) {
                regs->reg_dct_strm1_base[i - 6] = fd | (addr << 10);
            } else {
                regs->reg_dct_strm_base[i - 6] = fd + addr;
            }
        }
#endif
        switch (i) {
        case 0:
            regs->reg122.sw_strm_start_bit = byte_offset * 8;
            break;
        case 1:
            regs->reg121.sw_dct1_start_bit = byte_offset * 8;
            break;
        case 2:
            regs->reg121.sw_dct2_start_bit = byte_offset * 8;
            break;
        case 3:
            regs->reg150.sw_dct_start_bit_3 = byte_offset * 8;
            break;
        case 4:
            regs->reg150.sw_dct_start_bit_4 = byte_offset * 8;
            break;
        case 5:
            regs->reg150.sw_dct_start_bit_5 = byte_offset * 8;
            break;
        case 6:
            regs->reg150.sw_dct_start_bit_6 = byte_offset * 8;
            break;
        case 7:
            regs->reg150.sw_dct_start_bit_6 = byte_offset * 8;
            break;
        default:
            break;
        }
    }

    FUN_T("FUN_OUT");
    return MPP_OK;
}
void hal_vp8hw_asic_probe_update(DXVA_PicParams_VP8 *p, RK_U8 *probTbl)
{
    RK_U8   *dst;
    RK_U32  i, j, k;

    FUN_T("FUN_IN");
    /* first probs */
    dst = probTbl;

    dst[0] = p->probe_skip_false;
    dst[1] = p->prob_intra;
    dst[2] = p->prob_last;
    dst[3] = p->prob_golden;
    dst[4] = p->stVP8Segments.mb_segment_tree_probs[0];
    dst[5] = p->stVP8Segments.mb_segment_tree_probs[1];
    dst[6] = p->stVP8Segments.mb_segment_tree_probs[2];
    dst[7] = 0; /*unused*/

    dst += 8;
    dst[0] = p->intra_16x16_prob[0];
    dst[1] = p->intra_16x16_prob[1];
    dst[2] = p->intra_16x16_prob[2];
    dst[3] = p->intra_16x16_prob[3];
    dst[4] = p->intra_chroma_prob[0];
    dst[5] = p->intra_chroma_prob[1];
    dst[6] = p->intra_chroma_prob[2];
    dst[7] = 0; /*unused*/

    /* mv probs */
    dst += 8;
    dst[0] = p->vp8_mv_update_probs[0][0]; /* is short */
    dst[1] = p->vp8_mv_update_probs[1][0];
    dst[2] = p->vp8_mv_update_probs[0][1]; /* sign */
    dst[3] = p->vp8_mv_update_probs[1][1];
    dst[4] = p->vp8_mv_update_probs[0][8 + 9];
    dst[5] = p->vp8_mv_update_probs[0][9 + 9];
    dst[6] = p->vp8_mv_update_probs[1][8 + 9];
    dst[7] = p->vp8_mv_update_probs[1][9 + 9];
    dst += 8;
    for ( i = 0 ; i < 2 ; ++i ) {
        for ( j = 0 ; j < 8 ; j += 4 ) {
            dst[0] = p->vp8_mv_update_probs[i][j + 9 + 0];
            dst[1] = p->vp8_mv_update_probs[i][j + 9 + 1];
            dst[2] = p->vp8_mv_update_probs[i][j + 9 + 2];
            dst[3] = p->vp8_mv_update_probs[i][j + 9 + 3];
            dst += 4;
        }
    }
    for ( i = 0 ; i < 2 ; ++i ) {
        dst[0] =  p->vp8_mv_update_probs[i][0 + 2];
        dst[1] =  p->vp8_mv_update_probs[i][1 + 2];
        dst[2] =  p->vp8_mv_update_probs[i][2 + 2];
        dst[3] =  p->vp8_mv_update_probs[i][3 + 2];
        dst[4] =  p->vp8_mv_update_probs[i][4 + 2];
        dst[5] =  p->vp8_mv_update_probs[i][5 + 2];
        dst[6] =  p->vp8_mv_update_probs[i][6 + 2];
        dst[7] = 0; /*unused*/
        dst += 8;
    }

    /* coeff probs (header part) */
    dst = (RK_U8*)probTbl;
    dst += (8 * 7);
    for ( i = 0 ; i < 4 ; ++i ) {
        for ( j = 0 ; j < 8 ; ++j ) {
            for ( k = 0 ; k < 3 ; ++k ) {
                dst[0] = p->vp8_coef_update_probs[i][j][k][0];
                dst[1] = p->vp8_coef_update_probs[i][j][k][1];
                dst[2] = p->vp8_coef_update_probs[i][j][k][2];
                dst[3] = p->vp8_coef_update_probs[i][j][k][3];
                dst += 4;
            }
        }
    }

    /* coeff probs (footer part) */
    dst = (RK_U8*)probTbl;
    dst += (8 * 55);
    for ( i = 0 ; i < 4 ; ++i ) {
        for ( j = 0 ; j < 8 ; ++j ) {
            for ( k = 0 ; k < 3 ; ++k ) {
                dst[0] = p->vp8_coef_update_probs[i][j][k][4];
                dst[1] = p->vp8_coef_update_probs[i][j][k][5];
                dst[2] = p->vp8_coef_update_probs[i][j][k][6];
                dst[3] = p->vp8_coef_update_probs[i][j][k][7];
                dst[4] = p->vp8_coef_update_probs[i][j][k][8];
                dst[5] = p->vp8_coef_update_probs[i][j][k][9];
                dst[6] = p->vp8_coef_update_probs[i][j][k][10];
                dst[7] = 0; /*unused*/
                dst += 8;
            }
        }
    }
    FUN_T("FUN_OUT");
    return ;
}
MPP_RET hal_vp8d_gen_regs(void* hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;
    RK_U32 mb_width = 0, mb_height = 0;
#ifdef RKPLATFORM
    MppBuffer framebuf = NULL;
    RK_U8 *segmap_ptr = NULL;
    RK_U8 *probe_ptr = NULL;
#endif
    VP8DHalContext_t *ctx = (VP8DHalContext_t *)hal;
    VP8DRegSet_t *regs = (VP8DRegSet_t *)ctx->regs;
    DXVA_PicParams_VP8 *pic_param = (DXVA_PicParams_VP8 *)task->dec.syntax.data;

    FUN_T("FUN_IN");
    hal_vp8_init_hwcfg(ctx);
    mb_width = (pic_param->width + 15) >> 4;
    mb_height = (pic_param->height + 15) >> 4;
    // mpp_log("mb_width = %d mb_height = %d", mb_width, mb_height);
    regs->reg120.sw_pic_mb_width = mb_width & 0x1FF;
    regs->reg120.sw_pic_mb_hight_p =  mb_height & 0xFF;
    regs->reg120.sw_pic_mb_w_ext = mb_width >> 9;
    regs->reg120.sw_pic_mb_h_ext = mb_height >> 8;

#ifdef RKPLATFORM
    if (!pic_param->frame_type) {
        segmap_ptr = mpp_buffer_get_ptr(ctx->seg_map);
        if (NULL != segmap_ptr) {
            memset(segmap_ptr, 0, VP8D_MAX_SEGMAP_SIZE);
        }
    }

    probe_ptr = mpp_buffer_get_ptr(ctx->probe_table);
    if (NULL != probe_ptr) {
        hal_vp8hw_asic_probe_update(pic_param, probe_ptr);
    }
    mpp_buf_slot_get_prop(ctx->frame_slots, pic_param->CurrPic.Index7Bits, SLOT_BUFFER, &framebuf);
    regs->reg63_cur_pic_base = mpp_buffer_get_fd(framebuf);
    if (!pic_param->frame_type) { //key frame
        if ((mb_width * mb_height) << 8 > 0x400000) {
            mpp_log("mb_width*mb_height is big then 0x400000,iommu err");
        }
        regs->reg131_ref0_base = regs->reg63_cur_pic_base | ((mb_width * mb_height) << 18);
    } else if (pic_param->lst_fb_idx.Index7Bits < 0x7f) { //config ref0 base
        mpp_buf_slot_get_prop(ctx->frame_slots, pic_param->lst_fb_idx.Index7Bits, SLOT_BUFFER, &framebuf);
        regs->reg131_ref0_base = mpp_buffer_get_fd(framebuf);
    } else {
        regs->reg131_ref0_base = regs->reg63_cur_pic_base;
    }

    /* golden reference */
    if (pic_param->gld_fb_idx.Index7Bits < 0x7f) {
        mpp_buf_slot_get_prop(ctx->frame_slots, pic_param->gld_fb_idx.Index7Bits, SLOT_BUFFER, &framebuf);
        regs->reg136_golden_ref_base = mpp_buffer_get_fd(framebuf);
    } else {
        regs->reg136_golden_ref_base = regs->reg63_cur_pic_base;
    }

    if (VPUClientGetIOMMUStatus() > 0) {
        regs->reg136_golden_ref_base = regs->reg136_golden_ref_base | (pic_param->ref_frame_sign_bias_golden << 10);
    } else {
        regs->reg136_golden_ref_base =  regs->reg136_golden_ref_base + pic_param->ref_frame_sign_bias_golden;
    }

    /* alternate reference */
    if (pic_param->alt_fb_idx.Index7Bits < 0x7f) {
        mpp_buf_slot_get_prop(ctx->frame_slots, pic_param->alt_fb_idx.Index7Bits, SLOT_BUFFER, &framebuf);
        regs->reg137.alternate_ref_base = mpp_buffer_get_fd(framebuf);
    } else {
        regs->reg137.alternate_ref_base = regs->reg63_cur_pic_base;
    }

    if (VPUClientGetIOMMUStatus() > 0) {
        regs->reg137.alternate_ref_base = regs->reg137.alternate_ref_base | (pic_param->ref_frame_sign_bias_altref << 10);
    } else {
        regs->reg137.alternate_ref_base =  regs->reg137.alternate_ref_base + pic_param->ref_frame_sign_bias_altref;
    }

    if (VPUClientGetIOMMUStatus() > 0) {
        regs->reg149_segment_map_base = regs->reg149_segment_map_base |
                                        ((pic_param->stVP8Segments.segmentation_enabled + (pic_param->stVP8Segments.update_mb_segmentation_map << 1)) << 10);
    } else {
        regs->reg149_segment_map_base =  regs->reg149_segment_map_base +
                                         (pic_param->stVP8Segments.segmentation_enabled + (pic_param->stVP8Segments.update_mb_segmentation_map << 1));
    }

#endif
    regs->reg57_enable_ctrl.sw_pic_inter_e = pic_param->frame_type;
    regs->reg50_dec_ctrl.sw_skip_mode = !pic_param->mb_no_coeff_skip;

    if (!pic_param->stVP8Segments.segmentation_enabled) {
        regs->reg129.sw_filt_level_0 = pic_param->filter_level;
    } else if (pic_param->stVP8Segments.update_mb_segmentation_data) {
        regs->reg129.sw_filt_level_0 = pic_param->stVP8Segments.segment_feature_data[1][0];
        regs->reg129.sw_filt_level_1 = pic_param->stVP8Segments.segment_feature_data[1][1];
        regs->reg129.sw_filt_level_2 = pic_param->stVP8Segments.segment_feature_data[1][2];
        regs->reg129.sw_filt_level_3 = pic_param->stVP8Segments.segment_feature_data[1][3];
    } else {
        regs->reg129.sw_filt_level_0 = CLIP3(0, 63, (RK_S32)pic_param->filter_level + pic_param->stVP8Segments.segment_feature_data[1][0]);
        regs->reg129.sw_filt_level_1 = CLIP3(0, 63, (RK_S32)pic_param->filter_level + pic_param->stVP8Segments.segment_feature_data[1][1]);
        regs->reg129.sw_filt_level_2 = CLIP3(0, 63, (RK_S32)pic_param->filter_level + pic_param->stVP8Segments.segment_feature_data[1][2]);
        regs->reg129.sw_filt_level_3 = CLIP3(0, 63, (RK_S32)pic_param->filter_level + pic_param->stVP8Segments.segment_feature_data[1][3]);
    }

    regs->reg132.sw_filt_type = pic_param->filter_type;
    regs->reg132.sw_filt_sharpness = pic_param->sharpness;

    if (pic_param->filter_level == 0) {
        regs->reg50_dec_ctrl.sw_filtering_dis = 1;
    }

    if (pic_param->version != 3) {
        regs->reg121.sw_romain_mv = 1;
    }

    if (pic_param->decMode == VP8HWD_VP8 && (pic_param->version & 0x3)) {
        regs->reg121.sw_eable_bilinear = 1;
    }
    regs->reg122.sw_boolean_value = pic_param->bool_value;
    regs->reg122.sw_boolean_range = pic_param->bool_range;

    {
        if (!pic_param->stVP8Segments.segmentation_enabled)
            regs->reg130.sw_quant_0 = pic_param->y1ac_delta_q;
        else if (pic_param->stVP8Segments.update_mb_segmentation_data) { /* absolute mode */
            regs->reg130.sw_quant_0 = pic_param->stVP8Segments.segment_feature_data[0][0];
            regs->reg130.sw_quant_1 = pic_param->stVP8Segments.segment_feature_data[0][1];
            regs->reg151.sw_quant_2 = pic_param->stVP8Segments.segment_feature_data[0][2];
            regs->reg151.sw_quant_3 = pic_param->stVP8Segments.segment_feature_data[0][3];
        } else { /* delta mode */
            regs->reg130.sw_quant_0 = CLIP3(0, 127, pic_param->y1ac_delta_q + pic_param->stVP8Segments.segment_feature_data[0][0]);
            regs->reg130.sw_quant_1 = CLIP3(0, 127, pic_param->y1ac_delta_q + pic_param->stVP8Segments.segment_feature_data[0][1]);
            regs->reg151.sw_quant_2 = CLIP3(0, 127, pic_param->y1ac_delta_q + pic_param->stVP8Segments.segment_feature_data[0][2]);
            regs->reg151.sw_quant_3 = CLIP3(0, 127, pic_param->y1ac_delta_q + pic_param->stVP8Segments.segment_feature_data[0][3]);
        }

        regs->reg130.sw_quant_delta_0 = pic_param->y1dc_delta_q;
        regs->reg130.sw_quant_delta_1 = pic_param->y2dc_delta_q;
        regs->reg151.sw_quant_delta_2 = pic_param->y2ac_delta_q;
        regs->reg151.sw_quant_delta_3 = pic_param->uvdc_delta_q;
        regs->reg152.sw_quant_delta_4 = pic_param->uvac_delta_q;

        if (pic_param->mode_ref_lf_delta_enabled) {
            regs->reg133.sw_filt_ref_adj_0 = pic_param->ref_lf_deltas[0];
            regs->reg133.sw_filt_ref_adj_1 = pic_param->ref_lf_deltas[1];
            regs->reg133.sw_filt_ref_adj_2 = pic_param->ref_lf_deltas[2];
            regs->reg133.sw_filt_ref_adj_3 = pic_param->ref_lf_deltas[3];
            regs->reg132.sw_filt_mb_adj_0  = pic_param->mode_lf_deltas[0];
            regs->reg132.sw_filt_mb_adj_1  = pic_param->mode_lf_deltas[1];
            regs->reg132.sw_filt_mb_adj_2  = pic_param->mode_lf_deltas[2];
            regs->reg132.sw_filt_mb_adj_3  = pic_param->mode_lf_deltas[3];
        }

    }

    if ((pic_param->version & 0x3) == 0)
        hal_vp8d_pre_filter_tap_set(ctx);

    hal_vp8d_dct_partition_cfg(ctx, task);
    regs->reg57_enable_ctrl.sw_dec_e = 1;

    FUN_T("FUN_OUT");
    return ret;
}

MPP_RET hal_vp8d_start(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;

#ifdef RKPLATFORM
    RK_U32 i = 0;
    VP8DHalContext_t *ctx = (VP8DHalContext_t *)hal;
    VP8DRegSet_t *regs = (VP8DRegSet_t *)ctx->regs;
    RK_U8 *p = ctx->regs;


    FUN_T("FUN_IN");

    for (i = 0; i < 159; i++) {
        vp8h_dbg(VP8H_DBG_REG, "vp8d: regs[%02d]=%08X\n", i, *((RK_U32*)p));
        // mpp_log("vp8d: regs[%02d]=%08X\n", i, *((RK_U32*)p));
        p += 4;
    }
    ret = VPUClientSendReg(ctx->vpu_socket, (RK_U32 *)regs, VP8D_REG_NUM);
    if (ret != 0) {
        mpp_err("VPUClientSendReg Failed!!!\n");
        return MPP_ERR_VPUHW;
    }

    FUN_T("FUN_OUT");
#endif
    (void)task;
    (void)hal;
    return ret;
}

MPP_RET hal_vp8d_wait(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;
#ifdef RKPLATFORM
    VP8DRegSet_t reg_out;
    VPU_CMD_TYPE cmd = 0;
    RK_S32 length = 0;
    VP8DHalContext_t *ctx = (VP8DHalContext_t *)hal;
    FUN_T("FUN_IN");
    memset(&reg_out, 0, sizeof(VP8DRegSet_t));
    ret = VPUClientWaitResult(ctx->vpu_socket, (RK_U32 *)&reg_out,
                              VP8D_REG_NUM, &cmd, &length);
    FUN_T("FUN_OUT");
#endif
    (void)hal;
    (void)task;
    return ret;
}

MPP_RET hal_vp8d_reset(void *hal)
{
    MPP_RET ret = MPP_OK;

    FUN_T("FUN_IN");
    (void)hal;
    FUN_T("FUN_OUT");
    return ret;
}

MPP_RET hal_vp8d_flush(void *hal)
{
    MPP_RET ret = MPP_OK;

    FUN_T("FUN_IN");
    (void)hal;
    FUN_T("FUN_OUT");
    return ret;
}

MPP_RET hal_vp8d_control(void *hal, RK_S32 cmd_type, void *param)
{
    MPP_RET ret = MPP_OK;

    FUN_T("FUN_IN");
    (void)hal;
    (void)cmd_type;
    (void)param;
    FUN_T("FUN_OUT");
    return ret;
}
