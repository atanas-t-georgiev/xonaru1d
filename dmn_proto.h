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

#ifndef DMN_PROTO_H
#define	DMN_PROTO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ipc_channel.h"
    
#include <stdint.h>

#define DMN_PROTO_VALID_INPUT 1
#define DMN_PROTO_INVALID_INPUT 0

    int dmn_get_pckt_for_input (t_ipccomm _in_cmd, uint8_t *_out_pkt) ;
    int dmn_get_cmd_for_value (uint64_t _val, t_ipccomm _out_cmd) ; 
    int dmn_get_cmd_for_state (int _prev_state, int _current_state, t_ipccomm _out_cmd) ; 

#ifdef	__cplusplus
}
#endif

#endif	/* DMN_PROTO_H */

