// ---------------------------------------------------------------------------
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>
#include <Adafruit_NeoPixel.h>


#define SONAR_NUM     3 // Number or sensors.
#define MAX_DISTANCE 51 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 29 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

#define PIN 11
#define PIX_TIME 30;
unsigned long pixTimer = 100; // Holds the times when the next ping should happen for each sensor.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(240, PIN, NEO_GRB + NEO_KHZ800);

unsigned long ActualColor = 0;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(3, 2, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(5, 4, MAX_DISTANCE),
  NewPing(7, 6, MAX_DISTANCE)
};

void setup() {
	randomSeed(analogRead(0));
  Serial.begin(115200);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
	
	if (millis() >= pixTimer) 
	{
		pixTimer = millis() + PIX_TIME;
		for(int i = strip.numPixels()-1; i > 0; i--)
			strip.setPixelColor(i, strip.getPixelColor(i-1));
		
		strip.setPixelColor(0, ActualColor);
		strip.show();
	}
  }
  // The rest of your code would go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  /*for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();*/
  
  /*  ActualColor = 256-cm[0]*4;
  ActualColor = ActualColor << 8;
  ActualColor += 256-cm[1]*4;
  ActualColor = ActualColor << 8;
  ActualColor += 256-cm[2]*4;
  */
  if(cm[0] == 0) cm[0] = random(MAX_DISTANCE-2,MAX_DISTANCE);
  if(cm[1] == 0) cm[1] = random(MAX_DISTANCE-2,MAX_DISTANCE);
  if(cm[2] == 0) cm[2] = random(MAX_DISTANCE-2,MAX_DISTANCE);
  ActualColor = (MAX_DISTANCE-cm[0])*3;
  ActualColor = ActualColor << 8;  
  ActualColor += (MAX_DISTANCE-cm[1])*3;
  ActualColor = ActualColor << 8;
  ActualColor += (MAX_DISTANCE-cm[2])*3;
   Serial.println(ActualColor,16);
  
}
