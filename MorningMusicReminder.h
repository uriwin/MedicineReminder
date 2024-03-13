int morningMelody[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};

int morningNoteDurations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};


void playMorningMusicReminder(){
    int size = sizeof(morningNoteDurations) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / morningNoteDurations[thisNote];
      ledcWriteTone(TONE_PWM_CHANNEL, morningMelody[thisNote]);
      delay(noteDuration);
      ledcWrite(TONE_PWM_CHANNEL, 0);
      // to distinguish the not/es, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 0.20;
      delay(pauseBetweenNotes);
  }
}
