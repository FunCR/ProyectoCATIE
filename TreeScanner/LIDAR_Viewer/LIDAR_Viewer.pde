//Tomas de Camino Beck

import processing.serial.*;
Serial port;
String data;
float[] val = new float[2];
float x, y=0;


void setup() {
  println(Serial.list());
  //Seleccionar puerto de la lista
  port = new Serial(this, "/dev/tty.HC-06-DevB", 9600);
  port.bufferUntil('\n');  
  size(600, 600);
  background(0);
}

void draw() {
  //background(255);
  fill(0);
  //text(val[0],10,10);
  pushMatrix();
  translate(width/2, height/2);
  float px = x;
  float py = y;
  x = cos(radians(val[0]))*val[1];
  y = sin(radians(val[0]))*val[1];
  if (dist(x, y, px, py)<60) {
    strokeWeight(1);
    stroke(255, 100);
    line(px/2, py/2, x/2, y/2);
    stroke(255, 0, 0, 30);
  } else {
    stroke(0, 10);
  }
  strokeWeight(3);
  point(x/2, y/2);

  popMatrix();
}

void serialEvent(Serial port) {
  data = port.readString();
  String[] list = split(data, ',');
  val[0]=float(list[0]);
  val[1]=float(list[1]);
}
