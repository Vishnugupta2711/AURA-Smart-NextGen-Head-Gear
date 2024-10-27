void setup() 
{
Serial.begin(9600);
pinMode(12, OUTPUT);
digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
}

void loop()
{
  Serial.print(digitalRead(2));
  Serial.print("   ");
  Serial.println(digitalRead(3));
  
 if(digitalRead(2)==0 || digitalRead(3)==1)
 { 
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
 }


 if(digitalRead(2)==1 && digitalRead(3)==0)
 { 
  digitalWrite(12, LOW);   // turn the LED on (HIGH is the voltage level)
 }
}
