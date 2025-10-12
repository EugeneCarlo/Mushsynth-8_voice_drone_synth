#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// соответствие нота → пин
const byte notePins[8] = {9, 8, 7, 6, 5, 4, 3, 2};
const byte firstNote = 1;     // нота 1 соответствует D9
const byte lastNote  = 8;     // нота 8 соответствует D2
const byte midiChannel = 10;  // 10-й канал (нумерация от 1)

// --- обработка NoteOn/NoteOff ---
void handleNoteOn(byte channel, byte note, byte velocity) {
  if (channel == midiChannel && note >= firstNote && note <= lastNote) {
    byte index = note - firstNote;
    digitalWrite(notePins[index], HIGH);   // держим высокий уровень, пока нота активна
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  if (channel == midiChannel && note >= firstNote && note <= lastNote) {
    byte index = note - firstNote;
    digitalWrite(notePins[index], LOW);    // отпускаем
  }
}

void setup() {
  // пины гейтов — выходы и LOW по умолчанию
  for (byte i = 0; i < 8; i++) {
    pinMode(notePins[i], OUTPUT);
    digitalWrite(notePins[i], LOW);
  }

  // запускаем MIDI приём на скорости 31250 бод (стандарт)
  MIDI.begin(MIDI_CHANNEL_OMNI);  // слушаем все, но фильтруем по каналу в хэндлерах
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
}

void loop() {
  MIDI.read();   // постоянно читаем входящий поток
}
