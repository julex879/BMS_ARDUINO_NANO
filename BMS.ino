//https://github.com/bitwiseAr/Curso-Arduino-desde-cero/blob/master/Capitulo22/Capitulo22-Programa1.txt
//https://github.com/bitwiseAr/Curso-Arduino-desde-cero/tree/master/Capitulo21
//https://github.com/bitwiseAr/Curso-Arduino-desde-cero/tree/master/Capitulo47
//https://hetpro-store.com/TUTORIALES/arduino-timer/
//https://github.com/n0m1/Sleep_n0m1
//https://github.com/electgpl/Firmware-Electronica/blob/master/ARDUINO/ARDUINO%20Media%20Movil%20como%20Filtro%20Pasa%20Bajos.ino
//https://www.youtube.com/watch?v=Pl79Ni3NUsY
//https://thekurks.net/blog/2018/1/24/guide-to-arduino-sleep-mode
/*comentarios para el siguiente cambio 

leer la corriente 
activar y desactivar el relay 

mas menus 

pitido de buzzer cuando sea necesario 


sleep mode


*/


float alpha =5.10;


#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
//https://www.youtube.com/watch?v=usKaGRzwIMI

#include <Wire.h>      // libreria para bus I2C
#include <Adafruit_GFX.h>    // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64       // reemplaza ocurrencia de ALTO por 64
#define OLED_RESET 4      // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto
float Baterry[6];
bool Estado=0;
const int PinesAnalogicos[6] ={A0,A1,A2,A3,A6};//valores de baterias 


float I=0.00;


int ANTERIOR = 0;    // almacena valor anterior de la variable POSICION
volatile int POSICION = 0; // variable POSICION con valor inicial de 50 y definida
int selector=4;//

float Sensibilidad=0.066;
bool estado=true;


int ventana=3;
/////////////////////////////////////////////////////"Esclavo   "
String MENUS[4]={"Estado    ","Relay     ","Esclavo   ","Sleep mod "};
/*
         oled.print("Estado    "); 
        oled.print("Relay     "); 
        oled.print("Esclavo   "); 
 */








void setup() {
  Definir_Pines();//define pines y pantalla
  Serial.begin(19200);Serial.println("Baterias");// inicia la comunicacion serie e imprime "Bateria"
  Pantalla();//inicializa pantalla y muestra un mensaje en paantalla 
  delay(500);
  digitalWrite(4, LOW);//led del arduino
  selector=4;

}
void loop() {
Lectura_Baterias();
delay(1000);
switch (selector) {
  case 0:

   Analisis_de_Bateria();
    break;
  case 1:
    // RELAY

    digitalWrite(5, estado );//Relay
    Estado_Relay();
    break;
  case 2:
    // Esclavo
    break;
      case 3:
    // Sleep mod


//oled.clearDisplay();
 //    oled.display(); 
    Going_To_Sleep();
    break;
  default:
    MENU();
    digitalWrite(4, LOW);//led del arduino
    Luz_Led(0,0,0);
    break;


  
}
  

  


  
}


      void MENU(){
        oled.clearDisplay();      // limpia pantalla
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(1);      // establece tamano de texto en 1
        oled.print("MENU");  // escribe en pantalla el texto}
        oled.setCursor(0, 16);   
        oled.setTextSize(2);





        if(POSICION<3){
          for(int i=0;i<3;i++){
            oled.print(MENUS[i]); 
           }
            oled.drawCircle(121, 16+6+POSICION*17, 6, WHITE); 
        }
        else{
         

        for(int i=1;i<4;i++){
            oled.print(MENUS[i]); 
           }
           oled.drawCircle(121, 16+6+2*17, 6, WHITE); 
        }




        
        //estado sueño
        //
         //oled.drawCircle(110, 16+6+POSICION*17, 6, WHITE); 

        






        
        oled.display();

        
      }
      void Estado_Del_Sistema(){
       
        Analisis_de_Bateria();//limpia los datos v=RI
      }
      void Estado_Relay(){

        oled.clearDisplay();      // limpia pantalla
        oled.setCursor(0, 16);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(3);      // establece tamano de texto en 1
        oled.print("Relay:");  // escribe en pantalla el texto
        oled.print(estado);  // escribe en pantalla el texto
         oled.display(); 
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
          attachInterrupt(digitalPinToInterrupt(3), Boton_Encoder, RISING);//Preciona el boton
          attachInterrupt(digitalPinToInterrupt(2), encoder, LOW);// interrupcion sobre pin A con
          digitalWrite(5, HIGH );//Relay
          digitalWrite(4, LOW);//Buzzer
          Wire.begin();          // inicializa bus I2C
          oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C   
      }
      
      void Lectura_Baterias(){
        for(int i=0;i<6;i++){ 
              Baterry[i]= (analogRead(PinesAnalogicos[i]) * (5.00/ 1023.00))+0.09;//convierte bites en voltaje para analisar y lo guarda en baterry/////////////////////////////////////////////////////////////////////
        
        
        /*
         adc_raw = analogRead(A0);
         adc_filtrado = (alpha*adc_raw) + ((1-alpha)*adc_filtrado);
         */

                 /*

         Baterry[i] = (alpha*analogRead(A0)) + ((1-alpha)*Baterry[i]);
         */
       
         Baterry[i] = (alpha*Baterry[i]) + ((1-alpha)*Baterry[i]);
        
        }


        
          I=obten_corriente(200); 
        
      }
      
      void Analisis_de_Bateria(){//analiza el voltaje 
        Analisis_Voltajes();//Comprueba el estado critico o bueno de la bateria
        Analisis_voltaje_total();//Suma de todos los voltajes para voltaje total
        Despliega_Voltajes();//Despliega valores en la pantalla oled
      }


      
  
     
      void Analisis_Voltajes(){
        for(int i=0;i<5;i++){ //revisa los voltajes uno por uno 
              if (Baterry[i]> 2.8   &&  Baterry[i] <  4.3){
                //estado bueno de bateria
                digitalWrite(4, LOW);//Buzzer
                Estado=false;
                Luz_Led(0,1,0);
                
              }
              else{
                //estado critico de bateria
                //Desactiva salida 
                //digitalWrite(4, HIGH);//Buzzer
                digitalWrite(5, LOW);//Relay
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
        Baterry[5]=Voltaje_total+0.2;//lo guarda al final del arreglo ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        
        oled.setCursor (65, 16+8);
        oled.print("I : ");    // escribe valor de millis() dividido por 1000
        oled.print(I);    // escribe valor de millis() dividido por 1000
        oled.print("A");    // escribe valor de millis() dividido por 1000
        
        if(Estado==0){//Estado saludable de bateria 
          oled.drawCircle(99, 48, 14, WHITE);  
          oled.drawCircle(99, 48, 13, WHITE); 
          oled.drawLine(89 ,50-3,  95 ,55, WHITE);//oled.drawLine(startX, startY, endX, endY, color)   x=127 y=63 esquina inferior
          oled.drawLine(95 ,55,   110 ,40, WHITE);
        }
        else{//Estado critico de bateria       
          oled.drawTriangle(80,57 ,110, 57    ,95 ,35 ,   WHITE);
          oled.drawLine(95 ,40,95 ,40+8, WHITE);
          oled.drawCircle(95, 48+3, 1, WHITE);  
        
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





      float obten_corriente(int n_muestras)
      {
        float voltajeSensor;
        float V0 = 2.460; //voltaje en OUT cuando la corriente a medir es 0A (varia de dispositivo a dispositivo)
        float corriente=0;
        for(int i=0;i<n_muestras;i++)
        {
         voltajeSensor = analogRead(A7) * (5.0 / 1023.0); //lectura del sensor    
         corriente=corriente+(voltajeSensor-V0)/Sensibilidad; //Ecuación para obtener la corriente
        }
      corriente=corriente/n_muestras;
      if(corriente>=0.70){
        corriente-=0.70;
        
        }

      
      return(corriente);
      }


void Going_To_Sleep(){
    CLKPR=0X80;
    CLKPR=0X04;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    sleep_enable();//Enabling sleep mode
    sleep_cpu();//activating sleep mode}
}



/////////////////////////////////////////interrupciones////////////////////////////////
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
      
          POSICION = min(3, max(0, POSICION));  // establece limite inferior de 0 y    //0,1,2,3
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



          if(selector==3){
            CLKPR=0X80;
            CLKPR=0X00;
           sleep_disable();//Disable sleep mode
          }



          if(selector==1){
              estado=!estado;
          }

        
          //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)


          
        
      }






      


      
