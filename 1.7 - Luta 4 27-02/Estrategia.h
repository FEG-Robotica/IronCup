// LED
#define LED 6

// left motor
#define pwmL 9
#define leftMotor1 7
#define leftMotor2 8

// right motor
#define pwmR 3
#define rightMotor1 5
#define rightMotor2 4

// DIP switch
#define DIP1 10
#define DIP2 11
#define DIP3 12
#define DIP4 13

// Robocore's line sensor
#define lineL A0
#define lineR A1

// Jsumo's distance sensor
#define distL A2
#define distR A3

// Jsumo's micro-start
#define microST 2
/*******PINOUT DEFINES - END*********/

//Minhas variaveis Globais
int tempo = millis();
int vezesQueFezOSonar = 0;
int tempoAtual;
int flag = 0;
int flagTravamento = 0;
unsigned int tempoPassadoAvancar = 0;
int estrategiaH;
int flagTravamentoG;

/****Painel de Controle****/
int brancoSensorLinha = 630;
int pretoSensorLinha = 800;
//Cerqueira
int tempoParaChegarNoCentroAposOArco = 500;
//Sonar
int tempoParaDarReParaBorda = 200;
int velocidadeParaDarRe = -255;
int tempoDeMovimentoDoSonar = 400;
int velocidadeDoSonar = 200;
//Arco
float relacaoParaArco = 0.6; //Numero pelo qual o valor da velocidade do arco de dentro deve ser multiplicado para fazer um arco
int tempoDeMovimentoDeArco = 1000;
int tempoDeGiroArco = 500;

int tempoParaVirar = 500;
/****Painel de Controle****/

/*******FUNCTIONS*******/
void MotorL(int pwm); // left motor / motor esquerdo / motor izquierdo
void MotorR(int pwm); // right motor / motor direito / motor derecho
int readDIP();        // read DIP switch / ler chave DIP / leer el interruptor DIP
/*******FUNCTIONS - END*******/

void avancar();
void procuraEstrela();

/**LEFT MOTOR CONTROL / CONTROLE DO MOTOR ESQUERDO / CONTROL DEL MOTOR IZQUIERDO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / para frente / seguir adelante
// -255<=pwm<0 -> backward / para tras / seguir espalda
void MotorL(int pwm) {
  // leftMotor1=0 and leftMotor2=0 -> stopped / parado / parado
  // leftMotor1=0 and leftMotor2=1 -> moves forward / avanca / avanzar
  // leftMotor1=1 and leftMotor2=0 -> moves back / recua / retrocede
  // leftMotor1=1 and leftMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)

  if (pwm == 0) { // desliga o motor
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
  }
  else if (pwm == 500) // trava o motor
  {
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, HIGH);
    analogWrite(pwmR, 0);
  }
  else if (pwm < 0)
  {
    analogWrite(pwmL, -pwm);
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
  }
  else if (pwm > 0 && pwm <= 255)
  {
    analogWrite(pwmL, pwm);
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
  }
}

/**RIGHT MOTOR CONTROL / CONTROLE DO MOTOR DIREITO / CONTROL DEL MOTOR DERECHO**/
// pwm = 0 -> stopped / parado / parado
// 0<pwm<=255 -> forward / frente / adelante
// -255<=pwm<0 -> backward / tras / espalda
void MotorR(int pwm)
{
  // rightMotor1=0 and rightMotor2=0 -> stopped / parado / parado
  // rightMotor1=0 and rightMotor2=1 -> moves forward / avanca / avanzar
  // rightMotor1=1 and rightMotor2=0 -> moves back / recua / retrocede
  // rightMotor1=1 and rightMotor2=1 -> stopped (braked) / parado (travado) / parado (frenado)
  if (pwm == 0)
  {
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  }
  else if (pwm == 500)
  {
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, HIGH);
    analogWrite(pwmR, 0);
  }
  else if (pwm < 0)
  {
    analogWrite(pwmR, -pwm);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
  }
  else if (pwm > 0 && pwm <= 255)
  {
    analogWrite(pwmR, pwm);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
  }
}

/** read DIP switch / ler chave DIP / leer el interruptor DIP **/
// returns a value between 0 and 15
// retorna um valor entre 0 e 15
// devuelve un valor entre 0 y 15
int readDIP() {
  int n = 0;
  if (digitalRead(DIP4) == HIGH)
    n = 1;
  if (digitalRead(DIP3) == HIGH)
    n |= (1 << 1);
  if (digitalRead(DIP2) == HIGH)
    n |= (1 << 2);
  if (digitalRead(DIP1) == HIGH)
    n |= (1 << 3);
  return n;
}

int sensoresDist() {

  /*
    frente = 0
    direita = 1
    esquerda = 2
    desligados = 3
  */

  //os dois ligados
  if (digitalRead(distL) == HIGH && digitalRead(distR) == HIGH) {
    return 0;
  }

  //direita ligado
  if (digitalRead(distL) == LOW && digitalRead(distR) == HIGH) {
    return 1;
  }

  //esquerda ligado
  if (digitalRead(distL) == HIGH && digitalRead(distR) == LOW) {
    return 2;
  }

  //os dois desligados
  if (digitalRead(distL) == LOW && digitalRead(distR) == LOW) {
    return 3;
  }
}

int sensoresLinha() {

  /*
    frente = 0
    direita = 1
    esquerda = 2
    desligados = 3
  */

  //os dois ligados
  if (analogRead(lineL) <= brancoSensorLinha && analogRead(lineR) <= brancoSensorLinha) {
    return 0;
  }

  //direita ligado
  if (analogRead(lineL) >= pretoSensorLinha && analogRead(lineR) <= brancoSensorLinha) {
    return 1;
  }

  //esquerda ligado
  if (analogRead(lineL) <= brancoSensorLinha && analogRead(lineR) >= pretoSensorLinha) {
    return 2;
  }

  //os dois desligados
  if (analogRead(lineL) >= pretoSensorLinha && analogRead(lineR) >= pretoSensorLinha) {
    return 3;
  }
}

//Ação para fugir da borda
// CERQUEIRA
void saiBorda(){
  if(digitalRead(microST) == LOW){
    MotorR(0);
    MotorL(0);
  }
  if (sensoresLinha() == 0 && digitalRead(microST) == HIGH) // ve nos dois sensores de linha
  {
    MotorR(150);
    MotorL(-150);
    delay(120); // gira 180 graus anti-horário
  }
  if (sensoresLinha() == 1 && digitalRead(microST) == HIGH) //ve na direita
  {
    MotorR(150);
    MotorL(-150);
    delay(60); // gira 60 graus anti-horário
  }
  if (sensoresLinha() == 2 && digitalRead(microST) == HIGH) //ve na esquerda
  {
    MotorR(-150);
    MotorL(150);
    delay(60); // gira 60 graus horário
  }
  if(readDIP() == 1 && digitalRead(microST) == HIGH){
    MotorR(255);
    MotorL(255);
    delay(100);
  }
}

// -------------------------------------------------Estratégia: Procura girando ----------------------------------------

void estrategiaGirando(int lado){// B4 0001
  
  while (flag == 0) // Fazer um arco na primeira vez que entrar na estratégia
  {
    flag = 1;
    if (lado == 0) { // quando lado = 0 arco pela direita
      MotorR(255);
      MotorL(255 * relacaoParaArco);
      delay(100);
    } else { // quando lado != 0 arco pela esquerda
      MotorR(255 * relacaoParaArco);
      MotorL(255);
      delay(100);
    }
  }
  flag = 1;
  avancar();
  if (sensoresLinha() != 3)
  {
    saiBorda();
  }
}

void estrategiaGirando50(int lado){// B4 0001
  
  while (flag == 0) // Fazer um arco na primeira vez que entrar na estratégia
  {
    flag = 1;
    if (lado == 0) { // quando lado = 0 arco pela direita
      MotorR(255);
      MotorL(255 * relacaoParaArco);
      delay(50);
    } else { // quando lado != 0 arco pela esquerda
      MotorR(255 * relacaoParaArco);
      MotorL(255);
      delay(50);
    }
  }
  flag = 1;
  avancar();
  if (sensoresLinha() != 3)
  {
    saiBorda();
  }
}

void estrategiaGirandoSimples()
{
  avancar();
  if (sensoresLinha() != 3)
  {
    saiBorda();
  }
}

//Procura girando
//CERQUEIRA
void procuraGirando()
{
  if (sensoresLinha() == 3)
  {
    while (sensoresDist() == 3)
    {
      MotorR(150);
      MotorL(-150);
      if (sensoresLinha() != 3)
      {
        while (1) {
          saiBorda();
          if (digitalRead(microST) == LOW){
            MotorR(0);
            MotorL(0);
            break;
          }
          if (sensoresLinha() == 3){
            break;
          }
        }
      }
    }
  }
  if (sensoresDist() != 3) {
    if (digitalRead(microST) == LOW){
      MotorR(0);
      MotorL(0);
    }else{
      avancar();
    }
  }
}

// Função avançar para estratégia de busca girando
//CERQUEIRA
void avancar()
{
  /*
    frente = 0
    direita = 1
    esquerda = 2
    desligados = 3

  if (sensoresDist() == 0)
  {
    if (flagTravamentoG == 0) {
      flagTravamentoG = 1;
      tempoPassadoAvancar = millis();
    }
    if (millis() - tempoPassadoAvancar > 2000) {
      flagTravamentoG = 0;
      MotorR(-255);
      MotorL(-255);
      digitalWrite(LED, HIGH);
      delay(70);
    }
    digitalWrite(LED, LOW);
    MotorR(255);
    MotorL(255);
  }
  */
  if (sensoresDist() == 0){
    MotorR(255);
    MotorL(255);
  }
  if (sensoresDist() == 1)
  {
    MotorR(255 * relacaoParaArco);
    MotorL(255);
  }
  if (sensoresDist() == 2)
  {
    MotorR(255);
    MotorL(255 * relacaoParaArco);
  }
  if (sensoresDist() == 3)
  {
    MotorR(500);
    MotorL(500);
    procuraGirando();
  }
}

// ----------------------------------------------- Estrategia: Sonar ------------------------------------------------------

void sonar(int achouNoSonar) {
  tempoAtual = millis() - tempo;
  //dando Re para Borda
  if (tempoAtual < tempoParaDarReParaBorda) {
    MotorL(velocidadeParaDarRe);
    MotorR(velocidadeParaDarRe);
  } else {
    int comecoSonar = millis();
    while (1) {
      //gira
      if (digitalRead(microST) == LOW){
        MotorR(0);
        MotorL(0);
        break;
      }
      if (millis() - comecoSonar < tempoDeMovimentoDoSonar / 2) {
        MotorL(-velocidadeDoSonar);
        MotorR(velocidadeDoSonar);
        if (sensoresDist() < 3) {
          break;
        }
      }
      //gira
      if (millis() - comecoSonar > tempoDeMovimentoDoSonar / 2) {
        MotorL(velocidadeDoSonar);
        MotorR(-velocidadeDoSonar);
        if (sensoresDist() < 3) {
          break;
        }
      }

      if (millis() - comecoSonar > tempoDeMovimentoDoSonar) {
        comecoSonar = millis();
        if (sensoresDist() < 3) {
          break;
        }
      }
    }
    while(1){
      if (digitalRead(microST) == LOW){
        MotorR(0);
        MotorL(0);
        break;
      }
      if (sensoresDist() < 3) {
        avancar();
      } else {
        break;
      }
    }
  }
}

// ----------------------------------------------- Estrategia: Arco ------------------------------------------------------

void arcoDireita() { //A4 0011
  tempoAtual = millis() - tempo;
  if (sensoresDist() < 3) { //viu o inimigo(3:nao le sensores)
    avancar();
  } else if (tempoAtual < tempoDeMovimentoDeArco) {
    MotorR(200);
    MotorL(200 * relacaoParaArco);
  } else if (tempoAtual < tempoDeGiroArco + tempoDeMovimentoDeArco) {
    MotorR(255);
    MotorL(-255);
  } else {
    procuraEstrela();
  }
}


void arcoEsquerda() {//A2 0100
  tempoAtual = millis() - tempo;
  if (sensoresDist() < 3) {
    avancar();
  } else if (tempoAtual < tempoDeMovimentoDeArco) {
    MotorR(255 * relacaoParaArco);
    MotorL(255);
  } else if (tempoAtual < tempoDeGiroArco + tempoDeMovimentoDeArco) {
    MotorR(-255);
    MotorL(255);
  } else {
    procuraEstrela();
  }
}

void procuraEstrela() {//A2 0101
  tempoAtual = millis() - tempo;
  if (sensoresDist() == 3 && sensoresLinha() == 3) { // nao ve linha e inimigo, vai pra frente
    MotorR(200);
    MotorL(200);
  } else if (sensoresLinha() == 3 && sensoresDist() < 3) { //se ele ve o inimigo e nao ve a linha e ele avanaca para o inimigo
    avancar();
  } else { //se estiver vendo linha sai da linha
    saiBorda();
  }
}

// --------------------------------------------------- TESTES ------------------------------------------------------

void testeTempoDeDarReDoSonar() {
  tempoAtual = millis() - tempo;
  if (tempoAtual < tempoParaDarReParaBorda ) {
    MotorL(velocidadeParaDarRe);
    MotorR(velocidadeParaDarRe);
  } else {
    MotorL(0);
    MotorR(0);
  }
}

void testeVerLinhaESai() {
  MotorL(100);
  MotorR(100);
  if (sensoresLinha() < 3) {
    MotorR(-255);
    MotorL(-255);
    delay(200); // gira 180 graus anti-horário
  }
}

void testeVerLinhaELED() {
  MotorL(100);
  MotorR(100);
  if (sensoresLinha() < 3) {
    MotorR(0);
    MotorL(0);
    digitalWrite(LED, HIGH);
    delay(10000); // gira 180 graus anti-horário
  }
}

void testaArcoEsquerda() {
  MotorR(255 * relacaoParaArco);
  MotorL(255);
}

void testaArcoDireita() {
  MotorR(255);
  MotorL(255 * relacaoParaArco);
}

void testeprocuraEstrela() {//A2 0101
  tempoAtual = millis() - tempo;
  if (sensoresLinha() >= 3) { // nao ve linha e inimigo, vai pra frente
    MotorR(200);
    MotorL(200);
  } else { //se estiver vendo linha sai da linha
    saiBorda();
  }
}
