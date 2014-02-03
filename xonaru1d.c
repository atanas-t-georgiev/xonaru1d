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
#include "u1_device.h"
#include "u1_proto.h"
#include "dmn_proto.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>

#define U1_FOUND 1
#define U1_NOT_FOUND 0

#define CONNECT_WAIT 2000000
#define POLL_WAIT 5000

#define PORT_FILE "/var/tmp/xonaru1d.port"

#define VERSION "xonaru1d v0.8"

void io_loop(sp_ipcchan _channel, char* _prefix, int _from, int _to);
int find_u1(char* _out_path, char* _prefix, int _from, int _to);

static sp_ipcchan channel;

static void sigterm_handler(int _signum) {

#ifdef DEBUG
    printf("Received SIGTERM\n", _signum);
#endif

    close_ipcchan(channel);
    exit (0) ;

}

static void daemonize(void) {

    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/tmp")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

}

static void write_port_file(int *_port) {

    int file_h = open(PORT_FILE, O_WRONLY | O_CREAT);
    write(file_h, _port, sizeof (int));
    fchmod(file_h, S_IROTH);
    close(file_h);

}

int main(int argc, char* argv[]) {

    int ipc_port, dev_from, dev_to;
    char* hiddev_prefix;

    if (argc < 5) {
        printf("Usage: xonaru1d <ipc_port> <hiddev_prefix> <dev_from> <dev_to>\n");
        exit(1);
    }
    
#ifndef DEBUG
    daemonize();
#endif

    ipc_port = atoi(argv[1]);
    dev_from = atoi(argv[3]);
    dev_to = atoi(argv[4]);
    hiddev_prefix = argv[2];

    channel = new_ipcchan(ipc_port);

    int open_ret = open_ipcchan(channel);

    if (open_ret != IPC_SUCCESS) {
        printf("IPC Open Failed: %d\n", open_ret);
        exit(2);
    }
    
    signal(SIGTERM, sigterm_handler);

    write_port_file(&ipc_port);

    io_loop(channel, hiddev_prefix, dev_from, dev_to);

}

int find_u1(char* _out_path, char* _prefix, int _from, int _to) {

    int i;
    for (i = _from; i <= _to; i++) {
        sprintf(_out_path, "%s%d", _prefix, i);
        if (is_u1(_out_path) == U1_DEV_IS_U1) {
            return U1_FOUND;
        }
    }

    return U1_NOT_FOUND;

}

void io_loop(sp_ipcchan _channel, char* _prefix, int _from, int _to) {

    sp_u1_dev device = NULL;
    char dev_path[128];

    int prev_dev_state = U1_NOT_CONNECTED;
    int dev_state = U1_NOT_CONNECTED;
    int getstate_res;

    int open_res;

    int has_data;
    uint64_t u1_data;
    t_ipccomm ipc_out = (t_ipccomm) malloc(IPC_COMM_LEN);
    int getcmd_res;

    uint8_t cmd_pkt[U1_CTRL_PACK_LEN];

    int ipc_res;
    t_ipccomm ipc_data;

    int getpkg_res;

    while (1) {

        fflush(stdout);

        // Check state

        if (device != NULL) {
            dev_state = u1_get_state(device);
        }

        // Check for state change

        getstate_res = dmn_get_cmd_for_state(prev_dev_state, dev_state, ipc_out);
        if (getstate_res == DMN_PROTO_VALID_INPUT) {

            // Send state change to IPC

            ipc_send(_channel, ipc_out);
        }

        prev_dev_state = dev_state;

        if (dev_state == U1_NOT_CONNECTED) {

            if (device != NULL) {
                free_u1dev(device);
            }

            while (1) {

                if (find_u1(dev_path, _prefix, _from, _to) == U1_FOUND) {
                    device = new_u1dev(dev_path);

                    open_res = u1_open(device);
                    if (open_res == U1_SUCCESS) {

#ifdef DEBUG
                        printf("Device connected: %s\n", dev_path);
#endif

                        break;
                    }

                }

#ifdef DEBUG
                printf("Device not found (Waiting...)\n");
#endif

                usleep(CONNECT_WAIT);

            }

            continue;

        }

        // U1 Read

        has_data = u1_has_data(device);
        if (has_data == U1_HAS_DATA) {
            u1_data = u1_get_data(device);

#ifdef DEBUG
            printf("DEV: %lld \n", u1_data);
#endif

            // Get Response

            getcmd_res = dmn_get_cmd_for_value(u1_data, ipc_out);

            if (getcmd_res == DMN_PROTO_VALID_INPUT) {

                // IPC Write

                ipc_send(_channel, ipc_out);

            }


        }

        // IPC Read

        ipc_res = ipc_receive(_channel);

        if (ipc_res == IPC_RECV_HAS_DATA) {
            ipc_data = ipc_get_data(_channel);

#ifdef DEBUG
            printf("IPC: %c \n", *ipc_data);
#endif

            // Get Response

            getpkg_res = dmn_get_pckt_for_input(ipc_data, cmd_pkt);
            if (getpkg_res == DMN_PROTO_VALID_INPUT) {

                // U1 Write

                u1_write(device, cmd_pkt);
            }
        }

        usleep(POLL_WAIT);

    }

}