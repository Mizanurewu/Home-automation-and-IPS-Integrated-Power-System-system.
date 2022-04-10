int btnValue, count = 0, upperBound = 99, lowerBound = 80, flag = 0, Delaycount = 0;

#define LATCH 4//RCLK in serial 13
#define CLK 3// 14 in serial
#define DATA 2//11 no in serial

//This is the hex value of each number stored in an array by index num
byte digitOne[10] = {0x6F, 0x09, 0x73, 0x3B, 0x1D, 0x3E, 0x7C, 0x0B, 0x7F, 0x1F};
byte digitTwo[10] = {0x7B, 0x11, 0x67, 0x37, 0x1D, 0x3E, 0x7C, 0x13, 0x7F, 0x1F};

//int displatPin=5;

int relayAc1 = 8;
int relayAc2 = 9 ;
int relayDc = 10;
int acLed = 7;
int inerLed = 6;

void setup()
{
  Serial.begin(9600);
  // pinMode(displatPin, OUTPUT);
  digitalWrite(relayDc, HIGH);
  digitalWrite(relayAc1, HIGH);
  digitalWrite(relayAc2, HIGH);

  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);


  pinMode(relayDc, OUTPUT);
  pinMode(relayAc1, OUTPUT);
  pinMode(relayAc2, OUTPUT);
  pinMode(A3, INPUT);//button input
  pinMode(A4, INPUT);//battery input
}

void loop()
{

  // digitalWrite(displatPin,HIGH);
  int batteryValue = 0, temp = 0;
  for (int i = 0; i < 5; i++)
  {

    batteryValue = analogRead(A4);//battery input
    temp = temp + batteryValue;
    delay(500);

  }
  batteryValue = temp / 5;
  Serial.print("Battery Value: ");
  Serial.println(batteryValue);
  batteryValue = map(batteryValue, 510, 650, 0, 99);
  if (batteryValue > 99)
    batteryValue = 99;
  if (batteryValue < 4)
    batteryValue = 5;

  Serial.print("After map 0 to 100 Battery Value is: ");
  Serial.println(batteryValue);
  Display(batteryValue);


  btnValue = analogRead(A3);
  Serial.print("Button value :");
  Serial.println(btnValue);

  if (btnValue >= 200 && btnValue <= 300)
  {
    count++;
    Display(count);
    Serial.println(count);
    if (count >= 3)
    {
      delay(1000);
      Display(0);
      upperBound = commonFunc();
      count = 0;
      Display(0);
      delay(2000);
      lowerBound = commonFunc();
      Display(0);
      count = 0;

    }

  }
  Serial.println("In loop");
  Serial.print("Upper bound");
  Serial.println(upperBound);
  Serial.print("Lower bound");
  Serial.println(lowerBound);

  if (batteryValue <= lowerBound)
  {
    flag = 0;//Running of current
  }
  if (batteryValue >= upperBound)
  {
    flag = 1;// Running on solar
  }


  if (flag == 0) {
    //Running of current
    Serial.println("Running of current");

    digitalWrite(inerLed, LOW);
    digitalWrite(acLed, HIGH);
    digitalWrite(relayDc, HIGH);
    delay(500);
    digitalWrite(relayAc1, HIGH);
    digitalWrite(relayAc2, HIGH);
    // delay(1000);
    Serial.println("*******");


  }

  if (flag == 1) {
    //Running on solar
    Serial.println("Running of solar");
    digitalWrite(inerLed, HIGH);
    digitalWrite(acLed, LOW);
    digitalWrite(relayAc1, LOW);
    digitalWrite(relayAc2, LOW);
    delay(500);
    digitalWrite(relayDc, LOW);
    Delaycount++;


    //delay(1000);
    Serial.println("*******");

  }
  if (Delaycount == 5268) //6 hour 5268
  {
    Delaycount=1800;//30 min
    while (Delaycount != 0) {

      //Running of current
    Serial.println("Running of current");

    digitalWrite(inerLed, LOW);
    digitalWrite(acLed, HIGH);
    digitalWrite(relayDc, HIGH);
    delay(500);
    digitalWrite(relayAc1, HIGH);
    digitalWrite(relayAc2, HIGH);
    // delay(1000);

      Delaycount--;
      Serial.print("Runnin on delay: ");
      Serial.println(Delaycount);
      delay(500);
    }

  }



  Serial.print("Delay count: ");
  Serial.print(Delaycount);
  delay(1000);
}







int commonFunc() {
  int tempCount = 0;

  while (true) {
    delay(100);

    btnValue = analogRead(A3);
    Serial.print("In common: ");
    Serial.println(btnValue);

    if (btnValue >= 450 && btnValue <= 550) {        //check if the pushbutton is pressed
      tempCount++;
      Display(tempCount);
      Serial.println(tempCount);
    }
    if (btnValue >= 750  && btnValue <= 800) {       //check if the pushbutton is pressed
      tempCount--;
      Display(tempCount);
      Serial.println(tempCount);
    }

    if (btnValue >= 200 && btnValue <= 300) {
      return tempCount;
      break;

    }


  }
}

void Display(int value) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, ~digitTwo[value / 10]); // digitTwo
  shiftOut(DATA, CLK, MSBFIRST, ~digitOne[value % 10]); // digitOne
  digitalWrite(LATCH, HIGH);
  delay(100);


}
