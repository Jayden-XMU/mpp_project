/*
 *
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

#ifndef __VP8D_CODEC_H__
#define __VP8D_CODEC_H__

#include "rk_type.h"
#include "mpp_common.h"
#include "vpu_api.h"
#include "mpp_frame.h"
#include "mpp_dec.h"
#include "mpp_packet.h"


typedef struct VP8DContext_t {
    void        *parse_ctx;

} VP8DContext;

#endif
