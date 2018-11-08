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

#define MODULE_TAG "rmvbd_parser"
#include <string.h>
#include<math.h>
#include "rmvbd_parser.h"
#include "rmvbd_api.h"
#include "mpp_frame.h"
#include "mpp_env.h"
#include "rk_type.h"
#include "mpp_mem.h"
#include "mpp_bitread.h"
#include "rmvbd_syntax.h"
#include "mpp_log.h"

RK_U32 rmvbd_debug = 0;
#define rmvbd_dbg(flag, fmt, ...)   _mpp_dbg(rmvbd_debug, flag, fmt, ## __VA_ARGS__)
#define rmvbd_dbg_f(flag, fmt, ...) _mpp_dbg_f(rmvbd_debug, flag, fmt, ## __VA_ARGS__)

#define rmvbd_dbg_func(fmt, ...)    rmvbd_dbg_f(RMVBD_DBG_FUNCTION, fmt, ## __VA_ARGS__)
#define rmvbd_dbg_bit(fmt, ...)     rmvbd_dbg(RMVBD_DBG_BITS, fmt, ## __VA_ARGS__)
#define rmvbd_dbg_status(fmt, ...)  rmvbd_dbg(RMVBD_DBG_STATUS, fmt, ## __VA_ARGS__)


/*
static void rmvbd_syntax_init(rmvb_Syntax *syntax)
{

    (void)syntax;
}
*/








static MPP_RET rmvbd_parser_init_ctx(rmvbdParserContex *ctx, ParserCfg *cfg)
{
    MPP_RET ret = MPP_OK;
    RK_S32 i;
    RK_U8 *stream;
    memset(ctx, 0, sizeof(*ctx));
    BitReadCtx_t *bit_ctx = mpp_calloc(BitReadCtx_t, 1);
    rmvbd_dbg_func("rmvbd_fun_in");
    //rmvbdParserContex *p = mpp_calloc(rmvbdParserContex, 1);
    //rmvb_Syntax *syntax = mpp_calloc(rmvb_Syntax, 1);
    if ( NULL == bit_ctx ) {
        mpp_err_f("malloc context failed\n");
        return MPP_NOK;
    }
    MppPacket task_pkt = NULL;
    size_t stream_size = RMVBD_STREAM_BUFF_SIZE;
    stream = mpp_malloc_size(RK_U8, stream_size);
    if (NULL == stream) {
        mpp_err_f("failed to malloc stream buffer size %d\n", stream_size);
        return MPP_ERR_MALLOC;
    }
    ret = mpp_packet_init(&task_pkt, stream, stream_size);
    if (ret) {
        mpp_err_f("failed to create mpp_packet for task\n");
        return MPP_NOK;
    }
    mpp_packet_set_pos(task_pkt, stream);
    mpp_packet_set_length(task_pkt, 0);

    ctx->task_pkt = task_pkt;

    mpp_buf_slot_setup(ctx->frame_slots, 4);//4 represents the number of buffer that maybe used(usually larger than the number of frames)
    ctx->stream = stream;
    ctx->frame_slots = cfg->frame_slots;
    ctx->rmvbsyn = mpp_calloc(rmvb_Syntax, 1);
    ctx->packet_slots = cfg->packet_slots;
//  ctx->pos_frm_start = -1;
//  ctx->pos_frm_end = -1;
    ctx->bit_ctx = bit_ctx;

    memset(&ctx->Framehead, 0, 3 * sizeof(RVFrameHead));
    {
        ctx->frame_ref0 = &ctx->Framehead[0];
        ctx->frame_ref1 = &ctx->Framehead[1];
        ctx->frame_cur = &ctx->Framehead[2];
    }

    memset(&ctx->rmvbsyn->Framehead, 0, 3 * sizeof(RVFrameHead));
    {
        ctx->rmvbsyn->frame_ref0 = &ctx->rmvbsyn->Framehead[0];
        ctx->rmvbsyn->frame_ref1 = &ctx->rmvbsyn->Framehead[1];
        ctx->rmvbsyn->frame_cur = &ctx->rmvbsyn->Framehead[2];
    }


    for (i = 0; i < 3; i++) {
        mpp_frame_init(&ctx->Framehead[i].f);
        mpp_frame_init(&ctx->rmvbsyn->Framehead[i].f);
        if (!ctx->Framehead[i].f || !ctx->rmvbsyn->Framehead[i].f) {
            mpp_err("Failed to allocate frame buffer %d\n", i);
            return MPP_ERR_NOMEM;
        }
        (ctx->rmvbsyn->Framehead[i].picCodingType) = (ctx->Framehead[i].picCodingType) = 0xffffffff;
        (ctx->rmvbsyn->Framehead[i].picCodingType) = (ctx->Framehead[i].slot_index) = 0x7f; //set buffer index unused
    }

    ctx->notify_cb = cfg->notify_cb;
    ctx->needSkipToKeyFrm = 0;
    ctx->need_split = cfg->need_split;
    ctx->internal_pts = cfg->internal_pts;
    ctx->stream = stream;
    ctx->max_stream_size = RMVBD_STREAM_BUFF_SIZE;
    mpp_log("max_stream_size=%d, %p\n", ctx->max_stream_size, ctx->stream);
    ctx->decode_width_no_alignment = 0;
    ctx->decode_height_no_alignment = 0;
    ctx->getFromhd = 1;
    mpp_env_get_u32("rmvbd_debug", &rmvbd_debug, 0);
    mpp_log("rmvbd_parser_init_ctx leave!\n");
    return MPP_OK;
}

MPP_RET rmvbd_parser_init(void *ctx, ParserCfg *cfg)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_init enter ctx %p\n", ctx);
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    //  mpp_calloc(rmvbdParserContex, 1);
    if (NULL == ctx && cfg == NULL && p == NULL) {
        p = mpp_calloc(rmvbdParserContex, 1);
        mpp_err_f("found NULL intput ctx %p cfg %p\n", ctx, cfg);
        return MPP_ERR_NULL_PTR;
    }

    if (rmvbd_parser_init_ctx(p, cfg) != MPP_OK) {
        mpp_err_f("failed to init parser\n");
        return MPP_NOK;
    }
    p->fp_dbg_file = fopen("/data/tmp/dump_data.txt", "wb");
    p->frame_no = 0;
    mpp_log("rmvbd_parser_init leave!\n");

    return ret;

}


MPP_RET rmvbd_parser_deinit(void *ctx)
{
    RK_U32 k = 0;
    rmvbd_dbg_func("rmvbd_parser_deinit enter ctx %p\n", ctx);
    rmvbdParserContex*p = (rmvbdParserContex*)ctx;
    p->ref_frame_cnt = 0;
    p->needSkipToKeyFrm = RV_SKIP_TO_KEY_FRAME;


    for (k = 0; k < 3; k++) {
        mpp_free(p->Framehead[k].f);
        mpp_free(p->rmvbsyn->Framehead[k].f);
    }

    if (p->task_pkt) {
        mpp_packet_deinit(&p->task_pkt);
    }
    if (p->bit_ctx) {
        mpp_free(p->bit_ctx);
        p->bit_ctx = NULL;
    }
    if (p->rmvbsyn) {
        mpp_free(p->rmvbsyn);
        p->rmvbsyn = NULL;
    }
    if (p->stream) {
        mpp_free(p->stream);
        p->stream = NULL;
    }
    if (p->fp_dbg_file) {
        fclose(p->fp_dbg_file);
        p->fp_dbg_file = NULL;
    }
    if (p) {
        mpp_free(p);
    }
    mpp_log("rmvbd_parser_deinit leave!\n");
    return MPP_OK;
}
/*!
***********************************************************************
* \brief
*   reset
***********************************************************************
*/
MPP_RET rmvbd_parser_reset(void *ctx)
{

    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_reset enter ctx %p\n", ctx);
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    if (p->frame_cur->slot_index != 0x7f) {
        mpp_buf_slot_clr_flag(p->frame_slots, p->frame_cur->slot_index, SLOT_CODEC_USE);


    }
    if (p->frame_ref0->slot_index != 0x7f) {

        if (p->frame_ref0->flags) {
            mpp_buf_slot_set_flag(p->frame_slots, p->frame_ref0->slot_index, SLOT_QUEUE_USE);
            mpp_buf_slot_enqueue(p->frame_slots, p->frame_ref0->slot_index, QUEUE_DISPLAY);
            p->frame_ref0->flags = 0;
        }
        mpp_buf_slot_clr_flag(p->frame_slots, p->frame_ref0->slot_index, SLOT_CODEC_USE);
    }

    if (p->frame_ref1->slot_index != 0x7f) {
        mpp_buf_slot_clr_flag(p->frame_slots, p->frame_ref1->slot_index, SLOT_CODEC_USE);
    }

    p->frame_cur->slot_index = 0x7f;
    p->frame_ref0->slot_index = 0x7f;
    p->frame_ref1->slot_index = 0x7f;
    p->ref_frame_cnt = 0;
//  p->resetFlag = 1;
    p->eos = 0;
    mpp_log("rmvbd_parser_reset leave!\n");
    return ret;
}

/*!
***********************************************************************
* \brief
*   flush
***********************************************************************
*/
MPP_RET  rmvbd_parser_flush(void *ctx)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_flush enter ctx %p\n", ctx);
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    if (p->frame_ref0->slot_index == 0xff) {
        mpp_err_f("failed to flush\n");
        return MPP_NOK;
    }
    mpp_buf_slot_set_flag(p->frame_slots, p->frame_ref0->slot_index, SLOT_QUEUE_USE);
    mpp_buf_slot_enqueue(p->frame_slots, p->frame_ref0->slot_index, QUEUE_DISPLAY);
    p->frame_ref0->flags = 0;
    mpp_log("rmvbd_parser_flush leave!\n");
    return ret;
}

/*!
***********************************************************************
* \brief
*   control/perform
***********************************************************************
*/
MPP_RET  rmvbd_parser_control(void *ctx, RK_S32 cmd_type, void *param)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_control enter ctx %p\n", ctx);
    (void)ctx;
    (void)cmd_type;
    (void)param;
    rmvbd_dbg_func("rmvbd_parser_control leave!\n");
    return ret;
}


/*!
***********************************************************************
* \brief
*   prepare
***********************************************************************
*/

MPP_RET rmvbd_parser_split_frame(RK_U8 *src, RK_U32 src_size, RK_U8 *dst, RK_U32 *dst_size)
{

    MPP_RET ret = MPP_OK;
    mpp_log_f("rmvbd_parser_IN");
    RK_U32 val = 0;
    val = *((RK_U32*)src);

    if (VPU_BITSTREAM_START_CODE == val) { // if input data is rk format styl skip those 32 byte
        memcpy(dst, src + 32, src_size - 32);
        *dst_size = src_size - 32;
    } else {
        memcpy(dst, src, src_size);
        *dst_size = src_size;
    }
    mpp_log_f("rmvbd_parser_OUT");
    return ret;
}


MPP_RET rmvbd_parser_prepare(void *ctx, MppPacket pkt, HalDecTask *task)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_prepare enter ctx %p\n", ctx);
    if (NULL == ctx || NULL == pkt || NULL == task) {
        mpp_err_f("rmvbd_parser_prepare found NULL intput ctx %p pkt %p task %p\n", ctx, pkt, task);
        return MPP_ERR_NULL_PTR;
    }
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    MppPacket input_packet = p->task_pkt;
    RK_U32 out_size = 0, len;
    RK_U8 *pos = NULL;
    RK_U8 *buf = NULL;
    buf = pos = mpp_packet_get_pos(pkt);
    mpp_log("buf=%p,pos=%p \n", buf, pos);
    len = mpp_packet_get_length(pkt);
    mpp_log("packet_in_len=%d,max_stream_size=%d\n", len, p->max_stream_size);
    //p->pts = mpp_packet_get_pts(pkt);
    //mpp_log("pts=%d\n", p->pts);
    p->eos = mpp_packet_get_eos(pkt);
    mpp_log("eos=%d\n", p->eos);
    /*  if (NULL == p->stream) {
            mpp_err("input stream is null,failed to malloc task buffer for hardware with size %d\n", len);
            return MPP_ERR_UNKNOW;
        }
    */
    if (len > p->max_stream_size) {
        mpp_free(p->stream);
        p->stream = NULL;
        p->stream = mpp_malloc(RK_U8, (len << 2));
        if (NULL == p->stream) {
            mpp_err("rmvbd_parser realloc stream fail");
            return MPP_NOK;
        }
        p->max_stream_size = len + 1024;

    }
    pos += out_size;
    mpp_log("p->stream=0x%x set pos enter \n", p->stream);
    if (MPP_OK == rmvbd_parser_split_frame(buf, len, p->stream, &out_size))
        task->valid = 1;
    pos += out_size;
    mpp_log("pos=%p,len=%d out_size=%d rmvbd_parser packet set pos enter \n", pos, len, out_size);
    mpp_packet_set_pos(pkt, pos);
    //  mpp_log("rmvbd_parser packet set pos enter \n");

    if (out_size == 0 && p->eos) {
        mpp_log("out_size equal to 0 or eos!!!");
        rmvbd_parser_flush(ctx);//bug occur here!!!!
        task->flags.eos = 1;
        return ret;
    }
    if (p->fp_dbg_file && task->valid && p->frame_no < 10) {
        fwrite(p->stream, out_size, 1, p->fp_dbg_file);
        fflush(p->fp_dbg_file);
    }
    p->frame_no += task->valid ? 1 : 0;
    mpp_packet_set_data(input_packet, p->stream);

    mpp_packet_set_size(input_packet, p->max_stream_size);//total buffer size
    mpp_packet_set_length(input_packet, out_size);//valid data length
    //mpp_packet_set_pts(input_packet,p->pts);// need to be modified????
    task->valid = 0;
    task->input_packet = task->valid ? input_packet : NULL;
    mpp_log("rmvbd_parser_prepare leave!\n");
    return ret;
}

static RK_S32 rmvbd_read_bits(BitReadCtx_t *bx, RK_U32 bits)
{
    RK_S32 ret = 0;
    if (bits < 32)
        mpp_read_bits(bx, bits, &ret);
    else
        mpp_read_longbits(bx, bits, (RK_U32 *)&ret);
    return ret;
}

RK_U32 MbNumLen(RK_U32 numMbs)
{
    RK_U32 tmp;

    if (numMbs <= 48)          tmp = 6;
    else if (numMbs <= 99)   tmp = 7;
    else if (numMbs <= 396)   tmp = 9;
    else if (numMbs <= 1584)   tmp = 11;
    else if (numMbs <= 6336)   tmp = 13;
    else                        tmp = 14;
    return tmp;
}


RK_U32 GetPicsize(rmvbdParserContex*ctx, RK_U32 *width, RK_U32 *height)
{
    RK_U32 code;
    RK_U32 w, h;
    RK_U32  len = 3;
    BitReadCtx_t *bit = ctx->bit_ctx;
    const RK_U32 code_width[8] = { 160, 176, 240, 320, 352, 640, 704, 0 };
    const RK_U32 code_height1[8] = { 120, 132, 144, 240, 288, 480, 0, 0 };
    const RK_U32 code_height2[4] = { 180, 360, 576, 0 };

    /* width */
    code = rmvbd_read_bits(bit, 3);
    w = code_width[code];
    if (w == 0) {
        do {
            code = rmvbd_read_bits(bit, 8);
            w += (code << 2);
        } while (code == 255);
    }

    /* height */
    code = rmvbd_read_bits(bit, 3);
    h = code_height1[code];
    if (h == 0) {
        code <<= 1;
        code |= rmvbd_read_bits(bit, 1);
        len += 1;
        code &= 3;
        h = code_height2[code];
        if (h == 0) {
            do {
                code = rmvbd_read_bits(bit, 8);
                len += 8;
                h += (code << 2);
            } while (code == 255);
        }
    }

    *width = w;
    *height = h;

    return len;
}







MPP_RET rmvb_dec_framehd_rv8(rmvbdParserContex*ctx)
{
    MPP_RET ret = MPP_OK;
    //RK_U32    versionEncoding;
    RK_U32  index;
    RK_U32  size[9] = { 0, 1, 1, 2, 2, 3, 3, 3, 3 };
    BitReadCtx_t *bit = ctx->bit_ctx;
    RVFrameHead*frame_cur = ctx->frame_cur;

    rmvbd_dbg_func("rmvbd_dec_framehd_rv8 enter!\n");
    //versionEncoding = rmvbd_read_bits(bit, 3);
    rmvbd_read_bits(bit, 3);
    frame_cur->picCodingType = rmvbd_read_bits(bit, 2);
    rmvbd_read_bits(bit, 1);
    frame_cur->qp = rmvbd_read_bits(bit, 5);
    rmvbd_read_bits(bit, 1);
    frame_cur->tr = rmvbd_read_bits(bit, 13);
    index = rmvbd_read_bits(bit, size[ctx->pic_rpr_num]);
    {
        RK_U32  mbw, mbh;
        frame_cur->frameWidth = ctx->pic_rpr_size[2 * index];
        frame_cur->frameHeight = ctx->pic_rpr_size[2 * index + 1];
        mbw = (frame_cur->frameWidth + 15) >> 4;
        mbh = (frame_cur->frameHeight + 15) >> 4;
        frame_cur->totalMbInFrame = mbw * mbh;
    }
    frame_cur->mb_width = (frame_cur->frameWidth + 15) >> 4;
    frame_cur->mb_height = (frame_cur->frameHeight + 15) >> 4;
    ctx->decode_width_no_alignment = frame_cur->frameWidth;
    ctx->decode_height_no_alignment = frame_cur->frameHeight;
    ctx->display_width = frame_cur->frameWidth;
    ctx->display_height = frame_cur->frameHeight;
    ctx->decode_width = frame_cur->mb_width << 4;
    ctx->decode_height = frame_cur->mb_height << 4;
    index = rmvbd_read_bits(bit, MbNumLen(frame_cur->totalMbInFrame));
    if (index == END_OF_STREAM)
        return MPP_NOK;
    rmvbd_read_bits(bit, 1);
    mpp_log("rmvb_dec_framehd_rv8 leave!\n");
    return ret;

}



MPP_RET rmvb_dec_framehd_rv9(rmvbdParserContex*ctx)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_dec_framehd_rv9 enter!\n");
    RK_U32  index;
    RK_U32  mbw, mbh;
    BitReadCtx_t *bit = ctx->bit_ctx;
    RVFrameHead*frame_cur = ctx->frame_cur;
    index = rmvbd_read_bits(bit, 1);
    if (index)
        return MPP_NOK;
    frame_cur->picCodingType = rmvbd_read_bits(bit, 2);
    frame_cur->qp = rmvbd_read_bits(bit, 5);
    index = rmvbd_read_bits(bit, 2);    //Bistream version
    if (index)
        return MPP_NOK;
    frame_cur->vlcSet = rmvbd_read_bits(bit, 2);
    rmvbd_read_bits(bit, 1); //Deblocking pass
    frame_cur->tr = rmvbd_read_bits(bit, 13);
    frame_cur->frameWidth = ctx->frame_width;
    frame_cur->frameHeight = ctx->frame_height;
    frame_cur->frame_codelen = 0;
    if (frame_cur->picCodingType < RV_P_PIC)
        frame_cur->frame_codelen = GetPicsize(ctx, &frame_cur->frameWidth, &frame_cur->frameHeight);
    else if (!rmvbd_read_bits(bit, 1))
        frame_cur->frame_codelen = GetPicsize(ctx, &frame_cur->frameWidth, &frame_cur->frameHeight);

    ctx->frame_width = frame_cur->frameWidth;
    ctx->frame_height = frame_cur->frameHeight;

    mbw = (frame_cur->frameWidth + 15) >> 4;
    mbh = (frame_cur->frameHeight + 15) >> 4;
    frame_cur->totalMbInFrame = mbw * mbh;
    frame_cur->mb_width = mbw;
    frame_cur->mb_height = mbh;
    ctx->decode_width_no_alignment = frame_cur->frameWidth;
    ctx->decode_height_no_alignment = frame_cur->frameHeight;
    ctx->display_width = frame_cur->frameWidth;
    ctx->display_height = frame_cur->frameHeight;
    ctx->decode_width = frame_cur->mb_width << 4;
    ctx->decode_height = frame_cur->mb_height << 4;
    index = rmvbd_read_bits(bit, MbNumLen(frame_cur->totalMbInFrame));

    if (index == END_OF_STREAM)
        return MPP_NOK;
    mpp_log("rmvb_dec_framehd_rv9 leave!\n");
    return ret;
}


MPP_RET rmvb_decoder_head(rmvbdParserContex *ctx)
{
    MPP_RET ret = MPP_OK;
    RK_U32  SPOExtra;
    RK_U32  temp;
    RK_U32  EncodeSize = 0;
    RK_U32  StreamVersion;
    RK_U32  Major, Minor;
    rmvbd_dbg_func("rmvbd_decoder_head enter ctx %p\n", ctx);
    BitReadCtx_t *bit = ctx->bit_ctx;
    if (ctx->getFromhd) {
        ctx->framesize = rmvbd_read_bits(bit, 32);
        temp = rmvbd_read_bits(bit, 32);
        if (temp != HX_MEDIA_VIDEO)
            return MPP_NOK;
        temp = rmvbd_read_bits(bit, 32);
        if ((temp != HX_RV30VIDEO_ID) && (temp != HX_RV40VIDEO_ID))
            return MPP_NOK;
        ctx->display_width = rmvbd_read_bits(bit, 16);
        ctx->display_height = rmvbd_read_bits(bit, 16);
        mpp_skip_bits(bit, 12);
        ctx->framesize -= 26;
        //mpp_log("ctx->framesize is %d\n",ctx->framesize);
        RMVB_ASSERT(ctx->framesize > 0);
    }
    ctx->isRV8 = 0;
    ctx->pic_rpr_size[0] = ctx->display_width;
    ctx->pic_rpr_size[1] = ctx->display_height;
    if (ctx->framesize >= 8) {
        SPOExtra = rmvbd_read_bits(bit, 32);
        StreamVersion = rmvbd_read_bits(bit, 32);
        ctx->framesize -= 8;
        Major = HX_GET_MAJOR_VERSION(StreamVersion);
        Minor = HX_GET_MINOR_VERSION(StreamVersion);
    }
    if (!(Minor & RAW_BITSTREAM_MINOR_VERSION)) {
        if (Major == RV30_MAJOR_BITSTREAM_VERSION) {
            RK_U32 i;
            ctx->isRV8 = 1;
            ctx->pic_rpr_num = (SPOExtra & RV40_SPO_BITS_NUMRESAMPLE_IMAGES) >> RV40_SPO_BITS_NUMRESAMPLE_IMAGES_SHIFT;
            for (i = 0; i < ctx->pic_rpr_num; i++) {
                mpp_log("pass the value to pic_rpr_num \n");
                ctx->pic_rpr_size[2 * i + 2] = rmvbd_read_bits(bit, 8) << 2;
                ctx->pic_rpr_size[2 * i + 3] = rmvbd_read_bits(bit, 8) << 2;
            }
            bit += 2 * ctx->pic_rpr_num;
            ctx->framesize -= 2 * ctx->pic_rpr_num;
            ctx->pic_rpr_num++;
        } else if (Major == RV40_MAJOR_BITSTREAM_VERSION) {
            if (ctx->framesize >= 4) {
                EncodeSize = rmvbd_read_bits(bit, 32);
                ctx->framesize -= 4;
            }
        } else
            return MPP_NOK;
    } else
        return MPP_NOK;

    if (EncodeSize & 0xffff0000) {
        ctx->display_width = (EncodeSize >> 14) & 0x3fffc;
        ctx->pic_rpr_size[0] = ctx->display_width;
    }

    if (EncodeSize & 0xffff) {
        ctx->display_height = (EncodeSize << 2) & 0x3fffc;
        ctx->pic_rpr_size[1] = ctx->display_height;
    }
    ctx->decode_width_no_alignment = ctx->display_width;
    ctx->decode_height_no_alignment = ctx->display_height;
    ctx->decode_width = (ctx->display_width + 15) & (~15);
    ctx->decode_height = (ctx->display_height + 15) & (~15);
    ctx->frame_width = ctx->decode_width;
    ctx->frame_height = ctx->decode_height;
    ctx->TRWrap = 8192;
    ctx->ref_frame_cnt = 0;
    if (ctx->isRV8)
        rmvb_dec_framehd_rv8(ctx);
    else
        rmvb_dec_framehd_rv9(ctx);
    mpp_log("rmvb_decoder_head leave!\n");
    return ret;

}


MPP_RET rmvbd_alloc_frame(rmvbdParserContex *ctx)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_alloc_frame enter ctx %p\n", ctx);
    if (ctx->frame_cur->slot_index == 0xff) {
        //RK_U32 frametype = 0;
        mpp_frame_set_width(ctx->frame_cur->f, ctx->display_width);
        mpp_frame_set_height(ctx->frame_cur->f, ctx->display_height);
        mpp_frame_set_hor_stride(ctx->frame_cur->f, ctx->display_width);
        mpp_frame_set_ver_stride(ctx->frame_cur->f, ctx->display_height);
        mpp_buf_slot_set_prop(ctx->frame_slots, ctx->frame_cur->slot_index, SLOT_FRAME, ctx->frame_cur->f);
        mpp_buf_slot_set_flag(ctx->frame_slots, ctx->frame_cur->slot_index, SLOT_CODEC_USE);
        mpp_buf_slot_set_flag(ctx->frame_slots, ctx->frame_cur->slot_index, SLOT_HAL_OUTPUT);

    }
    mpp_log("rmvbd_alloc_frame leave!\n");
    return ret;
}






MPP_RET rmvbd_convert_to_dxva(rmvbdParserContex *ctx)
{
    MPP_RET ret = MPP_OK;

    rmvbd_dbg_func("rmvbd_convert_to_dxva enter ctx %p\n", ctx);
    rmvb_Syntax *dst = ctx->rmvbsyn;
    RVFrameHead *pfw = ctx->frame_ref1;
    RVFrameHead *pbw = ctx->frame_ref0;
    //BitReadCtx_t *bx = ctx->bit_ctx;
    RK_U32 i = 0;
    dst->qp = ctx->frame_cur->qp;
    dst->isRV8 = ctx->isRV8;
    dst->framesize = ctx->framesize;
    dst->slicenum = ctx->slicenum;
    dst->mb_width = ctx->mb_width;
    dst->mb_height = ctx->mb_height;
    dst->TRWrap = ctx->TRWrap;
    dst->decode_width_no_alignment = ctx->decode_width_no_alignment;
    dst->decode_height_no_alignment = ctx->decode_height_no_alignment;
    dst->display_height = ctx->display_height;
    dst->display_width = ctx->display_width;
    dst->decode_height = ctx->decode_height;
    dst->decode_width = ctx->decode_width;
    dst->CurrPic.Index7Bits = ctx->frame_cur->slot_index;
    dst->frame_cur = ctx->frame_cur;
    dst->frame_ref0 = ctx->frame_ref0;
    dst->frame_ref1 = ctx->frame_ref1;
    for (i = 0; i < 2 * 9; i++)
        dst->pic_rpr_size[i] = ctx->pic_rpr_size[i];
    dst->pic_rpr_num = ctx->pic_rpr_num;
    dst->frame_width = ctx->frame_width;
    dst->frame_height = ctx->frame_height;

    //may have risk below
    if (ctx->frame_ref0->slot_index == 0xff) {
        pbw = ctx->frame_cur;
    }
    if (ctx->frame_ref1->slot_index == 0xff) {
        pfw = pbw;
    }

    for (i = 0; i < 4; i++) {
        dst->frame_refs[i].bPicEntry = 0xff;
    }


    if (ctx->frame_cur->picCodingType == RV_B_PIC) {
        dst->frame_refs[0].Index7Bits = pfw->slot_index;
        dst->frame_refs[1].Index7Bits = pfw->slot_index;
        dst->frame_refs[2].Index7Bits = pbw->slot_index;
        dst->frame_refs[3].Index7Bits = pbw->slot_index;
    } else {
        dst->frame_refs[0].Index7Bits = pbw->slot_index;
        dst->frame_refs[1].Index7Bits = ctx->frame_cur->slot_index;
        dst->frame_refs[2].Index7Bits = ctx->frame_cur->slot_index;
        dst->frame_refs[3].Index7Bits = ctx->frame_cur->slot_index;
    }
    mpp_log("rmvbd_convert_to_dxva leave!\n");
    return ret;
}





MPP_RET rmvbd_update_ref_frame(rmvbdParserContex *ctx)
{
    rmvbd_dbg_func("rmvbd_update_ref_frame enter ctx %p\n", ctx);
    MPP_RET ret = MPP_OK;
    if (ctx->frame_cur->picCodingType == RV_B_PIC) {
        RK_S32  TRDelta;
        TRDelta = ctx->frame_cur->tr - ctx->frame_ref1->tr;
        if (TRDelta < 0)
            TRDelta += ctx->TRWrap;
        mpp_frame_set_pts(ctx->frame_cur->f, ctx->frame_ref1->frameNumber + TRDelta);
        mpp_buf_slot_set_flag(ctx->frame_slots, ctx->frame_cur->slot_index, SLOT_QUEUE_USE);
        mpp_buf_slot_enqueue(ctx->frame_slots, ctx->frame_cur->slot_index, QUEUE_DISPLAY);
        mpp_buf_slot_clr_flag(ctx->frame_slots, ctx->frame_cur->slot_index, SLOT_CODEC_USE);
        ctx->frame_cur->slot_index = 0xFF;
    }

    else if (ctx->frame_cur->picCodingType != 0xffffffff) {
        RVFrameHead*tmpHD;

        ctx->ref_frame_cnt++;
        if (ctx->frame_ref0->slot_index < 0x7f) {
            mpp_buf_slot_set_flag(ctx->frame_slots, ctx->frame_ref0->slot_index, SLOT_QUEUE_USE);
            mpp_buf_slot_enqueue(ctx->frame_slots, ctx->frame_ref0->slot_index, QUEUE_DISPLAY);
            mpp_frame_set_pts(ctx->frame_cur->f, ctx->frame_cur->frameNumber );//need to be modified ????
            ctx->frame_ref0->flags = 0;
        }
        if (ctx->frame_ref1->slot_index < 0x7f) {
            mpp_buf_slot_clr_flag(ctx->frame_slots, ctx->frame_ref1->slot_index, SLOT_CODEC_USE);
            mpp_frame_set_pts(ctx->frame_cur->f, ctx->frame_cur->frameNumber);//need to be modified ????
            ctx->frame_ref1->slot_index = 0xff;
        }
        tmpHD = ctx->frame_ref1;
        ctx->frame_ref1 = ctx->frame_ref0;
        ctx->frame_ref0 = ctx->frame_cur;
        ctx->frame_cur = tmpHD;
    }
    mpp_log("rmvbd_update_ref_frame leave!\n");
    return ret;

}





/*!
***********************************************************************
* \brief
*   parser
***********************************************************************
*/



MPP_RET rmvbd_parser_parse(void *ctx, HalDecTask *in_task)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_parse enter ctx %p\n", ctx);
    if (NULL == ctx) {
        mpp_err("input ctx is null");
        return MPP_NOK;
    }
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    in_task->valid = 0;
    p->framesize = (RK_U32)mpp_packet_get_length(in_task->input_packet);
    mpp_assert(p->framesize);
    mpp_set_bitread_ctx(p->bit_ctx, p->stream, p->framesize);
    ret = rmvb_decoder_head(p);
    if ((p->ref_frame_cnt < 2) && (p->frame_cur->picCodingType == RV_B_PIC)) {
        mpp_err("frame_cnt is %d < 2 or picodingtype is not rv_b_pic %d\n", p->ref_frame_cnt, p->frame_cur->picCodingType);
        return MPP_NOK;
    }
    if ((p->needSkipToKeyFrm == RV_SKIP_TO_KEY_FRAME) &&
        (p->frame_cur->picCodingType != RV_I_PIC) &&
        (p->frame_cur->picCodingType != RV_FI_PIC)) {
        mpp_err("frame_cur picodingtype is not correct\n");
        return MPP_NOK;
    } else
        p->needSkipToKeyFrm = 0;

    if (ret == MPP_OK)
        rmvbd_alloc_frame(p);

    rmvbd_convert_to_dxva(p);

    in_task->syntax.data = (void *)p->rmvbsyn;
    in_task->syntax.number = sizeof(rmvb_Syntax);
    in_task->output = p->frame_cur->slot_index;

    if (p->frame_ref0->slot_index < 0x7f && (p->frame_ref0->slot_index != p->frame_cur->slot_index)) {
        mpp_buf_slot_set_flag(p->frame_slots, p->frame_ref0->slot_index, SLOT_HAL_INPUT);
        in_task->refer[0] = p->frame_ref0->slot_index;
    }

    if (p->frame_ref1->slot_index < 0x7f && (p->frame_ref1->slot_index != p->frame_cur->slot_index)) {
        mpp_buf_slot_set_flag(p->frame_slots, p->frame_ref1->slot_index, SLOT_HAL_INPUT);
        in_task->refer[1] = p->frame_ref1->slot_index;
    }
    in_task->valid = 1;
    rmvbd_update_ref_frame(p);
    mpp_log("rmvbd_parser_parse leave!\n");
    return ret;
}

MPP_RET rmvbd_parser_callback(void *ctx, void *errinfo)
{
    MPP_RET ret = MPP_OK;
    rmvbd_dbg_func("rmvbd_parser_callback enter ctx %p\n", ctx);
    rmvbdParserContex *p = (rmvbdParserContex*)ctx;
    rmvbd_parser_reset(p);
    (void)errinfo;
    mpp_log("rmvbd_parser_callback leave!\n");
    return ret;
}




const ParserApi api_rmvbd_parser = {
    "rmvbd_parse",
    MPP_VIDEO_CodingRV,
    sizeof(rmvbdParserContex),
    0,
    rmvbd_parser_init,
    rmvbd_parser_deinit,
    rmvbd_parser_prepare,
    rmvbd_parser_parse,
    rmvbd_parser_reset,
    rmvbd_parser_flush,
    rmvbd_parser_control,
    rmvbd_parser_callback,
};
