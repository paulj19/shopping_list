//TODO replace functionality of the serial with the Webpage



#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include<shopping_list.h>


//LIST GLOBALS

list_items list("item_1,item_2,item_3,item_4");
list_items::iterator print_iterator;
list_items::iterator rfid_iterator;

int total_cost = 0;

char cost_string[16];

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
byte item3_uid[4] = {0x23, 0x7a, 0x4f, 0x1b};  
byte item4_uid[4]= {0x09, 0xcb, 0x8b, 0x63};

char cur_item[10];
byte readCard[4]; 
bool success;







//TODO always have to set print_iterator to begin when rfid item scanned and update
//      the lcd display

void init_prices()
{
  list.set_price(print_iterator, 10);
  print_iterator = list.next(print_iterator);
  list.set_price(print_iterator, 20);
  print_iterator = list.next(print_iterator);
  list.set_price(print_iterator, 30);
  print_iterator = list.next(print_iterator);
  list.set_price(print_iterator, 40);
  print_iterator = list.begin();
}
void init_uids()
{
  list.set_uid(print_iterator, item1_uid);
  print_iterator = list.next(print_iterator);
  list.set_uid(print_iterator, item2_uid);
  print_iterator = list.next(print_iterator);
  list.set_uid(print_iterator, item3_uid);
  print_iterator = list.next(print_iterator);
  list.set_uid(print_iterator, item4_uid);
  print_iterator = list.begin();
}
void interrupt_handler()
{
  if(!print_iterator.isEnd())
  {
    print_lcd_bottom(*print_iterator);
    print_iterator = list.next(print_iterator);
    delay(500);
  }  else{
    print_iterator = list.begin();
    lcd.print(*print_iterator);
    delay(500);
  }
}
    
#if 0
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
#endif


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
bool compare_uid(list_items::iterator it)
{
   for(int i = 0; i<4; ++i)
   {
    Serial.println(readCard[i], HEX);
    Serial.println((it->uid)[i], HEX);
      if(readCard[i] != (it->uid)[i])
        return false;
   }
   return true;
}
/*
* sees if the uid in the readCard byte array matches with the 
* uid of any of the items 
*
* 
* returns true if is present
*/
bool item_in_list()
{
  Serial.println("Item list check");

  for(rfid_iterator = list.begin(); !rfid_iterator.isEnd(); rfid_iterator = list.next(rfid_iterator))
  {
    if(compare_uid(rfid_iterator) == true)
      return true;
  }

  return false;
}
  



/*
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
*/

void print_lcd_top(String str)
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print(str);
}
void print_lcd_bottom(String str)
{
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(str);
}

void setup() {
	Serial.begin(9600);	
  lcd.begin(16, 2);

	while (!Serial);		

	SPI.begin();	
	mfrc522.PCD_Init();

  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt_handler, RISING);
  print_iterator = list.begin();
  init_prices();
  init_uids();

	Serial.println("STARTED RFID");
  sprintf(cost_string, "total cost  %d", total_cost);
  print_lcd_top(cost_string);
  print_lcd_bottom("press to scroll");
  
}

void loop() {


  success = getID();

  if(success){
          Serial.println ("new card read");
    if(item_in_list()){
      Serial.println("Item is in list");
      Serial.print(rfid_iterator->name);
      Serial.println ("  purchased");
      Serial.println ("  cost");
Serial.print(rfid_iterator->price);

      total_cost += rfid_iterator->price;
      rfid_iterator = list.erase(rfid_iterator);

      Serial.print("total cost: ");
      Serial.println (total_cost);
      sprintf(cost_string, "total cost  %d", total_cost);
      print_lcd_top(cost_string);
      

      print_iterator = list.begin();
      print_lcd_bottom("press to scroll");


    }
    delay(500);
  }

  if(list.isEmpty()){
    print_lcd_bottom("list empty");
    delay(1000);
    return;
  } 

#if 0
  if(l.isEmpty()){
    lcd.clear();
    lcd.print("list empty");
    delay(1000);
    return;
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
#endif
}
