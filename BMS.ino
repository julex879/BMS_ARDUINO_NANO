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
void setup() {
  Definir_Pines();
  Serial.begin(19200);Serial.println("Baterias");
  Pantalla();
  delay(1000);
}
void loop() {
  Lectura_Baterias();//Lee todos los puertos analogicos para las bates 
  Analisis_de_Bateria();
  Serial.println("  ");
  delay(500);


  
}
      void Lectura_Baterias(){
        for(int i=0;i<6;i++){ 
              Baterry[i]= (analogRead(PinesAnalogicos[i]) * (5.00/ 1023.00));   
        }
      }
      void Imprime_Voltaje(){
        for(int i=0;i<5;i++){ 
          Serial.print("Bateria: ");Serial.print(i+1);Serial.print("  ");Serial.print(Baterry[i]);Serial.println(" V");
        }
        Serial.print("Voltaje total:");Serial.print(Baterry[5]);Serial.println(" V");
      }
      void Definir_Pines(){
          pinMode(LED_BUILTIN, OUTPUT);//led del arduino
          Wire.begin();          // inicializa bus I2C
          oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C   
      }
      void Analisis_Voltajes(){
        for(int i=0;i<5;i++){ 
              if (Baterry[i]> 2.8   &&  Baterry[i] <  4.25){
                Serial.println("Buen estado de bateria ");
                Estado=false;
                
              }
              else{
                //estado critico 
                //Desactiva salida 
                Serial.println("Mal estado de bateria ");
                Estado=true;//uno si es critico
                break;
              }
        
        }
      }
      void Analisis_voltaje_total(){
      float Voltaje_total=0.00;
        for(int i=0;i<5;i++){ 
          Voltaje_total+=Baterry[i];
        }
        Baterry[5]=Voltaje_total;
      }
      void Analisis_de_Bateria(){//analiza el voltaje 
        Analisis_Voltajes();
        Analisis_voltaje_total();
        Despliega_Voltajes();
      }
      void Despliega_Voltajes(){
        oled.clearDisplay();      // limpia pantalla
        oled.setCursor(0, 0);     // ubica cursor en inicio de coordenadas 0,0
        oled.setTextSize(1);      // establece tamano de texto en 1
        oled.print("Voltajes de bateria:");  // escribe en pantalla el texto
        for(int i=0;i<5;i++){ 
          oled.setCursor (7, 16+(i*10));    // ubica cursor en coordenas 10,30
          oled.print("B");    // escribe valor de millis() dividido por 1000
          oled.print(i);    // escribe valor de millis() dividido por 1000
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
