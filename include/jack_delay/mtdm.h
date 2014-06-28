/* 
 * File:   mtdm.h
 * Author: callum
 *
 * Created on 28 June 2014, 16:24
 */


#ifndef __MTDM_H
#define __MTDM_H

#include <unistd.h>

extern "C" {

    class Freq {
    public:

        int p;
        int f;
        float xa;
        float ya;
        float x1;
        float y1;
        float x2;
        float y2;
    };

    class MTDM {
    public:

        MTDM(int fsamp);
        int process(size_t len, float *inp, float *out);
        int resolve();

        void invert() {
            _inv ^= 1;
        }

        int inv() {
            return _inv;
        }

        double del() {
            return _del;
        }

        double err() {
            return _err;
        }

    private:

        double _del;
        double _err;
        float _wlp;
        int _cnt;
        int _inv;
        Freq _freq [13];
    };

}


#endif

