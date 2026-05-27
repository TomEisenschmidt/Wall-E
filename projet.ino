#include <Servo.h>
#include <LiquidCrystal.h>

// =========================
// L298N - MOTEURS
// =========================

// Moteur gauche
const int IN1 = 2;
const int IN2 = 3;
const int ENA = 4;

// Moteur droit
const int IN3 = 5;
const int IN4 = 6;
const int ENB = 7;

// =========================
// CAPTEUR ULTRASON
// =========================

const int PIN_TRIG = 9;
const int PIN_ECHO = 8;

// =========================
// SERVOS
// =========================

// Porte
const int SERVO_PORTE = 11;

// Bras
const int SERVO_BRAS_GAUCHE = 26;
const int SERVO_BRAS_DROIT = 27;

Servo porte;
Servo brasGauche;
Servo brasDroit;

// =========================
// LCD 16x2
// RS, E, D4, D5, D6, D7
// =========================

const int LCD_RS = 30;
const int LCD_E  = 31;
const int LCD_D4 = 32;
const int LCD_D5 = 33;
const int LCD_D6 = 34;
const int LCD_D7 = 35;

LiquidCrystal lcd(
  LCD_RS,
  LCD_E,
  LCD_D4,
  LCD_D5,
  LCD_D6,
  LCD_D7
);

// =========================
// CONFIG
// =========================

const int DISTANCE_OBSTACLE = 20;

const int VITESSE_AVANCE = 160;
const int VITESSE_TOURNER = 120;

// Porte
const int ANGLE_PORTE_FERMEE = 140;
const int ANGLE_PORTE_OUVERTE = 15;

// Bras
const int ANGLE_BRAS_REPOS_G = 20;
const int ANGLE_BRAS_REPOS_D = 160;

const int ANGLE_BRAS_ACTIF_G = 90;
const int ANGLE_BRAS_ACTIF_D = 90;

// =========================
// SERVOS
// =========================

void ouvrirPorte() {
  porte.write(ANGLE_PORTE_OUVERTE);
}

void fermerPorte() {
  porte.write(ANGLE_PORTE_FERMEE);
}

void brasRepos() {

  brasGauche.write(ANGLE_BRAS_REPOS_G);
  brasDroit.write(ANGLE_BRAS_REPOS_D);
}

void brasActifs() {

  brasGauche.write(ANGLE_BRAS_ACTIF_G);
  brasDroit.write(ANGLE_BRAS_ACTIF_D);
}

// =========================
// ULTRASON
// =========================

long mesurerDistanceCM() {

  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_TRIG, LOW);

  long duree = pulseIn(PIN_ECHO, HIGH, 30000);

  if (duree == 0) {
    return -1;
  }

  return duree / 58;
}

// =========================
// LCD
// =========================

void afficherLCD(long distance) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Distance:");

  lcd.setCursor(10, 0);

  if (distance < 0) {
    lcd.print("---");
  } else {
    lcd.print(distance);
    lcd.print("cm");
  }

  lcd.setCursor(0, 1);

  if (distance > 0 && distance <= DISTANCE_OBSTACLE) {
    lcd.print("Obstacle !");
  } else {
    lcd.print("Avance");
  }
}

// =========================
// MOTEURS
// =========================

void moteurGauche(int vitesse, bool avant) {

  vitesse = constrain(vitesse, 0, 255);

  digitalWrite(IN1, avant ? HIGH : LOW);
  digitalWrite(IN2, avant ? LOW : HIGH);

  analogWrite(ENA, vitesse);
}

void moteurDroit(int vitesse, bool avant) {

  vitesse = constrain(vitesse, 0, 255);

  digitalWrite(IN3, avant ? HIGH : LOW);
  digitalWrite(IN4, avant ? LOW : HIGH);

  analogWrite(ENB, vitesse);
}

// =========================
// DEPLACEMENTS
// =========================

void avancer() {

  moteurGauche(VITESSE_AVANCE, true);
  moteurDroit(VITESSE_AVANCE, true);
}

void tournerDroite() {

  moteurGauche(VITESSE_AVANCE, true);
  moteurDroit(VITESSE_TOURNER, false);
}

void stopMoteurs() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =========================
// SETUP
// =========================

void setup() {

  Serial.begin(9600);

  // Moteurs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrason
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Servos
  porte.attach(SERVO_PORTE);
  brasGauche.attach(SERVO_BRAS_GAUCHE);
  brasDroit.attach(SERVO_BRAS_DROIT);

  fermerPorte();
  brasRepos();

  // LCD
  lcd.begin(16, 2);

  lcd.clear();
  lcd.print("Robot Wall-E");

  Serial.println("Robot Wall-E pret");

  delay(1500);
}

// =========================
// LOOP
// =========================

void loop() {

  long distance = mesurerDistanceCM();

  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  afficherLCD(distance);

  // =========================
  // OBSTACLE
  // =========================

  if (distance > 0 && distance <= DISTANCE_OBSTACLE) {

    Serial.println("Obstacle detecte");

    ouvrirPorte();
    brasActifs();

    while (distance > 0 && distance <= DISTANCE_OBSTACLE) {

      tournerDroite();

      delay(100);

      distance = mesurerDistanceCM();

      afficherLCD(distance);

      Serial.print("Nouvelle distance : ");
      Serial.println(distance);
    }

    fermerPorte();
    brasRepos();
  }

  // =========================
  // AVANCE
  // =========================

  else {

    fermerPorte();
    brasRepos();

    avancer();
  }

  delay(50);
}