#include <SoftwareSerial.h>
#include <TinyGPS.h>
//You would have to install the above libraries for the code to work

int state = 0;
const int pin = 9;
float gpslat, gpslon;

TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);

void setup()
{
  sgsm.begin(9600);
  sgps.begin(9600);
}

void loop()
{
sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if (digitalRead(pin) == HIGH && state == 0) {
      sgsm.listen();
  	  sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);
      /*Replace XXXXXXXXXX to 10 digit mobile number &
        ZZ to 2 digit country code*/
      sgsm.print("AT+CMGS=\"+ZZXXXXXXXXXX\"\r");
      delay(1000);
      //The text of the message to be sent.
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6); //To fetch latitude
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6); //To fetch longitude

      //Concate the latitude & longitude into Google Maps URL redirection to Google Maps
      sgsm.print("https://www.google.com/maps/?q="); 
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (digitalRead(pin) == LOW) {
      state = 0;
    }
      delay(100);
}