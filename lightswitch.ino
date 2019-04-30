#include <ESP8266WiFi.h>
#include "secrets.h"
#include <WiFiUdp.h>
#include <Servo.h>

Servo servo;
WiFiUDP Udp;
int angle1 = 82;
int angle2 = 117;
int xangle1 = 75;
int xangle2 = 124;
int xavg = 100;
bool x = true;
int angleCurrent = 100;
char incomingPacket[33];

void setup() {
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    servo.attach(D1);

    Udp.begin(8000);

    servo.write(angleCurrent);
}

void loop() {
    int packetSize = Udp.parsePacket();
    if (packetSize == 33) {
        Udp.read(incomingPacket, 33);
        if (strncmp(incomingPacket, secret, 32) == 0) {
            if (incomingPacket[32] == '+') {
                if (angleCurrent != xavg) {
                    angleCurrent += 2;
                }
                angle1 += 2;
                angle2 += 2;
                xangle1 += 2;
                xangle2 += 2;
            } else if (incomingPacket[32] == '-') {
                if (angleCurrent != xavg) {
                    angleCurrent -= 2;
                }
                angle1 -= 2;
                angle2 -= 2;
                xangle1 -= 2;
                xangle2 -= 2;
            } else if (incomingPacket[32] == '0') {
                angleCurrent = x ? xangle1 : angle1;
            } else if (incomingPacket[32] == '1') {
                angleCurrent = x ? xangle2 : angle2;
            } else if (incomingPacket[32] == 'x') {
                if (x) {
                    x = false;
                    if (angleCurrent == xangle1) {
                        angleCurrent = angle1;
                    } else if (angleCurrent == xangle2) {
                        angleCurrent = angle2;
                    }
                } else {
                    x = true;
                    if (angleCurrent == angle1) {
                        angleCurrent = xangle1;
                    } else if (angleCurrent == angle2) {
                        angleCurrent = xangle2;
                    }
                }
            }
            servo.write(angleCurrent);
            if (angleCurrent == xangle1 || angleCurrent == xangle2) {
                delay(250);
                angleCurrent = xavg;
                servo.write(angleCurrent);
            }
        }
    }
}