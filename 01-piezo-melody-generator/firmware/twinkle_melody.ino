/**
 * Project: Manual Square-Wave Melody Generator ("Twinkle, Twinkle, Little Star")
 * Hardware: Arduino Uno/Nano + Passive Piezo Buzzer
 * Description: Generates musical notes by manually toggling a GPIO pin HIGH and LOW
 *              at calculated half-period microsecond intervals (bit-banging).
 */

int speakerPin = 9; // Digital PWM/GPIO pin connected to the passive piezo buzzer (+)
int length = 15;    // Total number of notes (including the final rest) in the melody

/* Melody Breakdown:
   c-c-g-g-a-a-g  (Twin-kle, twin-kle, lit-tle star)
   f-f-e-e-d-d-c  (How I won-der what you are)
*/
char notes[] = "ccggaagffeeddc ";                // Sequence of notes; a space (' ') represents a silent rest
int beats[]  = {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4}; // Relative duration (beat count) for each note
int tempo = 300;                                 // Base tempo in milliseconds (1 beat = 300 ms)

/**
 * Generates a 50% duty-cycle square wave manually on speakerPin.
 * @param tone     Half-period of the target frequency in microseconds: T/2 = 1,000,000 / (2 * f)
 * @param duration Total time to hold the note in milliseconds
 */
void playTone(int tone, int duration) {
  // Convert duration from milliseconds to microseconds (1000L prevents 16-bit integer overflow)
  // Increment by (tone * 2) because one full wave cycle = HIGH pulse + LOW pulse
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH); // Drive piezo pin HIGH (5V)
    delayMicroseconds(tone);        // Hold HIGH for half the wave period
    digitalWrite(speakerPin, LOW);  // Drive piezo pin LOW (0V)
    delayMicroseconds(tone);        // Hold LOW for half the wave period
  }
}

/**
 * Looks up the character note in the frequency table and triggers playTone().
 * @param note     Character representing the musical note ('c' through 'C')
 * @param duration Note duration in milliseconds
 */
void playNote(char note, int duration) {
  // Parallel lookup arrays mapping note names to their half-period delays (in microseconds)
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  int tones[]  = {1915, 1700, 1432, 1519, 1275, 1136, 1014, 956};

  // Search the lookup table for a matching note character
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration); // Generate the corresponding square wave
    }
  }
}

void setup() {
  // Configure the buzzer pin as a digital output
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // Step through every note in the melody array sequentially
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      // If the character is a space, pause silently for the beat duration
      delay(beats[i] * tempo);
    } else {
      // Otherwise, play the note for (beats * tempo) milliseconds
      playNote(notes[i], beats[i] * tempo);
    }
    
    // Short articulation pause between consecutive notes so repeated notes sound distinct
    delay(tempo / 2);
  }
}
