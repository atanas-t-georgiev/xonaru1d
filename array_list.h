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

#ifndef ARRAY_LIST_H
#define	ARRAY_LIST_H

#ifdef	__cplusplus
extern "C" {
#endif

#define AL_END_OF_LIST 0

#define AL_SUCCESS 0

#define AL_ERR_REALLOC -110
#define AL_ERR_ELEM_DNE -210

#include <semaphore.h>
    
    struct s_arraylist {
        void** array;
        int init_cap ;
        int cap;
        int len;
        int ptr;
        int exp_fac;
        sem_t t_lock ;
    };

    typedef struct s_arraylist *sp_arraylist;

    sp_arraylist new_arraylist(int _cap, int _exp_fac);
    void free_arraylist(sp_arraylist _this);

    int al_add(sp_arraylist _this, void* _elem);
    int al_rem(sp_arraylist _this, void* _elem);

    void* al_next(sp_arraylist _this);
    void al_reset(sp_arraylist _this);

    void al_get_info(sp_arraylist _this, char* _buf);

#ifdef	__cplusplus
}
#endif

#endif	/* ARRAY_LIST_H */

