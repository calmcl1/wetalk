/* 
 * File:   wetalk.cpp
 * Author: callum
 *
 * Created on 25 June 2014, 14:06
 */

#include <iostream>
#include <stdio.h>
extern "C" {
#include <gstreamer-1.0/gst/gst.h>
}

using namespace std;

static GMainLoop * loop;
GstElement *jacksrc, *jacksink;
GstElement *pipeline;

/*
 * 
 */
int main(int argc, char** argv) {

    // Set program version
    const char version[] = "0.1.0";

    // Initialize GST
    const gchar *nano_str;
    guint gst_major, gst_minor, gst_micro, gst_nano;

    gst_init(&argc, &argv);
    gst_version(&gst_major, &gst_minor, &gst_micro, &gst_nano);
    loop = g_main_loop_new(NULL, FALSE);

    cerr << "Starting RTMPSwitch v" << version;
    cerr << " (linked against Gstreamer ";
    cerr << gst_major << "." << gst_minor << "." << gst_micro << ")" << endl;

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

    return 0;
}

