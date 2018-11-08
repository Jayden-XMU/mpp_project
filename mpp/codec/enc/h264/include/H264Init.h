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

#ifndef __H264_INIT_H__
#define __H264_INIT_H__

#include "h264encapi.h"
#include "H264Instance.h"

RK_S32 H264GetAllowedWidth(RK_S32 width, MppFrameFormat inputType);

H264EncRet H264Init(H264ECtx * pinst);
H264EncRet H264Cfg(const H264EncConfig * pEncCfg, H264ECtx * pinst);

#endif
