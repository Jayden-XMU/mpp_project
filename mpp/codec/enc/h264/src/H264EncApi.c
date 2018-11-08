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

#define MODULE_TAG "H264EncApi"

#include <string.h>

#include "mpp_env.h"
#include "mpp_log.h"

#include "h264encapi.h"
#include "enccommon.h"
#include "H264Instance.h"
#include "H264Init.h"
#include "H264PutBits.h"
#include "H264CodeFrame.h"
#include "H264Sei.h"
#include "H264RateControl.h"

/* Parameter limits */
#define H264ENCSTRMSTART_MIN_BUF        64
#define H264ENCSTRMENCODE_MIN_BUF       4096
#define H264ENC_MAX_PP_INPUT_WIDTH      4096
#define H264ENC_MAX_PP_INPUT_HEIGHT     4096
#define H264ENC_MAX_BITRATE             (50000*1200)    /* Level 4.1 limit */
#define H264ENC_MAX_USER_DATA_SIZE      2048

#define H264ENC_IDR_ID_MODULO           16

#define H264_BUS_ADDRESS_VALID(bus_address)  (((bus_address) != 0)/* &&*/ \
                                              /*((bus_address & 0x07) == 0)*/)
RK_U32 h264e_debug = 0;

/*------------------------------------------------------------------------------
    Function name : H264EncInit
    Description   : Initialize an encoder instance and returns it to application

    Return type   : H264EncRet
    Argument      : pEncCfg - initialization parameters
                    instAddr - where to save the created instance
------------------------------------------------------------------------------*/
H264EncRet H264EncInit(H264ECtx *pEncInst)
{
    H264EncRet ret;

    ret = H264Init(pEncInst);
    if (ret != H264ENC_OK) {
        mpp_err("H264EncInit: ERROR Initialization failed");
        return ret;
    }

    /* Status == INIT   Initialization succesful */


    return H264ENC_OK;
}

/*------------------------------------------------------------------------------

    Function name : H264EncRelease
    Description   : Releases encoder instance and all associated resource

    Return type   : H264EncRet
    Argument      : inst - the instance to be released
------------------------------------------------------------------------------*/
H264EncRet H264EncRelease(H264ECtx *pEncInst)
{
    h264e_dbg_func("enter\n");

    /* Check for illegal inputs */
    if (pEncInst == NULL) {
        h264e_dbg_func("H264EncRelease: ERROR Null argument");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        h264e_dbg_func("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

H264EncRet H264EncCfg(H264ECtx *pEncInst, const H264EncConfig * pEncConfig)
{
    H264EncRet ret;
    h264e_dbg_func("enter\n");

    ret = H264Cfg(pEncConfig, pEncInst);
    if (ret != H264ENC_OK) {
        mpp_err("H264EncCfg: ERROR Config failed");
        return ret;
    }

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

/*------------------------------------------------------------------------------

    Function name : H264EncSetCodingCtrl
    Description   : Sets encoding parameters

    Return type   : H264EncRet
    Argument      : inst - the instance in use
                    pCodeParams - user provided parameters
------------------------------------------------------------------------------*/
H264EncRet H264EncSetCodingCtrl(H264ECtx *pEncInst, const H264EncCodingCtrl * pCodeParams)
{
    h264e_dbg_func("enter\n");

    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (pCodeParams == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    /* Check for invalid values */
    if (pCodeParams->sliceSize > pEncInst->mbPerCol) {
        mpp_err_f("ERROR Invalid sliceSize\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Check status, only slice size allowed to be altered after start */
    if (pEncInst->encStatus != H264ENCSTAT_INIT) {
        goto set_slice_size;
    }

    if (pCodeParams->constrainedIntraPrediction > 1 ||
        pCodeParams->disableDeblockingFilter > 2 ||
        pCodeParams->enableCabac > 1 ||
        pCodeParams->transform8x8Mode > 2 ||
        pCodeParams->seiMessages > 1 || pCodeParams->videoFullRange > 1) {
        mpp_err_f("ERROR Invalid enable/disable\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    if (pCodeParams->sampleAspectRatioWidth > 65535 ||
        pCodeParams->sampleAspectRatioHeight > 65535) {
        mpp_err_f("ERROR Invalid sampleAspectRatio\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    if (pCodeParams->cabacInitIdc > 2) {
        mpp_err_f("ERROR Invalid cabacInitIdc\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Configure the values */
    if (pCodeParams->constrainedIntraPrediction == 0)
        pEncInst->picParameterSet.constIntraPred = ENCHW_NO;
    else
        pEncInst->picParameterSet.constIntraPred = ENCHW_YES;

    /* filter control header always present */
    pEncInst->picParameterSet.deblockingFilterControlPresent = ENCHW_YES;
    pEncInst->slice.disableDeblocking = pCodeParams->disableDeblockingFilter;

    if (pCodeParams->enableCabac == 0) {
        pEncInst->picParameterSet.entropyCodingMode = ENCHW_NO;
    } else {
        pEncInst->picParameterSet.entropyCodingMode = ENCHW_YES;
        pEncInst->slice.cabacInitIdc = pCodeParams->cabacInitIdc;
    }

    /* 8x8 mode: 2=enable, 1=adaptive (720p or bigger frame) */
    if ((pCodeParams->transform8x8Mode == 2) ||
        ((pCodeParams->transform8x8Mode == 1) &&
         (pEncInst->mbPerFrame >= 3600))) {
        pEncInst->picParameterSet.transform8x8Mode = ENCHW_YES;
    }

    H264SpsSetVuiAspectRatio(&pEncInst->seqParameterSet,
                             pCodeParams->sampleAspectRatioWidth,
                             pCodeParams->sampleAspectRatioHeight);

    H264SpsSetVuiVideoInfo(&pEncInst->seqParameterSet,
                           pCodeParams->videoFullRange);

    /* SEI messages are written in the beginning of each frame */
    if (pCodeParams->seiMessages) {
        pEncInst->rateControl.sei.enabled = ENCHW_YES;
    } else {
        pEncInst->rateControl.sei.enabled = ENCHW_NO;
    }

set_slice_size:
    /* Slice size is set in macroblock rows => convert to macroblocks */
    pEncInst->slice.sliceSize = pCodeParams->sliceSize * pEncInst->mbPerRow;

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

/*------------------------------------------------------------------------------

    Function name : H264EncSetRateCtrl
    Description   : Sets rate control parameters

    Return type   : H264EncRet
    Argument      : inst - the instance in use
                    pRateCtrl - user provided parameters
------------------------------------------------------------------------------*/
H264EncRet H264EncSetRateCtrl(H264ECtx *pEncInst,
                              const H264EncRateCtrl * pRateCtrl)
{
    h264RateControl_s *rc;

    RK_U32 i, tmp;

    h264e_dbg_func("enter\n");

    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (pRateCtrl == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    rc = &pEncInst->rateControl;

    /* after stream was started with HRD ON,
     * it is not allowed to change RC params */
    if (pEncInst->encStatus == H264ENCSTAT_START_FRAME && rc->hrd == ENCHW_YES) {
        mpp_err_f("ERROR Stream started with HRD ON. Not allowed to change any aprameters\n");
        return H264ENC_INVALID_STATUS;
    }

    /* Check for invalid input values */
    if (pRateCtrl->pictureRc > 1 ||
        pRateCtrl->mbRc > 1 || pRateCtrl->pictureSkip > 1 || pRateCtrl->hrd > 1) {
        mpp_err_f("ERROR Invalid enable/disable value\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    if (pRateCtrl->qpHdr > 51 ||
        pRateCtrl->qpMin > 51 ||
        pRateCtrl->qpMax > 51 ||
        pRateCtrl->qpMax < pRateCtrl->qpMin) {
        mpp_err_f("ERROR Invalid QP\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    if ((pRateCtrl->qpHdr != -1) &&
        (pRateCtrl->qpHdr < (RK_S32)pRateCtrl->qpMin ||
         pRateCtrl->qpHdr > (RK_S32)pRateCtrl->qpMax)) {
        mpp_err_f("ERROR QP out of range\n");
        return H264ENC_INVALID_ARGUMENT;
    }
    if ((RK_U32)(pRateCtrl->intraQpDelta + 12) > 24) {
        mpp_err_f("ERROR intraQpDelta out of range\n");
        return H264ENC_INVALID_ARGUMENT;
    }
    if (pRateCtrl->fixedIntraQp > 51) {
        mpp_err_f("ERROR fixedIntraQp out of range\n");
        return H264ENC_INVALID_ARGUMENT;
    }
    if (pRateCtrl->gopLen < 1 || pRateCtrl->gopLen > 150) {
        mpp_err_f("ERROR Invalid GOP length\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Bitrate affects only when rate control is enabled */
    if ((pRateCtrl->pictureRc || pRateCtrl->mbRc ||
         pRateCtrl->pictureSkip || pRateCtrl->hrd) &&
        (pRateCtrl->bitPerSecond < 10000 ||
         pRateCtrl->bitPerSecond > H264ENC_MAX_BITRATE)) {
        mpp_err_f("ERROR Invalid bitPerSecond\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    {
        RK_U32 cpbSize = pRateCtrl->hrdCpbSize;
        RK_U32 bps = pRateCtrl->bitPerSecond;
        RK_U32 level = pEncInst->seqParameterSet.levelIdx;

        /* Saturates really high settings */
        /* bits per unpacked frame */
        tmp = 3 * 8 * pEncInst->mbPerFrame * 256 / 2;
        /* bits per second */
        tmp = H264Calculate(tmp, rc->outRateNum, rc->outRateDenom);
        if (bps > (tmp / 2))
            bps = tmp / 2;

        if (cpbSize == 0) {
            cpbSize = H264MaxCPBS[level];
        } else if (cpbSize == (RK_U32) (-1)) {
            cpbSize = bps;
        }


        /* Limit minimum CPB size based on average bits per frame */
        tmp = H264Calculate(bps, rc->outRateDenom, rc->outRateNum);
        cpbSize = MAX(cpbSize, tmp);

        /* cpbSize must be rounded so it is exactly the size written in stream */
        i = 0;
        tmp = cpbSize;
        while (4095 < (tmp >> (4 + i++)));

        cpbSize = (tmp >> (4 + i)) << (4 + i);

        /* if HRD is ON we have to obay all its limits */
        if (pRateCtrl->hrd != 0) {
            if (cpbSize > H264MaxCPBS[level]) {
                mpp_err_f("ERROR. HRD is ON. hrdCpbSize higher than maximum allowed for stream level\n");
                return H264ENC_INVALID_ARGUMENT;
            }

            if (bps > H264MaxBR[level]) {
                mpp_err_f("ERROR. HRD is ON. bitPerSecond higher than maximum allowed for stream level\n");
                return H264ENC_INVALID_ARGUMENT;
            }
        }

        rc->virtualBuffer.bufferSize = cpbSize;

        /* Set the parameters to rate control */
        if (pRateCtrl->pictureRc != 0)
            rc->picRc = ENCHW_YES;
        else
            rc->picRc = ENCHW_NO;

        /* ROI and MB RC can't be used at the same time */
        if ((pRateCtrl->mbRc != 0) && (rc->roiRc == ENCHW_NO)) {
            rc->mbRc = ENCHW_YES;
        } else
            rc->mbRc = ENCHW_NO;

        if (pRateCtrl->pictureSkip != 0)
            rc->picSkip = ENCHW_YES;
        else
            rc->picSkip = ENCHW_NO;

        if (pRateCtrl->hrd != 0) {
            rc->hrd = ENCHW_YES;
            rc->picRc = ENCHW_YES;
        } else
            rc->hrd = ENCHW_NO;

        rc->qpHdr = pRateCtrl->qpHdr;
        rc->qpMin = pRateCtrl->qpMin;
        rc->qpMax = pRateCtrl->qpMax;
        rc->virtualBuffer.bitRate = bps;
        rc->gopLen = pRateCtrl->gopLen;
    }
    rc->intraQpDelta = pRateCtrl->intraQpDelta;
    rc->fixedIntraQp = pRateCtrl->fixedIntraQp;
    rc->mbQpAdjustment = pRateCtrl->mbQpAdjustment;
    (void) H264InitRc(rc);  /* new parameters checked already */

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

H264EncRet H264EncSetSeiUserData(H264ECtx *pEncInst, const RK_U8 * pUserData,
                                 RK_U32 userDataSize)
{
    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (userDataSize != 0 && pUserData == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    /* Disable user data */
    if ((userDataSize < 16) || (userDataSize > H264ENC_MAX_USER_DATA_SIZE)) {
        pEncInst->rateControl.sei.userDataEnabled = ENCHW_NO;
        pEncInst->rateControl.sei.pUserData = NULL;
        pEncInst->rateControl.sei.userDataSize = 0;
    } else {
        pEncInst->rateControl.sei.userDataEnabled = ENCHW_YES;
        pEncInst->rateControl.sei.pUserData = pUserData;
        pEncInst->rateControl.sei.userDataSize = userDataSize;
    }

    return H264ENC_OK;
}

/*------------------------------------------------------------------------------

    Function name : H264EncStrmStart
    Description   : Starts a new stream
    Return type   : H264EncRet
    Argument      : inst - encoder instance
    Argument      : pEncIn - user provided input parameters
                    pEncOut - place where output info is returned
------------------------------------------------------------------------------*/
H264EncRet H264EncStrmStart(H264ECtx *pEncInst, const H264EncIn * pEncIn,
                            H264EncOut * pEncOut)
{
    h264RateControl_s *rc;

    h264e_dbg_func("enter\n");

    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (pEncIn == NULL) || (pEncOut == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    rc = &pEncInst->rateControl;

    /* Check status */
    if (pEncInst->encStatus != H264ENCSTAT_INIT) {
        mpp_err_f("ERROR Invalid status\n");
        return H264ENC_INVALID_STATUS;
    }

    /* Check for invalid input values */
    if ((pEncIn->pOutBuf == NULL) ||
        (pEncIn->outBufSize < H264ENCSTRMSTART_MIN_BUF)) {
        mpp_err_f("ERROR Invalid input. Stream buffer\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Set stream buffer, the size has been checked */
    (void) H264SetBuffer(&pEncInst->stream, (RK_U8 *) pEncIn->pOutBuf,
                         (RK_U32) pEncIn->outBufSize);

    /* Set the profile to be used */
    if (pEncInst->seqParameterSet.profileIdc != 66 && pEncInst->seqParameterSet.profileIdc != 77
        && pEncInst->seqParameterSet.profileIdc != 100)
        pEncInst->seqParameterSet.profileIdc = 66; /* base profile */

    /* CABAC => main profile */
    if (pEncInst->picParameterSet.entropyCodingMode == ENCHW_YES)
        pEncInst->seqParameterSet.profileIdc = 77;

    /* 8x8 transform enabled => high profile */
    if (pEncInst->picParameterSet.transform8x8Mode == ENCHW_YES)
        pEncInst->seqParameterSet.profileIdc = 100;

    /* update VUI */
    if (rc->sei.enabled == ENCHW_YES) {
        H264SpsSetVuiPictStructPresentFlag(&pEncInst->seqParameterSet, 1);
    }

    if (rc->hrd == ENCHW_YES) {
        H264SpsSetVuiHrd(&pEncInst->seqParameterSet, 1);

        H264SpsSetVuiHrdBitRate(&pEncInst->seqParameterSet,
                                rc->virtualBuffer.bitRate);

        H264SpsSetVuiHrdCpbSize(&pEncInst->seqParameterSet,
                                rc->virtualBuffer.bufferSize);
    }

    /* Use the first frame QP in the PPS */
    pEncInst->picParameterSet.picInitQpMinus26 = (RK_S32) (rc->qpHdr) - 26;

    /* Init SEI */
    H264InitSei(&rc->sei, pEncInst->seqParameterSet.byteStream,
                rc->hrd, rc->outRateNum, rc->outRateDenom);

    H264SeqParameterSet(&pEncInst->stream, &pEncInst->seqParameterSet);

    H264PicParameterSet(&pEncInst->stream, &pEncInst->picParameterSet);

    if (pEncInst->stream.overflow == ENCHW_YES) {
        pEncOut->streamSize = 0;
        mpp_err_f("ERROR Output buffer too small\n");
        return H264ENC_OUTPUT_BUFFER_OVERFLOW;
    }

    /* Bytes generated */
    pEncOut->streamSize = pEncInst->stream.byteCnt;

    /* Status == START_STREAM   Stream started */
    pEncInst->encStatus = H264ENCSTAT_START_STREAM;

    pEncInst->slice.frameNum = 0;
    pEncInst->rateControl.fillerIdx = (RK_U32) (-1);

    if (rc->hrd == ENCHW_YES) {
        /* Update HRD Parameters to RC if needed */
        RK_U32 bitrate = H264SpsGetVuiHrdBitRate(&pEncInst->seqParameterSet);
        RK_U32 cpbsize = H264SpsGetVuiHrdCpbSize(&pEncInst->seqParameterSet);

        if ((rc->virtualBuffer.bitRate != (RK_S32)bitrate) ||
            (rc->virtualBuffer.bufferSize != (RK_S32)cpbsize)) {
            rc->virtualBuffer.bitRate = bitrate;
            rc->virtualBuffer.bufferSize = cpbsize;
            (void) H264InitRc(rc);
        }
    }

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

/*------------------------------------------------------------------------------

    Function name : H264EncStrmEncode
    Description   : Encodes a new picture
    Return type   : H264EncRet
    Argument      : inst - encoder instance
    Argument      : pEncIn - user provided input parameters
                    pEncOut - place where output info is returned
------------------------------------------------------------------------------*/
H264EncRet H264EncStrmEncode(H264ECtx *pEncInst, const H264EncIn * pEncIn,
                             H264EncOut * pEncOut, h264e_syntax *syntax_data)
{
    slice_s *pSlice;
    regValues_s *regs;

    h264e_dbg_func("enter\n");

    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (pEncIn == NULL) || (pEncOut == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    {
        RK_U32 nals;

        if (pEncInst->slice.sliceSize != 0) {
            /* how many  picture slices we have */
            nals = (pEncInst->mbPerFrame + pEncInst->slice.sliceSize - 1) /
                   pEncInst->slice.sliceSize;
        } else {
            nals = 1;   /* whole pic in one slice */
        }

        nals += 3;  /* SEI, Filler and "0-end" */

    }

    /* Clear the output structure */
    pEncOut->codingType = H264ENC_NOTCODED_FRAME;
    pEncOut->streamSize = 0;


    /* Check status, INIT and ERROR not allowed */
    if ((pEncInst->encStatus != H264ENCSTAT_START_STREAM) &&
        (pEncInst->encStatus != H264ENCSTAT_START_FRAME)) {
        mpp_err_f("ERROR Invalid status\n");
        return H264ENC_INVALID_STATUS;
    }

    /* Check for invalid input values */
    if ((!H264_BUS_ADDRESS_VALID(pEncIn->busOutBuf))) {
        mpp_err_f("ERROR Invalid input busOutBuf is NULL\n");
        return H264ENC_INVALID_ARGUMENT;
    }
    if (pEncIn->pOutBuf == NULL) {
        mpp_err_f("ERROR Invalid input pOutBuf is NULL\n");
        return H264ENC_INVALID_ARGUMENT;
    }
    if (pEncIn->outBufSize < H264ENCSTRMENCODE_MIN_BUF) {
        mpp_err_f("ERROR Invalid input outBufSize %d < min size %d\n",
                  pEncIn->outBufSize, H264ENCSTRMENCODE_MIN_BUF);
        return H264ENC_INVALID_ARGUMENT;
    }
    if (pEncIn->codingType > H264ENC_PREDICTED_FRAME) {
        mpp_err_f("ERROR Invalid input codingType %d < predicted %d\n",
                  pEncIn->codingType, H264ENC_PREDICTED_FRAME);
        return H264ENC_INVALID_ARGUMENT;
    }

    switch (pEncInst->preProcess.inputFormat) {
    case MPP_FMT_YUV420P:
        if (!H264_BUS_ADDRESS_VALID(pEncIn->busChromaV)) {
            mpp_err_f("ERROR Invalid input busChromaV\n");
            return H264ENC_INVALID_ARGUMENT;
        }
        /* fall through */
    case MPP_FMT_YUV420SP:
    case MPP_FMT_YUV420SP_VU:
        if (!H264_BUS_ADDRESS_VALID(pEncIn->busChromaU)) {
            mpp_err_f("ERROR Invalid input busChromaU\n");
            return H264ENC_INVALID_ARGUMENT;
        }
        /* fall through */
    case MPP_FMT_YUV422P:
    case MPP_FMT_YUV422SP:
    case MPP_FMT_YUV422SP_VU:
    case MPP_FMT_YUV422_YUYV:
    case MPP_FMT_YUV422_UYVY:
    case MPP_FMT_RGB565:
    case MPP_FMT_BGR565:
    case MPP_FMT_RGB555:
    case MPP_FMT_BGR555:
    case MPP_FMT_RGB444:
    case MPP_FMT_BGR444:
    case MPP_FMT_RGB888:
    case MPP_FMT_BGR888:
    case MPP_FMT_RGB101010:
    case MPP_FMT_BGR101010:
    case MPP_FMT_ARGB8888:
    case MPP_FMT_ABGR8888:
        if (!H264_BUS_ADDRESS_VALID(pEncIn->busLuma)) {
            mpp_err_f("ERROR Invalid input busLuma\n");
            return H264ENC_INVALID_ARGUMENT;
        }
        break;
    default:
        mpp_err_f("ERROR Invalid input format %d\n", pEncInst->preProcess.inputFormat);
        return H264ENC_INVALID_ARGUMENT;
    }

    if (pEncInst->preProcess.videoStab) {
        if (!H264_BUS_ADDRESS_VALID(pEncIn->busLumaStab)) {
            mpp_err_f("ERROR Invalid input busLumaStab\n");
            return H264ENC_INVALID_ARGUMENT;
        }
    }

    /* some shortcuts */
    pSlice = &pEncInst->slice;
    regs = &pEncInst->asic.regs;

    /* Set stream buffer, the size has been checked */
    if (H264SetBuffer(&pEncInst->stream, (RK_U8 *) pEncIn->pOutBuf,
                      (RK_S32) pEncIn->outBufSize) == ENCHW_NOK) {
        mpp_err_f("ERROR Invalid output buffer\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* update in/out buffers */
    regs->inputLumBase = pEncIn->busLuma;
    regs->inputCbBase = pEncIn->busChromaU;
    regs->inputCrBase = pEncIn->busChromaV;

    regs->outputStrmBase = pEncIn->busOutBuf;
    regs->outputStrmSize = pEncIn->outBufSize;

    {
        H264EncPictureCodingType ct = pEncIn->codingType;

        /* Status may affect the frame coding type */
        if (pEncInst->encStatus == H264ENCSTAT_START_STREAM) {
            ct = H264ENC_INTRA_FRAME;
        }
        pSlice->prevFrameNum = pSlice->frameNum;

        /* Frame coding type defines the NAL unit type */
        switch (ct) {
        case H264ENC_INTRA_FRAME:
            /* IDR-slice */
            pSlice->nalUnitType = IDR;
            pSlice->sliceType = ISLICE;
            pSlice->frameNum = 0;
            H264MadInit(&pEncInst->mad, pEncInst->mbPerFrame);
            break;
        case H264ENC_PREDICTED_FRAME:
        default:
            /* non-IDR slice */
            pSlice->nalUnitType = NONIDR;
            pSlice->sliceType = PSLICE;
            break;
        }
    }

    /* Rate control */
    H264BeforePicRc(&pEncInst->rateControl, pEncIn->timeIncrement,
                    pSlice->sliceType);


    /* time stamp updated */
    H264UpdateSeiTS(&pEncInst->rateControl.sei, pEncIn->timeIncrement);

    /* Rate control may choose to skip the frame */
    if (pEncInst->rateControl.frameCoded == ENCHW_NO) {
        h264e_dbg_func("H264EncStrmEncode: OK, frame skipped");
        pSlice->frameNum = pSlice->prevFrameNum;    /* restore frame_num */

        return H264ENC_FRAME_READY;
    }

    /* update any cropping/rotation/filling */
    EncPreProcess(&pEncInst->asic, &pEncInst->preProcess);

    /* SEI message */
    {
        sei_s *sei = &pEncInst->rateControl.sei;

        if (sei->enabled == ENCHW_YES || sei->userDataEnabled == ENCHW_YES) {

            H264NalUnitHdr(&pEncInst->stream, 0, SEI, sei->byteStream);

            if (sei->enabled == ENCHW_YES) {
                if (pSlice->nalUnitType == IDR) {
                    H264BufferingSei(&pEncInst->stream, sei);
                }

                H264PicTimingSei(&pEncInst->stream, sei);
            }

            if (sei->userDataEnabled == ENCHW_YES) {
                H264UserDataUnregSei(&pEncInst->stream, sei);
            }

            H264RbspTrailingBits(&pEncInst->stream);

            sei->nalUnitSize = pEncInst->stream.byteCnt;
        }
    }

    /* Code one frame */
    H264CodeFrame(pEncInst, syntax_data);

    // need to think about it also    modify by lance 2016.05.07
    return H264ENC_FRAME_READY;
}

// get the hw status of encoder after encode frame
RK_S32 EncAsicCheckHwStatus(asicData_s *asic)
{
    RK_S32 ret = ASIC_STATUS_FRAME_READY;
    /*
        RK_U32 status = asic->regs.hw_status;

        if (status & ASIC_STATUS_ERROR) {
            ret = ASIC_STATUS_ERROR;
        } else if (status & ASIC_STATUS_HW_RESET) {
            ret = ASIC_STATUS_HW_RESET;
        } else if (status & ASIC_STATUS_FRAME_READY) {
            ret = ASIC_STATUS_FRAME_READY;
        } else {
            ret = ASIC_STATUS_BUFF_FULL;
        }
    */
    (void)asic;
    return ret;
}

H264EncRet H264EncStrmEncodeAfter(H264ECtx *pEncInst,
                                  H264EncOut * pEncOut, MPP_RET vpuWaitResult)
{
    slice_s *pSlice;
    regValues_s *regs;
    h264EncodeFrame_e ret = H264ENCODE_OK;
    asicData_s *asic = &pEncInst->asic;

    pSlice = &pEncInst->slice;
    regs = &pEncInst->asic.regs;
    if (vpuWaitResult != MPP_OK) {
        if (vpuWaitResult == MPP_NOK) {
            /* IRQ error => Stop and release HW */
            ret = H264ENCODE_SYSTEM_ERROR;
        } else { /*if(ewl_ret == EWL_HW_WAIT_TIMEOUT) */
            /* IRQ Timeout => Stop and release HW */
            ret = H264ENCODE_TIMEOUT;
        }
    } else {
        RK_S32 status = EncAsicCheckHwStatus(asic);
        switch (status) {
        case ASIC_STATUS_ERROR:
            ret = H264ENCODE_HW_ERROR;
            break;
        case ASIC_STATUS_BUFF_FULL:
            ret = H264ENCODE_OK;
            pEncInst->stream.overflow = ENCHW_YES;
            break;
        case ASIC_STATUS_HW_RESET:
            ret = H264ENCODE_HW_RESET;
            break;
        case ASIC_STATUS_FRAME_READY: {
            /* last not full 64-bit counted in HW data */
            const RK_U32 hw_offset = pEncInst->stream.byteCnt & (0x07U);
            pEncInst->stream.byteCnt +=
                (asic->regs.outputStrmSize - hw_offset);
            pEncInst->stream.stream +=
                (asic->regs.outputStrmSize - hw_offset);
            ret = H264ENCODE_OK;
            break;
        }
        default:
            /* should never get here */
            ASSERT(0);
            ret = H264ENCODE_HW_ERROR;
        }
    }

    if (ret != H264ENCODE_OK) {
        /* Error has occured and the frame is invalid */
        H264EncRet to_user;

        switch (ret) {
        case H264ENCODE_TIMEOUT:
            mpp_err_f("ERROR HW timeout\n");
            to_user = H264ENC_HW_TIMEOUT;
            break;
        case H264ENCODE_HW_RESET:
            mpp_err_f("ERROR HW reset detected\n");
            to_user = H264ENC_HW_RESET;
            break;
        case H264ENCODE_HW_ERROR:
            mpp_err_f("ERROR HW bus access error\n");
            to_user = H264ENC_HW_BUS_ERROR;
            break;
        case H264ENCODE_SYSTEM_ERROR:
        default:
            /* System error has occured, encoding can't continue */
            pEncInst->encStatus = H264ENCSTAT_ERROR;
            mpp_err_f("ERROR Fatal system error\n");
            to_user = H264ENC_SYSTEM_ERROR;
        }
        return to_user;
    }

    /* Filler data if needed */
    if (0) {
        RK_U32 s = H264FillerRc(&pEncInst->rateControl, pEncInst->frameCnt);

        if (s != 0) {
            s = H264FillerNALU(&pEncInst->stream,
                               (RK_S32) s, pEncInst->seqParameterSet.byteStream);
        }
        pEncInst->fillerNalSize = s;
    }

    /* After stream buffer overflow discard the coded frame */
    if (pEncInst->stream.overflow == ENCHW_YES) {
        /* Error has occured and the frame is invalid */
        /* pEncOut->codingType = H264ENC_NOTCODED_FRAME; */
        mpp_err_f("ERROR Output buffer too small\n");
        return H264ENC_OUTPUT_BUFFER_OVERFLOW;
    }

    /* Rate control action after vop */
    {
        RK_S32 stat;

        stat = H264AfterPicRc(&pEncInst->rateControl, regs->rlcCount,
                              pEncInst->stream.byteCnt, regs->qpSum);
        H264MadThreshold(&pEncInst->mad, regs->madCount);

        /* After HRD overflow discard the coded frame and go back old time,
         * just like not coded frame */
        if (stat == H264RC_OVERFLOW) {
            /* pEncOut->codingType = H264ENC_NOTCODED_FRAME; */
            pSlice->frameNum = pSlice->prevFrameNum;    /* revert frame_num */
            mpp_err("H264EncStrmEncode: OK, Frame discarded (HRD overflow)!");
            return H264ENC_FRAME_READY;
        }
    }

    /* Store the stream size and frame coding type in output structure */
    pEncOut->streamSize = pEncInst->stream.byteCnt;

    if (pSlice->nalUnitType == IDR) {
        pEncOut->codingType = H264ENC_INTRA_FRAME;
        pSlice->idrPicId += 1;
        if (pSlice->idrPicId == H264ENC_IDR_ID_MODULO)
            pSlice->idrPicId = 0;
    } else {
        pEncOut->codingType = H264ENC_PREDICTED_FRAME;
    }

    /* Frame was encoded so increment frame number */
    pEncInst->frameCnt++;
    pSlice->frameNum++;
    pSlice->frameNum %= (1U << pSlice->frameNumBits);

    pEncInst->encStatus = H264ENCSTAT_START_FRAME;
    h264e_dbg_func("leave\n");
    return H264ENC_FRAME_READY;
}

/*------------------------------------------------------------------------------

    Function name : H264EncStrmEnd
    Description   : Ends a stream
    Return type   : H264EncRet
    Argument      : inst - encoder instance
    Argument      : pEncIn - user provided input parameters
                    pEncOut - place where output info is returned
------------------------------------------------------------------------------*/
H264EncRet H264EncStrmEnd(H264ECtx *pEncInst, const H264EncIn * pEncIn,
                          H264EncOut * pEncOut)
{
    h264e_dbg_func("enter\n");
    /* Check for illegal inputs */
    if ((pEncInst == NULL) || (pEncIn == NULL) || (pEncOut == NULL)) {
        mpp_err_f("ERROR Null argument\n");
        return H264ENC_NULL_ARGUMENT;
    }

    /* Check for existing instance */
    if (pEncInst->inst != pEncInst) {
        mpp_err_f("ERROR Invalid instance\n");
        return H264ENC_INSTANCE_ERROR;
    }

    /* Check status, this also makes sure that the instance is valid */
    if ((pEncInst->encStatus != H264ENCSTAT_START_FRAME) &&
        (pEncInst->encStatus != H264ENCSTAT_START_STREAM)) {
        mpp_err_f("ERROR Invalid status\n");
        return H264ENC_INVALID_STATUS;
    }

    pEncOut->streamSize = 0;

    /* Check for invalid input values */
    if (pEncIn->pOutBuf == NULL ||
        (pEncIn->outBufSize < H264ENCSTRMSTART_MIN_BUF)) {
        mpp_err_f("ERROR Invalid input. Stream buffer\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Set stream buffer and check the size */
    if (H264SetBuffer(&pEncInst->stream, (RK_U8 *) pEncIn->pOutBuf,
                      (RK_U32) pEncIn->outBufSize) != ENCHW_OK) {
        mpp_err_f("ERROR Output buffer too small\n");
        return H264ENC_INVALID_ARGUMENT;
    }

    /* Write end-of-stream code */
//    H264EndOfSequence(&pEncInst->stream, &pEncInst->seqParameterSet);

    /* Bytes generated */
    pEncOut->streamSize = pEncInst->stream.byteCnt;

    /* Status == INIT   Stream ended, next stream can be started */
    pEncInst->encStatus = H264ENCSTAT_INIT;

    h264e_dbg_func("leave\n");
    return H264ENC_OK;
}

