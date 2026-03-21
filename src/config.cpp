volatile bool gameOverFlag = false;

//Handling buttons
volatile bool isUp = false;
volatile bool isDown = false;
volatile bool isLeft = false;
volatile bool isRight = false;
volatile unsigned long lastInterruptTime[4] = {0};