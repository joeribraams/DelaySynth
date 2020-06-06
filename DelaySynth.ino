#include <MIDI.h>                                                 // Recieving midi messages
#include <SPI.h>                                                  // Communicating with digipot

#define pwmPin 6                                                  // PWM speed controls delay time
#define chipSel 10                                                // To prime digipot for recieving message
#define timePot A5                                                // Controls the fb pitch when not played to control texture

int noteOn = 0;                                                   // NoteOn counter for legato playing
int noteOff = 0;                                                  // NoteOff counter for legato playing

MIDI_CREATE_DEFAULT_INSTANCE();                                   // Initialize midi library with basic settings 

void handleNoteOn(byte channel, byte pitch, byte velocity)        // Called by MIDI.read when a noteOn is recieved
{
  noteOn++;                                                       // Increment noteOn counter
  
  if(pitch > 57 && pitch < 89)                                    // If pitch is within range of BBD
  {
    setTime((pitch - 57) * 8);                                    // Changing delay time changes pitch approximately logarithmic
    setLevel(velocity * 2);                                       // Sets level for velocity
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)       // Called by MIDI.read when a noteOff is recieved
{
  noteOff++;                                                      // Increment noteOff counter
  
  if(pitch >= 57 && pitch <= 89 && noteOn == noteOff)             // Filters extranious noteOffs for legato playing
  {
    setTime(analogRead(timePot) / 4);                             // Sets time back to not-played value
    setLevel(0);                                                  // Cuts output
    
    noteOn = 0;                                                   // Resets counter
    noteOff = 0;                                                  // Resets counter
  }
}

void setTime(byte value)                                          // Modifies PWM value to change time
{
  analogWrite(pwmPin, value);                                     // Arduino nano PWM accepts 8 bit value
}

void setFeedback(byte value)                                      // Sends SPI message to digipot to change feedback 
{
  digitalWrite(chipSel, LOW);                                     // Pull CS low to start message
  SPI.transfer(0 << 4);                                           // Send 0 to select first of the dual digipot
  SPI.transfer((255-value) & 255);                                // Send value
  digitalWrite(chipSel, HIGH);                                    // Pull CS high to end message
}

void setLevel(byte value)                                         // Sends SPI message to digipot to change level
{
  digitalWrite(chipSel, LOW);                                     // Pull CS low to start message
  SPI.transfer(1 << 4);                                           // Send 1 to select second of the dual digipot
  SPI.transfer((255-value) & 255);                                // Send value
  digitalWrite(chipSel, HIGH);                                    // Pull CS high to end message
}
 
void setup()
{
  pinMode(pwmPin, OUTPUT);                                        // AnalogWrite does require output to be declared
  pinMode(chipSel, OUTPUT);

  digitalWrite(chipSel, HIGH);                                    // Pull CS high before initializing SPI library
  
  MIDI.setHandleNoteOn(handleNoteOn);                             // Tells midi library what function to call upon recieving noteOn
  MIDI.setHandleNoteOff(handleNoteOff);                           // Tells midi library what function to call upon recieving noteOff
  
  MIDI.begin();                                                   // Start midi communication 
  SPI.begin();                                                    // Start spi communication

  setTime(analogRead(timePot) / 4);                               // Init time value
  setLevel(0);                                                    // Init level value
  setFeedback(255);                                               // Init feedback value
}

void loop()
{
  MIDI.read();                                                    // Calls connected functions upon recieving midi message
}
