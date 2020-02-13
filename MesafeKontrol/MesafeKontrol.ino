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

int genelHiz = 120;
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

  analogWrite(sagEn, genelHiz);
  analogWrite(solEn, genelHiz);

}

void loop() {
  zaman = millis();
  if (mesafeO("On: ", echoOn2, trigOn2) < 20 || mesafeO("OnSag: ", echoOn3, trigOn3) < 7 || mesafeO("OnSol: ", echoOn1, trigOn1) < 7) {
    if (mesafeO("SolOn: ", echoOn1, trigOn1) > 28 && sollamaDurum) {
      Serial.println("+++Sollama Basi+++  ");

      solKontrolTur(LOW, HIGH, 5);
      sagKontrolTur(HIGH, LOW, 3);
      CiftTekerTur(HIGH, LOW, HIGH, LOW, 10);
      solKontrolTur(HIGH, LOW, 2);
      sagKontrolTur(LOW, HIGH, 2);
      Serial.println("SSSTOPPP");
      digitalWrite(motorSol1, LOW);
      digitalWrite(motorSol2, LOW);
      digitalWrite(motorSag1, LOW);
      digitalWrite(motorSag2, LOW);
      while (1);
    }
    else {
      Serial.println("Etraf DOLU -STOP-");
      digitalWrite(motorSol1, LOW);
      digitalWrite(motorSol2, LOW);
      digitalWrite(motorSag1, LOW);
      digitalWrite(motorSag2, LOW);
    }
  }
  else {
    sollamaDurum = true;
    solTekerSayac = 0;
    sagTekerSayac = 0;
    Serial.println("++Ileri++");
    digitalWrite(motorSol1, HIGH);
    digitalWrite(motorSol2, LOW);
    digitalWrite(motorSag1, HIGH);
    digitalWrite(motorSag2, LOW);

  }
}

void sagKontrolTur(bool sag1, bool sag2, int tur) {
  StopMillis(500);
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
  StopMillis(500);
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
  StopMillis(500);
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
