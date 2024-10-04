#include <Wire.h>                        // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>           // LCD 2004 I2C용 라이브러리

LiquidCrystal_I2C lcd(0x27, 16, 2);      // I2C 주소 0x27로 LCD 초기화
int water_pin_1 = A0;  // External water level sensor connected to A0
int water_pin_2 = A1;  // Internal water level sensor connected to A1
int buzzerPin = 8;
int rightSensor = 12;

int threshold =100;    // Threshold value


// 스마일 이모티콘
byte smile[8] = {
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000,
};

void setup() {
  Serial.begin(9600);     // Initialize the serial monitor
  pinMode(water_pin_1, INPUT);  // Set water_pin_1 as input
  pinMode(water_pin_2, INPUT);  // Set water_pin_2 as input
  pinMode(buzzerPin, OUTPUT);
  pinMode(rightSensor, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(rightSensor, LOW);

  lcd.init();                      // LCD 초기화
  lcd.backlight();                 // 백라이트 켜기
  lcd.createChar(1, smile);        // 스마일 모양 커스텀 캐릭터 생성
  
  // 첫 번째 메시지 출력
  lcd.setCursor(0,0);
  lcd.print("Your Safety");
  lcd.setCursor(0,1);
  lcd.print("is our priority");
  delay(1000);

  // 두 번째 메시지 출력
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SafeEscape");
  lcd.setCursor(0,1);
  lcd.print("always with U!");
  delay(1000);

  // 세 번째 메시지 출력
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("HopeBellight");
  
  // 스마일 이모티콘 깜빡이기
  blinkSmile();
  lcd.setCursor(7, 1);            // 스마일 이모티콘 위치 설정 (0열, 1행)
  lcd.write(byte(1));             // 스마일 이모티콘 출력
}

void blinkSmile() {
  for (int i = 0; i < 10; i++) {    // 10번 깜빡이도록 설정
    lcd.setCursor(7, 1);            // 스마일 이모티콘 위치 설정 (0열, 1행)
    lcd.write(byte(1));             // 스마일 이모티콘 출력
    delay(500);                     // 0.5초 대기
    
    lcd.setCursor(7, 1);            // 스마일 이모티콘 위치에 다시 커서 설정
    lcd.print(" ");                 // 스마일 이모티콘 지우기
    delay(500);                     // 0.5초 대기
  }
}

void loop() {
  int water_level_1 = analogRead(water_pin_1); // Read water level from sensor 1
  int water_level_2 = analogRead(water_pin_2); // Read water level from sensor 2

  int difference = water_level_1 - water_level_2; // Calculate the difference

  Serial.print("Water_1: ");
  Serial.print(water_level_1);
  Serial.print("   Water_2: ");
  Serial.println(water_level_2);

  if (water_level_1 > 300){
    if (difference < threshold) {
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Escape!");
      digitalWrite(buzzerPin, HIGH);
      lcd.setCursor(0,1);
      lcd.print("Right");
      digitalWrite(rightSensor, HIGH);

    } 
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Don't panic!");
      lcd.setCursor(0,1);
      lcd.print("Just a moment!");
      digitalWrite(buzzerPin, LOW);
      digitalWrite(rightSensor, LOW);

  }

  delay(1000);  // Wait for 1 second before the next loop iteration
}
}