#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int touchPin = 11; 
int motor = 7;
int buttion = 3;

// משתנים שמשתנים בתוך פסיקה חייבים להיות מוגדרים כ-volatile
volatile int memo = 0; 
volatile bool buttonPressed = false; 

void setup() {
  // פתיחת ערוץ התקשורת הטורית
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();
  
  pinMode(touchPin, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(buttion, INPUT_PULLUP);

  // הדפסת הודעה ראשונית כדי לדעת שהמסך עובד


  // הגדרת ה-IRQ (הפסיקה) לפין 3
  attachInterrupt(digitalPinToInterrupt(buttion), buttonISR, FALLING);
}

void loop() {
  // קריאת המצב הנוכחי של חיישן המגע
  int sensorState = digitalRead(touchPin);

  // אם החיישן מזהה מגע (HIGH) - מדליק את המנוע
  if (sensorState == HIGH) {
    digitalWrite(motor, HIGH);
    lcd.setCursor(2,0);
    lcd.print("hello");
    delay(1000);
    lcd.setCursor(2,0);
    lcd.print("to shoot");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("press the button");
    delay(2000);
    lcd.clear();
  }

  // בדיקה בטוחה בתוך הלולאה אם הפסיקה זיהתה לחיצה
  if (buttonPressed) {
    // 1. מכבים את המנוע (אם תרצי שהוא ייכבה בלחיצה)
    digitalWrite(motor, LOW); 
    
    // 2. מדפיסים למסך ה-LCD בצורה בטוח 
    
    lcd.setCursor(2, 1);
    lcd.print("shoot");
    delay(800);
    lcd.clear();

    // 4. רק עכשיו, אחרי שטיפלנו בלחיצה, מאפסים את המשתנה ומחכים ללחיצה הבאה
    buttonPressed = false; 
  }
}

// פונקציית הפסיקה (ISR) - עכשיו היא נקייה, מהירה ובטוחה!
void buttonISR() {
  memo = memo + 1; 
  buttonPressed = true; // מסמנת ל-loop שהייתה לחיצה
}
