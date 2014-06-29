/* 
 * File:   jack_test.h
 * Author: callum
 *
 * Created on 28 June 2014, 13:17
 */

#ifndef JACK_TEST_H
#define	JACK_TEST_H

//int test_gst_loop(int argc, char * argv[]);

int test_jack_mix_cb(jack_nframes_t nframes, void *arg);
int test_jack_mix();
int main(int argc, char * argv[]);


#endif	/* JACK_TEST_H */

