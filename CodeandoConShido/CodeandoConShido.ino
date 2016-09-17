
#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>


#define YP A3 
#define XM A2 
#define YM 9  
#define XP 8 

#define TS_MINX 96
#define TS_MINY 90
#define TS_MAXX 850
#define TS_MAXY 853

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int X; // Variables que almacenaran la coordenada
int Y; // X, Y donde presionemos y la variable Z 
int Z; // almacenara la presion realizada

int a = 0; // variable "flag" para control rebotes
#define ledA 21
#define ledK 20



int oldcolor, currentcolor;

void setup() {

  Serial.begin(9600);
  Serial.println(F("Hola!"));
  
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);

  
drawBorder();

    
 
  
  tft.setCursor (30, 35);
  tft.setTextSize (2);
  tft.setTextColor(RED);
  tft.println("Programando con Fede");
  tft.setCursor (50, 55);
  tft.println("Podra Fede lograrlo?");
  tft.setCursor (10, 100);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("OBVIO QUE SI!!A no ser :v");

  tft.setCursor (90, 150);
  tft.setTextSize (1);
  tft.setTextColor(BLACK);
  tft.println("Tocamela para seguir");

   waitOneTouch();

   
  tft.fillScreen(BLACK);

  currentcolor = RED;

  
  pinMode(13, OUTPUT);

  tft.fillRect(140, 170, 160, 60, WHITE); //Dibujamos el boton
  tft.setCursor(170,185);  // Colocamos el cursor
  tft.setTextSize(3);    // Especificamos el tamaño del texto
  tft.setTextColor(RED); // Definimos el color del texto 
  tft.println("Alarma");

}
#define MINPRESSURE 10
#define MAXPRESSURE 1000
void loop() {
  lecturaPanel(); // Realizamos lectura del panel para detectar presion y coordenadas
      
    // Si la pulsación del eje X se produce entre los puntos 40 y 160
    // Y la pulsacion del eje Y se produce entre los puntos 20 y 60
    // Y la presión realizada esta entre el margen determinado
    if((X > 20 && X < 100) && (Y > 110 && Y < 230) && (Z > MINPRESSURE && Z < MAXPRESSURE)) 
    {
      if (a == 0) // Si la variable flag esta en 0
      {
        tft.fillRect(140, 170, 160, 60, RED); // Dibujamos nuestro boton relleno de color
        
        tft.setCursor(170,175); // Colocamos el cursor
        tft.setTextSize(3);   // Especificamos el tamaño del texto
        tft.setTextColor(WHITE); // Definimos el color del texto 
        tft.println("Alarma"); // Escribimos por pantalla
        tft.setCursor(150,200);  // Colocamos el cursor
        tft.println("Activada"); // Escribimos por pantalla   
     
        digitalWrite(ledA, HIGH);
        digitalWrite(ledK, LOW);

        a = 1; // Ponemos la variable flag en 1
        delay(150);
      }
      else if (a == 1) // Si la variable flag esta en 1
      {
         tft.fillRect(141, 171, 158, 58, WHITE); // Dibujamos el fondo de nuestro boton en cyan
         
         tft.setCursor(170,185); // Colocamos el cursor
         tft.setTextSize(3); // Especificamos el tamaño del texto
         tft.setTextColor(RED); // Definimos el color del texto 
         tft.println("Alarma"); // Escribimos por pantalla
         
        digitalWrite(ledA, LOW);
        digitalWrite(ledK, LOW);
         
         a = 0; // Ponemos la variable flag en 0 para evitar los rebotes
         delay(150);
      }
    } 
}

TSPoint waitOneTouch() {

  // wait 1 touch to exit function
  
  TSPoint p;
  
  do {
    p= ts.getPoint(); 
  
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
  
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  
  return p;
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  //tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
  
}

void lecturaPanel()
{
    digitalWrite(13, HIGH); 
    TSPoint p = ts.getPoint(); // Realizamos lectura de las coordenadas
    digitalWrite(13, LOW);
  
    pinMode(XM, OUTPUT); // La librería utiliza estos pines como entrada y salida
    pinMode(YP, OUTPUT); // por lo que es necesario declararlos como salida justo
                         // despues de realizar una lectura de coordenadas.    
  
    // Mapeamos los valores analogicos leidos del panel tactil (0-1023)
    // y los convertimos en valor correspondiente a la medida del LCD 320x240
    X = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
    Y = map(p.y, TS_MAXY, TS_MINY, tft.height(), 0);
    Z = p.z;
}

