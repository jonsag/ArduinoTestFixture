#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include <config.h>

void setup()
{
  Serial.begin(9600);
}

void intTwo()
{
  interruptTwo = true;
}

void intThree()
{
  interruptThree = true;
}

void checkInput()
{
  if (Serial.available() > 0)
  {
    usbInput = Serial.read();
  }
}

void clearInput()
{
  usbInput = '0';
}

void digitalHighRead()
{
  for (a = 2; a < 20; a++)
  {
    pinMode(a, INPUT);
    if (digitalRead(a) == 0)
    {
      Serial.print("Failure to read HIGH on pin ");
      Serial.println(a);
    }
  }
  Serial.println("Digital read HIGH test complete");
  clearInput();
}

void digitalLowRead()
{
  for (a = 2; a < 20; a++)
  {
    pinMode(a, INPUT);
    if (digitalRead(a) == 1)
    {
      Serial.print("Failure to read LOW on pin ");
      Serial.println(a);
    }
  }
  Serial.println("Digital read LOW test complete");
  clearInput();
}

void digitalHighWrite()
{ // to be read on other arduino
  for (a = 2; a < 20; a++)
  {
    pinMode(a, OUTPUT);
    digitalWrite(a, HIGH);
  }
  clearInput();
}

void digitalLowWrite()
{ // to be read on other arduino
  for (a = 2; a < 20; a++)
  {
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
  }
  clearInput();
}

void pullUpTest()
{
  for (a = 2; a < 20; a++)
  {
    pinMode(a, INPUT_PULLUP);
  }
  clearInput();
}

void analogTest()
{
  for (ain = 14; ain < 22; ain++)
  {
    adc = analogRead(ain);
    if (adc < 95 or adc > 108)
    {
      Serial.print("ADC -5v Ref- out of range on Pin ");
      Serial.println(ain);
    }
  }
  analogReference(EXTERNAL);
  for (ain = 14; ain < 22; ain++)
  {
    adc = analogRead(ain);
    if (adc < 140 or adc > 170)
    {
      Serial.print("ADC -3.3v Ref- out of range on Pin ");
      Serial.println(ain);
    }
  }
  analogReference(INTERNAL);
  for (ain = 14; ain < 22; ain++)
  {
    adc = analogRead(ain);
    if (adc < 450 or adc > 490)
    {
      Serial.print("ADC -1.1v Ref- out of range on Pin ");
      Serial.println(ain);
    }
  }

  Serial.println("Analog read test complete");
  clearInput();
}

void interruptTest()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), intTwo, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), intThree, CHANGE);
  interruptTwo = false;
  interruptThree = false;
  delay(100);
  if (interruptTwo == false)
  {
    Serial.println("Interrupt failed on Pin 2");
  }
  if (interruptThree == false)
  {
    Serial.println("Interrupt failed on Pin 3");
  }
  detachInterrupt(digitalPinToInterrupt(2));
  detachInterrupt(digitalPinToInterrupt(3));

  Serial.println("Interrupt test complete");
  clearInput();
}

void pwmTest()
{
  for (pin = 0; pin < pwmPinCount; pin++)
  {
    pinMode(pwmPins[pin], OUTPUT);
    analogWrite(pwmPins[pin], 127);
  }
  clearInput();
}

void twiOn()
{
  Wire.begin();
  Wire.requestFrom(7, 1);
  twiChar = Wire.read();
  if (twiChar != '7')
  {
    Serial.println("I2C Communication Failure");
  }
  Serial.println("I2C test complete");
  clearInput();
}

void spiOn()
{
  digitalWrite(SS, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8); // divide the clock by 8
  digitalWrite(SS, LOW);
  SPI.transfer(1);
  delayMicroseconds(15);
  spiData = SPI.transfer(0);
  if (spiData != spiReadVal)
  {
    Serial.println("SPI Communication failure");
  }
  digitalWrite(SS, HIGH);
  Serial.println("SPI test complete");
  clearInput();
}

void respond()
{
  Serial.println("test");
  clearInput();
}

void loop()
{

  switch (usbInput)
  {
  case 'a':
    digitalHighRead();
    break;
  case 'b':
    digitalLowRead();
    break;
  case 'c':
    digitalHighWrite();
    break;
  case 'd':
    digitalLowWrite();
    break;
  case 'e':
    pullUpTest();
    break;
  case 'f':
    analogTest();
    break;
  case 'g':
    interruptTest();
    break;
  case 'h':
    pwmTest();
    break;
  case 'i':
    twiOn();
    break;
  case 'j':
    spiOn();
    break;
  case '1':
    respond();
    break;
  default:
    checkInput();
    break;
  }
}
