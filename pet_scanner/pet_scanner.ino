int PET_radius = 100;
int PET_diameter;
int center_x = 0;
int center_y = 0;

// Tumor location
float theta_tumor;
float radial_tumor;

///////////////////////////////////////////
// Instructor can change these
///////////////////////////////////////////
float x_tumor = -25;
float y_tumor = -60;

int delay_time = 100;
///////////////////////////////////////////

int nleds = 12;
int i = 0;

long MAX_RAND = 1000000;
float rnd;

int led[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

 // initialize the digital pin as an output.

 Serial.write("\nInitializing!\n");

 for (i=0;i<nleds;i++) {
  pinMode(led[i], OUTPUT);
 }

   // Turn them all off
 for (int i=0;i<nleds;i++) {
    Serial.println(i);
    digitalWrite(led[i],LOW);
  }

   PET_diameter = 2*PET_radius;

  // Location of the tumor
  // Uncomment the section below if we want the tumor
  // to be placed at a random location
  /*
  rnd = random(0,MAX_RAND)/float(MAX_RAND);
  //Serial.println(rnd);
  theta_tumor = acos((2*rnd)-1);  
  rnd = random(0,MAX_RAND)/float(MAX_RAND);
  //Serial.println(rnd);
  radial_tumor =((2*PET_radius*rnd)-PET_radius);

  x_tumor = radial_tumor*cos(theta_tumor);
  y_tumor = radial_tumor*sin(theta_tumor);

  //x_tumor = 50;
  //y_tumor = -50;
  */
  
  Serial.println("Location of tumor\n");
  //Serial.println(theta_tumor);
  //Serial.println(radial_tumor);
  Serial.println(x_tumor);
  Serial.println(y_tumor);
  
  Serial.println("Setup finished!");
  
}

//////////////////////////////////////////////////////////////
void loop() {
 // put your main code here, to run repeatedly:
 // the loop routine runs over and over again forever:
  
  // This is to test!
  /*
  for (int i=0;i<nleds;i++) {
        Serial.println(i,DEC);
        digitalWrite(led[i],HIGH);
        delay(500);
  }

  delay(1000); // Wait 1 second
  Serial.println("We made it here!");

  // This is to test!
  for (int i=0;i<nleds;i++) {
    Serial.println(i);
    digitalWrite(led[i],LOW);
  }
  */

  // Generate the gamma rays!!!!!!

  rnd = random(0,MAX_RAND)/float(MAX_RAND);

  float line_theta = acos((2*rnd) - 1);
  float m = tan(line_theta); // slope
  float b = (y_tumor) - (m*x_tumor); // intercept 

  // Intercept points of the gamma rays. 
  float xintercept[2] = {0,0};
  float yintercept[2] = {0,0};

  points_of_intersection(PET_radius,m,b, xintercept, yintercept);

  /*
  Serial.println("------");
  Serial.println("Location of tumor\n");
  Serial.println(theta_tumor);
  Serial.println(radial_tumor);
  Serial.println(x_tumor);
  Serial.println(y_tumor);

  Serial.println("Intercepts ---");
  Serial.println(xintercept[0]);
  Serial.println(yintercept[0]);

  Serial.println(xintercept[1]);
  Serial.println(yintercept[1]);
  */
  
  float r1 = xintercept[0]*xintercept[0] + yintercept[0]*yintercept[0];
  float r2 = xintercept[1]*xintercept[1] + yintercept[1]*yintercept[1];
  
  //Serial.println(r1);
  //Serial.println(r2);

  //Serial.println("------");

  // Which one of the lights do they light up!!!!!!!!!
  float angle_of_detector1 = atan2(yintercept[0],xintercept[0]);
  float angle_of_detector2 = atan2(yintercept[1],xintercept[1]);

  //Serial.println(angle_of_detector1);
  //Serial.println(angle_of_detector2);

  float detector_arc = 2*PI/nleds;

  int led_hit1 =  which_detector(angle_of_detector1, nleds, detector_arc);
  int led_hit2 =  which_detector(angle_of_detector2, nleds, detector_arc);

  /*
  Serial.println("LEDs to turn on");
  Serial.println(led_hit1);
  Serial.println(led_hit2);
  Serial.println(led[led_hit1]);
  Serial.println(led[led_hit2]);
  */

  digitalWrite(led[led_hit1], HIGH);
  digitalWrite(led[led_hit2], HIGH);
    
  delay(delay_time);

  digitalWrite(led[led_hit1], LOW);
  digitalWrite(led[led_hit2], LOW);

  // Turn them all off
  //Serial.println("Turn them all off!");
 for (int i=0;i<nleds;i++) {
    //Serial.println(i);
    digitalWrite(led[i],LOW);
  }
  
 }



////////////////////////

void quadratic(float a, float b, float c, float n[2]) {

  //Serial.println("In quadratic!!!!\n");
  
  float radicand = b*b - (4*a*c);
  //Serial.println(radicand);
  float n1 = -999;
  float n2 = -999;
  if ( radicand < 0 ) {
    n[0] = -999;
    n[1] = -999;
  }
  else {
    n[0] = (-b + sqrt(radicand))/(2*a);
    n[1] = (-b - sqrt(radicand))/(2*a);
  }

  //Serial.println("n1,n2: ",n1,n2)
  //return [n1,n2]

}

void points_of_intersection(float r, float m, float b, float x[2], float y[2]){

  //print("r,m,b: " + r,m,b)
  float A = m*m + 1;
  float B = 2*m*b;
  float C = b*b - r*r;
  //print("A,B,C " + A,B,C)

  float xpoints[2] = {0,0};
  quadratic(A,B,C, xpoints);
  
  if (xpoints[0]==-999 && xpoints[1]==-999) {
    //return [0,0,1,1]
    x[0] = 0;
    y[0] = 0;
    x[1] = 1;
    y[1] = 1;
  }
  x[0] = xpoints[0];
  x[1] = xpoints[1];
  //print(x1,x2)
  y[0] = m*x[0] + b;
  y[1] = m*x[1] + b;
  
  //print("x1,y1,x2,y2: ",x1,y1,x2,y2)

  //return [x1,y1, x2,y2]
}

int which_detector(float theta, int narcs, float arc_width){

   //Serial.println("In which detector...");
   int detector = 0;
   for (i=0;i<narcs;i++) {
    //Serial.println(i);
    //Serial.println(theta);
    //Serial.println(theta+PI);
    //Serial.println(i*arc_width);
    //Serial.println((i+1)*arc_width);
    if (theta+PI >= i*arc_width && theta+PI < (i+1)*arc_width) {
      detector = i;
      break;
    }
   }

   return detector;

}
