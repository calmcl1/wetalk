
/* 
 * File:   jack_test.cpp
 * Author: callum
 *
 * Created on 28-Jun-2014, 13:10:12
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
extern "C" {
#include <gstreamer-1.0/gst/gst.h>
#include <jack/jack.h>
#include <string.h>
#include "jack_delay.h"
#include "mtdm.h"
}

#ifdef WIN32
#define jack_sleep(val) Sleep((val))
#else
#define jack_sleep(val) usleep((val) * 1000)
#endif

/*
 * Simple C++ Test Suite
 */
using namespace std;

jack_client_t * client;
jack_status_t status;
jack_port_t *output_port_1, *output_port_2, *input_port_1;
jack_latency_range_t range;
static MTDM *mtdm = 0;
static bool active = false;

/*
int test_gst_loop(int argc, char * argv[]) {

    static GMainLoop* loop;
    GstElement *jacksrc, *jacksink;
    GstElement *pipeline;

    // Set program version
    const char version[] = "0.1.0";

    // Initialize GST
    const gchar *nano_str;
    guint gst_major, gst_minor, gst_micro, gst_nano;

    cerr << "Starting weTalk v" << version;
    cerr << " (linked against Gstreamer ";
    cerr << gst_major << "." << gst_minor << "." << gst_micro << ")" << endl;
    cout << "Testing JACK loop through" << endl;
    cout << "Creating JACK src...";

    gst_init(&argc, &argv);
    gst_version(&gst_major, &gst_minor, &gst_micro, &gst_nano);
    loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_element_factory_make("pipeline", "jack-pipeline");

    jacksrc = gst_element_factory_make("jackaudiosrc", "jacksrc");
    jacksink = gst_element_factory_make("jackaudiosink", "jacksink");
    g_object_set(G_OBJECT(jacksink), "connect", 0, NULL);
    g_object_set(G_OBJECT(jacksrc), "connect", 0, NULL);
    g_object_set(G_OBJECT(jacksink), "sync", false, NULL);

    gst_bin_add_many(GST_BIN(pipeline), jacksrc, jacksink, NULL);

    gst_element_link(jacksrc, jacksink);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(loop);
}
 */

int test_jack_mix_cb(jack_nframes_t nframes, void *arg) {
    float *ip, *opf, *op1, *op2;
    //float opf1, opf2, opf;

    if (active) {
        op1 = (float *) (jack_port_get_buffer(output_port_1, nframes));
        op2 = (float *) (jack_port_get_buffer(output_port_2, nframes));
        ip = (float *) (jack_port_get_buffer(input_port_1, nframes));


        //printf("Starting floating point conversion\n");
        for (jack_nframes_t i = 0; i < nframes; i++) {

            ip[i] = op1[i] + op2[i];
        }
        mtdm->process(nframes, ip, op1);

        //op2=op1;
        memcpy(op2, op1, sizeof(jack_default_audio_sample_t) * nframes);

        /*for (jack_nframes_t i = 0; i < nframes; i++) {
            //op2[i]=op1[i];
            printf("OP1: %f\t\tOP2: %f\t\tSum: %f\n", op1[i], op2[i], (op1[i] + op2[i]));
        }*/
    }
    return 0;
}

int test_jack_mix() {

    jack_set_process_callback(client, test_jack_mix_cb, 0);
    mtdm = new MTDM(jack_get_sample_rate(client));

    output_port_1 = jack_port_register(client, "wt_attn_op1", JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsOutput, 0);
    output_port_2 = jack_port_register(client, "wt_attn_op2", JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsOutput, 0);
    input_port_1 = jack_port_register(client, "wt_attn_ip1", JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsInput, 0);

    if ((output_port_1 == NULL) || (output_port_2 == NULL) || (input_port_1 == NULL)) {
        fprintf(stdout, "\tCould not create one of the ports. Failing.\n");
    }


    if (jack_activate(client)) {
        fprintf(stdout, "\tCan't activate the client!\n");
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "\tClient activated.\n");

    if (jack_connect(client, jack_port_name(output_port_1), jack_port_name(input_port_1))) {
        fprintf(stdout, "\tFailed to connect output 1 to input 1. Failing.\n");
        return (EXIT_FAILURE);
    }
    if (jack_connect(client, jack_port_name(output_port_2), jack_port_name(input_port_1))) {
        fprintf(stdout, "\tFailed to connect output 2 to input 1. Failing.\n");
        return (EXIT_FAILURE);
    }

    double d, t;
    t = 1000.0 / jack_get_sample_rate(client);
    active = true;
    while (1) {
        //usleep(250000);
        jack_sleep(250);
        if (mtdm->resolve() < 0) printf("Signal below threshold...\n");
        else {
            if (mtdm->err() > 0.35f) {
                mtdm->invert();
                mtdm->resolve();
            }
            d = mtdm->del();
            if (mtdm->err() > 0.30f) printf("???  ");
            else if (mtdm->inv()) printf("Inv  ");
            else printf("     ");
            printf("%10.3lf frames %8.3lf ms\n", d, d * t);
        }
    }

    fprintf(stdout, "\tJACK client running...\n");

}

int main(int argc, char * argv[]) {
    client = jack_client_open("wt_jack_test_client", JackNullOption,\
            &status);

    if (client == NULL) {
        fprintf(stdout, "Problem connecting to JACK server...\n");
        if (status & JackServerFailed) {
            fprintf(stdout, "JACK client could not connect. \
Ensure server is active.\n");
            return (EXIT_FAILURE);
        }

        if (status & JackServerStarted) {
            fprintf(stdout, "Started JACK server.\n");
        }

    }

    //int test_gst_loop_result = test_gst_loop(argc, argv);

    fprintf(stdout, "Testing JACK 2-to-1 mix without attenuation...\n");
    int test_jack_attenuate_result = test_jack_mix();
    if (!test_jack_attenuate_result) {
        fprintf(stdout, "JACK 2-to-1 mix without attenuation: Pass\n");
    } else {
        fprintf(stdout, "JACK 2-to-1 mix without attenuation: Fail\n");
    }

    return (EXIT_SUCCESS);
}

