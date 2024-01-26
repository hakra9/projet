#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>





const int ledPin = 5;         //GPIO5 --->D1 
const int ledPin2 = 4;        //GPIO4 --->D2
const int oneWireBus = 14;    //GPI14 --->D5



const char *ssid = "TOPNET_D5C0";
const char *password = "96lcpz16rb";


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


ESP8266WebServer server(80);

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP8266 Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>detecteur de temperature et de liminosité!!</h2>\
      <p>\
        <img src='https://media.istockphoto.com/id/1184204481/vector/thermometer-vector-flat-design-stock-illustration.jpg?s=612x612&w=0&k=20&c=xLVkQcL5KscGB3lUNtqTKIPdorT-H7xbAOgkUzhVfTc=' alt='Temperature Image' style='width: 100px; height: 100px;'>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <img src='https://thumbs.dreamstime.com/b/concept-d-id%C3%A9e-illustration-ic%C3%B4ne-ampoule-signe-de-lampe-%C3%A0-lueur-125982309.jpg' alt='Light Intensity Image' style='width: 100px; height: 100px;'>\
        <span class='dht-labels'>luminosité</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDS18B20(), (readphotores()*100)/350
          );
  server.send(200, "text/html", msg);
}


void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  // Start the DS18B20 sensor
  sensors.begin();


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());




  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  delay(1000);


}

void loop() {
  server.handleClient();
  float analogValue = readphotores();
  
  Serial.print("Analog Value = ");
  Serial.print(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 40) {
    Serial.println(" => Dark");
    digitalWrite(ledPin, 1);
    digitalWrite(ledPin2, 1);

  } else if (analogValue < 150) {
    Serial.println(" => Bright");
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, LOW);


  } else {
    Serial.println(" => Very bright");
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);

  }
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  delay(2);


}




float readphotores(){
  // reads the input on analog pin (value between 0 and 4095)
  int analogValue = analogRead(A0);

  return analogValue;
}


float readDS18B20(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  return temperatureC ;
}
