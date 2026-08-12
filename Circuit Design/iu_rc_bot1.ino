const int trigPin = 9;
const int echoPin = 10;

const int redLed = 12;
const int buzzer = 13;

// L298N motor driver
const int ENA = 5;
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 7;
const int ENB = 6;

const int motorSpeed = 180;
const int stopDistance = 60;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A0));

  // LED and buzzer OFF initially
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);

  stopMotors();
}

long readDistanceCM() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 400;
  }

  long distance = duration * 0.034 / 2;

  return distance;
}

// ---------------- FORWARD ----------------
void moveForward() {

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// ---------------- BACKWARD ----------------
void moveBackward() {

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ---------------- LEFT ----------------
void turnLeft() {

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// ---------------- RIGHT ----------------
void turnRight() {

  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ---------------- STOP ----------------
void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------------- MAIN LOOP ----------------
void loop() {

  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // OBSTACLE DETECTED
  if (distance <= stopDistance) {

    // LED ON
    digitalWrite(redLed, HIGH);

    // BUZZER ON
    digitalWrite(buzzer, HIGH);

    Serial.println(">> OBSTACLE DETECTED!");

    // Stop
    stopMotors();
    delay(200);

    // Move backward
    Serial.println(">> Moving BACKWARD");

    moveBackward();
    delay(600);

    // Stop
    stopMotors();
    delay(200);

    // Randomly turn left or right
    if (random(0, 2) == 0) {

      Serial.println(">> Turning LEFT");

      turnLeft();
      delay(500);

    } else {

      Serial.println(">> Turning RIGHT");

      turnRight();
      delay(500);
    }

    // Stop after turning
    stopMotors();
    delay(200);

    // LED OFF
    digitalWrite(redLed, LOW);

    // BUZZER OFF
    digitalWrite(buzzer, LOW);
  }

  // PATH CLEAR
  else {

    // LED OFF
    digitalWrite(redLed, LOW);

    // BUZZER OFF
    digitalWrite(buzzer, LOW);

    Serial.println("Path clear - moving FORWARD");

    moveForward();
  }

  delay(50);
}