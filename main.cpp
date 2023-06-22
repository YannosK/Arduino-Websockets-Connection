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

//setting up the buttons
//will use pins 2 to 9 (8 pins)
int pin_snap1 = 2;
int pin_snap2 = 3;
int pin_snap3 = 4;
int pin_snap4 = 5;
int pin_snap5 = 6;
int pin_snap6 = 7;
int pin_snap7 = 8;
int pin_snap8 = 9;
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;
int val7 = 0;
int val8 = 0;
int oldval1=0;
int oldval2=0;
int oldval3=0;
int oldval4=0;
int oldval5=0;
int oldval6=0;
int oldval7=0;
int oldval8=0;


void setup() {
  Serial.begin(115200);

  // Setup the pins to read
   pinMode(pin_snap1, INPUT);
   pinMode(pin_snap2, INPUT);
   pinMode(pin_snap3, INPUT);
   pinMode(pin_snap4, INPUT);
   pinMode(pin_snap5, INPUT);
   pinMode(pin_snap6, INPUT);
   pinMode(pin_snap7, INPUT);
   pinMode(pin_snap8, INPUT);

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
  val3 = digitalRead(pin_snap3);
  val4 = digitalRead(pin_snap4);
  val5 = digitalRead(pin_snap5);
  val6 = digitalRead(pin_snap6);
  val7 = digitalRead(pin_snap7);
  val8 = digitalRead(pin_snap8);
  
  
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
      Serial.println("Snap 1.2");
    }
  }

  if (oldval3 != val3) {
    oldval3 = val3;
    if (val3 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.3");
      Serial.println("Snap 1.3");
    }
  }

  if (oldval4 != val4) {
    oldval4 = val4;
    if (val4 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.4");
      Serial.println("Snap 1.4");
    }
  }

  if (oldval5 != val5) {
    oldval5 = val5;
    if (val5 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.5");
      Serial.println("Snap 1.5");
    }
  }

  if (oldval6 != val6) {
    oldval6 = val6;
    if (val6 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.6");
      Serial.println("Snap 1.6");
    }
  }

  if (oldval7 != val7) {
    oldval7 = val7;
    if (val7 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.7");
      Serial.println("Snap 1.7");
    }
  }

  if (oldval8 != val8) {
    oldval8 = val8;
    if (val8 == 1) {
      client.println("LOADSNAPSHOT^Show 1^Snap 1.8");
      Serial.println("Snap 1.8");
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