/*
Concept based on aREST by Marco Schwartz,
completely rebuilt from scratch and optimized
for serial connection to ESP8266.
Includes 6 independent A/C line dimming capability.
*/

/* 
// constants, variables, functions, and definitions for REST
*/ 
String input_string = "";
String reply = "";
boolean string_complete = false;
String arg1;
String arg2;
String arg3;

typedef struct {
  String key;
  int * value;
} int_variable;
typedef struct {
  String key;
  float * value;
} float_variable;
typedef struct {
  String key;
  String * value;
} string_variable;

#define MAX_INT_VARS 5
#define MAX_FLOAT_VARS 5
#define MAX_STRING_VARS 5

int_variable int_variables[MAX_INT_VARS];
float_variable float_variables[MAX_FLOAT_VARS];
string_variable string_variables[MAX_STRING_VARS];

#define thermistor A0
float temperature = 0;

/* 
// constants, variables, functions, and definitions for dimming
*/ 

volatile int dim_level[6] = {0,0,0,0,0,0};    //will be updated by aREST functions
volatile int station_pins[6] = {9,8,7,4,5,6}; //digital pins connected to opto-isolated triacs
volatile int dim_mode[6] = {0,0,0,0,0,0}; //turn on/off dimming for each station
volatile int dim_step = 0;		      //start dim cycle at beginning



void setup() {
  for(int a = 0; a < 6; a++) {
    pinMode(station_pins[a], OUTPUT);
    digitalWrite(station_pins[a], LOW);
  }
  for(int a = 10; a <= 13; a++) {
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
  }
  pinMode(2, INPUT);      //used for external interrupt INT0
  pinMode(3, INPUT);      //used for external switch/INT1
  digitalWrite(3, HIGH);  //activate pull-up resistor
  cli(); //disable global interrupts
  setup_timer1(); //set up timer1 compare interrupt
  sei(); //enable global interrupts
  Serial.begin(9600);
  new_variable("temp",&temperature);
}

void loop() {
  temperature = readTemp(2);
  if(string_complete == true) {
    //make sure reply is empty
    reply = ""; 

    //formulate new reply
    parse_input();         //parse input_string to arg1, arg2, arg3
    process_input();       //process commands based on args and populate reply with JSON string

    //send reply
    Serial.print(reply);

    //reset loop to wait for next serial event
    string_complete = false; 
  }
}

//on serial connection, populate input_string
void serialEvent() {
  while(Serial.available()) {
    char in_char = (char)Serial.read();
    if(in_char == '\n') {
      string_complete = true;
    }
    else {
      input_string += in_char;
    }
  }
}


void parse_input() {
  //if blank, all args will be blank
  if(input_string == "") {
    arg1 = "";
    arg2 = "";
    arg3 = "";
  }
  else {
    //input_string is not blank, but no slash
    //then arg1 is input_string
    int slash_pos = input_string.indexOf('/');
    if(slash_pos <= 0) {
      arg1 = input_string;
      arg2 = "";
      arg3 = "";
    }
    else {
      arg1 = input_string.substring(0, slash_pos);
      input_string = input_string.substring(slash_pos+1);
      slash_pos = input_string.indexOf('/');
      if(slash_pos <= 0) {
        arg2 = input_string;
        arg3 = "";
      }
      else {
        arg2 = input_string.substring(0, slash_pos);
        arg3 = input_string.substring(slash_pos+1);
      }
    }
  }
  input_string = "";
}

void process_input() {
  if(arg1 == "variables" || arg1 == "v") {
    //handle processing and reply of variables function
    process_variable_command();
  }
  else if(arg1 == "mode" || arg1 == "m") {
    //handle pinMode functions
    process_mode_command();
  }
  else if(arg1 == "digital" || arg1 == "d") {
    //handle digitalRead/digitalWrite functions
    process_digital_command();
  }
  else if(arg1 == "analog" || arg1 == "a") {
    //handle analogRead/analogWrite functions
    process_analog_command();
  }
  else if(arg1 == "dimmer" || arg1 == "dim") {
    //handle dim interrupts
    process_dim_command(); 
  }
  else if(arg1 == "momentary") {
    //handle momentary command
    process_momentary_command(); 
  }
  else if(arg1 == "") {
    //send device id & status info via serial
   reply = "{\"ack\":\"ok\",\"id\":\"1\",\"status\":\"connected\"}";
  }
  else {
    reply = "{\"ack\":\"err\",\"reason\":\"invalid input\"}";
  }
}

//will return variables from int/float/string_variables arrays
//and format reply in JSON
void process_variable_command() {
  reply = "{\"ack\":\"ok\"";
  for(int a = 0; a < MAX_INT_VARS; a++) {
    if(int_variables[a].key != "") {
      reply += ",\"";
      reply += int_variables[a].key;
      reply += "\":\"";
      reply += String(*int_variables[a].value);
      reply += "\"";
    }
  }
  for(int a = 0; a < MAX_FLOAT_VARS; a++) {
    if(float_variables[a].key != "") {
      reply += ",\"";
      reply += float_variables[a].key;
      reply += "\":\"";
      reply += to_string(*float_variables[a].value,2);
      reply += "\"";
    }
  }
  for(int a = 0; a < MAX_STRING_VARS; a++) {
    if(string_variables[a].key != "") {
      reply += ",\"";
      reply += string_variables[a].key;
      reply += "\":\"";
      reply += *string_variables[a].value;
      reply += "\"";
    }
  }
  reply += "}";
}
//create new int/float/string variable
void new_variable(String new_name, int *new_variable) {
  boolean variable_assigned = false;
  int var_index = 0;
  while(variable_assigned == false && var_index < MAX_INT_VARS) {
    if(int_variables[var_index].key == "") {
      int_variables[var_index].key = new_name;
      int_variables[var_index].value = new_variable;
      variable_assigned = true;
      break; 
    }
  }
  var_index++;
}
void new_variable(String new_name, float *new_variable) {
  boolean variable_assigned = false;
  int var_index = 0;
  while(variable_assigned == false && var_index < MAX_FLOAT_VARS) {
    if(float_variables[var_index].key == "") {
      float_variables[var_index].key = new_name;
      float_variables[var_index].value = new_variable;
      variable_assigned = true;
      break; 
    }
  }
  var_index++;
}
void new_variable(String new_name, String *new_variable) {
  boolean variable_assigned = false;
  int var_index = 0;
  while(variable_assigned == false && var_index < MAX_STRING_VARS) {
    if(string_variables[var_index].key == "") {
      string_variables[var_index].key = new_name;
      string_variables[var_index].value = new_variable;
      variable_assigned = true;
      break; 
    }
  }
  var_index++;
}


void process_mode_command() {
  //if arg2 is blank, "read", or "r", return pinMode of all pins.
  if(arg2 == "" || arg2 == "r" || arg2 == "read") {
    //read all DDR states and format to JSON
    reply = "{\"ack\":\"ok\",";
    for(int a = 0; a <= 7; a++) {
      reply += "\"D";
      reply += String(a);
      reply += "_mode\":\"";
      reply += String(((1<<a)&DDRD)>>a);
      reply += "\",";
    }
    for(int a = 0; a <= 5; a++) {
      reply += "\"D";
      reply += String(a+8);
      reply += "_mode\":\"";
      reply += String(((1<<a)&DDRB)>>a);
      reply += "\",";
    }
    for(int a = 0; a <= 5; a++) {
      reply += "\"D";
      reply += String(a+14);
      reply += "_mode\":\"";
      reply += String(((1<<a)&DDRC)>>a);
      if(a < 5) {
        reply += "\",";
      }
      else {
        reply += "\"}";
      }
    }
  }
  //otherwise, continue to specific pin command
  else {
    int pin = arg2.toInt();
    //make sure pin is an integer from 0 to 19
    if(pin < 0 || pin > 19) {
      //if pin is outside integer range 0 to 19, return some kind of error
      reply = "{\"ack\":\"err\",\"reason\":\"pin number out of range (0-19)\"}";
    }
    else {
      //if arg3 is blank, "read", or "r", return pinMode of pin from arg2
      if(arg3 == "" || arg3 == "r" || arg3 == "read") {
        //read DDR state and format to JSON
        int pin_mode;
        if(pin <= 7) {
          if(((1<<pin)&DDRD) == 1) {
            pin_mode = 1;
          }
          else {
            pin_mode = 0;
          }
        }
        else if((pin >= 8) && (pin <= 13)) {
          if(((1<<(pin-8))&DDRB) == 1) {
            pin_mode = 1;
          }
          else {
            pin_mode = 0;
          }
        }
        else if((pin >= 14) && (pin <= 19)) {
          if(((1<<(pin-14))&DDRB) == 1) {
            pin_mode = 1;
          }
          else {
            pin_mode = 0;
          }
        }
        reply = "{\"ack\":\"ok\",\"D";
        reply += String(pin);
        reply += "_mode\":\"";
        reply += String(pin_mode);
        reply += "\"}";
      }
      //otherwise, set pinMode based on arg3
      else {
        if(arg3 == "output" || arg3 == "o" || arg3.toInt() == 1) {
          pinMode(pin, OUTPUT);
          //format reply in JSON, example: {"ack":"ok","D1_mode":"1"}
          reply = "{\"ack\":\"ok\",\"D";
          reply += String(pin);
          reply += "_mode\":\"1\"}";
        }
        else if(arg3 == "input" || arg3 == "i" || arg3.toInt() == 0) {
          pinMode(pin, INPUT);
          //format reply in JSON, example: {"ack":"ok","D1_mode":"0"}
          reply = "{\"ack\":\"ok\",\"D";
          reply += String(pin);
          reply += "_mode\":\"0\"}";
        }
        else {
          //return some kind of error
          reply = "{\"ack\":\"err\",\"reason\":\"invalid option\"}";
        }
      }
    }
  }
}

void process_digital_command() {
  //if arg2 is blank, "read", or "r", return digitalRead of all pins in JSON format {"D0":"1","D1":"0", ... "D13":"1"}
  if(arg2 == "" || arg2 == "r" || arg2 == "read") {
    //digitalRead all pins D0-13 and format to JSON
    reply = "{\"ack\":\"ok\"";
    for(int a = 0; a < 14; a++) {
      reply += ",\"D";
      reply += String(a);
      reply += "\":\"";
      reply += String(digitalRead(a));
      reply += "\"";
    }
    reply += "}";
  }
  //otherwise, continue to specific pin command
  else {
    int pin = arg2.toInt();
    //make sure pin is an integer from 0 to 19
    if(pin < 0 || pin > 19) {
      //if out of range, return some kind of error
      reply = "{\"ack\":\"err\",\"reason\":\"pin number out of range (0-19)\"}";
    }
    else {
      //if arg3 is blank, "read", or "r", return digitalRead of pin from arg2
      if(arg3 == "" || arg3 == "r" || arg3 == "read") {
        reply = "{\"ack\":\"ok\",\"D";
        reply += String(pin);
        reply += "\":\"";
        reply += String(digitalRead(pin));
        reply += "\"}";
      }
      //otherwise, digitalWrite to pin based on arg3
      else {
        if(arg3 == "high" || arg3 == "h" || arg3.toInt() == 1) {
          digitalWrite(pin, HIGH);
          reply = "{\"ack\":\"ok\",\"D";
          reply += String(pin);
          reply += "\":\"1\"}";
        }
        else if(arg3 == "low" || arg3 == "l" || arg3.toInt() == 0) {
          digitalWrite(pin, LOW);
          reply = "{\"ack\":\"ok\",\"D";
          reply += String(pin);
          reply += "\":\"0\"}";
        }
        else {
          //any other argument, return some kind of error
          reply = "{\"ack\":\"err\",\"reason\":\"bad argument\"}";
        }
      }
    }
  }
}

void process_analog_command() {
  //if arg2 is blank, "read", or "r", return pinMode of all pins.
  if(arg2 == "" || arg2 == "r" || arg2 == "read") {
    //analogRead all pins A0-5 and format to JSON
    reply = "{\"ack\":\"ok\"";
    for(int a = 0; a < 6; a++) {
       reply += ",\"A";
       reply += String(a);
       reply += "\":\"";
       reply += String(analogRead(a));
       reply += "\"";
    }
    reply += "}";
  }
  //otherwise, continue to specific pin command
  else {
    int pin = arg2.toInt();
    //if arg3 is blank, "read", or "r", return digitalRead of pin from arg2
    if(arg3 == "" || arg3 == "r" || arg3 == "read") {
      //make sure pin is an integer from 0 to 5
      if(pin < 0 || pin > 5) {
        //if out of range, return some kind of error
        reply = "{\"ack\":\"err\",\"reason\":\"pin number out of range (0-5)\"}";
      }
      else {
        //analogRead of pin and format to JSON
        reply = "{\"ack\":\"ok\",\"D";
        reply += String(pin);
        reply += "\":\"";
        reply += String(analogRead(pin));
        reply += "\"}";
      }
    }
    //otherwise, PWM/analogWrite to pin based on arg3
    else {
      int cmd = arg3.toInt();
      if(cmd < 0 || cmd > 255) {
        //return some kind of error
        reply = "{\"ack\":\"err\",\"reason\":\"PWM value out of range (0-255)\"}";
      }
      else {
        if(pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) {
          analogWrite(pin, cmd);
          reply = "{\"ack\":\"ok\",\"D";
          reply += String(pin);
          reply += "\":\"";
          reply += String(cmd);
          reply += "\"}";
        }
        else {
          //if out of range, return some kind of error
          reply = "{\"ack\":\"err\",\"reason\":\"invalid pin for PWM output\"}";
        }
      }
    }
  }
}

void process_momentary_command() {
  if(arg2 == "") {
     reply = "{\"ack\":\"err\",\"reason\":\"no pin selected\"}";
  }
  else {
    int pin = arg2.toInt();
    if(pin < 0 || pin > 19) {
      reply = "{\"ack\":\"err\",\"reason\":\"pin number out of range (0-19)\"}";
    }
    else {
      if(arg3 == "") {
        reply = "{\"ack\":\"err\",\"reason\":\"need time (ms)\"}";
      } 
      else if(is_int(arg3)) {
        int cmd = arg3.toInt();
        digitalWrite(pin, HIGH);
        delay(cmd);
        digitalWrite(pin, LOW);
        reply = "{\"ack\":\"ok\",\"M";
        reply += String(pin);
        reply += "\":\"";
        reply += String(cmd);
        reply += "ms\"}";
      }
      else {
        reply = "{\"ack\":\"err\",\"reason\":\"invalid time (ms)\"}";
      }
    } 
  }
}

void process_dim_command() {
  //if arg2 is blank, "read", or "r", return digitalRead of all pins in JSON format {"D0":"1","D1":"0", ... "D13":"1"}
  if(arg2 == "" || arg2 == "r" || arg2 == "read") {
    //digitalRead all pins D0-13 and format to JSON
    reply = "{\"ack\":\"ok\"";
    for(int a = 0; a < 6; a++) {
      reply += ",\"Dim";
      reply += String(a);
      reply += "\":\"";
      if(dim_level[a] >= 10) {
        reply += "on";
      }
      else if(dim_level[a] == 0) {
        reply += "off";
      }

      else {
        reply += String(dim_level[a]);
      }
      reply += "\"";
    }
    reply += "}";
  }
  else if(arg2 == "on") {
    dimmer_on();
    reply = "{\"ack\":\"ok\",\"dimmer\":\"on\"}";
  }
  else if(arg2 == "off") {
    dimmer_off();
    reply = "{\"ack\":\"ok\",\"dimmer\":\"off\"}";
  }
  //otherwise, continue to specific pin command
  else {
    int pin = arg2.toInt();
    //make sure pin is an integer from 0 to 19
    if(pin < 0 || pin > 5) {
      //if out of range, return some kind of error
      reply = "{\"ack\":\"err\",\"reason\":\"pin number out of range (0-5)\"}";
    }
    else {
      //if arg3 is blank, "read", or "r", return dim level of pin from arg2
      if(arg3 == "" || arg3 == "r" || arg3 == "read") {
        reply = "{\"ack\":\"ok\",\"Dim";
        reply += String(pin);
        reply += "\":\"";
        if(dim_level[pin] >= 10) {
          reply += "on";
        }
        else if(dim_level[pin] == 0) {
          reply += "off";
        }
        else {
          reply += String(dim_level[pin]);
        }
        reply += "\"}";
      }
      
      //otherwise, set dim level of pin based on arg3
      else {
        int cmd = arg3.toInt();
        if(cmd < 0 || cmd > 10) {
          reply = "{\"ack\":\"err\",\"reason\":\"dim value out of range (0-10)\"}";
        }
        //if individual station is 0, turn off dimming for that station
        else if(cmd == 0) {
          dim_mode[pin] = 0;
          dim_level[pin] = cmd;
          digitalWrite(station_pins[pin], LOW);
          reply = "{\"ack\":\"ok\",";
          int chkmode = 0;
          for(int a = 0; a < 6; a++) {
            if(dim_mode[a] != 0) {
              chkmode++;
            }
          }
          //if all stations are off, turn off timer and zero cross interrupts
          if(chkmode == 0) {
            cli();
            detachInterrupt(0);
            TIMSK1 &= ~(1<<OCIE1A);
            TCNT1 = 0;
            sei();
            reply += "\"dimmer\":\"off\",";
          }
          else {
            reply += "\"dimmer\":\"on\","; 
          }
          reply += "\"Dim";
          reply += String(pin);
          reply += "\":\"off\"}";
        }
        //if individual station is 10, turn off dimming for that station
        //and just digitalwrite to high
        else if(cmd == 10) {
          dim_mode[pin] = 0;
          dim_level[pin] = cmd;
          digitalWrite(station_pins[pin], HIGH);
          reply = "{\"ack\":\"ok\",";
          int chkmode = 0;
          for(int a = 0; a < 6; a++) {
            if(dim_mode[a] != 0) {
              chkmode++;
            }
          }
          //if all stations are 10/off, turn off zero cross
          //and timer interrupts
          if(chkmode == 0) {
            cli();
            detachInterrupt(0);
            TIMSK1 &= ~(1<<OCIE1A);
            TCNT1 = 0;
            sei();
            reply += "\"dimmer\":\"off\",";
            reply += "\"D";
            reply += String(station_pins[pin]);
            reply += "\":\"";
            reply += String(digitalRead(station_pins[pin]));
            reply += "\",";
          }
          else {
            reply += "\"dimmer\":\"on\",";
          }
          reply += "\"Dim";
          reply += String(pin);
          reply += "\":\"";
          reply += "on";
          reply += "\"}";
        }
        else {
          int chkmode = 0;
          for(int a = 0; a < 6; a++) {
            if(dim_mode[a] != 0) {
              chkmode++;
            }
          }
          if(chkmode == 0) {
            dimmer_on();

          }
          dim_mode[pin] = 1;
          dim_level[pin] = cmd;
          reply = "{\"ack\":\"ok\","; 
          reply += "\"dimmer\":\"on\",";
          reply += "\"Dim";
          reply += String(pin);
          reply += "\":\"";
          reply += String(cmd);
          reply += "\"}";
        }
      }
    }
  }
}
//if dimmer explicitly turned on, start zero cross detection
//and timer interrupt
//also make sure all station pins are outputs
//no need to worry about turning pins on/off, will be handled
//by timer and zero cross interrupts.
void dimmer_on() {
  cli();
  pinMode(2, INPUT);
  attachInterrupt(0, zero_cross_int, RISING); //set up external interrupt INT0 for zero cross detection
  TIMSK1 = ((1<<OCIE1A)); //compare match interrupt A
  OCR1A = 0x0640; //default to 0x640 = 800microseconds
  TCNT1 = 0; //start counter at 0
  for(int a = 0; a < 6; a++) {
    pinMode(station_pins[a], OUTPUT);
  }
  sei();
}
//if dimmer explicitly turned off or all stations are individually
//set to off, turn off interrupt routine and zero cross
void dimmer_off() {
  cli();
  detachInterrupt(0);
  TIMSK1 &= ~(1<<OCIE1A);
  TCNT1 = 0;
  for(int a = 0; a < 6; a++) {
    digitalWrite(station_pins[a], LOW);
    dim_mode[a] = 0;       
  }
  sei();
}
void setup_timer1() {  
  TCCR1A = 0; //clear register
  TCCR1B = ((1<<WGM12)|(1<<CS11)); //CTC mode top=OCR1A, prescale=clk/8
  OCR1A = 0x0640; //default to 0x640 = 800microseconds
  TCNT1 = 0; //start counter at 0
}
//turns on when zero cross, triggers 10x, 800us apart, then turns off any pins not set to fully on (10)
ISR(TIMER1_COMPA_vect) {
  TCNT1 = 0;
  //if it has not triggered 10 times, increment and check against station setting
  //zero cross will reset dim_step to 0
  if(dim_step < 10) {
    dim_step++;
    for(int a = 0; a < 6; a++) {
      //if station dimming is turned on
      if(dim_mode[a] == 1) {
        //and if dim level is not 0/off
        if(dim_level[a] > 0) {
    	  //if dim step has has passed the station setting turn on time, turn on station
          if(dim_step >= 10-dim_level[a]) {
            digitalWrite(station_pins[a], HIGH);    
          }  
          else {
            digitalWrite(station_pins[a], LOW);
          }
        }
      }
    }
  }
  //after 10 times, turn off any not set to fully on and wait until next zero cross
  else {
    for(int a = 0; a < 6; a++) {
      if(dim_mode[a] == 1) {
        if(dim_level[a] < 10) {
          digitalWrite(station_pins[a], LOW);
        }
      }
    }
  }
}

//zero cross interrupt routine
void zero_cross_int(void) {
  TCNT1 = 0;     //reset timer1
  dim_step = 0;  //reset timer1 cycle to beginning
  for(int a = 0; a < 6; a++) {
    //if station's dimming is turned on
    if(dim_mode[a] == 1) {
      //keep on any stations set to "fully on"
      if(dim_level[a] >= 10) {
        digitalWrite(station_pins[a], HIGH);
      }
      //turn the rest off until next timer cycle
      else {
        digitalWrite(station_pins[a], LOW);
      }
    }
  }
}

float readTemp(int scale) {
  #define THERMISTORNOMINAL 10000      // resistance at 25 degrees C
  #define TEMPERATURENOMINAL 25   // temp. for nominal resistance (almost always 25 C)
  #define BCOEFFICIENT 3375 // The beta coefficient of the thermistor (usually 3000-4000)
  #define SERIESRESISTOR 10000  // the value of the 'other' resistor
  float val = analogRead(thermistor);
  float resistance = 1023 / val - 1;
  resistance = SERIESRESISTOR / resistance;
  float kelvin = resistance / THERMISTORNOMINAL;
  kelvin = log(kelvin);
  kelvin /= BCOEFFICIENT;
  kelvin += 1.0 / (TEMPERATURENOMINAL + 273.15);
  kelvin = 1.0 / kelvin;
  float celsius = kelvin - 273.15;
  float farenheit = celsius * 9;
  farenheit = farenheit / 5;
  farenheit += 32;
  switch(scale) {
    case 0:
      return kelvin;
      break;
    case 1:
      return celsius;
      break;
    case 2:
      return farenheit;
      break;
  }
}

//check if string is integer, return true/false
boolean is_int(String chkstr) {
  int non_digit_count = 0;
  for(int a = 0; a < chkstr.length(); a++) {
    if(!isdigit(chkstr[a])) {
      non_digit_count++;
    }
  }
  if(non_digit_count == 0) {
    return true;
  }
  else {
    return false;
  }
}

//manually convert float to string
//sstream library not available in arduino ide
String to_string(float myfloat, int precision) {
  String mystring = "";
  int whole = (int)myfloat;
  int decimals = (int)((myfloat - whole) * pow(10,precision));
  //round to nearest precision
  if( (((int)(myfloat*pow(10,precision+1))) %10) >= 5) {
    decimals += 1;
  }
  mystring += String(whole);
  mystring += ".";
  mystring += String(decimals);
  return mystring;
}
