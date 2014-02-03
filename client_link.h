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

#ifndef CLIENT_LINK_H
#define	CLIENT_LINK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CL_READ_HAS_DATA 1
#define CL_READ_NO_DATA 0
    
#include "ipc_channel.h"

    struct s_clientlink {
        int csock_h;
        struct sockaddr_in cli_addr;
        int caddr_len;
        int id ;
        t_ipccomm in_data ;
    };

    typedef struct s_clientlink *sp_clientlink;

    sp_clientlink new_clink();
    void free_clink(sp_clientlink _this);

    int cl_accept (sp_clientlink _this, sp_ipcchan _channel) ;
    int cl_close(sp_clientlink _this);

    ssize_t cl_write (sp_clientlink _this, t_ipccomm _data) ;
    int cl_read (sp_clientlink _this) ;
    t_ipccomm cl_get_data (sp_clientlink _this) ;
    
#ifdef	__cplusplus
}
#endif

#endif	/* CLIENT_LINK_H */

