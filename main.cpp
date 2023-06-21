#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x00, 0xAD, 0xBE, 0xEF, 0xFE, 0x02};
IPAddress ip(192, 168, 100, 55);
// gateway, DNS and subnet are optional:
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress myDns(192, 168, 100, 254);

/*
EthernetServer creates a server that listens to a port
telnet defaults to port 23
*/
EthernetServer server(23);

// whether or not you got a message from the client yet
bool gotAMessage = false; 


void setup() {
  /*
  You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet
  */

  Serial.begin(9600);

  // start the Ethernet connection:
  Serial.println("\nTrying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // initialize the Ethernet device not using DHCP:
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }

  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  // start listening for clients
  server.begin();
}


void loop() {
  // wait for a new client. server.available() gets a client that is connected to the server and has data available for reading
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!gotAMessage) {
      Serial.println("We have a new client");
      //client.println prints data to the server a client is connected to.
      client.println("Hello, client!");
      Serial.println("\nThe client says:");
      gotAMessage = true;
    }

    // read the bytes incoming from the client:
    char thisChar = client.read();
    // echo the bytes back to the client:
    // server.write() writes data to all the clients connected to a server.
    //server.write(thisChar);
    Serial.print(thisChar);
    Ethernet.maintain();
  }
}