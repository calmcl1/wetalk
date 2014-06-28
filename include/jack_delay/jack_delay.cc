// ----------------------------------------------------------------------------
//
//  Copyright (C) 2003-2011 Fons Adriaensen <fons@linuxaudio.org>
//    
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// ----------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <jack/jack.h>
#include "mtdm.h"
#include "jack_delay.h"


int jack_callback(jack_nframes_t nframes, void *arg) {
}

int do_delay() {
    double d, dcapt, dplay, t;
    char b [1024];
    jack_status_t s;
    jack_latency_range_t range;

    jack_set_process_callback(jack_handle, jack_callback, 0);
    mtdm = new MTDM(jack_get_sample_rate(jack_handle));

    
    t = 1000.0 / jack_get_sample_rate(jack_handle);
    active = true;
    while (1) {
        usleep(250000);
        if (mtdm->resolve() < 0) printf("Signal below threshold...\n");
        else {
            if (mtdm->err() > 0.35f) {
                mtdm->invert();
                mtdm->resolve();
            }
            d = mtdm->del();
            if (E_opt) d -= dcapt + dplay;
            if (mtdm->err() > 0.30f) printf("???  ");
            else if (mtdm->inv()) printf("Inv  ");
            else printf("     ");
            printf("%10.3lf frames %8.3lf ms\n", d, d * t);
        }
    }

    return 0;
}

