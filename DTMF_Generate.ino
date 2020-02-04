// Arduino synth library modulation example
// Library by DZL:  https://github.com/dzlonline/the_synth
//
// Arduino Uno Hardware Connections:
//
//                        +10µF
// PIN 11 ---[ 1kohm ]--+---||--->> Audio out
//                      |
//                     === 10nF
//                      |
//                     GND
//
// This demo uses the_synth library to generate two simultaneous tones
// in frequency pairs that represent various DTMF combinations including
// North American touch tone keypad tones and other call progress tones.
//
//     DTMF keypad frequencies
//             1209Hz 1336Hz 1477Hz 1633Hz
//      697Hz    1      2      3      A
//      770Hz    4      5      6      B
//      852Hz    7      8      9      C
//      941Hz    *      0      #      D
//
//  DTMF keypad optimized function provided by Tengelgeer
//
//  Gadget Reboot



#include <synth.h>

synth pwmSynth;    // create pwmSynth object
int onTime;        // time in mS to keep playing DTMF tones
int offTime;       // time in mS to stay silent after playing a DTMF tone

void setup() {
  Serial.begin(9600);

  // init synth with sine wave audio
  pwmSynth.begin();
  pwmSynth.setupVoice(0, SINE, 60, ENVELOPE1, 127, 64); //-Set up voice 0
  pwmSynth.setupVoice(1, SINE, 60, ENVELOPE1, 127, 64); //-Set up voice 1

  Serial.println("DTMF Generator\n");
}

void loop()
{
  // work in progress if I want to get these functions working
  //keypadTone('4', 200);
  //delay(200);

  //keypadTone("0123456789*#ABCD", 200, 200);
  //delay(200);

  // continually output the test pattern of DTMF tones: 0123456789*#ABCD
  // starting with 120 mS of tone playback time and 120 mS of silence following
  // then keep reducing time by 10mS to speed up playback to test the
  // receiver's capability to detect tones
  // each tone is manually played - ideally in the future I can use a
  // function like keypadTone("0123456789*#ABCD", 200, 200);
  onTime = 120;
  offTime = 120;
  for (int i = 0; i <= 12; i++) {
    Serial.print("On/Off Time: ");
    Serial.print(onTime);
    Serial.print("mS / ");
    Serial.print(offTime);
    Serial.println("mS    Sending: 0123456789*#ABCD");
    keypadTone(0, onTime);
    delay(offTime);
    keypadTone(1, onTime);
    delay(offTime);
    keypadTone(2, onTime);
    delay(offTime);
    keypadTone(3, onTime);
    delay(offTime);
    keypadTone(4, onTime);
    delay(offTime);
    keypadTone(5, onTime);
    delay(offTime);
    keypadTone(6, onTime);
    delay(offTime);
    keypadTone(7, onTime);
    delay(offTime);
    keypadTone(8, onTime);
    delay(offTime);
    keypadTone(9, onTime);
    delay(offTime);
    keypadTone(10, onTime);
    delay(offTime);
    keypadTone(11, onTime);
    delay(offTime);
    keypadTone(12, onTime);
    delay(offTime);
    keypadTone(13, onTime);
    delay(offTime);
    keypadTone(14, onTime);
    delay(offTime);
    keypadTone(15, onTime);
    delay(offTime);

    onTime -= 10;
    offTime -= 10;
  }
}

// keypad row/column frequency selection functions by Tengelgeer
// play a tone identified as an integer 0..15
// key order 0123456789*#ABCD (0 = 0, D = 15)
void keypadTone(int n, unsigned int onTime) {
  const unsigned int ColumnFrequencies[] = {1209, 1336, 1477, 1633};
  const unsigned int RowFrequencies[] = {697, 770, 852, 941};

  //no tone beyond D
  if (n >= 16) {
    return;
  }

  //shift a bit to get the order 123456789*0# for the first three columns
  if (n == 0) {
    n = 10;
  }
  else if (n <= 10) {
    n -= 1;
  }

  //1 to # (first three columns)
  if (n < 12) {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[n % 3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n / 3]);
  }
  //A to D (4th column)
  else {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n - 12]);
  }

  pwmSynth.trigger(0);
  pwmSynth.trigger(1);

  // if onTime=0 the sound continuously plays until another sound is commanded elsewhere
  // if onTime>0 the sound plays for the duration in mS
  if (onTime > 0) {
    delay(onTime);
    pwmSynth.setFrequency(0, 0);
    pwmSynth.setFrequency(1, 0);
  }
}

// *can't get working yet
// play a tone requested by a char by using a char: 0123456789*#ABCDabcd
void keypadTone(char n, unsigned int onTime) {
  //if small letter, make capital
  if (n >= 'a') {
    n -= 32;
  }

  if (n == '*') {
    keypadTone(10, onTime);
  }
  else if (n == '#') {
    keypadTone(11, onTime);
  }
  else if ((n >= 'A') && (n <= 'D')) {
    keypadTone((byte)(n - 'A' + 12), onTime);
  }
}

// *can't get working yet
// request tones as a string
// so to call Jenny: keypadTone("8675309", 100, 100);
void keypadTone( const char str[], unsigned int onTime, unsigned int keyReleaseDelay) {
  if (str != nullptr) {
    return;
  }
  for (byte i = 0; str[i] != '\0'; i++) {
    keypadTone(str[i], onTime);
    delay(keyReleaseDelay);
  }
}
