
//TODO replace functionality of the serial with the Webpage



#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include<shopping_list.h>


// PINS
const byte interruptPin = 3;
int erase_btn_state = 0;

const int rs = 2, en = 6, d4 = 5, d5 = 4, d6 = 8, d7 = 7;

constexpr uint8_t RST_PIN = 9;
constexpr uint8_t SS_PIN = 10;

//INITIALIZATIONS
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
MFRC522 mfrc522(SS_PIN, RST_PIN);

//RFID SPECIFIC
byte item1_uid[4] = {0x23, 0x7a, 0x4f, 0x1b};  
byte item2_uid[4]= {0x09, 0xcb, 0x8b, 0x63};

char cur_item[10];
byte readCard[4]; 
bool success;

//ITEM SPECIFIC
bool is_item1_purchased = false;
bool is_item2_purchased = false;

int total_cost = 0;
int cur_item_cost;


list_items l("item_1,item_2,item_3,item_4");
list_items::iterator it;

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

/*
* get the current id read from the rfid card
*
*/

bool getID() {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { 
      return false;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {  
      return false;
    }
    for ( uint8_t i = 0; i < 4; i++) { 
      readCard[i] = mfrc522.uid.uidByte[i];
    }
    Serial.println("");

    mfrc522.PICC_HaltA();

    return true;
}
/*
* sees if the uid in the readCard byte array matches with the 
* uid of any of the items 
* 
* return: the cost of the item if it matches with the uid
*/
int uid_matches()
{
    uint8_t i;

   for(i = 0; i<4; ++i)
   {
    
      if(readCard[i] != item1_uid[i])
        break;
   }
   if(i == 4 && !is_item1_purchased)
   {
      is_item1_purchased = true;
      lcd.clear();
      lcd.print("item_1 purchased");
      delay(500);
      return 10;
   }
      
          
   for(i = 0; i<4; ++i)
   {
      if(readCard[i] != item2_uid[i])
        break;
   }
   if(i == 4 && !is_item2_purchased)
   {
      is_item2_purchased = true;
      lcd.clear();
      lcd.print("item_2 purchased");
      delay(500);
      return 20;
   }
  return 0;
}

void setup() {
	Serial.begin(9600);	
    lcd.begin(16, 2);

	while (!Serial);		

	SPI.begin();	
	mfrc522.PCD_Init();
    lcd.setCursor(0,1);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_handler, RISING);
    lcd.print("press button");
    lcd.setCursor(0,1);
    lcd.print("to see list");
    it = l.end();

	Serial.println("Scan PICC to see UID, SAK, type, and data blocks...");
}

void loop() {

  success = getID();

  if(success){
    if(cur_item_cost = uid_matches()){
      Serial.print(cur_item);
      Serial.println ("  purchased");

      total_cost += cur_item_cost;

      Serial.print("total cost: ");
      Serial.println (total_cost);

      lcd.clear();

      lcd.print("total cost: ");
      lcd.print (total_cost);
    }
    delay(500);
  }
  if(l.isEmpty()){
    lcd.clear();
    lcd.print("list empty");
    delay(1000);
  } else{
//TODO erase button functionality replaced by the rfid  
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
