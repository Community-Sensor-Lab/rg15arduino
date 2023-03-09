#ifndef _RG15ARDUINO_H_
#define _RG15ARDUINO_H_
#include "Stream.h"

enum Unit {
    in,mm,
    iph,mmph,
    tips
};


class RG15Arduino {
private:
    Stream* stream;

    bool sendCommandLetter(char a);
public:
    float acc, eventAcc, totalAcc, rInt;
//    unsigned int xTBTips;
//    float xTbEventAcc, xTbInt;
    bool metric;
    char unit[4];

    //arduino::String getAvailable();
	String getAvailable();
    void setStream(Stream* stream);

    //Reset the tracking retrieved by poll
    bool resetAccumulation();
    bool restartDevice();

    //Updates acc, eventAcc, totalAcc, rInt & metric
    bool poll();
    //Check if the device is still alive
    bool ping();
    bool setBaud(int baudCode);
    bool useExternalTippingBucket(bool isTippingBucketPresent);
    bool setContinuous();
    bool setPolling();
    bool setMetric();
    bool setImperial();
    bool setLowResolution();
    bool setHighResolution();
    bool resetSwitchToPhysical();
};

#endif //_RG15ARDUINO_H_
