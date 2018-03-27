
const int LDR_top = A0;
const int LDR_wall = A1;
const int smoke_detector = A2;
const int IR = A3;
const int LM35 = A4;
const int sensor_A5 = A5;

const int motor_gate_enable = 10;
const int motor_fan_enable = 11;
const int motor_gate_direction = 12;
const int motor_fan_direction = 13;

const int ultrasonic_echo = 4;
const int ultrasonic_trigger = 2;

const int laser = 9;

const int  push_button_1 = 5;
const int  push_button_2 = 7;
const int  push_button_3 = 6;

const int LED_1 = 0;
const int LED_2 = 1;
const int LED_3 = 3;

const int buzzer = 8;

void setup()
{

  pinMode(motor_gate_enable, OUTPUT);
  pinMode(motor_gate_direction, OUTPUT);
  pinMode(motor_fan_enable, OUTPUT);
  pinMode(motor_fan_direction, OUTPUT);

  pinMode(ultrasonic_echo, INPUT);
  pinMode(ultrasonic_trigger, OUTPUT);

  pinMode(push_button_1, INPUT);
  pinMode(push_button_2, INPUT);
  pinMode(push_button_3, INPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(laser, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(LDR_top, INPUT);
  pinMode(LDR_wall, INPUT);
  pinMode(smoke_detector, INPUT);
  pinMode(IR, INPUT);
  pinMode(LM35, INPUT);


  Serial.begin(9600);

}





int count = 0;

void loop()
{

  int LDR_Top = analogRead(LDR_top);
  int LDR_Wall = analogRead(LDR_wall);
  int Smoke_Detector = analogRead(smoke_detector);
  int Ir = analogRead(IR);
  int Lm = (analogRead(LM35) * 500) / 1024;
  digitalWrite(laser, HIGH);
  if (LDR_Wall < 100)
  { digitalWrite(buzzer, HIGH);
  }
  else
    digitalWrite(buzzer, LOW);

  if (Ir < 100 && count == 0)
  {
    analogWrite(motor_gate_enable, 100);

    digitalWrite(motor_gate_direction, HIGH);
    delay(3000);
    analogWrite(motor_gate_enable, 0);
    delay(1500);
    digitalWrite(motor_gate_direction, LOW);
    analogWrite(motor_gate_enable, 100);
    delay(3000);
    analogWrite(motor_gate_enable, 0);

    count++;
  }
  else if (Ir > 100)
  {
    analogWrite(motor_gate_enable, 0);
    count = 0;
  }

  if (Lm > 30)
  {
    analogWrite(motor_fan_enable, 50);
    digitalWrite(motor_fan_direction, HIGH);
  }
  else
    analogWrite(motor_fan_enable, 0);
  Serial.println("LDR top = " + String(LDR_Top));
  Serial.println("LDR Wall = " + String(LDR_Wall));
  Serial.println("Smoke = " + String(Smoke_Detector));
  Serial.println("IR = " + String(Ir));
  Serial.println("LM35 = " + String(abs(Lm)));

  delay(3000);

}

