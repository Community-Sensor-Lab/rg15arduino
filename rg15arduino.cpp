#include "rg15arduino.h"

//"Acc 1.234 in, EventAcc 0.002 in, TotalAcc 0.003 in, RInt 0.082 iph";
bool RG15Arduino::poll() {
    stream->write('r');
    stream->write('\n');
	delay(500);

    String response = stream->readStringUntil('\n');
    if(response.startsWith(F("Acc"))) {

        char accB[7], eventAccB[7], totalAccB[7], rIntB[7];

        sscanf (response.c_str(),"%*s %s %[^,] , %*s %s %*s %*s %s %*s %*s %s", &accB, &unit, &eventAccB, &totalAccB, &rIntB);

        acc = atof(accB);
        eventAcc = atof(eventAccB);
        totalAcc = atof(totalAccB);
        rInt = atof(rIntB);

        metric = !(unit[0] == 'i' && unit[1] == 'n');

        return true;
    }
    return false;
}

bool RG15Arduino::ping() {
    return sendCommandLetter('p');
}

void RG15Arduino::setStream(Stream* stream) {
    this->stream = stream;
}

//Note that these will ignore the physical switch, undo this with useSwitchUnitAndResolution()
bool RG15Arduino::setMetric() {
    return sendCommandLetter('m');
}
bool RG15Arduino::setImperial() {
    return sendCommandLetter('i');
}

bool RG15Arduino::resetSwitchToPhysical() {
    return sendCommandLetter('s');
}

bool RG15Arduino::resetAccumulation() {
    return sendCommandLetter('o');
}

bool RG15Arduino::restartDevice() {
    return sendCommandLetter('k');
}

bool RG15Arduino::setLowResolution() {
    return sendCommandLetter('l');
}

bool RG15Arduino::setHighResolution() {
    return sendCommandLetter('h');
}

bool RG15Arduino::setContinuous() {
    return sendCommandLetter('c');
}

//arduino::String RG15Arduino::getAvailable() 
String RG15Arduino::getAvailable() {
    return stream->readStringUntil('\0');
}

bool RG15Arduino::useExternalTippingBucket(bool isTippingBucketPresent) {
    return sendCommandLetter( (isTippingBucketPresent) ? 'x' : 'y');
}

bool RG15Arduino::setBaud(int baudCode) {
    stream->write("b ");
    stream->write(baudCode);
    stream->write('\n');
    String response = stream->readStringUntil('\n');
    return response.charAt(2) == baudCode;
}


bool RG15Arduino::sendCommandLetter(char a) {
    stream->write(a);
    stream->write('\n');
    String response = stream->readStringUntil('\n');
    return response.charAt(0) == a;
}