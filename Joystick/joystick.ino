int pinX = A0; // Vry
int pinY = A1; // Vrx
int buttonPin = 2;

int posX = 0;
int posY = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600); 
  
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
    posX = analogRead(pinX);
    posY = analogRead(pinY);
    buttonState = digitalRead(buttonPin);
    
    Serial.print("X");    
    if(posX<1000)Serial.print("0");
    if(posX<100)Serial.print("0");
    if(posX<10)Serial.print("0");
    Serial.print(posX);
    
    Serial.print("B");
    if(buttonState == LOW)Serial.print("1");
    else Serial.print("0");
    
    Serial.print("Y");    
    if(posY<1000)Serial.print("0");
    if(posY<100)Serial.print("0");
    if(posY<10)Serial.print("0");
    Serial.print(posY);
   
    Serial.print("\n");
    Serial.flush();
    delay(15);
}
