// Versão para uma chave dupla (lambic), com geração do tom em speaker e buzzer (opcional) e com
// regulação de velocidade
// Por Pedro Ribeiro CT7ABP
// Coompativel com a maioria das placas Arduino, incluindo UNO e NANO

#define SERIALDEBUG

const byte keyditpin = 2; // Ligar a chave no pino correspondente ao ponto
const byte keydahpin = 3; // Ligar a chave no pino correspondente ao traço, o pino comum liga ao
                          // GND do Arduino
const byte ledpin = 13; // Pino para o led da board e para eventual buzzer activo, se ligar um led
                        // externo, não esquecer uma resistência de uns 390ohm
const byte spkpin = 5; // Pino da saida para speaker, ligar através de uma resistência de ~390ohm
                       // e um condensador de 10uF para evitar danos
const byte speedpin = A0; // Pino que irá ligar ao cursor do potenciometro/trimmer de ajuste da
                          // velocidade
const short cwspeed_min_ms = 1200/5; // Velociadade equivalente a 5 wpm
const short cwspeed_max_ms = 1200/50; // Velociadade equivalente a 50 wpm
const unsigned short cw_tone = 500; // Tom de 500Hz (onda quadrada) no pino definido como spkpin

enum cwsym_t { SYM_DIT, SYM_DAH };

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
  Serial.println("CW KEYER BY CT7ABP Pedro");
#endif
// Configurar o Pin 2 onde vamos ligar a chave de CW, o pino correspondente aos pontos como entrada
// colocar uma resistência de 10k entre o pino e 5V para o manter ao nível logico alto por omissao
 pinMode(keyditpin,INPUT);
// Configurar o Pin 3 onde vamos ligar a chave de CW, o pino correspondente aos traços como entrada
// colocar uma resistência de 10k entre o pino e 5V para o manter ao nível logico alto por omissao
 pinMode(keydahpin,INPUT);

// Configurar o Pin 13 como saida, este está nativamente ligado ao LED do UNO, NANO (e outros) e
// poder� ser opcionalmente usado para ligar o buzzer activo
 pinMode(ledpin,OUTPUT); 
 digitalWrite(ledpin,LOW); // Inicialmente o LED/Buzzer nao est�o activos

// Configurar o Pin 5 como saida a usar pelo autofalante, NAO LIGAR DIRECTO porque fará danos a ambos
// autofalante e Arduino, ligar com uma resistência de 390ohm e um condensador de 10uF com o positivo
// do lado do Arduino (valores a ajustar)
 pinMode(spkpin,OUTPUT); 
 digitalWrite(spkpin,LOW); // Inicialmente speaker está inactivo
}

void ditdah(const cwsym_t dd, const unsigned short dit_ms) {
    digitalWrite(ledpin,HIGH); // LED/buzzer activos
    tone(spkpin,cw_tone);
    if(dd == SYM_DIT)
      delay(dit_ms); // Gera som no speeker durante o tempo de ponto
    else
      delay(dit_ms*3); // Gera som no speeker durante o tempo de traço
    noTone(spkpin);
    digitalWrite(ledpin,LOW); // LED/buzzer inactivos
    delay(dit_ms); // Esperar o tempo de ponto antes de qualquer outro simbolo
}

void loop() {
// true = HIGH significa que o pullup está a condicionar o estado da entrada, false = LOW, se a chave estiver primida
 short potval = analogRead(speedpin);
 unsigned short ditspeed_ms = map(potval,0,(1<<10),cwspeed_min_ms,cwspeed_max_ms);
 bool ditstate = digitalRead(keyditpin);
 bool dahstate = digitalRead(keydahpin);
#ifdef SERIALDEBUG
 Serial.print("DIT = "); 
 Serial.print(ditstate);
 Serial.print(", DAH = "); 
 Serial.print(dahstate);
 Serial.print(", POTVAL = "); 
 Serial.print(potval);
 Serial.print(", DITMS = "); 
 Serial.println(ditspeed_ms);
#endif
 if(dahstate == false) { // traço primido
   ditdah(SYM_DAH,ditspeed_ms);
 }
 if(ditstate == false) { // ponto primido
   ditdah(SYM_DIT,ditspeed_ms);
 }
}
