#include <Servo.h>
Servo servoSol, servoSag;

int trigArka1 = 41;
int echoArka1 = 40;
int trigArka2 = 43;
int echoArka2 = 42;
int trigOn1 = 45;
int echoOn1 = 44;
int trigOn2 = 47;
int echoOn2 = 46;
int trigOn3 = 49;
int echoOn3 = 48;

int motorSag1 = 34;
int motorSag2 = 35;
int motorSol1 = 37;
int motorSol2 = 36;
int sagEn = 9;
int solEn = 8;

int sure;
int mesafe;

unsigned long zaman;
unsigned long eskiZaman = 0;
bool sollamaDurum = true;

int solSen = 50;
int solTekerSayac;
bool solBirKere;
int sagSen = 51;
int sagTekerSayac;
bool sagBirKere;

int genelHiz = 85;

void setup() {
  Serial.begin(9600);
  pinMode(trigArka1, OUTPUT);
  pinMode(echoArka1, INPUT);
  pinMode(trigArka2, OUTPUT);
  pinMode(echoArka2, INPUT);
  pinMode(trigOn1, OUTPUT);
  pinMode(echoOn1, INPUT);
  pinMode(trigOn2, OUTPUT);
  pinMode(echoOn2, INPUT);
  pinMode(trigOn3, OUTPUT);
  pinMode(echoOn3, INPUT);

  pinMode(motorSag1, OUTPUT);
  pinMode(motorSag2, OUTPUT);
  pinMode(motorSol1, OUTPUT);
  pinMode(motorSol2, OUTPUT);
  pinMode(sagEn, OUTPUT);
  pinMode(solEn, OUTPUT);

  pinMode(solSen, INPUT);
  pinMode(sagSen, INPUT);

  servoSol.attach(31);
  servoSag.attach(30);

  analogWrite(sagEn, genelHiz);
  analogWrite(solEn, genelHiz);

}

void loop() {
  servoSol.write(70);
  servoSag.write(15);

  if (mesafeO("Sag Arka: ", echoArka2, trigArka2) <= 25) {

    GeriKontrol();
    digitalWrite(motorSol1, LOW);
    digitalWrite(motorSol2, HIGH);
    digitalWrite(motorSag1, LOW);
    digitalWrite(motorSag2, HIGH);

    if (mesafeO("Sag Arka: ", echoArka2, trigArka2) < 3) {
      Serial.println("3 alti");
      sagKontrolTur(LOW, HIGH, 3);
      GeriKontrol();
      CiftTekerTur(LOW, HIGH, LOW, HIGH, 3);
      GeriKontrol();
      solKontrolTur(LOW, HIGH, 3);
      GeriKontrol();
    }
  }
  else if (mesafeO("Sag Arka: ", echoArka2, trigArka2) > 25) {
    CiftTekerTur(LOW, HIGH, LOW, HIGH, 2);
    GeriKontrol();
    Serial.println("1.KONTROL");
    if (mesafeO("Sag Arka: ", echoArka2, trigArka2) > 25) {
      CiftTekerTur(LOW, HIGH, LOW, HIGH, 3);
      GeriKontrol();
      Serial.println("2.KONTROL");
      if (mesafeO("Sag Arka: ", echoArka2, trigArka2) > 25) {
        CiftTekerTur(LOW, HIGH, LOW, HIGH, 3);
        GeriKontrol();
        Serial.println("3.KONTROL");
        if (mesafeO("Sag Arka: ", echoArka2, trigArka2) > 25) {
          GeriKontrol();
          CiftTekerTur(LOW, HIGH, LOW, HIGH, 3);
          GeriKontrol();
          sagKontrolTur(HIGH, LOW, 5);
          GeriKontrol();
          solKontrolTur(LOW, HIGH, 6);
          GeriKontrol();
          CiftTekerTur(LOW, HIGH, LOW, HIGH, 5);
          GeriKontrol();
          Serial.println("PARK TAMAM .D");
          StopMillis(1);
          while (1);
        }
      }
    }


  }
}

void GeriKontrol() {
  if (mesafeO("Arka Sol: ", echoArka1, trigArka1) < 10) {
    StopMillis(1);
    Serial.println("ARKADA CISIM ALGILANDI !!!");
    while (1);
  }
}

void sagKontrolTur(bool sag1, bool sag2, int tur) {
  StopMillis(100);
  sagTekerSayac = 0;
  while (1) {
    if (digitalRead(sagSen) && sagBirKere) sagTekerSayac++; sagBirKere = false;
    if (!digitalRead(sagSen)) sagBirKere = true;
    Serial.print("SagTekerSayac"); Serial.println(sagTekerSayac);
    digitalWrite(motorSol1, LOW);
    digitalWrite(motorSol2, LOW);
    digitalWrite(motorSag1, sag1);
    digitalWrite(motorSag2, sag2);
    if (sagTekerSayac >= tur) {
      sollamaDurum = false;
      Serial.println("---Sollama SON---");
      break;
    }
  }
}

void solKontrolTur(bool sol1, bool sol2, int tur) {
  StopMillis(100);
  solTekerSayac = 0;
  while (1) {
    if (digitalRead(solSen) && solBirKere) solTekerSayac++; solBirKere = false;
    if (!digitalRead(solSen)) solBirKere = true;
    Serial.print("SolTekerSayac"); Serial.println(solTekerSayac);
    digitalWrite(motorSol1, sol1);
    digitalWrite(motorSol2, sol2);
    digitalWrite(motorSag1, LOW);
    digitalWrite(motorSag2, LOW);
    if (solTekerSayac >= tur) {
      sollamaDurum = false;
      Serial.println("---Sollama SON---");
      break;
    }
  }
}

void CiftTekerTur(bool sol1, bool sol2, bool sag1, bool sag2, int tur) {
  StopMillis(300);
  solTekerSayac = 0;
  while (1) {
    if (digitalRead(solSen) && solBirKere) solTekerSayac++; solBirKere = false;
    if (!digitalRead(solSen)) solBirKere = true;
    Serial.print("SolTekerSayac"); Serial.println(solTekerSayac);
    digitalWrite(motorSol1, sol1);
    digitalWrite(motorSol2, sol2);
    digitalWrite(motorSag1, sag1);
    digitalWrite(motorSag2, sag2);
    if (solTekerSayac >= tur) {
      sollamaDurum = false;
      Serial.println("---Cift Son---");
      break;
    }
  }
}

void StopMillis(int durdur) {
  eskiZaman = zaman;
  digitalWrite(motorSol1, LOW);
  digitalWrite(motorSol2, LOW);
  digitalWrite(motorSag1, LOW);
  digitalWrite(motorSag2, LOW);
  while (1) {
    zaman = millis();
    if (zaman - eskiZaman > durdur) {
      break;
    }
  }
}

int mesafeO(String msg, int echo, int trig) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  sure = pulseIn(echo, HIGH) / 2;
  mesafe = sure / 29.1;

  if (!(mesafe > 0 && mesafe < 30)) {
    mesafe = 30;
  }
  return mesafe;
}
