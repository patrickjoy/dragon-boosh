#define MP3_RX_PIN 7  // White
#define MP3_TX_PIN 6  // Green

#define MP3_SEL_DEV  0x09
#define MP3_TF       0x02

#define MP3_PLAY_NEXT         0x01
#define MP3_PLAY_X_INDEX      0x03 // Play X file (1-255)
#define MP3_SET_VOLUME        0x06 // 0 - 30

#define MP3_FIRST_SONG 1 // first song, starting at index 1

const int mp3_numSongs = 5;
unsigned long mp3_songLengths[5] = {
  1000, // Song 1
  1000, // Song 2
  1000, // Song 3
  1000, // Song 4
  1000, // Song 5
};

int mp3_currentSong = MP3_FIRST_SONG;
bool mp3_celebrationPlayed = false;
unsigned long mp3_songStartedTime;

SoftwareSerial mp3(MP3_TX_PIN, MP3_RX_PIN);

void mp3_setup() {
  Serial.println("Setup mp3 player");
  randomSeed(analogRead(0)); // set random seed
  mp3.begin(BAUD_RATE); // being mp3 player
  delay(500); // delay to send command
  mp3_sendCommand(MP3_SEL_DEV, MP3_TF); // select the TF card
  delay(500); // delay to send command
  mp3_setVolume(30);
  delay(500); // delay to send command
}

void mp3_setVolume(int volume) {
  mp3_sendCommand(MP3_SET_VOLUME, 30);
}

void mp3_playNextSong() {
  if (mp3_currentSong >= mp3_numSongs) {
    mp3_playSong(1);
  } else {
    mp3_playSong(mp3_currentSong + 1);
  }
}

void mp3_playRandomSong() {
  int randomSong = random(MP3_FIRST_SONG, mp3_numSongs + 1); // choose random song to play
  // Serial.print("Play song: "); // TODO remove
  // Serial.println(songNumber); // TODO remove
  mp3_playSong(randomSong);
}

void mp3_playSong(int song) {
  mp3_sendCommand(MP3_PLAY_X_INDEX, song);
  mp3_songStartedTime = millis();
  mp3_currentSong = song;
}

void mp3_playCelebration(State state) {
  if (mp3_celebrationPlayed) return;
  if (state == ONE) mp3_playSong(1);
  else if (state == TWO) mp3_playSong(2);
  else if (state == THREE) mp3_playSong(3);
  else if (state == FOUR) mp3_playSong(4);
  else if (state == FIVE) mp3_playSong(5);
  else if (state == SIX) mp3_playSong(6); // There's only five songs
  mp3_celebrationPlayed = true;
}

bool mp3_isPlaying() {
  unsigned long currentTime = millis();
  unsigned long songLength = mp3_songLengths[mp3_currentSong - 1];
  if (currentTime - mp3_songStartedTime > songLength) {
    return false;
  }
  return true;
}

void mp3_reset() {
  mp3_celebrationPlayed = false;
  mp3_currentSong = random(MP3_FIRST_SONG, mp3_numSongs + 1); // set currentSong to random in the correct range
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