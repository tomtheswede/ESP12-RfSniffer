//Updated 17/4/17


const int readPin = 16; //16 for v0.1 of PWM board print

//1 = 690 up then 310 down.  990 up, 370 down
//0== 245 up then 775 down.   310 up, 1050 down
const long t1=250;
const long t2=700;
const long t3=t2+1;
const long t4=1800;


long lastRead=millis();
byte rfBuffer=0;
byte bitBuffer=0;
long riseTime=lastRead;
long fallTime=lastRead;
long upTime=10;
long downTime=10;
bool fallFlag=false;
bool riseFlag=false;
bool newByte=false;
byte bitCount=0;
byte byteCount=0;
byte byteStore[9]={0,0,0,0,0,0,0,0,0};
bool record=false;
unsigned int msg=0;
long lastTriggered=0;
bool msgFlag=false;
byte msgType=0;
String outputString="";
long lastMsgTime=0;
String lastString1="";
String lastString2="";
boolean transmitFlag=false;
long lastTransmit=0;
//long lengthBuffer[300];
//int m=0;


void setup() {
  //For Mosfets if connected
  pinMode(12, OUTPUT); //Set as output
  digitalWrite(12, 0); //Turn off LED while connecting
  pinMode(13, OUTPUT); //Set as output
  digitalWrite(13, 0); //Turn off LED while connecting
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  //ConnectWifi();  //Only include connection if in produduction mode

  Serial.println("Ready");
}


void loop() {
  //For detecting rising and falling edges of a waveform
  if (micros()-lastRead>10) { //Takes 80us to fill buffer
    rfBuffer=rfBuffer<<1;
    rfBuffer=rfBuffer+digitalRead(readPin);
    if (rfBuffer==7) { //Indicates a rise 00001111
      riseTime=micros();
      downTime=riseTime-fallTime;
      riseFlag=true;
    }
    else if (rfBuffer==240) { //Indicates a fall 11110000
      fallTime=micros();
      upTime=fallTime-riseTime;
      fallFlag=true;
    }
  }
  //For catching bit durations and adding to the buffer, then saying when a new byte is ready
  if (riseFlag) {
    riseFlag=false;
    if (downTime>t4+t1) { //Message must have ended since no new bits detected
      bitBuffer=0;
      bitCount=0;
      byteCount=0;
    }
    else if (upTime>t1 && upTime<t2) { //Indicates a 0
      bitBuffer=bitBuffer<<1;
      bitCount=bitCount+1;
//      lengthBuffer[m]=downTime;
//      m++;
//      lengthBuffer[m]=upTime;
//      m++;
      if (bitCount>=8) {
        newByte=true;
      }
    }
    else if (upTime>t3 && upTime<t4) { //Indicates a 1
      bitBuffer=bitBuffer<<1;
      bitBuffer=bitBuffer+1;
      bitCount=bitCount+1;
//      lengthBuffer[m]=downTime;
//      m++;
//      lengthBuffer[m]=upTime;
//      m++;
      if (bitCount>=8) {
        newByte=true;
      }
    }
  }
  //For recognising if a preable criteria is met and starting record
  if (newByte) {
    newByte=false;
    Serial.println(bitBuffer); //Uncomment to get a stream of read bytes----------
    bitBuffer=0;
    bitCount=0;
  }

}

