#include "rg15arduino.h" 

//"Acc 1.234 in, EventAcc 0.002 in, TotalAcc 0.003 in, RInt 0.082 iph";
// Edited by Ricardo Toledo-Crow, Jason Cayetano 
// March 13. 2023 
// part of the RG15Arduino class 
// should look the same as the code from the sketch, except with RG15Arduino:: in front of the names 
// of the functions 

int RG15Arduino::poll() { //Constructor, which defines what will happen when an object is created 
  //long int start; // for timing debug
  int flag = 0; 
   
  if (Serial1.available() == 0){
    Serial1.print("r\n"); 
  }
  else
	  Serial.print('-'); // to check if sending command twice or more
  
  start = millis();
  //Serial.print(start);Serial.print(" diff ");
  while(!Serial1.available()) {
	  if ((millis()-start)>100) {
		  Serial.println("timeout = 1");
		  flag = 3;
		  return flag;
	  };
	  Serial.print('.');
  }
  //Serial.println(millis()-start);
  
  static int rxmessage_pos = 0; // why is satic needed here?
  static char rxmessage[MAX_MESSAGE_LENGTH]; 
	
  while (Serial1.available())  { 
    char inByte = (char)Serial1.read();
	delay(3);	// magic delay lets rg15 respond
    
	if ( inByte != '\n' && (rxmessage_pos < MAX_MESSAGE_LENGTH - 1) ) //if the char is not a new line and the message is less than 1
    { rxmessage[rxmessage_pos] = inByte; 
      rxmessage_pos++; 
    } 
    else // end of transmission. 
    { rxmessage[rxmessage_pos] = '\0'; 

      if (strstr(rxmessage, "Acc ")) { // see if we have valid data and parse
        char accB[12], eventAccB[12], totalAccB[12], rIntB[12]; 
        sscanf (rxmessage,"%*s %s %[^,], %*s %s %*s %*s %s %*s %*s %s", &accB, &unt, &eventAccB, &totalAccB, &rIntB);

        acc = atof(accB);
        eventAcc = atof(eventAccB);
        totalAcc = atof(totalAccB);
        rInt = atof(rIntB);

        // metric = !(unt[0] == 'i' && unt[1] == 'n');
		flag = 1; // got valid data
	  }
	  else 
		flag = 2; // got response but no data in it

	  Serial.println(rxmessage); 
	  rxmessage_pos = 0; 
    } 
  } 
  return flag; 
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