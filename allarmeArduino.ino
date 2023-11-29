int pinAlimentazioneSirena = 13;//pin del mosfet che giostra alimentazione a sirena (la sirena suona con positivo a mancare)
int pinControlloVolumetrici = 7;//pin di lettura normalmente chiuso sensore volumetrico

int pinCH[4];//centralina radio esterna 4ch-4funzioni
//pin di controllo radio

int releInputCount = 0;
bool CH1state, CH1oldstate;
bool CH2state, CH2oldstate;
bool CH3state, CH3oldstate;
bool CH4state, CH4oldstate;//per sistema di lettura comandi radio

bool CHstate[4];
bool CHoldstate[4];//comandi radio revisione

//  controllo radio

int keycode;
int encoderInternalCount;
bool panicButton = false;//inizializzazione allarme, sullo stato di disattivato
int internalcode[] = { 0,1,2 };

bool AllarmIsActive = false;

bool AllarmDetected = false;

unsigned long MAXtimeSinceKeyPress = 3000;//dimentica le sequenze radio 
unsigned long timeSinceKeyPress;
unsigned long timeSinceAllarm;
unsigned long MAXTimeSinceAllarm = 60000; //2minuti e mezzo--legislatura italiana impone un limite al tempo di allarme, ricomincia se volumetrio sollecitato

int lastVolumetricLecture = 0;//se allarme attivo e ultima lettura vera, scorre il timer
void setup() {

  Serial.begin(9600);//debug usb
  
 pinCH[3] = 4; // A collegamenti centrlina radio
pinCH[2] = 11; // B
pinCH[1] = 10; // C
pinCH[0] = 12; // D
  // put your setup code here, to run once:
pinMode( pinAlimentazioneSirena, OUTPUT );
pinMode(pinControlloVolumetrici, INPUT);

//pin di controllo radio impostazione stato

pinMode(pinCH[0],INPUT);
pinMode(pinCH[1],INPUT);
pinMode(pinCH[2],INPUT);
pinMode(pinCH[3],INPUT);

}

void loop() {

  
  tickRadioModule();//prendiamo gli input
   // interpretiamo gli input attiva disattiva panico
  //sezione controllo stato allarme
  if (AllarmIsActive){
    tickAllarme();
    
  }else{
    digitalWrite(pinAlimentazioneSirena,HIGH); 
  }
    
}

bool inputInCH(int pinIndex){
  bool ret_val;
  bool ch1;
    ch1 =(bool) digitalRead(pinCH[pinIndex]);
  
    CHstate[pinIndex] ^= ch1; 
    ret_val = (CHstate[pinIndex] & !CHoldstate[pinIndex]);
    CHoldstate[pinIndex] ^= CHstate[pinIndex]; 
    CHstate[pinIndex] = CHoldstate[pinIndex];
    if (ret_val) {
        Serial.print(pinIndex);
      }
  return ret_val;
}





void tickRadioModule(){
  for (int i = 0; i<4; i++){
    if (inputInCH(i)){
      keyEncode(i);
      timeSinceKeyPress = millis();
      releInputCount++;
 
            codeDetector();


    }
  }
  //Serial.println("endtickRadioFase");
  if (timeSinceKeyPress > 0){//sorta di sistema codice di attivazione(pin code 4 cifre)
  if (( millis() - timeSinceKeyPress )>MAXtimeSinceKeyPress) {//limite lettura sequenza comandi radio

      timeSinceKeyPress  = 0 ;
      releInputCount = 0;
      keyEncode(4);//sistema da rivedere, forzato su 4//non usato
      keyEncode(4);
      keyEncode(4);
    
  }
  }
}

void codeDetector(){
  bool foo[3];
  for (int offsi = 0; offsi<3; offsi++){
    
      for (int i = 0; i <3 ; i++){
        if (keycode == 3){//leggiamo i canali radio, se pulsante 3 va in panico, forza allarme sirena
          AllarmIsActive = true;
          panicButton = true;
          Serial.println("PANIC DETECTED!");
          keycode = 4;
         // Serial.println(keycode[i]);
        }
        foo[i] = (internalcode[i] == keycode);
      /*  Serial.println(internalcode[i]);
        Serial.print("at Offset: ");
        Serial.print((offsi+i)%3);
        Serial.print("to");
        Serial.print(i);
        Serial.print(" re at ");
        Serial.print(i);
        Serial.print(" with foo : ");
        Serial.println(foo[i]?"true":"false");*/
        
      }
   
    
      if ( keycode == 0){
        if (AllarmIsActive) {//disinseriamo allarme e blocchiamo panico, resettiamo i parametri
          panicButton = false;
          timeSinceAllarm = 0;
          AllarmDetected = false;
           
        }
        
           AllarmIsActive = true;//inseriamo sequenza di allarme e controllo volumetrico
      /*
        Serial.print("Allarm is ");
        Serial.println(AllarmIsActive?"true":"false");*/
        keyEncode(4);

        
        digitalWrite(pinAlimentazioneSirena,LOW);//segnalazione uditiva ricezione comando
        delay(500);
        digitalWrite(pinAlimentazioneSirena,HIGH);
        
      }

          
      if ( keycode == 1){//anche qui resettiamo i parametri
        if (AllarmIsActive) {
          panicButton = false;
          timeSinceAllarm = 0;
          AllarmDetected = false;
           
        }
        
        AllarmIsActive = false;//disinseriamo ciclo di controllo volumetrico e allarme//ALLARME DISINSERITO
      
        keyEncode(4);

        
        digitalWrite(pinAlimentazioneSirena,LOW);
        delay(500);
        digitalWrite(pinAlimentazioneSirena,HIGH);
        delay(500);
        digitalWrite(pinAlimentazioneSirena,LOW);
        delay(500);
        digitalWrite(pinAlimentazioneSirena,HIGH);
        
      }
  }
}


void keyEncode(int CH){ // internal use only
 /* Serial.println( "old value");
  Serial.println(keycode[0]);
   Serial.println(keycode[1]);
   Serial.println(keycode[2]);
  
  Serial.print("keyEcode internal count : ");
  Serial.println(encoderInternalCount);*/
  keycode = CH;
  //encoderInternalCount++;
  //encoderInternalCount = (encoderInternalCount > 2)? 0 : encoderInternalCount;
/*  Serial.println(encoderInternalCount);
  
  Serial.println( "new one");
   Serial.println(keycode[0]);
   Serial.println(keycode[1]);
   Serial.println(keycode[2]);*/
}
void tickAllarme(){
  tickVolumetrico();//lettura effettiva volumetrico
  if ((AllarmDetected == true) | (panicButton)){//togliamo alimentazione a sirena, sirena suona
    digitalWrite(pinAlimentazioneSirena,LOW);
    if (timeSinceAllarm == 0){
      timeSinceAllarm = millis();
    }//iniziamo timer tempo di suono sirena
    if ( ( millis() - timeSinceAllarm ) >= MAXTimeSinceAllarm ){//resettiamo lo stato di allarme, lasciando inalterato il suo stato
      AllarmDetected = false;
      panicButton = false;
      timeSinceAllarm = 0;
      digitalWrite(pinAlimentazioneSirena,HIGH);
    }
    return;
  }
  
  digitalWrite(pinAlimentazioneSirena,HIGH);
}

void tickVolumetrico(){
  lastVolumetricLecture = digitalRead(pinControlloVolumetrici);
  if (lastVolumetricLecture == LOW){//il volumetrico toglie continuita se rileva movimento, una lettura high e una lettura sicura
    //se una manomissione hardware avviene il sistema va in allarme
    Serial.println(" volumetrico suscitato ");
    AllarmDetected = true;
    return;
  }
  if (!(AllarmDetected)){//vecchio codice, inutile
  AllarmDetected = false;
  }
  
}
