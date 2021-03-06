/** @file hf_util.c
 * @brief Utilities
 *
 * @copyright
 * Copyright (c) 2014, HashFast Technologies LLC
 * All rights reserved.
 *
 * @page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1.  Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *   3.  Neither the name of HashFast Technologies LLC nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL HASHFAST TECHNOLOGIES LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"
#include "hf_util.h"

/**
 * CRC-8 Table
 */
const uint8_t const crc8_table[256] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d, 
    0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65, 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d, 
    0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd, 
    0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd, 
    0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2, 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea, 
    0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a, 
    0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a, 
    0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42, 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a, 
    0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4, 
    0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4, 
    0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c, 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44, 
    0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34, 
    0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63, 
    0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b, 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13, 
    0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83, 
    0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};

/**
 * CRC-32 Table
 */
const uint32_t const crc32_table[256] = {
    (uint32_t)0x00000000, (uint32_t)0x04c11db7, (uint32_t)0x09823b6e, (uint32_t)0x0d4326d9, (uint32_t)0x130476dc, (uint32_t)0x17c56b6b, (uint32_t)0x1a864db2, (uint32_t)0x1e475005, 
    (uint32_t)0x2608edb8, (uint32_t)0x22c9f00f, (uint32_t)0x2f8ad6d6, (uint32_t)0x2b4bcb61, (uint32_t)0x350c9b64, (uint32_t)0x31cd86d3, (uint32_t)0x3c8ea00a, (uint32_t)0x384fbdbd, 
    (uint32_t)0x4c11db70, (uint32_t)0x48d0c6c7, (uint32_t)0x4593e01e, (uint32_t)0x4152fda9, (uint32_t)0x5f15adac, (uint32_t)0x5bd4b01b, (uint32_t)0x569796c2, (uint32_t)0x52568b75, 
    (uint32_t)0x6a1936c8, (uint32_t)0x6ed82b7f, (uint32_t)0x639b0da6, (uint32_t)0x675a1011, (uint32_t)0x791d4014, (uint32_t)0x7ddc5da3, (uint32_t)0x709f7b7a, (uint32_t)0x745e66cd, 
    (uint32_t)0x9823b6e0, (uint32_t)0x9ce2ab57, (uint32_t)0x91a18d8e, (uint32_t)0x95609039, (uint32_t)0x8b27c03c, (uint32_t)0x8fe6dd8b, (uint32_t)0x82a5fb52, (uint32_t)0x8664e6e5, 
    (uint32_t)0xbe2b5b58, (uint32_t)0xbaea46ef, (uint32_t)0xb7a96036, (uint32_t)0xb3687d81, (uint32_t)0xad2f2d84, (uint32_t)0xa9ee3033, (uint32_t)0xa4ad16ea, (uint32_t)0xa06c0b5d, 
    (uint32_t)0xd4326d90, (uint32_t)0xd0f37027, (uint32_t)0xddb056fe, (uint32_t)0xd9714b49, (uint32_t)0xc7361b4c, (uint32_t)0xc3f706fb, (uint32_t)0xceb42022, (uint32_t)0xca753d95, 
    (uint32_t)0xf23a8028, (uint32_t)0xf6fb9d9f, (uint32_t)0xfbb8bb46, (uint32_t)0xff79a6f1, (uint32_t)0xe13ef6f4, (uint32_t)0xe5ffeb43, (uint32_t)0xe8bccd9a, (uint32_t)0xec7dd02d, 
    (uint32_t)0x34867077, (uint32_t)0x30476dc0, (uint32_t)0x3d044b19, (uint32_t)0x39c556ae, (uint32_t)0x278206ab, (uint32_t)0x23431b1c, (uint32_t)0x2e003dc5, (uint32_t)0x2ac12072, 
    (uint32_t)0x128e9dcf, (uint32_t)0x164f8078, (uint32_t)0x1b0ca6a1, (uint32_t)0x1fcdbb16, (uint32_t)0x018aeb13, (uint32_t)0x054bf6a4, (uint32_t)0x0808d07d, (uint32_t)0x0cc9cdca, 
    (uint32_t)0x7897ab07, (uint32_t)0x7c56b6b0, (uint32_t)0x71159069, (uint32_t)0x75d48dde, (uint32_t)0x6b93dddb, (uint32_t)0x6f52c06c, (uint32_t)0x6211e6b5, (uint32_t)0x66d0fb02, 
    (uint32_t)0x5e9f46bf, (uint32_t)0x5a5e5b08, (uint32_t)0x571d7dd1, (uint32_t)0x53dc6066, (uint32_t)0x4d9b3063, (uint32_t)0x495a2dd4, (uint32_t)0x44190b0d, (uint32_t)0x40d816ba, 
    (uint32_t)0xaca5c697, (uint32_t)0xa864db20, (uint32_t)0xa527fdf9, (uint32_t)0xa1e6e04e, (uint32_t)0xbfa1b04b, (uint32_t)0xbb60adfc, (uint32_t)0xb6238b25, (uint32_t)0xb2e29692, 
    (uint32_t)0x8aad2b2f, (uint32_t)0x8e6c3698, (uint32_t)0x832f1041, (uint32_t)0x87ee0df6, (uint32_t)0x99a95df3, (uint32_t)0x9d684044, (uint32_t)0x902b669d, (uint32_t)0x94ea7b2a, 
    (uint32_t)0xe0b41de7, (uint32_t)0xe4750050, (uint32_t)0xe9362689, (uint32_t)0xedf73b3e, (uint32_t)0xf3b06b3b, (uint32_t)0xf771768c, (uint32_t)0xfa325055, (uint32_t)0xfef34de2, 
    (uint32_t)0xc6bcf05f, (uint32_t)0xc27dede8, (uint32_t)0xcf3ecb31, (uint32_t)0xcbffd686, (uint32_t)0xd5b88683, (uint32_t)0xd1799b34, (uint32_t)0xdc3abded, (uint32_t)0xd8fba05a, 
    (uint32_t)0x690ce0ee, (uint32_t)0x6dcdfd59, (uint32_t)0x608edb80, (uint32_t)0x644fc637, (uint32_t)0x7a089632, (uint32_t)0x7ec98b85, (uint32_t)0x738aad5c, (uint32_t)0x774bb0eb, 
    (uint32_t)0x4f040d56, (uint32_t)0x4bc510e1, (uint32_t)0x46863638, (uint32_t)0x42472b8f, (uint32_t)0x5c007b8a, (uint32_t)0x58c1663d, (uint32_t)0x558240e4, (uint32_t)0x51435d53, 
    (uint32_t)0x251d3b9e, (uint32_t)0x21dc2629, (uint32_t)0x2c9f00f0, (uint32_t)0x285e1d47, (uint32_t)0x36194d42, (uint32_t)0x32d850f5, (uint32_t)0x3f9b762c, (uint32_t)0x3b5a6b9b, 
    (uint32_t)0x0315d626, (uint32_t)0x07d4cb91, (uint32_t)0x0a97ed48, (uint32_t)0x0e56f0ff, (uint32_t)0x1011a0fa, (uint32_t)0x14d0bd4d, (uint32_t)0x19939b94, (uint32_t)0x1d528623, 
    (uint32_t)0xf12f560e, (uint32_t)0xf5ee4bb9, (uint32_t)0xf8ad6d60, (uint32_t)0xfc6c70d7, (uint32_t)0xe22b20d2, (uint32_t)0xe6ea3d65, (uint32_t)0xeba91bbc, (uint32_t)0xef68060b, 
    (uint32_t)0xd727bbb6, (uint32_t)0xd3e6a601, (uint32_t)0xdea580d8, (uint32_t)0xda649d6f, (uint32_t)0xc423cd6a, (uint32_t)0xc0e2d0dd, (uint32_t)0xcda1f604, (uint32_t)0xc960ebb3, 
    (uint32_t)0xbd3e8d7e, (uint32_t)0xb9ff90c9, (uint32_t)0xb4bcb610, (uint32_t)0xb07daba7, (uint32_t)0xae3afba2, (uint32_t)0xaafbe615, (uint32_t)0xa7b8c0cc, (uint32_t)0xa379dd7b, 
    (uint32_t)0x9b3660c6, (uint32_t)0x9ff77d71, (uint32_t)0x92b45ba8, (uint32_t)0x9675461f, (uint32_t)0x8832161a, (uint32_t)0x8cf30bad, (uint32_t)0x81b02d74, (uint32_t)0x857130c3, 
    (uint32_t)0x5d8a9099, (uint32_t)0x594b8d2e, (uint32_t)0x5408abf7, (uint32_t)0x50c9b640, (uint32_t)0x4e8ee645, (uint32_t)0x4a4ffbf2, (uint32_t)0x470cdd2b, (uint32_t)0x43cdc09c, 
    (uint32_t)0x7b827d21, (uint32_t)0x7f436096, (uint32_t)0x7200464f, (uint32_t)0x76c15bf8, (uint32_t)0x68860bfd, (uint32_t)0x6c47164a, (uint32_t)0x61043093, (uint32_t)0x65c52d24, 
    (uint32_t)0x119b4be9, (uint32_t)0x155a565e, (uint32_t)0x18197087, (uint32_t)0x1cd86d30, (uint32_t)0x029f3d35, (uint32_t)0x065e2082, (uint32_t)0x0b1d065b, (uint32_t)0x0fdc1bec, 
    (uint32_t)0x3793a651, (uint32_t)0x3352bbe6, (uint32_t)0x3e119d3f, (uint32_t)0x3ad08088, (uint32_t)0x2497d08d, (uint32_t)0x2056cd3a, (uint32_t)0x2d15ebe3, (uint32_t)0x29d4f654, 
    (uint32_t)0xc5a92679, (uint32_t)0xc1683bce, (uint32_t)0xcc2b1d17, (uint32_t)0xc8ea00a0, (uint32_t)0xd6ad50a5, (uint32_t)0xd26c4d12, (uint32_t)0xdf2f6bcb, (uint32_t)0xdbee767c, 
    (uint32_t)0xe3a1cbc1, (uint32_t)0xe760d676, (uint32_t)0xea23f0af, (uint32_t)0xeee2ed18, (uint32_t)0xf0a5bd1d, (uint32_t)0xf464a0aa, (uint32_t)0xf9278673, (uint32_t)0xfde69bc4, 
    (uint32_t)0x89b8fd09, (uint32_t)0x8d79e0be, (uint32_t)0x803ac667, (uint32_t)0x84fbdbd0, (uint32_t)0x9abc8bd5, (uint32_t)0x9e7d9662, (uint32_t)0x933eb0bb, (uint32_t)0x97ffad0c, 
    (uint32_t)0xafb010b1, (uint32_t)0xab710d06, (uint32_t)0xa6322bdf, (uint32_t)0xa2f33668, (uint32_t)0xbcb4666d, (uint32_t)0xb8757bda, (uint32_t)0xb5365d03, (uint32_t)0xb1f740b4
};

/**
 * Calculate CRC-8 of hf_packet
 * @param h
 * @return CRC-8
 */
uint8_t hf_crc8(uint8_t *h) {
    int i;
    uint8_t crc;

    h++;                            // Preamble not included
    for (i = 1, crc = 0xff; i < 7; i++) {
        crc = crc8_table[crc ^ *h++];
    }
    return (crc);
}

/**
 * Calculate CRC-32 of USB packet
 * @param p
 * @param len
 * @param plug_in
 * @return CRC-32
 */
uint32_t hf_crc32(unsigned char *p, int len, int plug_in) {
    uint32_t crc = (uint32_t) 0xffffffff, crc_sav;
    uint32_t i;

    while (len--) {
        i = ((crc >> 24) ^ *p++) & 0xff;
        crc = (crc << 8) ^ crc32_table[i];
    }

    crc_sav = crc;

    if (plug_in) {
        for (i = 0; i < 4; i++, crc >>= 8) {
            *p++ = crc & 0xff;
        }
    }
    return (crc_sav);
}
