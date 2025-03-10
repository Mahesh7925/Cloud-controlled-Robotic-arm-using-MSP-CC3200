#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include <Servo.h>

char ssid[] = "Mahesh";

char password[] = "79257925";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);

Servo myservo1;  //base
Servo myservo2;  //mid
Servo myservo3;  //end
Servo myservo4;  //gripper

int pos = 0;



void setup() {
  myservo1.attach(9);      
  myservo2.attach(10);
  myservo3.attach(29);
  myservo4.attach(31);
  Serial.begin(115200);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the LED pin mode

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");
}

void loop() {
  int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<html><head><title>Cloud Controlled Robotic Arm</title></head><body align=center>");
            client.println("<h1 align=center><font color=\"red\">Cloud Controlled Robotic Arm</font></h1>");
            client.print("TASK :  <button onclick=\"location.href='/H'\">Task1</button>");
            client.println(" <button onclick=\"location.href='/L'\">Task2</button><br>");
            client.print(" <button onclick=\"location.href='/I'\">Initial pos</button><br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L" or "GET /I":
        if (endsWith(buffer, "GET /H")) {
          task1();                           //Executes Task1 action
          
        }
        if (endsWith(buffer, "GET /L")) {
          task2();                           //Executes Task2 action
          
        }
        if (endsWith(buffer, "GET /I")) {
          initial_position()                 //goes to initial position
          
          
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void task1(){
     myservo2.write(180);
     myservo3.write(0);
     delay(2000);
     myservo4.write(180);  // gripper close
     delay(2000);
     myservo2.write(0);
     myservo3.write(180);
     delay(2000);
     myservo1.write(180);
     delay(2000);
     myservo2.write(180);
     myservo3.write(0);
     delay(2000);
     myservo4.write(120);  //gripper open
     delay(2000);
     initial_position()
     delay(1000);
     
}

void task2(){
     myservo1.write(180);
     delay(2000);
     myservo2.write(180);
     myservo3.write(0);
     delay(2000);
     myservo4.write(180);  // gripper close
     delay(2000);
     myservo2.write(0);
     myservo3.write(180);
     delay(2000);
     myservo1.write(0);
     delay(2000);
     myservo2.write(180);
     myservo3.write(0);
     delay(2000);
     myservo4.write(120);  //gripper open
     delay(2000);
     initial_position()
     delay(1000);
     
  
}

void initial_position(){

  myservo1.write(90);
  myservo2.write(90);
  myservo3.write(90);
  myservo4.write(120);
   
}
