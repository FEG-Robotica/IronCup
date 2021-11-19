/*******PINOUT DEFINES*********/
// it is not recommended to make changes
// nao e recomendado que se faca alteracoes
// no se recomienda hacer cambios

#include "Estrategia.h"

int achouNoSonar = 0;
int estrategia = 0;
int lado = 0;



void setup() {

  Serial.begin(9600);

  /****************PINOUT CONFIG****************/
  // OUTPUTS
  pinMode(LED, OUTPUT);         // led

  // right motor
  pinMode(pwmR, OUTPUT);        // right motor power
  pinMode(rightMotor1, OUTPUT); // right motor dir.
  pinMode(rightMotor2, OUTPUT); // right motor dir.

  // left motor
  pinMode(pwmL, OUTPUT);        // left motor power
  pinMode(leftMotor1, OUTPUT);  // left motor dir.
  pinMode(leftMotor2, OUTPUT);  // left motor dir.

  // INPUTS: DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  // DIP switch
  pinMode(DIP1, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP2, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP3, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(DIP4, INPUT_PULLUP);  // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // line sensor
  pinMode(lineL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(lineR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // distance sensor
  pinMode(distR, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  pinMode(distL, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR

  // micro-start
  pinMode(microST, INPUT); // DO NOT CHANGE / NAO MUDAR / NO CAMBIAR
  /****************PINOUT CONFIG - END***************/

  /***************INITIAL CONDITIONS*****************/
  digitalWrite(LED, LOW); // LED off / LED desligado / LED apagado
  MotorL(0); // left motor stopped / motor esquerdo parado / motor izquierdo parado
  MotorR(0); // right motor stopped / motor direito parado / motor derecho parado
  /*************INITIAL CONDITIONS - END*************/

  /*Setando interrupção para parar o robo*/
  attachInterrupt(digitalPinToInterrupt(2), pararRobo, FALLING);

  estrategia = readDIP();
  flag = 0;
  lado = 0;
}



void loop() {
  while (digitalRead(microST) == HIGH) {
    switch (estrategia) {
      case 1: 
        // pela direita
        // estrategia do cerqueira
        //flag = 0;
        //lado = 0;
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          estrategiaGirando(lado);
        }
        break;
      case 2:
        //Sonar
        tempo = millis();
        achouNoSonar = 0;
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          sonar(achouNoSonar);
        }
        break;
      case 3: // pela esquerda
        // estrategia do cerqueira
        //flag = 0;
        lado = 1;
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          estrategiaGirando(lado);
        }
        break;
      case 4:
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          arcoEsquerda();
        }
        break;
      case 5:
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          procuraEstrela();
        }
        break;
      case 6:
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          estrategiaGirandoSimples();
        }
        break;
      case 7:
        while (1)
        {
          if (microST == LOW)
          {
            MotorR(0);
            MotorL(0);
            break;
          }
          estrategiaGirando50(lado);
        }
        break;
      case 10:
        testeTempoDeDarReDoSonar();
        break;
      case 11:
        testeprocuraEstrela();
        break;
      case 12:
        //teste arco esquerdo
        testaArcoEsquerda();
        break;
      case 13:
        //teste arco direita
        testaArcoDireita();
        break;
      case 14:
        //teste de ver linha e gira
        testeVerLinhaESai();
        break;
      case 15:
        //estratégia simples do girando
        estrategiaGirandoSimples();
        break;
      case 16:
        MotorR(500);
        MotorL(500);
        break;
      default:
        // Colocar meio de ver que nao foi selecionada uma estrategia
        break;
    }
  }
  estrategia = readDIP();
}

void pararRobo()
{

  MotorR(500);
  MotorL(500);
  estrategia = 16;

}
