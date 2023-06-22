//see ReadMe in .git

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

void PrintRawData(void);

byte mac[] = {0x00, 0xAD, 0xBE, 0xEF, 0xFE, 0x02};

//Setting static IP settings in case DHCP fails
IPAddress ip(192, 168, 10, 15);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

//ui24 will be the server
IPAddress server(192, 168, 10, 5);  // numeric IP for ui24 (assigned via DHCP)
//char server[] = "ui.io/mixer";    // name address for Soundcraft GUI

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

bool printWebData = true;  // will print everything the client gets, if true

//setting up the button
int pin_snap1 = 7;
int pin_snap2 = 8;
int val1 = 0;
int val2 = 0;
int oldval1=0;
int oldval2=0;


void setup() {
  Serial.begin(115200);

  // Setup the pin to read
   pinMode(pin_snap1, INPUT);
   pinMode(pin_snap2, INPUT);

  // start the Ethernet connection:
  Ethernet.begin(mac, ip, gateway, subnet);

  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  // give the Ethernet shield a second to initialize:
  delay(1000);

  //connecting to ui24
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP()); //Returns the IP address of the client

    // Make a HTTP request:
    client.println("GET /raw HTTP/1.1\n\n");
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("Failed at connecting to server");
  }
}


void loop() {
  //PrintRawData();
  
  val1 = digitalRead(pin_snap1);
  val2 = digitalRead(pin_snap2);
  
  if (oldval1 != val1) {
    oldval1 = val1;
    if (val1 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.1");
      Serial.println("Snap 1.1");
    }
  }

  if (oldval2 != val2) {    
    oldval2 = val2;
    if (val2 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.2");
      Serial.println(val2);
    }
  }
}


void PrintRawData(){
  // if there are incoming bytes available from the server, read them and print them
  
  //client.available() Returns the number of bytes available for reading
  //that is, the amount of data that has been written to the client by the server it is connected to  
  int len = client.available();

  //setting up a read-print buffer:
  if (len > 0) {
    byte buffer[512];
    if (len > 512) len = 512;
    client.read(buffer, len); //Read the next byte received from the server the client is connected to
    if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("Server disconnected, stopping the session.");
    client.stop();
    // do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
}