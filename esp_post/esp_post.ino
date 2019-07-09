
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

String msg;
void setup() {
 
  Serial.begin(9600);                                  //Serial connection
  WiFi.begin("WLAN-NG5BKM", "1943260452241592");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }

  
  Serial.println("ESP started");
  while(!Serial)
  ;
 
}
 
void loop() {
  if(Serial.available())
  {
    msg = Serial.read();
    Serial.println(msg);
  } 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    Serial.println("wifi connected");
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("192.168.2.111", 8000);      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 
 }
delay(6000);
} 
