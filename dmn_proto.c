/*    
 *  xonaru1d - Daemon enabling communication with Xonar U1 device.
 * 
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

#include "dmn_proto.h"
#include "u1_proto.h"
#include "u1_device.h"

int dmn_get_pckt_for_input(t_ipccomm _in_cmd, uint8_t *_out_pkt) {

    switch (*_in_cmd) {
        case 'A':
            gen_u1_a_init_pckt(_out_pkt);
            break;
        case 'S':
            gen_u1_s_init_pckt(_out_pkt);
            break;
        case 'D':
            gen_u1_d_init_pckt(_out_pkt);
            break;
        case 'Q':
            gen_u1_q_led_pckt(_out_pkt);
            break;
        case 'W':
            gen_u1_w_led_pckt(_out_pkt);
            break;
        case 'E':
            gen_u1_e_led_pckt(_out_pkt);
            break;
        case 'Z':
            gen_u1_z_led_pckt(_out_pkt);
            break;
        case 'X':
            gen_u1_x_led_pckt(_out_pkt);
            break;
        case 'C':
            gen_u1_c_led_pckt(_out_pkt);
            break;
        default:
            return DMN_PROTO_INVALID_INPUT;
    }

    return DMN_PROTO_VALID_INPUT;

}

int dmn_get_cmd_for_value(uint64_t _val, t_ipccomm _out_cmd) {

    switch (_val) {
        case U1_SIG_A4:
        case U1_SIG_S4:
        case U1_SIG_D4:
            *_out_cmd = '4';
            break;
        case U1_SIG_A3:
        case U1_SIG_S3:
        case U1_SIG_D3:
            *_out_cmd = '3';
            break;
        case U1_SIG_A2:
        case U1_SIG_S2:
        case U1_SIG_D2:
            *_out_cmd = '2';
            break;
        case U1_SIG_A1:
        case U1_SIG_S1:
        case U1_SIG_D1:
            *_out_cmd = '1';
            break;
        default:
            *_out_cmd = '*';
            break;
    }

    return DMN_PROTO_VALID_INPUT;

}

int dmn_get_cmd_for_state(int _prev_state, int _current_state, t_ipccomm _out_cmd) {

    if (_prev_state == _current_state) {
        return DMN_PROTO_INVALID_INPUT;
    }

    if (_current_state == U1_CONNECTED) {
        *_out_cmd = '#';
    }

    if (_current_state == U1_NOT_CONNECTED) {
        *_out_cmd = '!';
    }

    return DMN_PROTO_VALID_INPUT;

}
