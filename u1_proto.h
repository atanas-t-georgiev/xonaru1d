/*    
 *  xonaru1d - Daemon enabling communication with Xonar U1 device.
 * 
 *  Copyright 2012-2013 Alessandro Pignotti
 *  Copyright (C) 2014  Atanas Georgiev
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef U1_PROTO_H
#define	U1_PROTO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define U1_VENDOR_ID 0x0b05
#define U1_PRODUCT_ID 0x1743

#define U1_PROTO_FIRST 0x00
#define U1_PROTO_UNKN 0x01
#define U1_PROTO_CFG_CMD 0x02

#define U1_ENABLE_INTERRUPT (1<<7)
#define U1_ENABLE_BLUE_LED_BLINKING (3<<3)
#define U1_ENABLE_BLUE_LED_SOLID (1<<3)
#define U1_ENABLE_RED_LED_BLINKING (3<<1)
#define U1_ENABLE_RED_LED_AUDIO (1<<1)
#define U1_DISABLE_RED_LED (2<<1)

#define U1_BLUE_LED_IDX 0x13
#define U1_RED_LED_IDX 0x11

#define U1_CTRL_PACK_LEN 17

#define U1_SIG_A1 0x2980000010000LL
#define U1_SIG_A2 0x980000010000LL
#define U1_SIG_A3 0x1980000010000LL
#define U1_SIG_A4 0x3980000010000LL
    
#define U1_SIG_S4 0x38E0000010000LL
#define U1_SIG_S3 0x18E0000010000LL
#define U1_SIG_S2 0x8E0000010000LL
#define U1_SIG_S1 0x28E0000010000LL

#define U1_SIG_D4 0x38C0000010000LL
#define U1_SIG_D3 0x18C0000010000LL
#define U1_SIG_D2 0x8C0000010000LL
#define U1_SIG_D1 0x28C0000010000LL

    void gen_u1_a_init_pckt(uint8_t* _pckt);

    void gen_u1_s_init_pckt(uint8_t* _pckt);

    void gen_u1_d_init_pckt(uint8_t* _pckt);

    void gen_u1_q_led_pckt(uint8_t* _pckt);

    void gen_u1_w_led_pckt(uint8_t* _pckt);

    void gen_u1_e_led_pckt(uint8_t* _pckt);

    void gen_u1_z_led_pckt(uint8_t* _pckt);

    void gen_u1_x_led_pckt(uint8_t* _pckt);

    void gen_u1_c_led_pckt(uint8_t* _pckt);

#ifdef	__cplusplus
}
#endif

#endif	/* U1_PROTO_H */

