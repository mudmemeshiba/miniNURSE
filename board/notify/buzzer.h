#pragma once

// buzzer variables
// semibreve       2000  1600
// dotted minim    1500  1200
// minim           1000  800
// dotted crotchet 750   600
// crotchet        500   400 
// quaver          250   200

constexpr const char* notes[] = {
  "B0", "C1", "CS1", "D1", "DS1", "E1", "F1", "FS1",
  "G1", "GS1", "A1", "AS1", "B1", "C2", "CS2", "D2",
  "DS2", "E2", "F2", "FS2", "G2", "GS2", "A2", "AS2",
  "B2", "C3", "CS3", "D3", "DS3", "E3", "F3", "FS3",
  "G3", "GS3", "A3", "AS3", "B3", "C4", "CS4", "D4",
  "DS4", "E4", "F4", "FS4", "G4", "GS4", "A4", "AS4",
  "B4", "C5", "CS5", "D5", "DS5", "E5", "F5", "FS5",
  "G5", "GS5", "A5", "AS5", "B5", "C6", "CS6", "D6",
  "DS6", "E6", "F6", "FS6", "G6", "GS6", "A6", "AS6",
  "B6", "C7", "CS7", "D7", "DS7", "E7", "F7", "FS7",
  "G7", "GS7", "A7", "AS7", "B7", "C8", "CS8", "D8", "DS8"
};

constexpr int frequencies[] = {
  31, 33, 35, 37, 39, 41, 44, 46,
  49, 52, 55, 58, 62, 65, 69, 73,
  78, 82, 87, 93, 98, 104, 110, 117,
  123, 131, 139, 147, 156, 165, 175, 185,
  196, 208, 220, 233, 247, 262, 277, 294,
  311, 330, 349, 370, 392, 415, 440, 466,
  494, 523, 554, 587, 622, 659, 698, 740,
  784, 831, 880, 932, 988, 1047, 1109, 1175,
  1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865,
  1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960,
  3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978
};

constexpr const char* scoreOdeToJoy[] = {
  "E4", "E4", "F4", "G4", "G4", "F4", "E4", "D4", "C4", "C4", "D4", "E4", "E4", "D4", "D4",
  "E4", "E4", "F4", "G4", "G4", "F4", "E4", "D4", "C4", "C4", "D4", "E4", "D4", "C4", "C4"
};

constexpr int durationOdeToJoy[] = {
  500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 750, 250, 1000,
  500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 750, 250, 1000,
};

const char* scoreIdol[] {
  "E4", "G4", "A4", "E5", "A4", "G4", "A4", "E5", "A4", "G4", "A4",
  "E4", "G4", "A4", "C5", "B4", "G4", "A4", "B4", "C5", "D5", "F5", "E5",
  "A4", "E5", "A4", "G4", "A4", "E5", "A4", "G4", "A4", "E4", "G4", "A4", "C5", "B4", "G4", "A4", "Rest", "A4", "G5", "A4"
};

const int durationIdol[] {
  250, 250, 500, 500, 500, 250, 500, 250, 250, 250, 500,
  250, 250, 500, 500, 500, 250, 500, 250, 250, 250, 500, 500,
  500, 500, 500, 250, 500, 250, 250, 250, 500, 250, 250, 500, 500, 500, 250, 250, 250, 500, 250, 500
};

constexpr const char* scoreDoAhDeer[] {
  "C4", "D4", "E4", "C4", "E4", "C4", "E4", "D4", "E4", "F4", "F4", "E4", "D4", "F4",
  "E4", "F4", "G4", "E4", "G4", "E4", "G4", "F4", "G4", "A4", "A4", "G4", "F4", "A4", "Rest",
  "G4", "C4", "D4", "E4", "F4", "G4", "A4",
  "A4", "D4", "E4", "F4", "G4", "A4", "B4",
  "B4", "E4", "F4", "G4", "A4", "B4", "C5",
  "C5", "B4", "A4", "F4", "B4", "G4", "C5", "G4", "E4", "C4",
  "Rest", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "G4", "C5"
  };

constexpr int durationDoAhDeer[] {
  600, 200, 600, 200, 400, 400, 800, 600, 200, 200, 200, 200, 200, 1600,
  600, 200, 600, 200, 400, 400, 800, 600, 200, 200, 200, 200, 200, 1200, 400,
  600, 200, 200, 200, 200, 200, 1600,
  600, 200, 200, 200, 200, 200, 1600,
  600, 200, 200, 200, 200, 200, 1200,
  200, 200, 400, 400, 400, 400, 400, 400, 400, 400,
  200, 200, 200, 200, 200, 200, 200, 200, 400, 400, 800, 800, 800
};