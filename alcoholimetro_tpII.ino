#include <dht.h>
#include <CayenneSerial.h>

dht DHT;

#define DHT11_PIN 5

char token[] = "6zujnxwlm3";

int adc_MQ;
float voltaje;
float Rs;
double alcohol;
int ok;

void setup()
{
  Cayenne.begin(token,9600);
  pinMode(A0, INPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  Cayenne.run();
}

// This function is called when the Cayenne widget requests data for the Virtual Pin.
//FUncion que calcula la conectracion de alcohol en sangre
CAYENNE_OUT(V0)
{
  if (ok==1){
    adc_MQ = analogRead(A0); //Leemos la salida analógica del MQ
    voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
    Rs=1000*((5-voltaje)/voltaje);  //Calculamos Rs con un RL de 1k
    alcohol=0.4091*pow(Rs/5463, -1.497); // calculamos la concentración  de mg alcohol/L en aire
    alcohol=alcohol*0.021; //adaptamos para calcular la proporcion de g alcohol/L sangre
    
    Cayenne.virtualWrite(V0, alcohol);
  } else {
    Cayenne.virtualWrite(V0, 0);
  }
}

//Salida de concentracion de alcohol a widget grafico
CAYENNE_OUT(V1)
{
  if (ok == 1){
    Cayenne.virtualWrite(V1, alcohol);
  } else {
    Cayenne.virtualWrite(V1, 0);
  }
}

//Salida de valores en infracion
CAYENNE_OUT(V2)
{ 
  if (ok == 1 and alcohol>0.5){
      Cayenne.virtualWrite(V2, alcohol);
      digitalWrite(4, HIGH); 
  } else {
    digitalWrite(4, LOW); 
    Cayenne.virtualWrite(V2, 0);
  } 
}

//Salida de gráfico de infraccion
CAYENNE_OUT(V3)
{
  if (ok == 1 and alcohol>0.5){
    Cayenne.virtualWrite(V3, alcohol);
  } else {
    Cayenne.virtualWrite(V3, 0);
  }
}

//Salida de humedad
CAYENNE_OUT(V4)
{
    if (ok == 1){
      int chk = DHT.read11(DHT11_PIN);
      Cayenne.virtualWrite(V4, DHT.humidity);
    } else {
      Cayenne.virtualWrite(V4, 0);
  }
}

//Salida de temperatura
CAYENNE_OUT(V5)
{
  if (ok == 1){
    int chk = DHT.read11(DHT11_PIN);
    Cayenne.virtualWrite(V5, DHT.temperature);
  } else {
    Cayenne.virtualWrite(V5, 0);
  }
}

//Guardo el valor del widget boton On/off
CAYENNE_IN(V6)
{
    ok = getValue.asInt();  
}
