/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <shopping_list.h>

constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


const int rs = 2, en = 6, d4 = 5, d5 = 4, d6 = 8, d7 = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


byte readCard[4]; 
byte item1_uid[4] = {0x23, 0x7a, 0x4f, 0x1b};  
byte item2_uid[4]= {0x09, 0xcb, 0x8b, 0x63};
bool is_item1_purchased = false;
bool is_item2_purchased = false;
uint8_t success;

//char* item1_uid = {"237a4f1b"};
//char* item2_uid = {"9cb8b63"};
byte readUid[4];
char temp[3];
char cur_item[10];
int total_cost = 0;
int cur_item_cost;



uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
 // memset(readUid, 0, sizeof(readUid));
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    //Serial.print(readCard[i], HEX);
   // sprintf(temp, "%x", readCard[i]);
    
    //Serial.print(temp);
    //strcat(readUid, temp);
  }
  //readUid[7] = '\0';
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop     Serial.println("item1 has been matched"); reading
    //Serial.println(readCard[1], HEX);
  return 1;
}

int uid_matches()
{
  uint8_t i;
  //go through the list one by one
  // if(!strcmp(item1_uid, readUid_)){
  //   strcpy(cur_item, "item1");
//     return 20;
//   }
 //  if(!strcmp(item2_uid, readUid_)){
  //   strcpy(cur_item, "item2");
  //   return 30;
   //}

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
    //Serial.print(readCard[i],HEX);
    //Serial.print(item2_uid[i],HEX);
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
	Serial.begin(9600);		// Initialize serial communications with the PC
  lcd.begin(16, 2);
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	//mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  lcd.setCursor(0,1);
	Serial.println("Scan PICC to see UID, SAK, type, and data blocks...");

lcd.print("started");
  
}

void loop() {
  success = getID();
  if(success){
      //for ( uint8_t i = 0; i < 4; i++) 
        //Serial.print(readCard[i], HEX);
    
    if(cur_item_cost = uid_matches()){
//      String current_item = get_cur_item();
      Serial.print(cur_item);
      Serial.println ("  purchased");
      total_cost += cur_item_cost;
      Serial.print("total cost: ");
      Serial.println (total_cost);
      lcd.clear();
      lcd.print("total cost: ");
      lcd.print (total_cost);
    }
    
    Serial.println ("");
    delay(500);
  }
  /*
	// Look for new cards
  new_card_present();

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
  ;
	}

 
  int largestModulo4Number=blockAddr/4*4;
  int trailerBlock=largestModulo4Number+3;

  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(mfrc522.GetStatusCodeName(status));
  }

  
  byte status_write = mfrc522.MIFARE_Write(blockAddr, toWrite, 16);
  
  if( status_write != MFRC522::STATUS_OK){
    Serial.println(status_write);
    Serial.println("error: write not succeeded");
  }
  else
    Serial.println("Write Succeeded");
 

  byte buffersize = 18;
  byte status_read = mfrc522.MIFARE_Read(blockAddr, toRead, &buffersize);
  String rcvdString = (char *)toRead;
  if( status_read != MFRC522::STATUS_OK){
    Serial.println(status_read);
    return;
  }
  else {
    Serial.println(rcvdString);      
  }
    byte item1_uid[0] = 0x23;
  byte item1_uid[1] = 0x7A;
  byte item1_uid[2] = 0x4F;
  byte item1_uid[3] = 0x1B;
  
  byte item2_uid[0] = 0x9C;
  byte item2_uid[1] = 0xB8;
  byte item2_uid[2] = 0xB6;
  byte item2_uid[3] = 0x63;
  */
   
}
