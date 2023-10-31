#define sensor_pin A0
#define led_pin 13 // Пин светодиода

bool streaming = false;
bool send_one_value = false;
bool manual_mode = false;
bool alarm_mode = false;
long previous_send_time = 0;
long send_count = 0;
int current_light = 0; 

void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW); 
  manual_mode = false;
}

void loop()
{
  long current_time = millis();
  data_reading();
  int light = analogRead(sensor_pin);
 // Serial.println(light);

  if ((!manual_mode) && (!alarm_mode)){
    
    if (light < 300 && current_light >= 300) {
      
      digitalWrite(led_pin, HIGH);
      alarm_mode = false;
    } else if (light > 500 && current_light <= 500) {
     
      digitalWrite(led_pin, LOW);
      alarm_mode = false;
    }
  }
  current_light = light;

  if (streaming == true && current_time / 100 != send_count) {
    send_photo_data();
    previous_send_time = previous_send_time + 100;
    send_count = current_time / 100;
    Serial.print("Time in millis:");
    Serial.println(millis());
  }

  if (send_one_value == true) {
    send_photo_data();
    send_one_value = false;
  }

  if (alarm_mode) {
   
    digitalWrite(led_pin, (current_time / 1000) % 2 == 0 ? HIGH : LOW);
  }
}

void send_photo_data() {
  int val = analogRead(sensor_pin);
  Serial.print("Sensor value:");
  Serial.println(val);
}

void data_reading() {
  if (Serial.available() > 0) {
    // m режим ручной, s выключить лампочку, p включить лампочку
    char command = Serial.read();
    if (command == 'p') {
      streaming = true;
      if (manual_mode) {
        
        digitalWrite(led_pin, HIGH);
      }
    } else if (command == 's') {
      streaming = false;
      send_one_value = true;
      if (manual_mode) {
        
        digitalWrite(led_pin, LOW);
      }
    } else if (command == 'm') {
    
      manual_mode = true;
      alarm_mode = false;
      // a режим автоматический
    } else if (command == 'a') {
   // r режим сигнализации
      manual_mode = false;
    } else if (command == 'r') {
     
      alarm_mode = !alarm_mode;
      manual_mode = false;
      digitalWrite(led_pin, LOW); 
    } else {
      streaming = false;
    }
  }
}
