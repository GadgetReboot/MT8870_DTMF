// DTMF decoding with the MT8870
// Q1-Q4 data inputs are read when an interrupt is received
// and touch tone keys are decoded for 0-9, *, #, A, B, C, D
//
// Gadget Reboot

// interrupt flag indicating a new DTMF tone has been located/received
volatile bool toneLoc = false;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);    // StQ (interrupt for data available)
  pinMode(3, INPUT);    // Q4 in
  pinMode(4, INPUT);    // Q3 in
  pinMode(5, INPUT);    // Q2 in
  pinMode(6, INPUT);    // Q1 in

  // run isr to set data received flag when interrupt occurs on pin 2
  attachInterrupt(digitalPinToInterrupt(2), newDTMF, RISING);

  Serial.println("DTMF Receiver\n");
}

void loop() {

  // decode DTMF data when data arrives
  if (toneLoc == true) {
    char keySymbol;            // decoded character
    bool Q1 = digitalRead(6);  // read Q1..Q4 data inputs
    bool Q2 = digitalRead(5);
    bool Q3 = digitalRead(4);
    bool Q4 = digitalRead(3);

    // combine Q1..Q4 bits into a byte
    byte toneData = (0x00 | Q1 | (Q2 << 1) | (Q3 << 2) | (Q4 << 3));

    // decode the DTMF character
    keySymbol = dtmf_decode(toneData);

    // we are expecting to receive characters in a specific test pattern
    // 0123456789*#ABCD
    // so format the received characters as a test pattern string per line
    if (keySymbol == '0') Serial.print("Receiving: ");   // display a message at start of each string
    Serial.print(keySymbol);                             // display each received character on the same line
    if (keySymbol == 'D') Serial.println();              // go to new line after string is complete

    // clear tone located flag to wait for next signal
    toneLoc = false;
  }
}

// isr to set data received flag upon interrupt
void newDTMF() {
  toneLoc = true;
}

// convert the Q1..Q4 data into a DTMF character
char dtmf_decode(byte recv_data) {
  switch (recv_data) {
    case 1:  return '1';
    case 2:  return '2';
    case 3:  return '3';
    case 4:  return '4';
    case 5:  return '5';
    case 6:  return '6';
    case 7:  return '7';
    case 8:  return '8';
    case 9:  return '9';
    case 10: return '0';
    case 11: return '*';
    case 12: return '#';
    case 13: return 'A';
    case 14: return 'B';
    case 15: return 'C';
    case 0:  return 'D';
    default: return '?';
  }
}
