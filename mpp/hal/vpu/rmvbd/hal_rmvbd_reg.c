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

#define MODULE_TAG "hal_rmvbd_reg"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpp_err.h"
#include "vpu.h"
#include "mpp_mem.h"
#include "hal_rmvbd_reg.h"
#include "mpp_log.h"
#include "mpp_env.h"
#include "mpp_hal.h"

RK_U32 rmvb_debug = 1;
#define RMVBD_HAL_DBG_REG_PUT       (0x00000001)
#define RMVBD_HAL_DBG_REG_GET       (0x00000002)
#define RMVBD_BUF_SIZE_QPTAB            (256)

MPP_RET hal_rmvbd_init(void *hal, MppHalCfg *cfg)
{
    MPP_RET ret = MPP_OK;

    rmvb_Syntax*p0 = (rmvb_Syntax*)hal;

    RMVBHalContext*p = (RMVBHalContext*)hal;

    RMFUN_TEST("RMFUN_IN");
    RV_Dec_Reg *reg = &p->regs;

    RMVB_ASSERT(reg);
    p->packet_slots = cfg->packet_slots;
    p->frame_slots = cfg->frame_slots;
//  p->int_cb = cfg->hal_int_cb;

#ifdef RKPLATFORM
    if (p->vpu_socket <= 0) {
        p->vpu_socket = VPUClientInit(VPU_DEC);
        if (p->vpu_socket <= 0) {
            mpp_err("get vpu_socket(%d) <=0, failed. \n", p->vpu_socket);
            return MPP_ERR_UNKNOW;
        } else {
            mpp_log("get vpu_socket(%d), success. \n", p->vpu_socket);
        }
    }
#endif

    if (p->group == NULL) {

#ifdef RKPLATFORM
        mpp_err("mpp_buffer_group_get_internal used ion in");
        ret = mpp_buffer_group_get_internal(&p->group, MPP_BUFFER_TYPE_ION);
#else
        ret = mpp_buffer_group_get_internal(&p->group, MPP_BUFFER_TYPE_NORMAL);
#endif
        if (MPP_OK != ret) {
            mpp_err("rmvbd_hal mpp_buffer_group_get failed\n");
            return ret;
        }
    }


    ret = mpp_buffer_get(p->group, &p->qp_table, RMVBD_BUF_SIZE_QPTAB);
    if (MPP_OK != ret) {
        mpp_err("rmvbd_hal qtable_base get buffer failed\n");
        return ret;
    }


    ret = mpp_buffer_get(p->group, &p->mv_buf, 64 * 2 * sizeof(RK_U8));
    if (MPP_OK != ret) {
        mpp_err_f("failed to get qp talbe buffer ret %d\n", ret);
        return ret;
    }

    RK_U32  size[9] = { 0, 1, 1, 2, 2, 3, 3, 3, 3 };
    memset(&p->regs, 0, sizeof(RV_Dec_Reg));
    reg->config3.sw_dec_axi_rn_id = 3;
    reg->control.sw_dec_timeout_e = 1;
    reg->config2.sw_dec_strswap32_e = 1;    //change
    reg->config2.sw_dec_strendian_e = DEC_X170_LITTLE_ENDIAN;
    reg->config2.sw_dec_inswap32_e = 1;     //change
    reg->config2.sw_dec_outswap32_e = 1; //change

    reg->control.sw_dec_clk_gate_e = 1;     //change
    reg->config2.sw_dec_in_endian = DEC_X170_LITTLE_ENDIAN; //change
    reg->config2.sw_dec_out_endian = DEC_X170_LITTLE_ENDIAN;

    reg->config1.sw_dec_out_tiled_e = 0;
    reg->config3.sw_dec_max_burst = DEC_X170_BUS_BURST_LENGTH_16;
    reg->config1.sw_dec_scmd_dis = 0;
    reg->config1.sw_dec_adv_pre_dis = 0;
    reg->prefetch_ctl.sw_apf_threshold = 8;

    reg->config1.sw_dec_latency = 0;
    reg->config3.sw_dec_data_disc_e = 0;

    reg->interrupt.sw_dec_irq = 0;
    reg->config3.sw_dec_axi_rn_id = 0;
    reg->config3.sw_dec_axi_wr_id = 0;
    if (p0->isRV8)
        reg->frame_len.sw_framenum_len = size[p0->pic_rpr_num];

    if (p0->isRV8) {
        reg->prefilt_tap.sw_pred_bc_tap_0_0 = -1;
        reg->prefilt_tap.sw_pred_bc_tap_0_1 = 12;
        reg->prefilt_tap.sw_pred_bc_tap_0_2 = 6;
        reg->pre_filter0.sw_pred_bc_tap_1_0 = 6;
        reg->pre_filter0.sw_pred_bc_tap_1_1 = 9;
        reg->pre_filter1.sw_pred_bc_tap_1_2 = 1;
    } else {
        reg->prefilt_tap.sw_pred_bc_tap_0_0 = 1;
        reg->prefilt_tap.sw_pred_bc_tap_0_1 = -5;
        reg->prefilt_tap.sw_pred_bc_tap_0_2 = 20;
        reg->pre_filter0.sw_pred_bc_tap_1_0 = 1;
        reg->pre_filter0.sw_pred_bc_tap_1_1 = -5;
        reg->pre_filter1.sw_pred_bc_tap_1_2 = 52;
    }

    reg->sw_dec_mode = 8;
    reg->frameinfo.sw_profile = 1 - p0->isRV8;
    reg->control.sw_pic_fieldmode_e = 0;
    reg->control.sw_pic_interlace_e = 0;
    reg->directmv_reg = mpp_buffer_get_fd(p->mv_buf);
    mpp_env_get_u32("rmvb_debug", &rmvb_debug, 0);
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}

MPP_RET hal_rmvbd_deinit(void *hal)
{

    MPP_RET ret = MPP_OK;
    RMFUN_TEST("RMFUN_IN");
    RMVBHalContext*p = (RMVBHalContext*)hal;

    RMVB_ASSERT(p);

    if (p->mv_buf) {
        ret = mpp_buffer_put(p->mv_buf);
        if (MPP_OK != ret) {
            mpp_err("rmvb_hal qp_table put buffer failed\n");
            return ret;
        }
        p->mv_buf = NULL;
    }

    if (p->group) {
        ret = mpp_buffer_group_put(p->group);
        if (MPP_OK != ret) {
            mpp_err("rmvb_hal group free buffer failed\n");
            return ret;
        }
        p->group = NULL;
    }

#ifdef RKPLATFORM
    if (p->vpu_socket >= 0) {
        VPUClientRelease(p->vpu_socket);
        p->vpu_socket = -1;
    }
#endif

    RMFUN_TEST("RMFUN_OUT");
    return ret;
}




static void hal_rmvbd_get_buffer_by_index(RMVBHalContext *ctx, RK_S32 index, MppBuffer *buffer)
{
    if (index >= 0) {
        mpp_buf_slot_get_prop(ctx->frame_slots, index, SLOT_BUFFER, buffer);
        mpp_assert(*buffer);
    }
}



MPP_RET hal_rmvbd_gen_regs(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;
    RMFUN_TEST("RMFUN_IN");

    MppBuffer buf_frm_curr = NULL;
    MppBuffer buf_frm_ref0 = NULL;
    MppBuffer buf_frm_ref1 = NULL;
    MppBuffer buf_pkt = NULL;
    HalDecTask *cur_task = &task->dec;
    RMVBHalContext*p = (RMVBHalContext*)hal;
    rmvb_Syntax *dx = (rmvb_Syntax *)task->dec.syntax.data;
    // RVFrameHead *frame_cur = dx->frame_cur;
    // RVFrameHead *frame_ref0 = dx->frame_ref0;
    // RVFrameHead *frame_ref1 = dx->frame_ref1;
    mpp_buf_slot_get_prop(p->packet_slots, cur_task->input, SLOT_BUFFER, &buf_pkt);
    mpp_assert(buf_pkt);
    hal_rmvbd_get_buffer_by_index(p, cur_task->output, &buf_frm_curr);//buf_frm_curr or frame_cur->f ???
    // hal_rmvbd_get_buffer_by_index(p, cur_task->output, frame_cur->f);
    hal_rmvbd_get_buffer_by_index(p, cur_task->refer[0], &buf_frm_ref0);
    hal_rmvbd_get_buffer_by_index(p, cur_task->refer[1], &buf_frm_ref1);
    p->fd_curr = mpp_buffer_get_fd(buf_frm_curr);
    p->fd_ref0 = (buf_frm_ref0) ? (mpp_buffer_get_fd(buf_frm_ref0)) : (-1);
    p->fd_ref1 = (buf_frm_ref1) ? (mpp_buffer_get_fd(buf_frm_ref1)) : (-1);
    void *q_table = NULL;

    RK_U32 TRWrap = dx->TRWrap;
    RMVB_ASSERT(dx);
    RV_Dec_Reg *regs = &p->regs;
    RMVB_ASSERT(regs);
    task->dec.valid = 0;
    q_table = mpp_buffer_get_ptr(p->qp_table);
    memcpy(q_table, &dx->qp, RMVBD_BUF_SIZE_QPTAB);//copy memory from dx->qp to q_table
    regs->stream_buffinfo.sw_init_qp = dx->frame_cur->qp;
    regs->stream_buffinfo.sw_stream_len = dx->framesize;
    regs->stream_bitinfo.sw_stream_start_bit = 0;
    regs->cur_pic_base = p->fd_curr;
    regs->slice_lum.sw_pic_slice_am = dx->slicenum;
    regs->slice_table = mpp_buffer_get_fd(q_table);
    if (!dx->isRV8)
        regs->frame_len.sw_framenum_len = dx->frame_cur->frame_codelen;

    regs->pic_params.sw_pic_mb_width = dx->frame_cur->mb_width;
    regs->pic_params.sw_pic_mb_height_p = dx->frame_cur->mb_height;
    regs->frameinfo.sw_rv_bwd_scale = 1 << 13;
    regs->frameinfo.sw_rv_fwd_scale = 1 << 13;

    if (dx->frame_cur->picCodingType == RV_B_PIC) {
        RK_S32 trd, trb;
        RMVB_ASSERT(dx->frame_ref0);
        RMVB_ASSERT(dx->frame_ref1);
        trd = (dx->frame_ref0->tr) - (dx->frame_ref1->tr);
        trb = (dx->frame_cur->tr) - (dx->frame_ref1->tr);
        if (trd < 0)
            trd += TRWrap;
        if (trb < 0)
            trb += TRWrap;
        if (trd == 0) {
            RMVB_ERROR_LOG("trd is equal to 0!\n");
            //may be current stream error we skip it no return error,modify by csy 2013.9.16
            return MPP_NOK;
        }
        regs->control.sw_pic_b_e = 1;
        regs->control.sw_pic_inter_e = 1;
        regs->control.sw_write_mvs_e = 0;
        regs->frameinfo.sw_rv_bwd_scale = (trb << 14) / trd;
        regs->frameinfo.sw_rv_fwd_scale = ((trd - trb) << 14) / trd;
        if (VPUClientGetIOMMUStatus() > 0) {
            regs->ref0_info.sw_refer0_base = p->fd_ref1 >> 2;
            regs->ref1_info.sw_refer1_base = p->fd_ref1 >> 2;
            regs->ref2_info.sw_refer2_base = p->fd_ref0 >> 2;
            regs->ref3_info.sw_refer3_base = p->fd_ref0 >> 2;
        } else {
            RK_U32 *curreg = NULL;
            curreg = (RK_U32 *)&regs->ref0_info;
            *curreg = p->fd_ref1;

            curreg = (RK_U32 *)&regs->ref1_info;
            *curreg = p->fd_ref1;

            curreg = (RK_U32 *)&regs->ref2_info;
            *curreg = p->fd_ref0;

            curreg = (RK_U32 *)&regs->ref3_info;
            *curreg = p->fd_ref0;
        }
    } else if (dx->frame_cur->picCodingType == RV_P_PIC) {
        // RMVB_ASSERT(frame_ref0);
        if (mpp_frame_get_buffer(dx->frame_ref0)) {
            regs->control.sw_pic_b_e = 0;
            regs->control.sw_pic_inter_e = 1;
            regs->control.sw_write_mvs_e = 1;
            if (VPUClientGetIOMMUStatus() > 0) {
                regs->ref0_info.sw_refer0_base = p->fd_ref0 >> 2;
                regs->ref1_info.sw_refer1_base = p->fd_ref0 >> 2;
                regs->ref2_info.sw_refer2_base = p->fd_curr >> 2;
                regs->ref3_info.sw_refer3_base = p->fd_curr >> 2;
            } else {
                RK_U32 *curreg = NULL;
                curreg = (RK_U32 *)&regs->ref0_info;
                *curreg = p->fd_ref0;

                curreg = (RK_U32 *)&regs->ref1_info;
                *curreg = p->fd_ref0;

                curreg = (RK_U32 *)&regs->ref2_info;
                *curreg = p->fd_curr;

                curreg = (RK_U32 *)&regs->ref3_info;
                *curreg = p->fd_curr;
            }
        } else {
            RMVB_ERROR_LOG("frame_ref0 has no buffer!\n");
            return MPP_NOK;
        }
    } else {
        regs->control.sw_pic_b_e = 0;
        regs->control.sw_pic_inter_e = 0;
        regs->control.sw_write_mvs_e = 0;
        if (VPUClientGetIOMMUStatus() > 0) {
            regs->ref0_info.sw_refer0_base = p->fd_curr >> 2;
            regs->ref1_info.sw_refer1_base = p->fd_curr >> 2;
            regs->ref2_info.sw_refer2_base = p->fd_curr >> 2;
            regs->ref3_info.sw_refer3_base = p->fd_curr >> 2;
        } else {
            RK_U32 *curreg = NULL;
            curreg = (RK_U32 *)&regs->ref0_info;
            *curreg = p->fd_curr;

            curreg = (RK_U32 *)&regs->ref1_info;
            *curreg = p->fd_curr;

            curreg = (RK_U32 *)&regs->ref2_info;
            *curreg = p->fd_curr;

            curreg = (RK_U32 *)&regs->ref3_info;
            *curreg = p->fd_curr;
        }
    }

    regs->prefetch_ctl.sw_startmb_x = 0;
    regs->prefetch_ctl.sw_startmb_y = 0;
    regs->control.sw_dec_out_dis = 0;
    regs->config1.sw_filtering_dis = 1;
    regs->pre_pictype.sw_prev_anc_type = (dx->frame_ref0->picCodingType) == RV_P_PIC;;
    regs->control.sw_dec_e = 1;
    (void)hal;
    (void)task;
    RMFUN_TEST("RMFUN_OUT");
    return ret;

}

MPP_RET hal_rmvbd_start(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;


#ifdef RKPLATFORM
    RMVBHalContext*p = (RMVBHalContext*)hal;
    RK_U32 *p_regs = (RK_U32 *)&p->regs;
    RMFUN_TEST("RMFUN_IN");

    if (rmvb_debug & RMVBD_HAL_DBG_REG_PUT) {
        RK_U32 i = 0;

        for (i = 0; i < VPU_REG_NUM_DEC; i++) {
            mpp_log("reg[%03d]: %08x\n", i, p_regs[i]);
        }
    }

    ret = VPUClientSendReg(p->vpu_socket, p_regs, VPU_REG_NUM_DEC);
    if (ret != 0) {
        mpp_err("VPUClientSendReg Failed!!!\n");
        return MPP_ERR_VPUHW;
    }

#endif
    (void)hal;
    (void)task;
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}

MPP_RET hal_rmvbd_wait(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_OK;

#ifdef RKPLATFORM

    RMVBHalContext*p = (RMVBHalContext*)hal;
    RMFUN_TEST("RMFUN_IN");
    VPU_CMD_TYPE cmd = 0;
    RV_Dec_Reg reg_out;
//  memset(&reg_out, 0, sizeof(RV_Dec_Reg));
    RK_U32* reg = (RK_U32 *)&reg_out;
    RK_S32 length = 0;
    ret = VPUClientWaitResult(p->vpu_socket, (RK_U32 *)&reg, VPU_REG_NUM_DEC, &cmd, &length);


    if (rmvb_debug & RMVBD_HAL_DBG_REG_GET) {
        RK_U32 i = 0;

        for (i = 0; i < VPU_REG_NUM_DEC; i++) {
            mpp_log("reg[%03d]: %08x\n", i, reg[i]);
        }
    }

#endif

    (void)hal;
    (void)task;
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}

MPP_RET hal_rmvbd_reset(void *hal)
{
    MPP_RET ret = MPP_OK;
    RMFUN_TEST("RMFUN_IN");
    (void)hal;
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}

MPP_RET hal_rmvbd_flush(void *hal)
{
    MPP_RET ret = MPP_OK;

    RMFUN_TEST("RMFUN_IN");
    (void)hal;
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}
MPP_RET hal_rmvbd_control(void *hal, RK_S32 cmd_type, void *param)
{
    MPP_RET ret = MPP_OK;
    RMFUN_TEST("RMFUN_IN");
    (void)hal;
    (void)cmd_type;
    (void)param;
    RMFUN_TEST("RMFUN_OUT");
    return ret;
}

const MppHalApi hal_api_rmvbd = {
    "rmvbd_rkdec",
    MPP_CTX_DEC,
    MPP_VIDEO_CodingRV,
    sizeof(RMVBHalContext),
    0,
    hal_rmvbd_init,
    hal_rmvbd_deinit,
    hal_rmvbd_gen_regs,
    hal_rmvbd_start,
    hal_rmvbd_wait,
    hal_rmvbd_reset,
    hal_rmvbd_flush,
    hal_rmvbd_control,
};