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
IPAddress server(192, 168, 100, 198);  // numeric IP for ui24 (assigned via DHCP)
//char server[] = "ui.io/mixer";    // name address for Soundcraft GUI

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

bool printWebData = true;  // will print everything the client gets, if true

//setting up the button
int inPin = 7;
int val = 0;
int oldval=0;


void setup() {
  Serial.begin(115200);

  // Setup the pin to read
   pinMode(inPin, INPUT);

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
  PrintRawData();

  // read the input pin
  val = digitalRead(inPin);

  //check if the value is the same, we don't want multiple sends
  if (oldval != val) {
    Serial.println(val);
    oldval=val;// Make the old value same as the new value
    if (val == 1) client.println("SETD^i.0.mute^1") ; // Send SETD Mute with NEW LINE
    if (val == 0) client.println("SETD^i.0.mute^0") ;
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