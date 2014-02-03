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

#ifndef U1_DEVICE_H
#define	U1_DEVICE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <semaphore.h>

#define U1_CONNECTED 1
#define U1_NOT_CONNECTED 0
#define U1_STATE_UNKNOWN 100

#define U1_HAS_DATA 1
#define U1_NO_DATA 0

#define U1_DEV_IS_U1 1
#define U1_DEV_NOT_U1 0
    
#define U1_SUCCESS 0

#define U1_ERR_OPEN -110 
#define U1_ERR_CLOSE -210 
#define U1_ERR_READ -310 
#define U1_ERR_WRITE -410 

    struct s_u1_dev {
        char* dev_path;
        int dev_h;
        int state;
        uint64_t in_data;
        int has_data ;
        sem_t data_lock ;
        pthread_t read_th;
    };

    typedef struct s_u1_dev *sp_u1_dev;

    sp_u1_dev new_u1dev(char* _path);
    void free_u1dev(sp_u1_dev _this);

    int u1_get_state(sp_u1_dev _this);

    int u1_open(sp_u1_dev _this);
    int u1_close(sp_u1_dev _this);

    uint64_t u1_get_data(sp_u1_dev _this);
    int u1_has_data(sp_u1_dev _this);

    int u1_write(sp_u1_dev _this, uint8_t* _ctrl_pckt);

    int is_u1 (char* _path) ;

#ifdef	__cplusplus
}
#endif

#endif	/* U1_DEVICE_H */

