#include <LiquidCrystal.h>
#include<shopping_list.h>

const int rs = 2, en = 6, d4 = 5, d5 = 4, d6 = 8, d7 = 7;
const byte interruptPin = 3;
const byte erase_btn = 9;
int erase_btn_state = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


list_items l("item_1:10,item_2:10");
list_items::iterator it;

char to_remove[10];

void interrupt_handler()
{
  Serial.println("pin 8 pressed");
  if(!it.isEnd())
  {
    lcd.clear();
    it = l.next(it);
    if(!it.isEnd()) lcd.print(*it);
    delay(500);
    
  }
  else
  {
    lcd.clear();
    it = l.begin();
    lcd.print(*it);
    delay(500);
  }

}

void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  Serial.println("started");
  lcd.begin(16, 2);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(erase_btn, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_handler, RISING);
  lcd.print("press button");
  lcd.setCursor(0,1);
  lcd.print("to see list");
  it = l.end();

}

void loop() {
  if(l.isEmpty()){
    lcd.clear();
    lcd.print("list empty");
    delay(1000);
  } else{
  
    erase_btn_state = digitalRead(erase_btn);
    if(erase_btn_state == HIGH){
      strcpy(*it, to_remove);
      it = l.erase(it);    
      
      lcd.setCursor(0,1);
        lcd.setCursor(0,1);
      delay(1500);
      lcd.clear();
      lcd.setCursor(0,0);
      if(!it.isEnd()) lcd.print(*it);
  
    }
  }

}
