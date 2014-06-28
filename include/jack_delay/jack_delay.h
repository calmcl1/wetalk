#ifndef JACK_DELAY_H
#define JACK_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif
    int jack_callback(jack_nframes_t nframes, void *arg);

    int do_delay();
#ifdef __cplusplus
}
#endif

#endif


