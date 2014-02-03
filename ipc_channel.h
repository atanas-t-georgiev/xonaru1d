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

#ifndef IPC_CHANNEL_H
#define	IPC_CHANNEL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "array_list.h"

#include <netinet/in.h>
#include <pthread.h>

#define IPC_STATE_OPEN 1
#define IPC_STATE_CLOSED 0
#define IPC_STATE_UNDEFINED -1

#define IPC_SUCCESS 0

#define IPC_ERR_STATE -110
#define IPC_ERR_SOCK_CREATE -120
#define IPC_ERR_BIND -130
#define IPC_ERR_LISTEN -140
#define IPC_ERR_THREAD -150

#define IPC_ERR_CLOSE -210

#define IPC_RECV_HAS_DATA 1
#define IPC_RECV_NO_DATA 0

#define IPC_COMM_LEN sizeof(char)*1

    typedef char *t_ipccomm;

    struct s_ipcchan {
        int sock_h;
        struct sockaddr_in srv_addr;
        int state;
        int port;
        sp_arraylist clients;
        pthread_t accept_th;
        t_ipccomm in_data;
    };

    typedef struct s_ipcchan *sp_ipcchan;

    sp_ipcchan new_ipcchan(int _port);
    void free_ipcchan(sp_ipcchan _this);

    int open_ipcchan(sp_ipcchan _this);
    int close_ipcchan(sp_ipcchan _this);

    void ipc_send(sp_ipcchan _this, t_ipccomm _cmnd);
    int ipc_receive(sp_ipcchan _this);
    t_ipccomm ipc_get_data(sp_ipcchan _this);

#ifdef	__cplusplus
}
#endif

#endif	/* IPC_CHANNEL_H */

