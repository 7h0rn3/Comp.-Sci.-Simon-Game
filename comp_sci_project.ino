const byte button1Pin = 0;
const byte button2Pin = 1;
const byte button3Pin = 2;
const byte button4Pin = 3;

const byte redPin = 4;
const byte greenPin = 5;
const byte bluePin = 6;
const byte led1Pin = 7;
const byte led2Pin = 8;
const byte led3Pin = 9;
const byte led4Pin = 10;

typedef struct{
  byte r;
  byte g;
  byte b;
} RGB;

const RGB red = {255,0,0};
const RGB green = {0,255,0};
const RGB blue = {0,0,255};
const RGB yellow = {255,255,0};

const RGB buttonColors[4] = {red, green, yellow, blue};

const byte ledPins[4] = {7,8,9,10};
byte ledIndex = 0;
byte ledLastIndex;

int millisDelay = 500;
unsigned long millisNow;
unsigned long millisLoop = 0;
unsigned long buttonReset = 0;
unsigned long loops = 0;

byte buttonIndex;
bool buttonPressed = false;
int difficulty[4] = {200, 500, 300, 400}; //red, green, yellow, blue
bool gameMode = false;
byte randIndex;
byte gameIndex = 0;
char gameSeq[33];

void setup() {
  Serial.begin(9600);
  pinMode(button1Pin, OUTPUT); //this should be INPUT or INPUT_PULLUP...
  pinMode(button2Pin, OUTPUT);
  pinMode(button3Pin, OUTPUT);
  pinMode(button4Pin, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  
  digitalWrite(led1Pin,HIGH);
  digitalWrite(led2Pin,HIGH);
  digitalWrite(led3Pin,HIGH);
  digitalWrite(led4Pin,HIGH);

  randomSeed(analogRead(A0));

  //to avoid stutter on start (or skipping)
  if (millis() < millisDelay) {
    delay(millisDelay);
  }
}
 
void color(RGB rgb) {
  analogWrite(redPin, rgb.r);
  analogWrite(greenPin, rgb.g);
  analogWrite(bluePin, rgb.b);
}

void pinwheel() {
  if (millis() - millisDelay >= millisLoop) {
    millisLoop = millis();
    digitalWrite(ledPins[ledLastIndex], HIGH);
    color(buttonColors[ledIndex]);
    digitalWrite(ledPins[ledIndex], LOW);
    ledLastIndex = ledIndex;
    ledIndex = ((ledIndex + 1) % 4); // 0->1, 1->2, 2->3, 3->0
    loops++;
  }

  start();
}

void gameGen() {
  randIndex = random(0,4);
  gameSeq[gameIndex] = randIndex;

  for (int i=0; i<(gameIndex+1); i++) {
    color(buttonColors[gameSeq[i]]);
    digitalWrite(ledPins[gameSeq[i]], LOW);
    delay(millisDelay);
    digitalWrite(ledPins[gameSeq[i]], HIGH);  
    delay(millisDelay/2);
  }
  //delay(millisDelay);
  gamePlay(0);
  if (gameIndex < sizeof(gameSeq)) {
    gameIndex++;
  }
}

void gameOver() {
  color(buttonColors[0]);
  for (int i = 0; i <= 3; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  delay(5000);
  gameMode = false;
}

void gamePlay(int playSeq) {
  millisLoop = millis();
  while (millisLoop + millisDelay >= millis()) {
    button();
  }
  if (buttonPressed && buttonIndex == gameSeq[playSeq]) {
    if (gameIndex > playSeq) {
      gamePlay(playSeq++);
    }
  } else {
    gameOver();
  }
}

void button() {
  buttonPressed = false;
  for (int i=0; i<=3; i++) {
    if (digitalRead(i) == 1) {
      buttonIndex = i;
      buttonReset = millis();
      buttonPressed = true;
    }
  }
}

void start() {
  button();
  if (buttonPressed && buttonReset + millisDelay >= millis()) {
    color(buttonColors[buttonIndex]);
    for (int i = 0; i <= 3; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(millisDelay);
    for (int i = 0; i <= 3; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    buttonReset = 0;
    millisDelay = difficulty[buttonIndex];
    gameMode = true;
  }
}

void loop() {
  if (!gameMode) {
    pinwheel();
    //start();
  } else {
    gameGen();
  }
  //Serial.println(gameSeq);



}
