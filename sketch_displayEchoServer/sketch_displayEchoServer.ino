char incoming = 0;	
char processFlag = false;
 
void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
 
void loop()
{
  incoming = Serial.read();	// receive 1 byte data form receive buffer
 
  if(processFlag)		
  {
    if(incoming < 0)
    {
      Serial.println();
      digitalWrite(13, LOW);
      processFlag = false;
    }
    else
    {
      Serial.print(incoming);	// display read character
    }
  }
  else if(!(incoming < 0))
  {
    digitalWrite(13, HIGH);
    //Serial.println("Characters are coming!!");
    //delay(100);
    //Serial.print(" ");
    Serial.print(incoming);
 
    processFlag = true;
  }
 
  delay(3);
}

