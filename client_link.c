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

#include "client_link.h"

#include <stdlib.h>
#include <stdio.h>

static int id_acc = 0;

sp_clientlink new_clink() {
    sp_clientlink link = (sp_clientlink) calloc(1, sizeof (struct s_clientlink));
    link->in_data = (t_ipccomm) malloc(IPC_COMM_LEN);
    return link;
}

void free_clink(sp_clientlink _this) {
    free(_this->in_data);
    free(_this);
}

int cl_accept(sp_clientlink _this, sp_ipcchan _channel) {

    _this->csock_h = accept(
            _channel->sock_h,
            (struct sockaddr *) &_this->cli_addr,
            &_this->caddr_len
            );

    _this->id = ++id_acc;

#ifdef DEBUG
    printf("Client %d accepted\n", _this->id);
#endif

    return _this->csock_h;

}

int cl_close(sp_clientlink _this) {

#ifdef DEBUG
    printf("Client %d disconnected\n", _this->id);
#endif

    return close(_this->csock_h);
}

ssize_t cl_write(sp_clientlink _this, t_ipccomm _data) {

    return send(_this->csock_h, _data, (size_t) IPC_COMM_LEN, MSG_NOSIGNAL);

}

int cl_read(sp_clientlink _this) {

    ssize_t rcv_res = recv(
            _this->csock_h,
            _this->in_data,
            (size_t) IPC_COMM_LEN,
            MSG_DONTWAIT);

    if (rcv_res <= 0) {
        return CL_READ_NO_DATA;
    }

    return CL_READ_HAS_DATA;

}

t_ipccomm cl_get_data(sp_clientlink _this) {

    return _this->in_data;

}