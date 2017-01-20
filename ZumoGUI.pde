import processing.serial.*;
import controlP5.*;

Serial myPort;  // Create object from Serial class

ControlP5 cp5;

int myColor = color(255);
int c1,c2;
String val;
float n,n1;

//ControlWindow controlWindow;

void setup() {
  size(700, 400);
  String portName = "COM4";
  myPort = new Serial(this, portName, 9600);
  
  cp5 = new ControlP5(this);
     
  cp5.addButton("Forward")
    .setValue(0)
    .setPosition(335,20)
    .setSize(75, 50)
    ;
  
  cp5.addButton("Left")
    .setValue(0)
    .setPosition(245,100)
    .setSize(70,50)
    ;
  
  cp5.addButton("Back")
    .setValue(0)
    .setPosition(335,180)
    .setSize(70,50)
    ;
   
  cp5.addButton("Right")
    .setValue(0)
    .setPosition(425,100)
    .setSize(70,50)
    ;
   
  cp5.addButton("Room")
    .setValue(0)
    .setPosition(100,75)
    .setSize(100,50)
    ;
   
  cp5.addButton("Complete")
    .setValue(0)
    .setPosition(100,130)
    .setSize(100,50)
    ;
   
  cp5.addButton("Stop")
    .setValue(0)
    .setPosition(335,100)
    .setSize(70,50)
    ;
    
  cp5.addButton("Update")
    .setValue(0)
    .setPosition(10,250)
    .setSize(30,130)
    ;
    
  cp5.addButton("Clear")
    .setValue(0)
    .setPosition(660,250)
    .setSize(30,130)
    ;
   
  cp5.addTextfield("Serial")
    .setPosition(50,250)
    .setSize(600,130)
    .setFont(createFont("arial",16))
    .setAutoClear(false)
    ;
}


void draw() {
  background(myColor);
  myColor = lerpColor(c1,c2,n);
  n += (1-n)* 0.1; 
}

public void controlEvent(ControlEvent theEvent) {
  println(theEvent.getController().getName());
  n = 0;
}


public void Left() {
  myPort.write('a'); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Right() {
  myPort.write('d'); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  }
  if(val != null) {
    cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Forward() {
  myPort.write('w'); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Back() {
  myPort.write('s'); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Room() {
  myPort.write('r');
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Stop() {
  myPort.write(' '); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Complete() {
  myPort.write('c'); 
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Update() {
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").setText(val);
  }
}

public void Clear() {
  if ( myPort.available() > 0) {
    val = myPort.readStringUntil('#');
  } 
  if(val != null) {
   cp5.get(Textfield.class, "Serial").clear();
  }
}


void keyPressed() {
  if(key == 'w') {
    Forward();
  }
  if(key == 'a') {
    Left(); 
  }
  if(key == 's') {
    Back();
  }
  if(key == 'd') {
    Right(); 
  }
  if(key == ' ') {
    Stop();
  }
  if(key == 'c') {
    Complete();
  }
  if(key == 'r') {
    Room();
  }
  if(key == 'u') {
    Update();
  }
}