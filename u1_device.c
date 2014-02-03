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

#include "u1_device.h"
#include "u1_proto.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include <linux/uinput.h>
#include <linux/hidraw.h>

static void* read_thread(void* _arg) {

#ifdef DEBUG
    printf("Read Thread started\n");
#endif

    sp_u1_dev device = (sp_u1_dev) _arg;

    int read_res;
    while (device->state == U1_CONNECTED) {

        sem_wait(&device->data_lock);

        read_res = read(device->dev_h, &device->in_data, sizeof (uint64_t));
        if (read_res < 0) {
            u1_close(device);
        } else {
            device->has_data = U1_HAS_DATA;
        }

    }

#ifdef DEBUG
    printf("Read Thread stopped\n");
#endif

}

sp_u1_dev new_u1dev(char* _path) {

    sp_u1_dev dev = (sp_u1_dev) malloc(sizeof (struct s_u1_dev));
    dev->dev_path = _path;
    dev->dev_h = 0;
    dev->state = U1_NOT_CONNECTED;
    dev->in_data = 0;

    return dev;

}

void free_u1dev(sp_u1_dev _this) {

    free(_this);

}

int u1_get_state(sp_u1_dev _this) {

    return _this->state;

}

int u1_open(sp_u1_dev _this) {

    _this->dev_h = open(_this->dev_path, O_RDWR);

    if (_this->dev_h < 0) {
        _this->state = U1_NOT_CONNECTED;
        return U1_ERR_OPEN;
    }

    int thread_err = pthread_create(
            &_this->read_th,
            NULL,
            read_thread,
            (void*) _this
            );

    if (thread_err < 0) {
        _this->state = U1_NOT_CONNECTED;
        return U1_ERR_OPEN;
    }

    _this->state = U1_CONNECTED;

    sem_init(&_this->data_lock, 0, 1);

    return U1_SUCCESS;

}

int u1_close(sp_u1_dev _this) {

    int close_err = close(_this->dev_h);

    _this->state = U1_NOT_CONNECTED;
    _this->has_data = U1_NO_DATA;
    sem_post(&_this->data_lock);

    if (close_err) {
        return U1_ERR_CLOSE;
    }

    return U1_SUCCESS;

}

uint64_t u1_get_data(sp_u1_dev _this) {

    uint64_t temp = _this->in_data;
    _this->in_data = 0;
    _this->has_data = U1_NO_DATA;
    sem_post(&_this->data_lock);
    return temp;

}

int u1_has_data(sp_u1_dev _this) {

    return _this->has_data;

}

int u1_write(sp_u1_dev _this, uint8_t* _ctrl_pckt) {

    ssize_t write_res = write(_this->dev_h, _ctrl_pckt, U1_CTRL_PACK_LEN);

    if (write_res <= 0) {
        u1_close(_this);
        return U1_ERR_WRITE;
    }

    return U1_SUCCESS;

}

int is_u1(char* _path) {

    int hid_h = open(_path, O_RDWR);
    if (hid_h < 0) {
        return U1_DEV_NOT_U1;
    }

    struct hidraw_devinfo dev_i;
    ioctl(hid_h, HIDIOCGRAWINFO, &dev_i);

    if (dev_i.vendor != U1_VENDOR_ID) {
        return U1_DEV_NOT_U1;
    }

    if (dev_i.product != U1_PRODUCT_ID) {
        return U1_DEV_NOT_U1;
    }

    close(hid_h);

    return U1_DEV_IS_U1;

}