#include "rg15arduino.h" 

// NGENS ASRC CUNY RToledo-Crow, JCayetano March 13 2023 
// from RG15Arduino class by Hydreon for RG15 rain gauge 

//"Acc 1.234 in, EventAcc 0.002 in, TotalAcc 0.003 in, RInt 0.082 iph";

int RG15Arduino::poll() { //Constructor, which defines what will happen when an object is created 

	long int start; // for timeout check, timing debug
	int flag =0;
   
	if (!stream->available()) {
		stream->write('r');
		stream->write('\n');
	}
	else {
		//Serial.println('*'); // to check if sending command twice or more
	}
	
	start = millis();
	//Serial.print(start);Serial.print(" diff ");
	
	do {
		int flag =0;

		if ((millis()-start)>300) {
			//Serial.println("timeout:1");
			flag = 0;
			return flag; // timeout error
		};
		//Serial.print('.');
		//Serial.println(millis()-start);
  
		static int rxmessage_pos = 0; // why is satic needed here?
		static char rxmessage[MAX_MESSAGE_LENGTH]; 
	
		while (stream->available())  { 
    
			char inByte = stream->read();
			//delay(5);	// magic delay lets rg15 respond
    
			if ( inByte == '\n' || (rxmessage_pos == MAX_MESSAGE_LENGTH - 1) ) { // end of transmission or too long
				
				rxmessage[rxmessage_pos] = '\0';
				rxmessage_pos = 0;
				
				if (strstr(rxmessage, "Acc ")) { 
					char accB[7], eventAccB[7], totalAccB[7], rIntB[7]; 

					sscanf (rxmessage,"%*s %s %[^,], %*s %s %*s %*s %s %*s %*s %s", &accB, &units, &eventAccB, &totalAccB, &rIntB);

					acc = atof(accB);		
					eventAcc = atof(eventAccB);
					totalAcc = atof(totalAccB);
					rInt = atof(rIntB);
					//metric = !(unt[0] == 'i' && unt[1] == 'n');
					flag = 1;
					//Serial.println(rxmessage);
					return flag; // data read successful
				}
				else {// end of transmission. 
					flag = 2;
					Serial.println(rxmessage);
					return flag; // no data but response from rg15
				}
			}
			else { // add char to message
				rxmessage[rxmessage_pos++] = inByte; 
			}
		}
	} while(1);//!Serial1.available());
	return 3; // unknown timing error  
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