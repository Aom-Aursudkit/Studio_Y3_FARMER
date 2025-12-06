#include "AIS_NB_BC95.h"

String serverIP = "35.213.140.100";  // Your Server IP
String serverPort = "5005";          // Your Server Port

String udpData = "HelloWorld";

AIS_NB_BC95 AISnb;

const long interval = 20000;  //millisecond
unsigned long previousMillis = 0;
// unsigned long previousMillis_sensor = 0;
unsigned int water_level = 0;

int pwmPin = 7;
unsigned long highT, lowT;
// float duty;

long cnt = 0;

void setup() {
  AISnb.debug = true;

  Serial.begin(115200);

  pinMode(pwmPin, INPUT);
  pinMode(13, OUTPUT);

  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();
  delay(1000);

  pingRESP pingR = AISnb.pingIP(serverIP);

  previousMillis = millis();
}

void loop() {
  digitalWrite(13,1);

  highT = pulseIn(pwmPin, HIGH);
  lowT  = pulseIn(pwmPin, LOW);

  if(highT > 0 && lowT > 0){
    water_level = ((float)highT * 2000) / (highT + lowT);
    Serial.println(water_level);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    cnt++;

    digitalWrite(13,0);

    // Send data in String
    // UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, udpData + String(cnt));
    UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, String(water_level));

    //Send data in HexString
    //udpDataHEX = AISnb.str2HexStr(udpData);
    //UDPSend udp = AISnb.sendUDPmsg(serverIP, serverPort, udpDataHEX);

    UDPReceive resp = AISnb.waitResponse();

    previousMillis = currentMillis;
  }
}
