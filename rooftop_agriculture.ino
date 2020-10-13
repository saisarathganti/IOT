#include <CircularBuffer.h>
CircularBuffer<int, 40> buffer;
unsigned long tim = 0;
#define treshold_value 50 //decide what to keep after testing both with light and without light
int light=0;
float avg = 0.0;
int reading=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  tim = millis();
}

void state2(){
  if(light==0){
    //send_pckt(0,0,1);
    digitalWrite(D1,HIGH);
    Serial.println("turning on light");
    light=1;

  }
}
void state3(){
  if(light==1){
    //send_pckt(0,0,0);
    digitalWrite(D1,LOW);
    Serial.println("turning off light");
    light=0;
  }  
}

void state1(){
  tim = millis();
    avg = 0.0;
    // the following ensures using the right type for the index variable
    using index_t = decltype(buffer)::index_t;
    for (index_t i = 0; i < buffer.size(); i++) {
      avg += buffer[i] / buffer.size();
    }
    Serial.print("Average is ");
    Serial.println(avg);
    if(avg < treshold_value){  // LOW-PASS FILTERING FOR STATE2
      state2();
    }
    else{
      state3();
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  reading=analogRead(A0);
  buffer.push(reading);
  if (millis() - tim >= 500) {
    state1();
  }
}   
