//creado por Tomas de Camino Beck

import controlP5.*;
import processing.serial.*;


Serial port;
String data;
float[] val = new float[2];
float x, y=0;

int threshold = 60;
int divide = 2;
float rotate=0;
float size=0.03;

ControlP5 cp5;

ArrayList<PVector> feature = new ArrayList<PVector>();

void setup() {

  cp5 = new ControlP5(this);

  //controles de menu
  cp5.addButton("Resetear")
    .setValue(0)
    .setPosition(10, 10)
    .setColorBackground(color(100, 0, 0))
    .setSize(100, 19)
    ;

  cp5.addSlider("threshold")
    .setPosition(10, 50)
    .setColorForeground(color(180, 0, 0))
    .setColorBackground(color(100, 0, 0))
    .setColorActive(color(255, 0, 0))
    .setRange(0, 200)
    ;

  cp5.addSlider("divide")
    .setPosition(10, 65)
    .setColorForeground(color(180, 0, 0))
    .setColorBackground(color(100, 0, 0))
    .setColorActive(color(255, 0, 0))
    .setRange(1, 8)
    ;

  cp5.addSlider("rotate")
    .setPosition(10, 80)
    .setColorForeground(color(180, 0, 0))
    .setColorBackground(color(100, 0, 0))
    .setColorActive(color(255, 0, 0))
    .setRange(0, 2*PI)
    ;

  cp5.addSlider("size")
    .setPosition(10, 95)
    .setColorForeground(color(180, 0, 0))
    .setColorBackground(color(100, 0, 0))
    .setColorActive(color(255, 0, 0))
    .setRange(0.001, 0.5)
    ;

  //fin controles de menu


  println(Serial.list());
  //Seleccionar puerto de la lista
  port = new Serial(this, "/dev/tty.HC-06-DevB", 9600);
  port.bufferUntil('\n');  
  size(600, 600);
  background(0);
}

void draw() {
  //background(0);
  fill(0);
  //text(val[0],10,10);
  pushMatrix();
  translate(width/2, height/2);
  rotate(rotate);
  float px = x;
  float py = y;
  x = cos(radians(val[0]))*val[1];
  y = sin(radians(val[0]))*val[1];

  stroke(255, 0, 0, 5);
  //strokeWeight(3);
  if (x!=0) {
    searchFeature(x, y, threshold);
  }
  drawPoints();
  popMatrix();
  if (mousePressed) {
    saveFrame("frames/####.jpg");
  }
}

void serialEvent(Serial port) {
  data = port.readString();
  String[] list = split(data, ',');
  val[0]=float(list[0]);
  val[1]=float(list[1]);
}

public void Resetear(int theValue) {
  println("reset");
  background(0);
}

public int searchFeature(float x, float y, float th) {
  //busqueda por distancia
  int i=0;
  PVector f;
  boolean found = false;
  while (i<feature.size() && !found) {
    f = feature.get(i);
    float d =dist(x, y, f.x, f.y);
    if (d<th ) {
      f.x=(f.x+x)/2;
      f.y=(f.y+y)/2;
      f.z=f.z+1;
      found=true;
    }
    i++;
  }
  if (!found) {
    feature.add(new PVector(x, y, 0));
  }
  return 0;
}

void drawPoints() {
  PVector f;
  for (int i=0; i<feature.size(); i++) {
    f = feature.get(i);
    if (f.z>0 && f.x!=0) {
      strokeWeight(f.z*size);
      point(f.x/divide, f.y/divide);
    }
  }
}
