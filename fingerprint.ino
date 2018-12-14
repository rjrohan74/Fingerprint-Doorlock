#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
void stepper_for();
void stepper_rev();
SoftwareSerial mySerial(2, 3);

int L1 = 6, L2= 5,c=0;
int p1=8,p2=9,p3=10,p4=11,bu=4;
const int threshold = 100;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(bu,OUTPUT);
  digitalWrite(7,HIGH);
  while (!Serial);
  digitalWrite(L2, HIGH);
  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  
  
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  getFingerprintID();
  digitalWrite(L2, HIGH);
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    c=0;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
    c++;
    if(c==3)
    {
      digitalWrite(bu,HIGH);
      delay(10000);
      digitalWrite(bu,LOW);
      c=0;
    }
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  
 if(finger.confidence>threshold){
  stepper_for();
  digitalWrite(L1, HIGH);
  delay(5000);
   stepper_rev();
    digitalWrite(L2, LOW);
    delay(12000);
    digitalWrite(L1, LOW);
  }
  
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
//  delay(5000);
  if(finger.confidence>threshold){
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
    stepper_for();
    delay(12000);
    digitalWrite(L1, LOW);
  }
 
  return finger.fingerID;
} 

void stepper_for()
  {
    digitalWrite(p1,HIGH);
    digitalWrite(p2,LOW);
    digitalWrite(p3,LOW);
    digitalWrite(p4,LOW);
    delay(4);
    digitalWrite(p1,LOW);
    digitalWrite(p2,HIGH);
    digitalWrite(p3,LOW);
    digitalWrite(p4,LOW);
    delay(4);
    digitalWrite(p1,LOW);
    digitalWrite(p2,LOW);
    digitalWrite(p3,HIGH);
    digitalWrite(p4,LOW);
    delay(4);
    digitalWrite(p1,LOW);
    digitalWrite(p2,LOW);
    digitalWrite(p3,LOW);
    digitalWrite(p4,HIGH);
    delay(4);
  }
  void stepper_rev()
  {
    digitalWrite(p1,LOW);
    digitalWrite(p2,LOW);
    digitalWrite(p3,LOW);
    digitalWrite(p4,HIGH);
    delay(4);
    digitalWrite(p1,LOW);
    digitalWrite(p2,LOW);
    digitalWrite(p3,HIGH);
    digitalWrite(p4,LOW);
    delay(4);
    digitalWrite(p1,LOW);
    digitalWrite(p2,HIGH);
    digitalWrite(p3,LOW);
    digitalWrite(p4,LOW);
    delay(4);
    digitalWrite(p1,HIGH);
    digitalWrite(p2,LOW);
    digitalWrite(p3,LOW);
    digitalWrite(p4,LOW);
    delay(4);
  }
