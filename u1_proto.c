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

#include "u1_proto.h"

#include <string.h>

void gen_u1_a_init_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_PROTO_CFG_CMD;
    _pckt[2] = U1_PROTO_UNKN;

    _pckt[3] = U1_ENABLE_INTERRUPT |
            U1_ENABLE_BLUE_LED_BLINKING ;

}

void gen_u1_s_init_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_PROTO_CFG_CMD;
    _pckt[2] = U1_PROTO_UNKN;

    _pckt[3] = U1_ENABLE_INTERRUPT |
            U1_ENABLE_BLUE_LED_SOLID|
            U1_ENABLE_RED_LED_BLINKING;

}

void gen_u1_d_init_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_PROTO_CFG_CMD;
    _pckt[2] = U1_PROTO_UNKN;

    _pckt[3] = U1_ENABLE_INTERRUPT |
            U1_ENABLE_BLUE_LED_SOLID |
            U1_DISABLE_RED_LED;

}

void gen_u1_q_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_RED_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 50;
    _pckt[4] = 90;

}

void gen_u1_w_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_RED_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 120;
    _pckt[4] = 180;

}

void gen_u1_e_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_RED_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 200;
    _pckt[4] = 250;

}

void gen_u1_z_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_BLUE_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 50;
    _pckt[4] = 90;

}

void gen_u1_x_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_BLUE_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 120;
    _pckt[4] = 180;

}
    
void gen_u1_c_led_pckt(uint8_t* _pckt) {

    memset(_pckt, 0, U1_CTRL_PACK_LEN);

    _pckt[0] = U1_PROTO_FIRST;
    _pckt[1] = U1_BLUE_LED_IDX;
    _pckt[2] = U1_PROTO_UNKN;
    _pckt[3] = 200;
    _pckt[4] = 250;

}
