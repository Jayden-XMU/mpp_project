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

#define  MODULE_TAG "mpp"

#include "mpp_log.h"
#include "mpp_mem.h"
#include "mpp_env.h"
#include "mpp_time.h"

#include "mpp.h"
#include "mpp_dec.h"
#include "mpp_enc.h"
#include "mpp_hal.h"
#include "mpp_task_impl.h"
#include "mpp_buffer_impl.h"
#include "mpp_frame_impl.h"
#include "mpp_packet_impl.h"

#define MPP_TEST_FRAME_SIZE     SZ_1M
#define MPP_TEST_PACKET_SIZE    SZ_512K

Mpp::Mpp()
    : mPackets(NULL),
      mFrames(NULL),
      mTasks(NULL),
      mPacketPutCount(0),
      mPacketGetCount(0),
      mFramePutCount(0),
      mFrameGetCount(0),
      mTaskPutCount(0),
      mTaskGetCount(0),
      mPacketGroup(NULL),
      mFrameGroup(NULL),
      mExternalFrameGroup(0),
      mInputPort(NULL),
      mOutputPort(NULL),
      mInputTaskQueue(NULL),
      mOutputTaskQueue(NULL),
      mThreadCodec(NULL),
      mThreadHal(NULL),
      mDec(NULL),
      mEnc(NULL),
      mType(MPP_CTX_BUTT),
      mCoding(MPP_VIDEO_CodingUnused),
      mInitDone(0),
      mPacketBlock(0),
      mInputBlock(0),
      mOutputBlock(0),
      mMultiFrame(0),
      mInputTask(NULL),
      mStatus(0),
      mParserFastMode(0),
      mParserNeedSplit(0),
      mParserInternalPts(0)
{
}

MPP_RET Mpp::init(MppCtxType type, MppCodingType coding)
{
    if (mpp_check_support_format(type, coding)) {
        mpp_err("unable to create unsupported type %d coding %d\n", type, coding);
        return MPP_NOK;
    }

    mType = type;
    mCoding = coding;
    switch (mType) {
    case MPP_CTX_DEC : {
        mPackets    = new mpp_list((node_destructor)mpp_packet_deinit);
        mFrames     = new mpp_list((node_destructor)mpp_frame_deinit);
        mTasks      = new mpp_list((node_destructor)NULL);

        MppDecCfg cfg = {
            coding,
            mParserFastMode,
            mParserNeedSplit,
            mParserInternalPts,
            this,
        };
        mpp_dec_init(&mDec, &cfg);

        if (mCoding != MPP_VIDEO_CodingMJPEG) {
            mThreadCodec = new MppThread(mpp_dec_parser_thread, this, "mpp_dec_parser");
            mThreadHal  = new MppThread(mpp_dec_hal_thread, this, "mpp_dec_hal");

            mpp_buffer_group_get_internal(&mPacketGroup, MPP_BUFFER_TYPE_ION);
            mpp_buffer_group_limit_config(mPacketGroup, 0, 3);

            mpp_task_queue_init(&mInputTaskQueue);
            mpp_task_queue_init(&mOutputTaskQueue);
            mpp_task_queue_setup(mInputTaskQueue, 4);
            mpp_task_queue_setup(mOutputTaskQueue, 4);
        } else {
            mThreadCodec = new MppThread(mpp_dec_advanced_thread, this, "mpp_dec_parser");

            mpp_task_queue_init(&mInputTaskQueue);
            mpp_task_queue_init(&mOutputTaskQueue);
            mpp_task_queue_setup(mInputTaskQueue, 1);
            mpp_task_queue_setup(mOutputTaskQueue, 1);
        }
    } break;
    case MPP_CTX_ENC : {
        mFrames     = new mpp_list((node_destructor)NULL);
        mPackets    = new mpp_list((node_destructor)mpp_packet_deinit);
        mTasks      = new mpp_list((node_destructor)NULL);

        mpp_enc_init(&mEnc, coding);
        mThreadCodec = new MppThread(mpp_enc_control_thread, this, "mpp_enc_ctrl");
        //mThreadHal  = new MppThread(mpp_enc_hal_thread, this, "mpp_enc_hal");

        mpp_buffer_group_get_internal(&mPacketGroup, MPP_BUFFER_TYPE_ION);
        mpp_buffer_group_get_internal(&mFrameGroup, MPP_BUFFER_TYPE_ION);

        mpp_task_queue_init(&mInputTaskQueue);
        mpp_task_queue_init(&mOutputTaskQueue);
        mpp_task_queue_setup(mInputTaskQueue, 1);
        mpp_task_queue_setup(mOutputTaskQueue, 1);
    } break;
    default : {
        mpp_err("Mpp error type %d\n", mType);
    } break;
    }

    mInputPort  = mpp_task_queue_get_port(mInputTaskQueue,  MPP_PORT_INPUT);
    mOutputPort = mpp_task_queue_get_port(mOutputTaskQueue, MPP_PORT_OUTPUT);

    if (mCoding == MPP_VIDEO_CodingMJPEG &&
        mFrames && mPackets &&
        (mDec) &&
        mThreadCodec/* &&
        mPacketGroup*/) {
        mThreadCodec->start();
        mInitDone = 1;
    } else if (mFrames && mPackets &&
               (mDec) &&
               mThreadCodec && mThreadHal &&
               mPacketGroup) {
        mThreadCodec->start();
        mThreadHal->start();
        mInitDone = 1;
    } else if (mFrames && mPackets &&
               (mEnc) &&
               mThreadCodec/* && mThreadHal */ &&
               mPacketGroup) {
        mThreadCodec->start();
        //mThreadHal->start();  // TODO
        mInitDone = 1;
    } else {
        mpp_err("error found on mpp initialization\n");
        clear();
    }

    mpp_env_get_u32("mpp_debug", &mpp_debug, 0);
    return MPP_OK;
}

Mpp::~Mpp ()
{
    clear();
}

void Mpp::clear()
{
    // MUST: release listener here
    if (mFrameGroup)
        mpp_buffer_group_set_listener((MppBufferGroupImpl *)mFrameGroup, NULL);

    if (mThreadCodec)
        mThreadCodec->stop();
    if (mThreadHal)
        mThreadHal->stop();

    if (mThreadCodec) {
        delete mThreadCodec;
        mThreadCodec = NULL;
    }
    if (mThreadHal) {
        delete mThreadHal;
        mThreadHal = NULL;
    }

    if (mInputTaskQueue) {
        mpp_task_queue_deinit(mInputTaskQueue);
        mInputTaskQueue = NULL;
    }
    if (mOutputTaskQueue) {
        mpp_task_queue_deinit(mOutputTaskQueue);
        mOutputTaskQueue = NULL;
    }

    mInputPort = NULL;
    mOutputPort = NULL;

    if (mDec || mEnc) {
        if (mType == MPP_CTX_DEC) {
            mpp_dec_deinit(mDec);
            mDec = NULL;
        } else {
            mpp_enc_deinit(mEnc);
            mEnc = NULL;
        }
    }
    if (mPackets) {
        delete mPackets;
        mPackets = NULL;
    }
    if (mFrames) {
        delete mFrames;
        mFrames = NULL;
    }
    if (mTasks) {
        delete mTasks;
        mTasks = NULL;
    }
    if (mPacketGroup) {
        mpp_buffer_group_put(mPacketGroup);
        mPacketGroup = NULL;
    }
    if (mFrameGroup && !mExternalFrameGroup) {
        mpp_buffer_group_put(mFrameGroup);
        mFrameGroup = NULL;
    }
}

MPP_RET Mpp::put_packet(MppPacket packet)
{
    if (!mInitDone)
        return MPP_NOK;

    AutoMutex autoLock(mPackets->mutex());
    RK_U32 eos = mpp_packet_get_eos(packet);

    if (mPackets->list_size() < 4 || eos) {
        MppPacket pkt;
        if (MPP_OK != mpp_packet_copy_init(&pkt, packet))
            return MPP_NOK;

        mPackets->add_at_tail(&pkt, sizeof(pkt));
        mPacketPutCount++;
        mThreadCodec->signal();

        // when packet has been send clear the length
        mpp_packet_set_length(packet, 0);
        return MPP_OK;
    }

    return MPP_NOK;
}

MPP_RET Mpp::get_frame(MppFrame *frame)
{
    if (!mInitDone)
        return MPP_NOK;

    AutoMutex autoLock(mFrames->mutex());
    MppFrame first = NULL;

    if (0 == mFrames->list_size()) {
        mThreadCodec->signal();
        if (mOutputBlock)
            mFrames->wait();
        msleep(1);
    }

    if (mFrames->list_size()) {
        mFrames->del_at_head(&first, sizeof(frame));
        mFrameGetCount++;
        mThreadHal->signal();

        if (mMultiFrame) {
            MppFrame prev = first;
            MppFrame next = NULL;
            while (mFrames->list_size()) {
                mFrames->del_at_head(&next, sizeof(frame));
                mFrameGetCount++;
                mThreadHal->signal();
                mpp_frame_set_next(prev, next);
                prev = next;
            }
        }
    }
    *frame = first;
    return MPP_OK;
}

MPP_RET Mpp::put_frame(MppFrame frame)
{
    if (!mInitDone)
        return MPP_NOK;

    MPP_RET ret = MPP_NOK;
    MppTask task = mInputTask;

    do {
        if (NULL == task) {
            ret = dequeue(MPP_PORT_INPUT, &task);
            if (ret) {
                mpp_log_f("failed to dequeue from input port ret %d\n", ret);
                break;
            }
        }

        /* FIXME: use wait to do block wait */
        if (mInputBlock && NULL == task) {
            msleep(2);
            continue;
        }

        mpp_assert(task);

        ret = mpp_task_meta_set_frame(task, KEY_INPUT_FRAME, frame);
        if (ret) {
            mpp_log_f("failed to set input frame to task ret %d\n", ret);
            break;
        }

        ret = enqueue(MPP_PORT_INPUT, task);
        if (ret) {
            mpp_log_f("failed to enqueue task to input port ret %d\n", ret);
            break;
        }

        if (mInputBlock) {
            while (MPP_NOK == mpp_port_can_dequeue(mInputPort)) {
                msleep(2);
            }

            ret = dequeue(MPP_PORT_INPUT, &task);
            if (ret) {
                mpp_log_f("failed to dequeue from input port ret %d\n", ret);
                break;
            }

            mpp_assert(task);
            ret = mpp_task_meta_get_frame(task, KEY_INPUT_FRAME, &frame);
            if (frame) {
                mpp_frame_deinit(&frame);
                frame = NULL;
            }
        }

        break;
    } while (1);

    mInputTask = task;

    return ret;
}

MPP_RET Mpp::get_packet(MppPacket *packet)
{
    if (!mInitDone)
        return MPP_NOK;

    MPP_RET ret = MPP_OK;
    MppTask task = NULL;

    do {
        if (NULL == task) {
            ret = dequeue(MPP_PORT_OUTPUT, &task);
            if (ret) {
                mpp_log_f("failed to dequeue from output port ret %d\n", ret);
                break;
            }
        }

        /* FIXME: use wait to do block wait */
        if (NULL == task) {
            if (mOutputBlock) {
                msleep(2);
                continue;
            } else {
                break;
            }
        }

        mpp_assert(task);

        ret = mpp_task_meta_get_packet(task, KEY_OUTPUT_PACKET, packet);
        if (ret) {
            mpp_log_f("failed to get output packet from task ret %d\n", ret);
            break;
        }

        mpp_assert(*packet);

        if (mpp_debug & MPP_DBG_PTS)
            mpp_log_f("pts %lld\n", mpp_packet_get_pts(*packet));

        ret = enqueue(MPP_PORT_OUTPUT, task);
        if (ret) {
            mpp_log_f("failed to enqueue task to output port ret %d\n", ret);
        }

        break;
    } while (1);

    return ret;
}

MPP_RET Mpp::dequeue(MppPortType type, MppTask *task)
{
    if (!mInitDone)
        return MPP_NOK;

    MPP_RET ret = MPP_NOK;
    AutoMutex autoLock(mPortLock);
    MppTaskQueue port = NULL;
    switch (type) {
    case MPP_PORT_INPUT : {
        port = mInputPort;
    } break;
    case MPP_PORT_OUTPUT : {
        port = mOutputPort;
    } break;
    default : {
    } break;
    }

    if (port)
        ret = mpp_port_dequeue(port, task);

    return ret;
}

MPP_RET Mpp::enqueue(MppPortType type, MppTask task)
{
    if (!mInitDone)
        return MPP_NOK;

    MPP_RET ret = MPP_NOK;
    AutoMutex autoLock(mPortLock);
    MppTaskQueue port = NULL;
    switch (type) {
    case MPP_PORT_INPUT : {
        port = mInputPort;
    } break;
    case MPP_PORT_OUTPUT : {
        port = mOutputPort;
    } break;
    default : {
    } break;
    }

    if (port) {
        mThreadCodec->lock();
        ret = mpp_port_enqueue(port, task);
        if (MPP_OK == ret) {
            // if enqueue success wait up thread
            mThreadCodec->signal();
        }
        mThreadCodec->unlock();
    }

    return ret;
}

MPP_RET Mpp::control(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_NOK;

    switch (cmd & CMD_MODULE_ID_MASK) {
    case CMD_MODULE_OSAL : {
        ret = control_osal(cmd, param);
    } break;
    case CMD_MODULE_MPP : {
        mpp_assert(cmd > MPP_CMD_BASE);
        mpp_assert(cmd < MPP_CMD_END);

        ret = control_mpp(cmd, param);
    } break;
    case CMD_MODULE_CODEC : {
        switch (cmd & CMD_CTX_ID_MASK) {
        case CMD_CTX_ID_DEC : {
            mpp_assert(mType == MPP_CTX_DEC || mType == MPP_CTX_BUTT);
            mpp_assert(cmd > MPP_DEC_CMD_BASE);
            mpp_assert(cmd < MPP_DEC_CMD_END);

            ret = control_dec(cmd, param);
        } break;
        case CMD_CTX_ID_ENC : {
            mpp_assert(mType == MPP_CTX_ENC);
            mpp_assert(cmd > MPP_ENC_CMD_BASE);
            mpp_assert(cmd < MPP_ENC_CMD_END);

            ret = control_enc(cmd, param);
        } break;
        case CMD_CTX_ID_ISP : {
            mpp_assert(mType == MPP_CTX_ISP);
            ret = control_isp(cmd, param);
        } break;
        default : {
            mpp_assert(cmd > MPP_CODEC_CMD_BASE);
            mpp_assert(cmd < MPP_CODEC_CMD_END);

            ret = control_codec(cmd, param);
        } break;
        }
    } break;
    default : {
    } break;
    }

    if (ret)
        mpp_err("command %x param %p ret %d\n", cmd, param, ret);

    return ret;
}

MPP_RET Mpp::reset()
{
    if (!mInitDone)
        return MPP_OK;

    MppPacket pkt = NULL;

    /*
     * On mp4 case extra data of sps/pps will be put at the beginning
     * If these packet was reset before they are send to decoder then
     * decoder can not get these important information to continue decoding
     * To avoid this case happen we need to save it on reset beginning
     * then restore it on reset end.
     */
    mPackets->lock();
    if (mPackets->list_size()) {
        mPackets->del_at_head(&pkt, sizeof(pkt));
    }
    mPackets->flush();
    mPackets->unlock();

    mFrames->lock();
    mFrames->flush();
    mFrames->unlock();

    mThreadCodec->lock(THREAD_RESET);

    if (mType == MPP_CTX_DEC) {
        mpp_dec_reset(mDec);

        if (mDec->coding != MPP_VIDEO_CodingMJPEG) {
            mThreadCodec->lock();
            mThreadCodec->signal();
            mThreadCodec->unlock();
            mThreadCodec->wait(THREAD_RESET);
        }
    } else {
        mpp_enc_reset(mEnc);
    }
    mThreadCodec->unlock(THREAD_RESET);

    if (pkt != NULL) {
        RK_U32 flags = mpp_packet_get_flag(pkt);

        if (flags & MPP_PACKET_FLAG_EXTRA_DATA) {
            put_packet(pkt);
        }
        mpp_packet_deinit(&pkt);
        pkt = NULL;
    }

    return MPP_OK;
}


MPP_RET Mpp::control_mpp(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_OK;

    switch (cmd) {
    case MPP_SET_INPUT_BLOCK: {
        RK_U32 block = *((RK_U32 *)param);
        mInputBlock = block;
    } break;
    case MPP_SET_OUTPUT_BLOCK: {
        RK_U32 block = *((RK_U32 *)param);
        mOutputBlock = block;
    } break;
    default : {
        ret = MPP_NOK;
    } break;
    }
    return ret;
}

MPP_RET Mpp::control_osal(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_NOK;

    mpp_assert(cmd > MPP_OSAL_CMD_BASE);
    mpp_assert(cmd < MPP_OSAL_CMD_END);

    (void)cmd;
    (void)param;
    return ret;
}

MPP_RET Mpp::control_codec(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_NOK;

    (void)cmd;
    (void)param;
    return ret;
}

MPP_RET Mpp::control_dec(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_NOK;

    switch (cmd) {
    case MPP_DEC_SET_FRAME_INFO: {
        ret = mpp_dec_control(mDec, cmd, param);
    } break;
    case MPP_DEC_SET_EXT_BUF_GROUP: {
        mFrameGroup = (MppBufferGroup)param;
        if (param) {
            mExternalFrameGroup = 1;
            ret = mpp_buffer_group_set_listener((MppBufferGroupImpl *)param, (void *)mThreadCodec);
            mThreadCodec->signal();
        } else {
            mExternalFrameGroup = 0;
            ret = mpp_buffer_group_set_listener(NULL, (void *)mThreadCodec);
        }
    } break;
    case MPP_DEC_SET_INFO_CHANGE_READY: {
        ret = mpp_buf_slot_ready(mDec->frame_slots);
    } break;
    case MPP_DEC_SET_INTERNAL_PTS_ENABLE: {
        if (mCoding == MPP_VIDEO_CodingMPEG2 || mCoding == MPP_VIDEO_CodingMPEG4) {
            ret = mpp_dec_control(mDec, cmd, param);
        } else {
            mpp_err("coding %x does not support use internal pts control\n", mCoding);
        }
    } break;
    case MPP_DEC_SET_PARSER_SPLIT_MODE: {
        RK_U32 flag = *((RK_U32 *)param);
        mParserNeedSplit = flag;
        ret = MPP_OK;
    } break;
    case MPP_DEC_SET_PARSER_FAST_MODE: {
        RK_U32 flag = *((RK_U32 *)param);
        mParserFastMode = flag;
        ret = MPP_OK;
    } break;
    case MPP_DEC_GET_STREAM_COUNT: {
        AutoMutex autoLock(mPackets->mutex());
        *((RK_S32 *)param) = mPackets->list_size();
        ret = MPP_OK;
    } break;
    case MPP_DEC_GET_VPUMEM_USED_COUNT: {
        ret = mpp_dec_control(mDec, cmd, param);
    } break;
    case MPP_DEC_SET_OUTPUT_FORMAT: {
        ret = mpp_dec_control(mDec, cmd, param);
    } break;
    default : {
    } break;
    }
    return ret;
}

MPP_RET Mpp::control_enc(MpiCmd cmd, MppParam param)
{
    mpp_assert(mEnc);
    return mpp_enc_control(mEnc, cmd, param);
}

MPP_RET Mpp::control_isp(MpiCmd cmd, MppParam param)
{
    MPP_RET ret = MPP_NOK;

    mpp_assert(cmd > MPP_ISP_CMD_BASE);
    mpp_assert(cmd < MPP_ISP_CMD_END);

    (void)cmd;
    (void)param;
    return ret;
}

