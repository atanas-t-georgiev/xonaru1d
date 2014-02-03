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

#include "array_list.h"

#include <stdlib.h>
#include <stdio.h>

sp_arraylist new_arraylist(int _cap, int _exp_fac) {

    sp_arraylist list = (sp_arraylist) malloc(sizeof (struct s_arraylist));
    list->array = (void**) malloc(_cap * sizeof (void*));
    sem_init (&list->t_lock, 0, 1) ;
    list->init_cap = _cap ;
    list->cap = _cap;
    list->len = 0;
    list->ptr = 0;
    list->exp_fac = _exp_fac;

    return list;

}

void free_arraylist(sp_arraylist _this) {

    _this->cap = -1;
    free(_this->array);
    free(_this);

}

int al_add(sp_arraylist _this, void* _elem) {

    sem_wait (&_this->t_lock) ;
    
    if (_this->len == _this->cap) {

        _this->cap *= _this->exp_fac;
        void** tmp = (void**) realloc(_this->array, _this->cap * sizeof (void*));

        if (tmp == NULL) {
            sem_post (&_this->t_lock) ;
            return AL_ERR_REALLOC;
        }

        _this->array = tmp;

    }

    _this->array[_this->len++] = _elem;

    sem_post (&_this->t_lock) ;
    return AL_SUCCESS;

}

int al_rem(sp_arraylist _this, void* _elem) {

    sem_wait (&_this->t_lock) ;
    
    int idx;
    for (idx = 0; idx < _this->len; idx++) {
        if (_this->array[idx] == _elem) {
            break;
        }
    }

    if (idx == _this->len) {
        sem_post (&_this->t_lock) ;
        return AL_ERR_ELEM_DNE;
    }

    for (_this->len--; idx < _this->len; idx++) {
        _this->array[idx] = _this->array[idx + 1];
    }

    int borderline_cap = _this->cap / _this->exp_fac ;
    if ( (_this->len < borderline_cap) && (borderline_cap >= _this->init_cap) ) {

        _this->cap = borderline_cap;
        void** tmp = (void**) realloc(_this->array, _this->cap * sizeof (void*));

        if (tmp == NULL) {
            sem_post (&_this->t_lock) ;
            return AL_ERR_REALLOC;
        }

        _this->array = tmp;

    }

    sem_post (&_this->t_lock) ;
    return AL_SUCCESS;

}

void* al_next(sp_arraylist _this) {

    sem_wait (&_this->t_lock) ;
    
    if (_this->ptr >= _this->len) {
        sem_post (&_this->t_lock) ;
        return AL_END_OF_LIST;
    }

    void *retval = _this->array[_this->ptr++];
    
    sem_post (&_this->t_lock) ;
    return retval ;

}

void al_reset(sp_arraylist _this) {

    sem_wait (&_this->t_lock) ;
    
    _this->ptr = 0;
    
    sem_post (&_this->t_lock) ;

}

void al_get_info(sp_arraylist _this, char* _buf) {
    
    sprintf (
            _buf,
            "-----\ncap: %d\n len: %d\n ptr: %d\n exp_fac: %d\n-----\n",
            _this->cap,
            _this->len,
            _this->ptr,
            _this->exp_fac
            ) ;
    
}