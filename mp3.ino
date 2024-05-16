#define MP3_RX_PIN 8
#define MP3_TX_PIN 9

#define MP3_SEL_DEV  0x09
#define MP3_TF       0x02

#define MP3_PLAY_NEXT         0x01
#define MP3_PLAY_X_INDEX      0x03 // Play X file (1-255)
#define MP3_SET_VOLUME        0x06 // 0 - 30

unsigned long mp3_songLengths[32] = {
  253000, // Song 1 // 240 + 13
  322000, // Song 2 // 300 + 22
  252000, // Song 3 // 240 + 12
  269000, // Song 4 // 240 + 29
  350000, // Song 5 // 300 + 50
  265000, // Song 6 // 240 + 25
  340000, // Song 7 // 300 + 40
  354000, // Song 8 // 300 + 54
  327000, // Song 9 // 300 + 27
  256000, // Song 10 // 240 + 16
  249000, // Song 11 // 240 + 09
  84000, // Song 12 // 60 + 24 // playing song 6
  271000, // Song 13 // 240 + 31
  492000, // Song 14 // 480 + 12
  667000, // Song 15 // 660 + 07
  168000, // Song 16 // 120 + 48
  152000, // Song 17 // 120 + 32
  324000, // Song 18 // 300 + 24
  408000, // Song 19 // 360 + 48
  115000, // Song 20 // 60 + 55
  345000, // Song 21 // 300 + 45
  28000, // Song 22 rat
  28000, // Song 23 rooster
  28000, // Song 24 horse
  28000, // Song 25 ox
  28000, // Song 26 tiger
  28000, // Song 27 dragon
  1000, // Song 28 one
  1000, // Song 29 two
  1000, // Song 30 three
  1000, // Song 31 four
  1000, // Song 32 five
};

const int mp3_lastSong = 21;

unsigned long mp3_songStartedTime = 0;

int mp3_currentSong = 1; // index starts at 1

bool mp3_celebrationPlayed = false;

SoftwareSerial mp3(MP3_TX_PIN, MP3_RX_PIN);

void mp3_reset() {
  mp3_celebrationPlayed = false;
  mp3_playRandomSong();
}

void mp3_setup() {
  Serial.println("Setup mp3 player");
  mp3.begin(BAUD_RATE);
  delay(500);
  mp3_sendCommand(MP3_SEL_DEV, MP3_TF); // select the TF card
  delay(500);
  mp3_setVolume(30);
  delay(500);
}

void mp3_setVolume(int volume) {
  mp3_sendCommand(MP3_SET_VOLUME, 30);
}

void mp3_playNextSong() {
  if (mp3_currentSong >= mp3_lastSong) {
    mp3_playSong(1);
  } else {
    mp3_playSong(mp3_currentSong + 1);
  }
}

void mp3_playRandomSong() {
  int randomSong = random(1, mp3_lastSong + 1);
  mp3_playSong(randomSong);
}

void mp3_playSong(int song) {
  mp3_sendCommand(MP3_PLAY_X_INDEX, song);
  mp3_songStartedTime = millis();
  mp3_currentSong = song;
}

void mp3_playCelebration(State state) {
  if (mp3_celebrationPlayed) return;
  if (state == ONE) mp3_playSong(23);
  else if (state == TWO) mp3_playSong(24);
  else if (state == THREE) mp3_playSong(25);
  else if (state == FOUR) mp3_playSong(26);
  else if (state == FIVE) mp3_playSong(27);
  else if (state == SIX) mp3_playSong(28);
  mp3_celebrationPlayed = true;
}

bool mp3_checkPlaying() {
  unsigned long currentTime = millis();
  unsigned long songLength = mp3_songLengths[mp3_currentSong - 1];
  if (currentTime - mp3_songStartedTime > songLength) {
    return false;
  }
  return true;
}

void mp3_sendCommand(int8_t command) {
  mp3_sendCommand(command, 0x00, 0x00);
}

void mp3_sendCommand(int8_t command, int8_t dat2) {
  mp3_sendCommand(command, 0x00, dat2);
}

void mp3_sendCommand(int8_t command, int8_t dat1, int8_t dat2) {
  int8_t frame[8] = { 0 };
  frame[0] = 0x7e;                // starting byte
  frame[1] = 0xff;                // version byte
  frame[2] = 0x06;                // the number of bytes of the command without starting byte and ending byte
  frame[3] = command;             // command byte
  frame[4] = 0x01;                // 0x00 = no feedback, 0x01 = feedback
  frame[5] = dat1;                // data high byte
  frame[6] = dat2;                // data low byte
  frame[7] = 0xef;                // ending byte

  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(frame[i]);
  }
}

void mp3_testSongs() {
  Serial.println("Test songs");
  mp3_playSong(18);

  while (true) {
    delay(15000);
    mp3_playNextSong();
  }

  Serial.println("Done testing songs");
}
