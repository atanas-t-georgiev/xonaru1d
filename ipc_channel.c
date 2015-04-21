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

#include "ipc_channel.h"
#include "client_link.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

static void* accept_thread(void* _arg) {

    sp_ipcchan channel = (sp_ipcchan) _arg;

    sp_clientlink clink;

    while (channel->state == IPC_STATE_OPEN) {

        clink = new_clink();

        int acc_res = cl_accept(clink, channel);

        if (acc_res < 0) {
            cl_close(clink);
            free_clink(clink);
        } else {
            al_add(channel->clients, clink);
        }

    }

}

sp_ipcchan new_ipcchan(int _port) {

    sp_ipcchan chan = (sp_ipcchan) malloc(sizeof (struct s_ipcchan));
    chan->in_data = (t_ipccomm) malloc(IPC_COMM_LEN);
    chan->clients = new_arraylist(5, 2);
    chan->state = IPC_STATE_CLOSED;
    chan->port = _port;

    return chan;

}

void free_ipcchan(sp_ipcchan _this) {

    free(_this->in_data);
    free_arraylist(_this->clients);
    free(_this);

}

int open_ipcchan(sp_ipcchan _this) {

    if (_this->state != IPC_STATE_CLOSED) {
        return IPC_ERR_STATE;
    }

    _this->sock_h = socket(AF_INET, SOCK_STREAM, 0);

    if (_this->sock_h < 0) {
        return IPC_ERR_SOCK_CREATE;
    }

    _this->srv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &_this->srv_addr.sin_addr);
    _this->srv_addr.sin_port = htons(_this->port);

    int reuse_opt = 1;
    setsockopt(
            _this->sock_h,
            SOL_SOCKET,
            SO_REUSEADDR,
            (char*) &reuse_opt,
            sizeof (reuse_opt)
            );

    int bind_err = bind(
            _this->sock_h,
            (struct sockaddr *) &_this->srv_addr,
            sizeof (_this->srv_addr)
            );

    if (bind_err) {
        return IPC_ERR_BIND;
    }

    int listen_err = listen(
            _this->sock_h,
            3
            );

    if (listen_err) {
        return IPC_ERR_LISTEN;
    }

    int thread_err = pthread_create(
            &_this->accept_th,
            NULL,
            accept_thread,
            (void*) _this
            );

    if (thread_err) {
        return IPC_ERR_THREAD;
    }

    _this->state = IPC_STATE_OPEN;

    return IPC_SUCCESS;

}

int close_ipcchan(sp_ipcchan _this) {

    _this->state = IPC_STATE_UNDEFINED;

    int sdwn_err = shutdown(_this->sock_h, SHUT_RDWR);

    sp_clientlink current;
    while ((current = al_next(_this->clients)) != AL_END_OF_LIST) {
        al_rem(_this->clients, current);
        cl_close(current);
        free_clink(current);
    }

    close(_this->sock_h);

    if (sdwn_err) {
        return IPC_ERR_CLOSE;
    }

    _this->state = IPC_STATE_CLOSED;

    return IPC_SUCCESS;

}

void ipc_send(sp_ipcchan _this, t_ipccomm _cmnd) {

    al_reset(_this->clients);

    ssize_t write_res;
    sp_clientlink current;
    while ((current = al_next(_this->clients)) != AL_END_OF_LIST) {

        write_res = cl_write(current, _cmnd);

        if (write_res < 0) {
            al_rem(_this->clients, current);
            cl_close(current);
            free_clink(current);
        }

    }

}

int ipc_receive(sp_ipcchan _this) {

    al_reset(_this->clients);

    int recv_res;
    sp_clientlink current;
    while ((current = al_next(_this->clients)) != AL_END_OF_LIST) {

        recv_res = cl_read(current);

        if (recv_res == CL_READ_HAS_DATA) {
            memcpy(_this->in_data, cl_get_data(current), IPC_COMM_LEN);
            return IPC_RECV_HAS_DATA;
        }

    }

    return IPC_RECV_NO_DATA;

}

t_ipccomm ipc_get_data(sp_ipcchan _this) {

    return _this->in_data;

}
