
// SERVO
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN_0  155 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_0  250 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_1  160 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_1  260 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_2  155 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_2  260 // This is the 'maximum' pulse length count (out of 4096)
//#define SERVOMIN_3  140 // This is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX_3  255 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// SERVO ADDRESS
uint8_t servonum_0 = 0;
uint8_t servonum_1 = 1;
uint8_t servonum_2 = 2;
//uint8_t servonum_3 = 3;

// PROXIMITY
const int e18_sensor_1 = 22;
const int e18_sensor_2 = 24;
int state_proximity_1;
int state_proximity_2;
boolean baca = true;

//Serial event to interrupt
String inputString = "";         // a string to hold incoming data  
boolean stringComplete = false;  // whether the string is complete
int data_masuk=0;
int data_benda[100];
int posisi_servo[100];
int iterasi=0;
int index_servo = 0;
char inChar = "";
boolean kertas_lewat = false;
boolean servo_sampek = false;

void setup() {
  // INISIALISASI SERIAL
  Serial.begin(9600);

  // SET PIN PROXIMITY
  pinMode(e18_sensor_1, INPUT);
  pinMode(e18_sensor_2, INPUT);

  // MENYEDIAKAN TEMPAT 200 bytes untuk inputstring
  inputString.reserve(200);

  // INISIALISASI PWM
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
  pwm.setPWM(servonum_0, 0, SERVOMIN_0+10);
  delay(100);
    for (uint16_t pulselen = SERVOMIN_0+10; pulselen > SERVOMIN_0; pulselen--) {
      pwm.setPWM(servonum_0, 0, pulselen);
      //delay(0.001);
//      Serial.print("SERVO 0 SUDUT ");Serial.println(pulselen);
    }
    pwm.setPWM(servonum_1, 0, SERVOMIN_1+10);
    delay(100);
    //Serial.println(servonum_0);
    for (uint16_t pulselen = SERVOMIN_1+10; pulselen > SERVOMIN_1; pulselen--) {
      pwm.setPWM(servonum_1, 0, pulselen);
      //delay(0.001);
//      Serial.print("SERVO 1 SUDUT ");Serial.println(pulselen);
    }
    pwm.setPWM(servonum_2, 0, SERVOMAX_2 - 5);
    delay(100);
    for (uint16_t pulselen = SERVOMAX_2 - 5; pulselen < SERVOMAX_2; pulselen++) {
      pwm.setPWM(servonum_2, 0, pulselen);
      delay(0.1);
////      Serial.print("SERVO 2 SUDUT ");Serial.println(pulselen);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(iterasi < data_masuk){
    iterasi++;
    
//    Serial.print("ITERASI "); Serial.println(iterasi);
//    Serial.print("BENDA "); Serial.println(data_benda[iterasi]);
//    Serial.print("POSISI SERVO "); Serial.println(posisi_servo[iterasi]);
    servo();
    while(baca){
      sensor_proximity();
    }
//    delay(100);
    reset_servo();
  }
}

void serialEvent(){
  while(Serial.available()){
    // get new byte
    char inChar = (char)Serial.read();
  
    // add to inputString
    inputString += inChar;
  
    // masukkan data ke array
    if (inputString == "0" || inputString == "1" || inputString == "2" || inputString == "3"){
      data_masuk++;
//      Serial.print("DATA MASUK "); Serial.println(data_masuk);
      data_benda[data_masuk] = inputString.toInt();
      posisi_servo[data_masuk] = data_benda[data_masuk];
//      Serial.print("BENDA YANG MASUK "); Serial.println(data_benda[data_masuk]);
    }
    inputString = "";
    if (inChar == '\n') {
        stringComplete = true;
    }
  }
}

void sensor_proximity(){
  state_proximity_1 = digitalRead(e18_sensor_1);
  state_proximity_2 = digitalRead(e18_sensor_2);
  int benda_masuk = data_benda[data_masuk];
  if (benda_masuk == 0){
    if (state_proximity_1 == LOW){
//      delay(1000);
      baca = false;
      Serial.println("BENDA 0");
    }
  }
  else if (benda_masuk == 1){
    if (state_proximity_2 == LOW){
//      delay(1000);
      baca = false;
      Serial.println("BENDA 1");
    }
  }
  else if(benda_masuk == 2){
    delay(3000);
    kertas_lewat = true;
    if(kertas_lewat || state_proximity_2 == LOW){
      baca = false;
    }
  }
  else if (benda_masuk == 3){
    //delay(1000);
    if (state_proximity_2 == LOW){
//      delay(1000);
    Serial.println("BENDA 3");
      baca = false;
    }
  }
}

void servo(){
//  Serial.print("SERVO YANG GERAK "); Serial.println(posisi_servo[iterasi]);
  if (posisi_servo[iterasi] == 0){
    pwm.setPWM(servonum_0, 0, SERVOMAX_0-40);
    delay(100);
    //Serial.println(servonum_0);
    for (uint16_t pulselen = SERVOMAX_0-40; pulselen < SERVOMAX_0; pulselen++) {
      pwm.setPWM(servonum_0, 0, pulselen);
      //delay(0.001);
      //Serial.print("SERVO 0 SUDUT ");Serial.println(pulselen);
      
    }    
  }
  else if (posisi_servo[iterasi] == 1){
    pwm.setPWM(servonum_1, 0, SERVOMAX_1-40);
    delay(100);
    //Serial.println(servonum_0);
    for (uint16_t pulselen = SERVOMAX_1-40; pulselen < SERVOMAX_1; pulselen++) {
      pwm.setPWM(servonum_1, 0, pulselen);
      //delay(0.001);
      //Serial.print("SERVO 1 SUDUT ");Serial.println(pulselen);
    }
  }
  else if (posisi_servo[iterasi] == 2){
    //Serial.print("KERTAS LEWAT");
  }
  else if (posisi_servo[iterasi] == 3){
    pwm.setPWM(servonum_2, 0, SERVOMIN_2 + 45);
    //for (uint16_t pulselen = SERVOMAX_2; pulselen > SERVOMIN_2+45; pulselen--) {
    //  pwm.setPWM(servonum_2, 0, pulselen);
    //}
    delay(100);
    for (uint16_t pulselen = SERVOMIN_2 + 45; pulselen > SERVOMIN_2; pulselen--) {
      pwm.setPWM(servonum_2, 0, pulselen);
      //delay(0.1);
      //Serial.print("SERVO 2 SUDUT ");Serial.println(pulselen);
      
    }
    delay(100);
  }
}

void reset_servo(){
  delay(5000);
  //Serial.println("SEMUA SERVO KEMBALI KE KEADAAN SEMULA (RESET)");
  if (posisi_servo[iterasi] == 0){
    pwm.setPWM(servonum_0, 0, SERVOMIN_0+45);
    delay(100);
    for (uint16_t pulselen = SERVOMIN_0+45; pulselen > SERVOMIN_0; pulselen--) {
      pwm.setPWM(servonum_0, 0, pulselen);
      //delay(0.001);
      //Serial.print("SERVO 0 SUDUT ");Serial.println(pulselen);
    }
  }
  else if (posisi_servo[iterasi] == 1){
    pwm.setPWM(servonum_1, 0, SERVOMIN_1+45);
    delay(100);
    for (uint16_t pulselen = SERVOMIN_1+45; pulselen > SERVOMIN_1; pulselen--) {
      pwm.setPWM(servonum_1, 0, pulselen);
      //delay(0.001);
      //Serial.print("SERVO 1 SUDUT ");Serial.println(pulselen);
    }
  }
  else if(posisi_servo[iterasi] == 2){
    //Serial.print("KERTAS SUDAH LEWAT");
  }
  else if (posisi_servo[iterasi] == 3){
    pwm.setPWM(servonum_2, 0, SERVOMAX_2 - 40);
    delay(100);
    for (uint16_t pulselen = SERVOMAX_2 - 40; pulselen < SERVOMAX_2; pulselen++) {
      pwm.setPWM(servonum_2, 0, pulselen);
      //delay(0.1);
      //Serial.print("SERVO 2 SUDUT ");Serial.println(pulselen);
    }
  }
  delay(10);
  baca = true;
}
