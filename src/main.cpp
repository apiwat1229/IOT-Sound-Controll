#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// Digital I/O used
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

#define CLK 32
#define DT 33
#define SW 34

int counter = 5;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

Audio audio;

String ssid = "sjr_family2.4G";
String password = "0624368558";

void getRotaryStat()
{

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK && currentStateCLK == 0)
  {

    if (digitalRead(DT) != currentStateCLK)
    {
      counter--;
      currentDir = "CCW";
      if (counter <= 0)
      {
        counter = 0;
      }

      audio.setVolume(counter);
    }
    else
    {
      // Encoder is rotating CW so increment
      counter++;
      if (counter >= 21)
      {
        counter = 21;
      }

      audio.setVolume(counter);
      currentDir = "CW";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);

  if (btnState == LOW)
  {
    if (millis() - lastButtonPress > 50)
    {
      Serial.println("Button pressed!");
    }
    lastButtonPress = millis();
  }
}

void setup()
{

  pinMode(SD_CS, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);

  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SPI.setFrequency(1000000);
  Serial.begin(115200);
  // SD.begin(SD_CS);

  // if (!SD.begin(SD_CS, SPI))
  //   {
  //       Serial.println("Card Mount Failed");
  //       while (1);
  //   }
  //   else
  //   {
  //       Serial.println("Card Mount OK!!");
  //   }

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(5); // 0...21

  //    audio.connecttoFS(SD, "/320k_test.mp3");
  //    audio.connecttoFS(SD, "test.wav");
  //    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
  //    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
  //    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac
  // audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
  // audio.connecttohost("0n-80s.radionetz.de:8000/0n-70s.mp3");
   audio.connecttohost("https://coolism-web.cdn.byteark.com/;stream/1");
}

void loop()
{
  // plot_Stat = map(analogRead(plot_Pin),0,4095,0,21);
  // Serial.println(plot_Stat);
  getRotaryStat();
  audio.loop();
  if (Serial.available())
  { // put streamURL in serial monitor
    audio.stopSong();
    String r = Serial.readString();
    r.trim();
    if (r.length() > 5)
      audio.connecttohost(r.c_str());
    log_i("free heap=%i", ESP.getFreeHeap());
  }
}

// optional
void audio_info(const char *info)
{
  Serial.print("info        ");
  Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}
void audio_eof_mp3(const char *info)
{ // end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);
}
void audio_showstation(const char *info)
{
  Serial.print("station     ");
  Serial.println(info);
}
void audio_showstreamtitle(const char *info)
{
  Serial.print("streamtitle ");
  Serial.println(info);
}
void audio_bitrate(const char *info)
{
  Serial.print("bitrate     ");
  Serial.println(info);
}
void audio_commercial(const char *info)
{ // duration in sec
  Serial.print("commercial  ");
  Serial.println(info);
}
void audio_icyurl(const char *info)
{ // homepage
  Serial.print("icyurl      ");
  Serial.println(info);
}
void audio_lasthost(const char *info)
{ // stream URL played
  Serial.print("lasthost    ");
  Serial.println(info);
}