int apin = 0;
int threshold = 750;

void setup() {
Serial.begin(9600);

}

void loop() {
  
 
   int b;
   b = deploy();
   if (b> threshold) {
     Serial.print(true);
     Serial.print('\n');

   }
   else {
      Serial.print(false);
      Serial.print('\n');
   }  
   delay(300); 
  
  

}

int deploy()
{
    int v;
    v = analogRead(apin);
    return v;
  
  
}
