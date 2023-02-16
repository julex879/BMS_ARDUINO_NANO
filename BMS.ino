#include <Wire.h>      // libreria para bus I2C
#include <Adafruit_GFX.h>    // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64       // reemplaza ocurrencia de ALTO por 64
#define OLED_RESET 4      // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto
float Baterry[6];
bool Estado=0;
const int PinesAnalogicos[6] ={A0,A1,A2,A3,A6};
const int corriente=A7;




int ANTERIOR = 0;    // almacena valor anterior de la variable POSICION
volatile int POSICION = 0; // variable POSICION con valor inicial de 50 y definida
int selector=4;

void setup() {
  Definir_Pines();//define pines y pantalla
  Serial.begin(19200);Serial.println("Baterias");// inicia la comunicacion serie e imprime "Bateria"
  Pantalla();//inicializa pantalla y muestra un mensaje en paantalla 
  delay(1000);
  digitalWrite(4, LOW);//led del arduino
  selector=4;

}
void loop() {
Lectura_Baterias();
switch (selector) {
  case 0:
   Analisis_de_Bateria();
    break;
  case 1:
    // statements
    break;
  case 2:
    // statements
    break;
  default:
    MENU();
    Luz_Led(0,0,0);
    break;


  
}
  
  Serial.println("  ");
  delay(100);


  
}


      void MENU(){
        oled.clearDisplay();      // limpia pantalla
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(1);      // establece tamano de texto en 1
        oled.print("MENU");  // escribe en pantalla el texto}
        oled.setCursor(0, 16);   
        oled.setTextSize(2);
        oled.print("Estado    "); 
        oled.print("Graficos  "); 
        oled.print("Esclavo   "); 

         oled.drawCircle(110, 16+6+POSICION*17, 6, WHITE); 

        






        
        oled.display();

        
      }
      void Estado_Del_Sistema(){
       
        Analisis_de_Bateria();//limpia los datos
      }
      void Definir_Pines(){
          pinMode(LED_BUILTIN, OUTPUT);//led del arduino


          
          
          pinMode(9,  INPUT);// B como entrada 
          pinMode(8, OUTPUT);//Luz Red
          pinMode(7, OUTPUT);//Luz Green
          pinMode(6, OUTPUT);//Luz Blue
          pinMode(5, OUTPUT);//Relay
          pinMode(4, OUTPUT);//Buzzer 
          pinMode(3, INPUT_PULLUP);//Swich Encoder        
          pinMode(2, INPUT);    // A como entrada
          attachInterrupt(digitalPinToInterrupt(3), Boton_Encoder, FALLING);//Preciona el boton
          attachInterrupt(digitalPinToInterrupt(2), encoder, LOW);// interrupcion sobre pin A con
          digitalWrite(5, LOW);//led del arduino
          digitalWrite(4, LOW);//led del arduino
          Wire.begin();          // inicializa bus I2C
          oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C   
      }
      
      void Lectura_Baterias(){
        for(int i=0;i<6;i++){ 
              Baterry[i]= (analogRead(PinesAnalogicos[i]) * (5.00/ 1023.00));//convierte bites en voltaje para analisar y lo guarda en baterry
        }
      }
      
      void Analisis_de_Bateria(){//analiza el voltaje 
        Analisis_Voltajes();//Comprueba el estado critico o bueno de la bateria
        Analisis_voltaje_total();//Suma de todos los voltajes para voltaje total
        Despliega_Voltajes();//Despliega valores en la pantalla oled
      }


      
      void Imprime_Voltaje(){
        for(int i=0;i<5;i++){ 
          Serial.print("Bateria: ");Serial.print(i+1);Serial.print("  ");Serial.print(Baterry[i]);Serial.println(" V");
        }
        Serial.print("Voltaje total:");Serial.print(Baterry[5]);Serial.println(" V");
      }
     
      void Analisis_Voltajes(){
        for(int i=0;i<5;i++){ //revisa los voltajes uno por uno 
              if (Baterry[i]> 2.8   &&  Baterry[i] <  4.25){
                //estado bueno de bateria
                Serial.println("Buen estado de bateria ");
                Estado=false;
                Luz_Led(0,1,0);
                
              }
              else{
                //estado critico de bateria
                //Desactiva salida 
                Serial.println("Mal estado de bateria ");
                Estado=true;//uno si es critico
                Luz_Led(1,0,0);
                break;
              }
        
        }
      }
      void Luz_Led(bool R, bool G,bool B){

      digitalWrite(8, R); 
      digitalWrite(7, G); 
      digitalWrite(6, B); 

        
      }
      void Analisis_voltaje_total(){
      float Voltaje_total=0.00;
        for(int i=0;i<5;i++){ 
          Voltaje_total+=Baterry[i];//sumatoria de todos los voltajes 
        }
        Baterry[5]=Voltaje_total;//lo guarda al final del arreglo 
      }

      void Despliega_Voltajes(){
        oled.clearDisplay();      // limpia pantalla
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(1);      // establece tamano de texto en 1
        oled.print("Voltajes de bateria:");  // escribe en pantalla el texto
        for(int i=0;i<5;i++){ 
          oled.setCursor (7, 16+(i*10));    // ubica cursor en coordenas 10,30
          oled.print("B");    // escribe valor de millis() dividido por 1000
          oled.print(i+1);    // escribe valor de millis() dividido por 1000
          oled.print(": ");    // escribe valor de millis() dividido por 1000
          oled.print(Baterry[i]);    // escribe valor de millis() dividido por 1000
          oled.print("V");    // escribe valor de millis() dividido por 1000
        }
        
        oled.setCursor (65, 16);    // ubica cursor en coordenas 10,30
        oled.print("BT: ");    // escribe valor de millis() dividido por 1000
        oled.print(Baterry[5]);    // escribe valor de millis() dividido por 1000
        oled.print("V");    // escribe valor de millis() dividido por 1000
        if(Estado==0){//Estado saludable de bateria 
          oled.drawCircle(95, 45, 15, WHITE);  
          oled.drawCircle(95, 45, 16, WHITE); 
          oled.drawLine(95-10 ,50,  95 ,40+15, WHITE);//oled.drawLine(startX, startY, endX, endY, color)   x=127 y=63 esquina inferior
          oled.drawLine(95 ,40+15,   110 ,30+2, WHITE);
        }
        else{//Estado critico de bateria       
          oled.drawTriangle(80,57 ,110, 57    ,95 ,35 ,   WHITE);
          oled.drawLine(95 ,40,95 ,40+8, WHITE);
          oled.drawCircle(95, 48+3, 1, WHITE);  
          Funcion_Blink(4);//alerta 
        }
         oled.display();     // muestra en pantalla todo lo establecido anteriormente
      }
      void Pantalla(){
        oled.clearDisplay();      // limpia pantalla
        oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(1);      // establece tamano de texto en 1
        oled.print("Hola, Julex:");  // escribe en pantalla el texto
        oled.setCursor (30, 30);    // ubica cursor en coordenas 10,30
        oled.setTextSize(2);      // establece tamano de texto en 2
        oled.print("BMS JX");      // escribe texto
        oled.display();     // muestra en pantalla todo lo establecido anteriormente
      }
  
      void Funcion_Blink(int pitido){
        for(int i =0;i<pitido;i++){
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(100);                       // wait for a second
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          delay(100);   
        }
      }



      void encoder()  {
        static unsigned long ultimaInterrupcion = 0;  // variable static con ultimo valor de
                  // tiempo de interrupcion
        unsigned long tiempoInterrupcion = millis();  // variable almacena valor de func. millis
      
        if (tiempoInterrupcion - ultimaInterrupcion > 5) {  // rutina antirebote desestima
                    // pulsos menores a 5 mseg.
          if (digitalRead(9) == LOW)     // si B es HIGH, sentido horario
          {
            POSICION++ ;        // incrementa POSICION en 1
          }
          else {          // si B es LOW, senti anti horario
            POSICION-- ;        // decrementa POSICION en 1
          }
      
          POSICION = min(2, max(0, POSICION));  // establece limite inferior de 0 y
                  // superior de 100 para POSICION
          ultimaInterrupcion = tiempoInterrupcion;  // guarda valor actualizado del tiempo
        }           // de la interrupcion en variable static
      }


      
      void Boton_Encoder(){

          if(selector==4){
             selector=POSICION;
         
          }
          else{
            selector=4;
          }
             

        
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)


          
        
      }


      
