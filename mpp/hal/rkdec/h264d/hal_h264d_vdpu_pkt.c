
/*
 * Copyright 2010 Rockchip Electronics S.LSI Co. LTD
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

#define MODULE_TAG "hal_h264d_vdpu_pkt"

#include <stdio.h>
#include <string.h>

#include "mpp_mem.h"
#include "hal_task.h"

#include "dxva_syntax.h"
#include "vpu.h"

#include "h264d_log.h"
#include "h264d_syntax.h"
#include "hal_h264d_fifo.h"
#include "hal_h264d_api.h"
#include "hal_h264d_global.h"
#include "hal_h264d_vdpu_pkt.h"
#include "hal_h264d_vdpu_reg.h"


const RK_U32 H264_VDPU_Cabac_table[VDPU_CABAC_TAB_SIZE / 4] = {
    0x14f10236, 0x034a14f1, 0x0236034a, 0xe47fe968, 0xfa35ff36, 0x07330000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0029003f, 0x003f003f, 0xf7530456, 0x0061f948, 0x0d29033e, 0x000b0137,
    0x0045ef7f, 0xf3660052, 0xf94aeb6b, 0xe57fe17f, 0xe87fee5f, 0xe57feb72,
    0xe27fef7b, 0xf473f07a, 0xf573f43f, 0xfe44f154, 0xf368fd46, 0xf85df65a,
    0xe27fff4a, 0xfa61f95b, 0xec7ffc38, 0xfb52f94c, 0xea7df95d, 0xf557fd4d,
    0xfb47fc3f, 0xfc44f454, 0xf93ef941, 0x083d0538, 0xfe420140, 0x003dfe4e,
    0x01320734, 0x0a23002c, 0x0b26012d, 0x002e052c, 0x1f110133, 0x07321c13,
    0x10210e3e, 0xf36cf164, 0xf365f35b, 0xf45ef658, 0xf054f656, 0xf953f357,
    0xed5e0146, 0x0048fb4a, 0x123bf866, 0xf164005f, 0xfc4b0248, 0xf54bfd47,
    0x0f2ef345, 0x003e0041, 0x1525f148, 0x09391036, 0x003e0c48, 0x18000f09,
    0x08190d12, 0x0f090d13, 0x0a250c12, 0x061d1421, 0x0f1e042d, 0x013a003e,
    0x073d0c26, 0x0b2d0f27, 0x0b2a0d2c, 0x102d0c29, 0x0a311e22, 0x122a0a37,
    0x1133112e, 0x00591aed, 0x16ef1aef, 0x1ee71cec, 0x21e925e5, 0x21e928e4,
    0x26ef21f5, 0x28f129fa, 0x26012911, 0x1efa1b03, 0x1a1625f0, 0x23fc26f8,
    0x26fd2503, 0x26052a00, 0x23102716, 0x0e301b25, 0x153c0c44, 0x0261fd47,
    0xfa2afb32, 0xfd36fe3e, 0x003a013f, 0xfe48ff4a, 0xf75bfb43, 0xfb1bfd27,
    0xfe2c002e, 0xf040f844, 0xf64efa4d, 0xf656f45c, 0xf137f63c, 0xfa3efc41,
    0xf449f84c, 0xf950f758, 0xef6ef561, 0xec54f54f, 0xfa49fc4a, 0xf356f360,
    0xf561ed75, 0xf84efb21, 0xfc30fe35, 0xfd3ef347, 0xf64ff456, 0xf35af261,
    0x0000fa5d, 0xfa54f84f, 0x0042ff47, 0x003efe3c, 0xfe3bfb4b, 0xfd3efc3a,
    0xf742ff4f, 0x00470344, 0x0a2cf93e, 0x0f240e28, 0x101b0c1d, 0x012c1424,
    0x1220052a, 0x01300a3e, 0x112e0940, 0xf468f561, 0xf060f958, 0xf855f955,
    0xf755f358, 0x0442fd4d, 0xfd4cfa4c, 0x0a3aff4c, 0xff53f963, 0xf25f025f,
    0x004cfb4a, 0x0046f54b, 0x01440041, 0xf249033e, 0x043eff44, 0xf34b0b37,
    0x05400c46, 0x0f060613, 0x07100c0e, 0x120d0d0b, 0x0d0f0f10, 0x0c170d17,
    0x0f140e1a, 0x0e2c1128, 0x112f1811, 0x15151916, 0x1f1b161d, 0x13230e32,
    0x0a39073f, 0xfe4dfc52, 0xfd5e0945, 0xf46d24dd, 0x24de20e6, 0x25e22ce0,
    0x22ee22f1, 0x28f121f9, 0x23fb2100, 0x2602210d, 0x17230d3a, 0x1dfd1a00,
    0x161e1ff9, 0x23f122fd, 0x220324ff, 0x2205200b, 0x2305220c, 0x270b1e1d,
    0x221a1d27, 0x13421f15, 0x1f1f1932, 0xef78ec70, 0xee72f555, 0xf15cf259,
    0xe647f151, 0xf2500044, 0xf246e838, 0xe944e832, 0xf54a17f3, 0x1af328f1,
    0x31f22c03, 0x2d062c22, 0x21361352, 0xfd4bff17, 0x0122012b, 0x0036fe37,
    0x003d0140, 0x0044f75c, 0xf26af361, 0xf15af45a, 0xee58f649, 0xf74ff256,
    0xf649f646, 0xf645fb42, 0xf740fb3a, 0x023b15f6, 0x18f51cf8, 0x1cff1d03,
    0x1d092314, 0x1d240e43, 0x14f10236, 0x034a14f1, 0x0236034a, 0xe47fe968,
    0xfa35ff36, 0x07331721, 0x17021500, 0x01090031, 0xdb760539, 0xf34ef541,
    0x013e0c31, 0xfc491132, 0x1240092b, 0x1d001a43, 0x105a0968, 0xd27fec68,
    0x0143f34e, 0xf541013e, 0xfa56ef5f, 0xfa3d092d, 0xfd45fa51, 0xf5600637,
    0x0743fb56, 0x0258003a, 0xfd4cf65e, 0x05360445, 0xfd510058, 0xf943fb4a,
    0xfc4afb50, 0xf948013a, 0x0029003f, 0x003f003f, 0xf7530456, 0x0061f948,
    0x0d29033e, 0x002dfc4e, 0xfd60e57e, 0xe462e765, 0xe943e452, 0xec5ef053,
    0xea6eeb5b, 0xee66f35d, 0xe37ff95c, 0xfb59f960, 0xf36cfd2e, 0xff41ff39,
    0xf75dfd4a, 0xf75cf857, 0xe97e0536, 0x063c063b, 0x0645ff30, 0x0044fc45,
    0xf858fe55, 0xfa4eff4b, 0xf94d0236, 0x0532fd44, 0x0132062a, 0xfc51013f,
    0xfc460043, 0x0239fe4c, 0x0b230440, 0x013d0b23, 0x12190c18, 0x0d1d0d24,
    0xf65df949, 0xfe490d2e, 0x0931f964, 0x09350235, 0x0535fe3d, 0x00380038,
    0xf33ffb3c, 0xff3e0439, 0xfa450439, 0x0e270433, 0x0d440340, 0x013d093f,
    0x07321027, 0x052c0434, 0x0b30fb3c, 0xff3b003b, 0x1621052c, 0x0e2bff4e,
    0x003c0945, 0x0b1c0228, 0x032c0031, 0x002e022c, 0x0233002f, 0x0427023e,
    0x062e0036, 0x0336023a, 0x043f0633, 0x06390735, 0x06340637, 0x0b2d0e24,
    0x0835ff52, 0x0737fd4e, 0x0f2e161f, 0xff541907, 0x1ef91c03, 0x1c042000,
    0x22ff1e06, 0x1e062009, 0x1f131a1b, 0x1a1e2514, 0x1c221146, 0x0143053b,
    0x0943101e, 0x12201223, 0x161d181f, 0x1726122b, 0x14290b3f, 0x093b0940,
    0xff5efe59, 0xf76cfa4c, 0xfe2c002d, 0x0034fd40, 0xfe3bfc46, 0xfc4bf852,
    0xef66f74d, 0x0318002a, 0x00300037, 0xfa3bf947, 0xf453f557, 0xe277013a,
    0xfd1dff24, 0x0126022b, 0xfa37003a, 0x0040fd4a, 0xf65a0046, 0xfc1d051f,
    0x072a013b, 0xfe3afd48, 0xfd51f561, 0x003a0805, 0x0a0e0e12, 0x0d1b0228,
    0x003afd46, 0xfa4ff855, 0x0000f36a, 0xf06af657, 0xeb72ee6e, 0xf262ea6e,
    0xeb6aee67, 0xeb6be96c, 0xe670f660, 0xf45ffb5b, 0xf75dea5e, 0xfb560943,
    0xfc50f655, 0xff46073c, 0x093a053d, 0x0c320f32, 0x12311136, 0x0a29072e,
    0xff330731, 0x08340929, 0x062f0237, 0x0d290a2c, 0x06320535, 0x0d31043f,
    0x0640fe45, 0xfe3b0646, 0x0a2c091f, 0x0c2b0335, 0x0e220a26, 0xfd340d28,
    0x1120072c, 0x07260d32, 0x0a391a2b, 0x0e0b0b0e, 0x090b120b, 0x150917fe,
    0x20f120f1, 0x22eb27e9, 0x2adf29e1, 0x2ee426f4, 0x151d2de8, 0x35d330e6,
    0x41d52bed, 0x27f61e09, 0x121a141b, 0x0039f252, 0xfb4bed61, 0xdd7d1b00,
    0x1c001ffc, 0x1b062208, 0x1e0a1816, 0x21131620, 0x1a1f1529, 0x1a2c172f,
    0x10410e47, 0x083c063f, 0x11411518, 0x17141a17, 0x1b201c17, 0x1c181728,
    0x18201c1d, 0x172a1339, 0x1635163d, 0x0b560c28, 0x0b330e3b, 0xfc4ff947,
    0xfb45f746, 0xf842f644, 0xed49f445, 0xf046f143, 0xec3eed46, 0xf042ea41,
    0xec3f09fe, 0x1af721f7, 0x27f929fe, 0x2d033109, 0x2d1b243b, 0xfa42f923,
    0xf92af82d, 0xfb30f438, 0xfa3cfb3e, 0xf842f84c, 0xfb55fa51, 0xf64df951,
    0xef50ee49, 0xfc4af653, 0xf747f743, 0xff3df842, 0xf242003b, 0x023b15f3,
    0x21f227f9, 0x2efe3302, 0x3c063d11, 0x37222a3e, 0x14f10236, 0x034a14f1,
    0x0236034a, 0xe47fe968, 0xfa35ff36, 0x07331619, 0x22001000, 0xfe090429,
    0xe3760241, 0xfa47f34f, 0x05340932, 0xfd460a36, 0x1a221316, 0x28003902,
    0x29241a45, 0xd37ff165, 0xfc4cfa47, 0xf34f0534, 0x0645f35a, 0x0034082b,
    0xfe45fb52, 0xf660023b, 0x024bfd57, 0xfd640138, 0xfd4afa55, 0x003bfd51,
    0xf956fb5f, 0xff42ff4d, 0x0146fe56, 0xfb48003d, 0x0029003f, 0x003f003f,
    0xf7530456, 0x0061f948, 0x0d29033e, 0x0d0f0733, 0x0250d97f, 0xee5bef60,
    0xe651dd62, 0xe866e961, 0xe577e863, 0xeb6eee66, 0xdc7f0050, 0xfb59f95e,
    0xfc5c0027, 0x0041f154, 0xdd7ffe49, 0xf468f75b, 0xe17f0337, 0x07380737,
    0x083dfd35, 0x0044f94a, 0xf758f367, 0xf35bf759, 0xf25cf84c, 0xf457e96e,
    0xe869f64e, 0xec70ef63, 0xb27fba7f, 0xce7fd27f, 0xfc42fb4e, 0xfc47f848,
    0x023bff37, 0xf946fa4b, 0xf859de77, 0xfd4b2014, 0x1e16d47f, 0x0036fb3d,
    0x003aff3c, 0xfd3df843, 0xe754f24a, 0xfb410534, 0x0239003d, 0xf745f546,
    0x1237fc47, 0x003a073d, 0x09291219, 0x0920052b, 0x092f002c, 0x0033022e,
    0x1326fc42, 0x0f260c2a, 0x09220059, 0x042d0a1c, 0x0a1f21f5, 0x34d5120f,
    0x1c0023ea, 0x26e72200, 0x27ee20f4, 0x66a20000, 0x38f121fc, 0x1d0a25fb,
    0x33e327f7, 0x34de45c6, 0x43c12cfb, 0x200737e3, 0x20010000, 0x1b2421e7,
    0x22e224e4, 0x26e426e5, 0x22ee23f0, 0x22f220f8, 0x25fa2300, 0x1e0a1c12,
    0x1a191d29, 0x004b0248, 0x084d0e23, 0x121f1123, 0x151e112d, 0x142a122d,
    0x1b1a1036, 0x07421038, 0x0b490a43, 0xf674e970, 0xf147f93d, 0x0035fb42,
    0xf54df750, 0xf754f657, 0xde7feb65, 0xfd27fb35, 0xf93df54b, 0xf14def5b,
    0xe76be76f, 0xe47af54c, 0xf62cf634, 0xf639f73a, 0xf048f945, 0xfc45fb4a,
    0xf7560242, 0xf7220120, 0x0b1f0534, 0xfe37fe43, 0x0049f859, 0x03340704,
    0x0a081108, 0x10130325, 0xff3dfb49, 0xff46fc4e, 0x0000eb7e, 0xe97cec6e,
    0xe67ee77c, 0xef69e579, 0xe575ef66, 0xe675e574, 0xdf7af65f, 0xf264f85f,
    0xef6fe472, 0xfa59fe50, 0xfc52f755, 0xf851ff48, 0x05400143, 0x09380045,
    0x01450745, 0xf945fa43, 0xf04dfe40, 0x023dfa43, 0xfd400239, 0xfd41fd42,
    0x003e0933, 0xff42fe47, 0xfe4bff46, 0xf7480e3c, 0x1025002f, 0x12230b25,
    0x0c290a29, 0x02300c29, 0x0d29003b, 0x03321328, 0x03421232, 0x13fa12fa,
    0x0e001af4, 0x1ff021e7, 0x21ea25e4, 0x27e22ae2, 0x2fd62ddc, 0x31de29ef,
    0x200945b9, 0x3fc142c0, 0x4db636d9, 0x34dd29f6, 0x240028ff, 0x1e0e1c1a,
    0x17250c37, 0x0b4125df, 0x27dc28db, 0x26e22edf, 0x2ae228e8, 0x31e326f4,
    0x28f626fd, 0x2efb1f14, 0x1d1e192c, 0x0c300b31, 0x1a2d1616, 0x17161b15,
    0x21141a1c, 0x1e181b22, 0x122a1927, 0x12320c46, 0x15360e47, 0x0b531920,
    0x15311536, 0xfb55fa51, 0xf64df951, 0xef50ee49, 0xfc4af653, 0xf747f743,
    0xff3df842, 0xf242003b, 0x023b11f6, 0x20f32af7, 0x31fb3500, 0x4003440a,
    0x421b2f39, 0xfb470018, 0xff24fe2a, 0xfe34f739, 0xfa3ffc41, 0xfc43f952,
    0xfd51fd4c, 0xf948fa4e, 0xf448f244, 0xfd46fa4c, 0xfb42fb3e, 0x0039fc3d,
    0xf73c0136, 0x023a11f6, 0x20f32af7, 0x31fb3500, 0x4003440a, 0x421b2f39,
    0x14f10236, 0x034a14f1, 0x0236034a, 0xe47fe968, 0xfa35ff36, 0x07331d10,
    0x19000e00, 0xf633fd3e, 0xe5631a10, 0xfc55e866, 0x05390639, 0xef490e39,
    0x1428140a, 0x1d003600, 0x252a0c61, 0xe07fea75, 0xfe4afc55, 0xe8660539,
    0xfa5df258, 0xfa2c0437, 0xf559f167, 0xeb741339, 0x143a0454, 0x0660013f,
    0xfb55f36a, 0x053f064b, 0xfd5aff65, 0x0337fc4f, 0xfe4bf461, 0xf932013c,
    0x0029003f, 0x003f003f, 0xf7530456, 0x0061f948, 0x0d29033e, 0x0722f758,
    0xec7fdc7f, 0xef5bf25f, 0xe754e756, 0xf459ef5b, 0xe17ff24c, 0xee67f35a,
    0xdb7f0b50, 0x054c0254, 0x054efa37, 0x043df253, 0xdb7ffb4f, 0xf568f55b,
    0xe27f0041, 0xfe4f0048, 0xfc5cfa38, 0x0344f847, 0xf362fc56, 0xf458fb52,
    0xfd48fc43, 0xf848f059, 0xf745ff3b, 0x05420439, 0xfc47fe47, 0x023aff4a,
    0xfc2cff45, 0x003ef933, 0xfc2ffa2a, 0xfd29fa35, 0x084cf74e, 0xf5530934,
    0x0043fb5a, 0x0143f148, 0xfb4bf850, 0xeb53eb40, 0xf31fe740, 0xe35e094b,
    0x113ff84a, 0xfb23fe1b, 0x0d5b0341, 0xf945084d, 0xf642033e, 0xfd44ec51,
    0x001e0107, 0xfd17eb4a, 0x1042e97c, 0x11252cee, 0x32deea7f, 0x0427002a,
    0x07220b1d, 0x081f0625, 0x072a0328, 0x08210d2b, 0x0d24042f, 0x0337023a,
    0x063c082c, 0x0b2c0e2a, 0x07300438, 0x04340d25, 0x0931133a, 0x0a300c2d,
    0x00451421, 0x083f23ee, 0x21e71cfd, 0x180a1b00, 0x22f234d4, 0x27e81311,
    0x1f19241d, 0x1821220f, 0x1e141649, 0x1422131f, 0x1b2c1310, 0x0f240f24,
    0x151c1915, 0x1e141f0c, 0x1b10182a, 0x005d0e38, 0x0f391a26, 0xe87fe873,
    0xea52f73e, 0x0035003b, 0xf255f359, 0xf35ef55c, 0xe37feb64, 0xf239f443,
    0xf547f64d, 0xeb55f058, 0xe968f162, 0xdb7ff652, 0xf830f83d, 0xf842f946,
    0xf24bf64f, 0xf753f45c, 0xee6cfc4f, 0xea45f04b, 0xfe3a013a, 0xf34ef753,
    0xfc51f363, 0xf351fa26, 0xf33efa3a, 0xfe3bf049, 0xf64cf356, 0xf753f657,
    0x0000ea7f, 0xe77fe778, 0xe57fed72, 0xe975e776, 0xe675e871, 0xe476e178,
    0xdb7cf65e, 0xf166f663, 0xf36ace7f, 0xfb5c1139, 0xfb56f35e, 0xf45bfe4d,
    0x0047ff49, 0x0440f951, 0x05400f39, 0x01430044, 0xf6430144, 0x004d0240,
    0x0044fb4e, 0x0737053b, 0x02410e36, 0x0f2c053c, 0x0246fe4c, 0xee560c46,
    0x0540f446, 0x0b370538, 0x00450241, 0xfa4a0536, 0x0736fa4c, 0xf552fe4d,
    0xfe4d192a, 0x11f310f7, 0x11f41beb, 0x25e229d8, 0x2ad730d1, 0x27e02ed8,
    0x34cd2ed7, 0x34d92bed, 0x200b3dc9, 0x38d23ece, 0x51bd2dec, 0x23fe1c0f,
    0x22012701, 0x1e111426, 0x122d0f36, 0x004f24f0, 0x25f225ef, 0x2001220f,
    0x1d0f1819, 0x22161f10, 0x23121f1c, 0x2129241c, 0x1b2f153e, 0x121f131a,
    0x24181817, 0x1b10181e, 0x1f1d1629, 0x162a103c, 0x0f340e3c, 0x034ef07b,
    0x15351638, 0x193d1521, 0x1332113d, 0xfd4ef84a, 0xf748f648, 0xee4bf447,
    0xf53ffb46, 0xef4bf248, 0xf043f835, 0xf23bf734, 0xf54409fe, 0x1ef61ffc,
    0x21ff2107, 0x1f0c2517, 0x1f261440, 0xf747f925, 0xf82cf531, 0xf638f43b,
    0xf83ff743, 0xfa44f64f, 0xfd4ef84a, 0xf748f648, 0xee4bf447, 0xf53ffb46,
    0xef4bf248, 0xf043f835, 0xf23bf734, 0xf54409fe, 0x1ef61ffc, 0x21ff2107,
    0x1f0c2517, 0x1f261440,
};

const RK_U32 g_ValueList[34] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
};


static RK_U32 check_dpb_buffer_is_valid(H264dHalCtx_t *p_hal, RK_U32 dpb_idx)
{

    (void)p_hal;
    (void)dpb_idx;

    return 1;
}
static MPP_RET vdpu_set_refer_pic_idx(H264dVdpuRegs_t *p_regs, RK_U32 i, RK_U16 val)
{
    switch (i) {
    case 0:
        p_regs->sw76.num_ref_idx0 = val;
        break;
    case 1:
        p_regs->sw76.num_ref_idx1 = val;
        break;
    case 2:
        p_regs->sw77.num_ref_idx2 = val;
        break;
    case 3:
        p_regs->sw77.num_ref_idx3 = val;
        break;
    case 4:
        p_regs->sw78.num_ref_idx4 = val;
        break;
    case 5:
        p_regs->sw78.num_ref_idx5 = val;
        break;
    case 6:
        p_regs->sw79.num_ref_idx6 = val;
        break;
    case 7:
        p_regs->sw79.num_ref_idx7 = val;
        break;
    case 8:
        p_regs->sw80.num_ref_idx8 = val;
        break;
    case 9:
        p_regs->sw80.num_ref_idx9 = val;
        break;
    case 10:
        p_regs->sw81.num_ref_idx10 = val;
        break;
    case 11:
        p_regs->sw81.num_ref_idx11 = val;
        break;
    case 12:
        p_regs->sw82.num_ref_idx12 = val;
        break;
    case 13:
        p_regs->sw82.num_ref_idx13 = val;
        break;
    case 14:
        p_regs->sw83.num_ref_idx14 = val;
        break;
    case 15:
        p_regs->sw83.num_ref_idx15 = val;
        break;
    default:
        break;
    }

    return MPP_OK;
}
static MPP_RET vdpu_set_refer_pic_list_p(H264dVdpuRegs_t *p_regs, RK_U32 i, RK_U16 val)
{
    switch (i) {
    case 0:
        p_regs->sw106.init_reflist_pf0 = val;
        break;
    case 1:
        p_regs->sw106.init_reflist_pf1 = val;
        break;
    case 2:
        p_regs->sw106.init_reflist_pf2 = val;
        break;
    case 3:
        p_regs->sw106.init_reflist_pf3 = val;
        break;
    case 4:
        p_regs->sw74.init_reflist_pf4 = val;
        break;
    case 5:
        p_regs->sw74.init_reflist_pf5 = val;
        break;
    case 6:
        p_regs->sw74.init_reflist_pf6 = val;
        break;
    case 7:
        p_regs->sw74.init_reflist_pf7 = val;
        break;
    case 8:
        p_regs->sw74.init_reflist_pf8 = val;
        break;
    case 9:
        p_regs->sw74.init_reflist_pf9 = val;
        break;
    case 10:
        p_regs->sw75.init_reflist_pf10 = val;
        break;
    case 11:
        p_regs->sw75.init_reflist_pf11 = val;
        break;
    case 12:
        p_regs->sw75.init_reflist_pf12 = val;
        break;
    case 13:
        p_regs->sw75.init_reflist_pf13 = val;
        break;
    case 14:
        p_regs->sw75.init_reflist_pf14 = val;
        break;
    case 15:
        p_regs->sw75.init_reflist_pf15 = val;
        break;
    default:
        break;
    }

    return MPP_OK;
}
static MPP_RET vdpu_set_refer_pic_list_b0(H264dVdpuRegs_t *p_regs, RK_U32 i, RK_U16 val)
{
    switch (i) {
    case 0:
        p_regs->sw100.init_reflist_df0 = val;
        break;
    case 1:
        p_regs->sw100.init_reflist_df1 = val;
        break;
    case 2:
        p_regs->sw100.init_reflist_df2 = val;
        break;
    case 3:
        p_regs->sw100.init_reflist_df3 = val;
        break;
    case 4:
        p_regs->sw100.init_reflist_df4 = val;
        break;
    case 5:
        p_regs->sw100.init_reflist_df5 = val;
        break;
    case 6:
        p_regs->sw101.init_reflist_df6 = val;
        break;
    case 7:
        p_regs->sw101.init_reflist_df7 = val;
        break;
    case 8:
        p_regs->sw101.init_reflist_df8 = val;
        break;
    case 9:
        p_regs->sw101.init_reflist_df9 = val;
        break;
    case 10:
        p_regs->sw101.init_reflist_df10 = val;
        break;
    case 11:
        p_regs->sw101.init_reflist_df11 = val;
        break;
    case 12:
        p_regs->sw102.init_reflist_df12 = val;
        break;
    case 13:
        p_regs->sw102.init_reflist_df13 = val;
        break;
    case 14:
        p_regs->sw102.init_reflist_df14 = val;
        break;
    case 15:
        p_regs->sw102.init_reflist_df15 = val;
        break;
    default:
        break;
    }

    return MPP_OK;
}
static MPP_RET vdpu_set_refer_pic_list_b1(H264dVdpuRegs_t *p_regs, RK_U32 i, RK_U16 val)
{
    switch (i) {
    case 0:
        p_regs->sw103.init_reflist_db0 = val;
        break;
    case 1:
        p_regs->sw103.init_reflist_db1 = val;
        break;
    case 2:
        p_regs->sw103.init_reflist_db2 = val;
        break;
    case 3:
        p_regs->sw103.init_reflist_db3 = val;
        break;
    case 4:
        p_regs->sw103.init_reflist_db4 = val;
        break;
    case 5:
        p_regs->sw103.init_reflist_db5 = val;
        break;
    case 6:
        p_regs->sw104.init_reflist_db6 = val;
        break;
    case 7:
        p_regs->sw104.init_reflist_db7 = val;
        break;
    case 8:
        p_regs->sw104.init_reflist_db8 = val;
        break;
    case 9:
        p_regs->sw104.init_reflist_db9 = val;
        break;
    case 10:
        p_regs->sw104.init_reflist_db10 = val;
        break;
    case 11:
        p_regs->sw104.init_reflist_db11 = val;
        break;
    case 12:
        p_regs->sw105.init_reflist_db12 = val;
        break;
    case 13:
        p_regs->sw105.init_reflist_db13 = val;
        break;
    case 14:
        p_regs->sw105.init_reflist_db14 = val;
        break;
    case 15:
        p_regs->sw105.init_reflist_db15 = val;
        break;
    default:
        break;
    }

    return MPP_OK;
}

static MPP_RET vdpu_set_refer_pic_base_addr(H264dVdpuRegs_t *p_regs, RK_U32 i, RK_U32 val)
{
    switch (i) {
    case 0:
        p_regs->sw84.ref0_st_addr = val;
        break;
    case 1:
        p_regs->sw85.ref1_st_addr = val;
        break;
    case 2:
        p_regs->sw86.ref2_st_addr = val;
        break;
    case 3:
        p_regs->sw87.ref3_st_addr = val;
        break;
    case 4:
        p_regs->sw88.ref4_st_addr = val;
        break;
    case 5:
        p_regs->sw89.ref5_st_addr = val;
        break;
    case 6:
        p_regs->sw90.ref6_st_addr = val;
        break;
    case 7:
        p_regs->sw91.ref7_st_addr = val;
        break;
    case 8:
        p_regs->sw92.ref8_st_addr = val;
        break;
    case 9:
        p_regs->sw93.ref9_st_addr = val;
        break;
    case 10:
        p_regs->sw94.ref10_st_addr = val;
        break;
    case 11:
        p_regs->sw95.ref11_st_addr = val;
        break;
    case 12:
        p_regs->sw96.ref12_st_addr = val;
        break;
    case 13:
        p_regs->sw97.ref13_st_addr = val;
        break;
    case 14:
        p_regs->sw98.ref14_st_addr = val;
        break;
    case 15:
        p_regs->sw99.ref15_st_addr = val;
        break;
    default:
        break;
    }
    return MPP_OK;
}
/*!
***********************************************************************
* \brief
*    set picture parameter and set register
***********************************************************************
*/
//extern "C"
MPP_RET vdpu_set_pic_regs(H264dHalCtx_t *p_hal, H264dVdpuRegs_t *p_regs)
{
    MPP_RET ret = MPP_ERR_UNKNOW;

    p_regs->sw110.pic_mb_w = p_hal->pp->wFrameWidthInMbsMinus1 + 1;
    p_regs->sw110.pic_mb_h = (2 - p_hal->pp->frame_mbs_only_flag) * (p_hal->pp->wFrameHeightInMbsMinus1 + 1);

    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    set vlc register
***********************************************************************
*/
//extern "C"
MPP_RET vdpu_set_vlc_regs(H264dHalCtx_t *p_hal, H264dVdpuRegs_t *p_regs)
{
    RK_U32 i = 0;
    MPP_RET ret = MPP_ERR_UNKNOW;
    DXVA_PicParams_H264_MVC *pp = p_hal->pp;

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    p_regs->sw57.dec_wr_extmen_dis = 0;
    p_regs->sw57.rlc_mode_en = 0;
    p_regs->sw51.qp_init_val = pp->pic_init_qp_minus26 + 26;
    p_regs->sw114.max_refidx0 = pp->num_ref_idx_l0_active_minus1 + 1;
    p_regs->sw111.max_refnum = pp->num_ref_frames;
    p_regs->sw112.cur_frm_len = pp->log2_max_frame_num_minus4 + 4;
    p_regs->sw112.curfrm_num = pp->frame_num;
    p_regs->sw115.const_intra_en = pp->constrained_intra_pred_flag;
    p_regs->sw112.dblk_ctrl_flag = pp->deblocking_filter_control_present_flag;
    p_regs->sw112.rpcp_flag = pp->redundant_pic_cnt_present_flag;
    p_regs->sw113.refpic_mk_len = p_hal->slice_long[0].drpm_used_bitlen;
    p_regs->sw115.idr_pic_flag = p_hal->slice_long[0].idr_flag;
    p_regs->sw113.idr_pic_id = p_hal->slice_long[0].idr_pic_id;
    p_regs->sw114.pps_id = p_hal->slice_long[0].active_pps_id;
    p_regs->sw114.poc_field_len = p_hal->slice_long[0].poc_used_bitlen;
    /* reference picture flags, TODO separate fields */
    if (pp->field_pic_flag) {
        RK_U32 validTmp = 0, validFlags = 0;
        RK_U32 longTermTmp = 0, longTermflags = 0;
        for (i = 0; i < 32; i++) {
            if (pp->RefFrameList[i / 2].bPicEntry == 0xff) { //!< invalid
                longTermflags <<= 1;
                validFlags <<= 1;
            } else {
                longTermTmp = pp->RefFrameList[i / 2].AssociatedFlag; //!< get long term flag
                longTermflags = (longTermflags << 1) | longTermTmp;

                validTmp = check_dpb_buffer_is_valid(p_hal, pp->RefFrameList[i / 2].Index7Bits);
                validTmp = validTmp && ((pp->UsedForReferenceFlags >> i) & 0x01);
                validFlags = (validFlags << 1) | validTmp;
            }
        }
        p_regs->sw107.refpic_term_flag = longTermflags;
        p_regs->sw108.refpic_valid_flag = validFlags;
    } else {
        RK_U32 validTmp = 0, validFlags = 0;
        RK_U32 longTermTmp = 0, longTermflags = 0;
        for (i = 0; i < 16; i++) {
            if (pp->RefFrameList[i].bPicEntry == 0xff) {  //!< invalid
                longTermflags <<= 1;
                validFlags <<= 1;
            } else {
                longTermTmp = pp->RefFrameList[i].AssociatedFlag;
                longTermflags = (longTermflags << 1) | longTermTmp;
                validTmp = check_dpb_buffer_is_valid(p_hal, pp->RefFrameList[i].Index7Bits);
                validTmp = validTmp && ((pp->UsedForReferenceFlags >> (2 * i)) & 0x03);
                validFlags = (validFlags << 1) | validTmp;
            }
        }
        p_regs->sw107.refpic_term_flag = (longTermflags << 16);
        p_regs->sw108.refpic_valid_flag = (validFlags << 16);
    }

    for (i = 0; i < 16; i++) {
        if (pp->RefFrameList[i].bPicEntry != 0xff) { //!< valid
            if (pp->RefFrameList[i].AssociatedFlag) { //!< longterm flag
                vdpu_set_refer_pic_idx(p_regs, i, pp->LongTermPicNumList[i]); //!< pic_num
            } else {
                vdpu_set_refer_pic_idx(p_regs, i, pp->FrameNumList[i]); //< frame_num
            }
        }
    }
    p_regs->sw57.rd_cnt_tab_en = 1;
    //!< set poc to buffer
    {
        RK_U32 *pocBase = NULL;
        pocBase = (RK_U32 *)((RK_U8 *)mpp_buffer_get_ptr(p_hal->cabac_buf) + VDPU_CABAC_TAB_SIZE);
        //!< set reference reorder poc
        for (i = 0; i < 32; i++) {
            if (pp->RefFrameList[i / 2].bPicEntry != 0xff) {
                *pocBase++ = pp->FieldOrderCntList[i / 2][i & 0x1];
            } else {
                *pocBase++ = 0;
            }
        }
        //!< set current poc
        if (pp->field_pic_flag || !pp->MbaffFrameFlag) {
            *pocBase++ = pp->CurrFieldOrderCnt[0];
            *pocBase++ = pp->CurrFieldOrderCnt[1];
        } else {
            *pocBase++ = pp->CurrFieldOrderCnt[0];
            *pocBase++ = pp->CurrFieldOrderCnt[1];
        }
    }
    p_regs->sw115.cabac_en = pp->entropy_coding_mode_flag;
    //!< stream position update
    {
        MppBuffer bitstream_buf = NULL;
        p_regs->sw57.st_code_exit = 1;
        mpp_buf_slot_get_prop(p_hal->packet_slots, p_hal->in_task->input, SLOT_BUFFER, &bitstream_buf);
        p_regs->sw109.strm_start_bit = 0; //!< sodb stream start bit
        if (VPUClientGetIOMMUStatus() > 0) {
            p_regs->sw64.rlc_vlc_st_adr = mpp_buffer_get_fd(bitstream_buf);
        }
        p_regs->sw51.stream_len = p_hal->strm_len;
    }
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);

    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    set vlc reference
***********************************************************************
*/
//extern "C"
MPP_RET vdpu_set_ref_regs(H264dHalCtx_t *p_hal, H264dVdpuRegs_t *p_regs)
{
    RK_U32 i = 0, j = 0;
    MPP_RET ret = MPP_ERR_UNKNOW;
    DXVA_Slice_H264_Long *p_long = &p_hal->slice_long[0];

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);
    //!< list0 list1 listP
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 16; i++) {
            RK_U16 val = 0;
            if (p_hal->pp->field_pic_flag) { //!< field
                RK_U32 nn = 0;
                nn = p_hal->pp->CurrPic.AssociatedFlag ? (2 * i + 1) : (2 * i);
                if (p_long->RefPicList[j][nn].bPicEntry == 0xff) {
                    val = g_ValueList[i];
                } else {
                    val = p_long->RefPicList[j][nn].Index7Bits;
                }
            } else { //!< frame
                if (p_long->RefPicList[j][i].bPicEntry == 0xff) {
                    val = g_ValueList[i];
                } else {
                    val = p_long->RefPicList[j][i].Index7Bits;
                }
            }
            switch (j) {
            case 0:
                vdpu_set_refer_pic_list_p(p_regs, i, val);
                break;
            case 1:
                vdpu_set_refer_pic_list_b0(p_regs, i, val);

                break;
            case 2:
                vdpu_set_refer_pic_list_b1(p_regs, i, val);

                break;
            default:
                break;
            }
        }
    }
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);

    return ret = MPP_OK;
}
/*!
***********************************************************************
* \brief
*    run Asic
***********************************************************************
*/
//extern "C"
MPP_RET vdpu_set_asic_regs(H264dHalCtx_t *p_hal, H264dVdpuRegs_t *p_regs)
{
    RK_U32 i = 0, j = 0;
    RK_U32 outPhyAddr = 0;
    MppBuffer frame_buf = NULL;
    MPP_RET ret = MPP_ERR_UNKNOW;
    DXVA_PicParams_H264_MVC *pp = p_hal->pp;
    DXVA_Slice_H264_Long *p_long = &p_hal->slice_long[0];

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);
    /* reference picture physic address */
    for (i = 0, j = 0xff; i < MPP_ARRAY_ELEMS(pp->RefFrameList); i++) {
        RK_U32 val = 0;
        RK_U32 top_closer = 0;
        RK_U32 field_flag = 0;
        if (pp->RefFrameList[i].bPicEntry != 0xff) {
            mpp_buf_slot_get_prop(p_hal->frame_slots, pp->RefFrameList[i].Index7Bits, SLOT_BUFFER, &frame_buf); //!< reference phy addr
            j = i;
        } else {
            mpp_buf_slot_get_prop(p_hal->frame_slots, pp->CurrPic.Index7Bits, SLOT_BUFFER, &frame_buf); //!< current out phy addr
        }

        field_flag = ((pp->RefPicFiledFlags >> i) & 0x1) ? 0x2 : 0;
        if (field_flag) {
            RK_U32 used_flag = 0;
            RK_S32 cur_poc = 0;
            RK_S32 ref_poc = 0;

            cur_poc = pp->CurrPic.AssociatedFlag ? pp->CurrFieldOrderCnt[1] : pp->CurrFieldOrderCnt[0];
            used_flag = ((pp->UsedForReferenceFlags >> (2 * i)) & 0x3);
            if (used_flag & 0x3) {
                ref_poc = MPP_MIN(pp->FieldOrderCntList[i][0], pp->FieldOrderCntList[i][1]);
            } else if (used_flag & 0x2) {
                ref_poc = pp->FieldOrderCntList[i][1];
            } else if (used_flag & 0x1) {
                ref_poc = pp->FieldOrderCntList[i][0];
            }
            top_closer = (cur_poc < ref_poc) ? 0x1 : 0;
        }
        val = top_closer | field_flag;

        if (VPUClientGetIOMMUStatus() > 0) {
            val = mpp_buffer_get_fd(frame_buf) | (val << 10);
        }
        vdpu_set_refer_pic_base_addr(p_regs, i, val);
    }
    /* inter-view reference picture */
    {
        H264dVdpuPriv_t *priv = (H264dVdpuPriv_t *)p_hal->priv;
        if (pp->curr_layer_id && priv->ilt_dpb && priv->ilt_dpb->valid /*pp->inter_view_flag*/) {
            mpp_buf_slot_get_prop(p_hal->frame_slots, priv->ilt_dpb->slot_index, SLOT_BUFFER, &frame_buf);

            if (VPUClientGetIOMMUStatus() > 0) {
                p_regs->sw99.ref15_st_addr = mpp_buffer_get_fd(frame_buf); //!< inter-view base, ref15
            }
            p_regs->sw108.refpic_valid_flag |= (pp->field_pic_flag ? 0x3 : 0x10000);
        }
    }
    p_regs->sw50.dec_fixed_quant = pp->curr_layer_id; //!< VDPU_MVC_E
    p_regs->sw50.dblk_flt_dis = 0; //!< filterDisable = 0;
    mpp_buf_slot_get_prop(p_hal->frame_slots, pp->CurrPic.Index7Bits, SLOT_BUFFER, &frame_buf); //!< current out phy addr

    if (VPUClientGetIOMMUStatus() > 0) {
        outPhyAddr = mpp_buffer_get_fd(frame_buf);
    }
    if (pp->field_pic_flag && pp->CurrPic.AssociatedFlag) {
        if (VPUClientGetIOMMUStatus() > 0) {
            outPhyAddr |= ((pp->wFrameWidthInMbsMinus1 + 1) * 16) << 10;
        } else {
            outPhyAddr += (pp->wFrameWidthInMbsMinus1 + 1) * 16;
        }
    }
    p_regs->sw63.dec_out_st_adr = outPhyAddr; //!< outPhyAddr, pp->CurrPic.Index7Bits
    p_regs->sw110.flt_offset_cb_qp = pp->chroma_qp_index_offset;
    p_regs->sw110.flt_offset_cr_qp = pp->second_chroma_qp_index_offset;
    /* set default value for register[41] to avoid illegal translation fd */
    {
        RK_U32 dirMvOffset = 0;
        RK_U32 picSizeInMbs = 0;

        picSizeInMbs = p_hal->pp->wFrameWidthInMbsMinus1 + 1;
        picSizeInMbs = picSizeInMbs * (2 - pp->frame_mbs_only_flag) * (pp->wFrameHeightInMbsMinus1 + 1);
        dirMvOffset = picSizeInMbs * ((p_hal->pp->chroma_format_idc == 0) ? 256 : 384);
        dirMvOffset += (pp->field_pic_flag && pp->CurrPic.AssociatedFlag) ? (picSizeInMbs * 32) : 0;
        if (VPUClientGetIOMMUStatus() > 0) {
            p_regs->sw62.dmmv_st_adr = (mpp_buffer_get_fd(frame_buf) | (dirMvOffset << 6));
        }
    }
    p_regs->sw57.dmmv_wr_en = (p_long->nal_ref_idc != 0) ? 1 : 0; //!< defalut set 1
    p_regs->sw115.dlmv_method_en = pp->direct_8x8_inference_flag;
    p_regs->sw115.weight_pred_en = pp->weighted_pred_flag;
    p_regs->sw111.wp_bslice_sel = pp->weighted_bipred_idc;
    p_regs->sw114.max_refidx1 = (pp->num_ref_idx_l1_active_minus1 + 1);
    p_regs->sw115.fieldpic_flag_exist = (!pp->frame_mbs_only_flag) ? 1 : 0;
    p_regs->sw57.curpic_code_sel = (!pp->frame_mbs_only_flag && (pp->MbaffFrameFlag || pp->field_pic_flag)) ? 1 : 0;
    p_regs->sw57.curpic_stru_sel = pp->field_pic_flag;
    p_regs->sw57.pic_decfield_sel = (!pp->CurrPic.AssociatedFlag) ? 1 : 0; //!< bottomFieldFlag
    p_regs->sw57.sequ_mbaff_en = pp->MbaffFrameFlag;
    p_regs->sw115.tranf_8x8_flag_en = pp->transform_8x8_mode_flag;
    p_regs->sw115.monochr_en = (p_long->profileIdc >= 100 && pp->chroma_format_idc == 0) ? 1 : 0;
    p_regs->sw115.scl_matrix_en = pp->scaleing_list_enable_flag;
    {
        RK_U32 offset = VDPU_CABAC_TAB_SIZE + VDPU_POC_BUF_SIZE;
        if (p_hal->pp->scaleing_list_enable_flag) {
            RK_U32 temp = 0;
            RK_U32 *ptr = NULL;
            ptr = (RK_U32 *)((RK_U8 *)mpp_buffer_get_ptr(p_hal->cabac_buf) + offset);
            for (i = 0; i < 6; i++) {
                for (j = 0; j < 4; j++) {
                    temp = (p_hal->qm->bScalingLists4x4[i][4 * j + 0] << 24) |
                           (p_hal->qm->bScalingLists4x4[i][4 * j + 1] << 16) |
                           (p_hal->qm->bScalingLists4x4[i][4 * j + 2] << 8) |
                           (p_hal->qm->bScalingLists4x4[i][4 * j + 3]);
                    *ptr++ = temp;
                }
            }
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 16; j++) {
                    temp = (p_hal->qm->bScalingLists8x8[i][4 * j + 0] << 24) |
                           (p_hal->qm->bScalingLists8x8[i][4 * j + 1] << 16) |
                           (p_hal->qm->bScalingLists8x8[i][4 * j + 2] << 8) |
                           (p_hal->qm->bScalingLists8x8[i][4 * j + 3]);
                    *ptr++ = temp;
                }
            }
        }
        if (VPUClientGetIOMMUStatus() > 0) {
            p_regs->sw61.qtable_st_adr = mpp_buffer_get_fd(p_hal->cabac_buf);
        }
    }
    p_regs->sw57.dec_wr_extmen_dis = 0; //!< set defalut 0
    p_regs->sw57.addit_ch_fmt_wen = 0;
    p_regs->sw57.dec_st_work = 1;
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    return ret = MPP_OK;
}


