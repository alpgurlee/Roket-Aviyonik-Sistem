#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 8); // CE, CSN pinleri

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_8);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}
  // nRF24L01 ile 32 bite kadar veri gönderilebiliyor
void receiveChunks(float* data, size_t dataSize) {
  size_t bytesReceived = 0; // şimdiye kadar alınan toplam byte sayısını saklamak için bir değişken tanımlanır.
  while (bytesReceived < dataSize) {  //küçük olduğu sürece döngü devam eder. Bu, tüm verileri okuyana kadar döngünün çalışacağı anlamına gelir.
    if (radio.available()) {   // nRF24L01'de okunacak veri olup olmadığını kontrol eder. Eğer veri mevcutsa, döngü devam eder.
      size_t remainingBytes = dataSize - bytesReceived;  // okunması gereken kalan byte sayısını hesaplar 
      size_t bytesToRead = min(remainingBytes, 32u); // 32 byte  arasındaki minimum değeri alır. Bu, bir seferde okunacak byte sayısını belirler.
      radio.read(((uint8_t*)data) + bytesReceived, bytesToRead);  // veriyi okur ve data dizisine ekler. Okunan verinin boyutu, bytesToRead ile belirlenir. data dizisine veri eklemek için, data'nın adresine bytesReceived kadar byte ekleyerek okuma işleminin nereden başlayacağını belirleriz.
      bytesReceived += bytesToRead; //  değerine eklenir ve döngü tekrar başa döner. Tüm veriler okunduğunda, döngü sona erer.
    }
  }
}

void loop() {
  if (radio.available()) {
    float sensorData[15];
    receiveChunks(sensorData, sizeof(sensorData));

    Serial.print("Basınç: ");
    Serial.print(sensorData[0]);

    Serial.print(" Pa, Sıcaklık: ");
    Serial.print(sensorData[1]);

    Serial.print(" C, Enlem: ");
    Serial.print(sensorData[2], 6);
    Serial.print(", Boylam: ");
    Serial.print(sensorData[3], 6);

    

    Serial.print(" , Metre:  ");
    Serial.print(sensorData[13]);
   
    Serial.print(" Hız, kmph: ");
    Serial.println(sensorData[14]);

    Serial.print(" Accel X: ");
    Serial.print(sensorData[4]);
    Serial.print(" m/s^2, Accel Y: ");
    Serial.print(sensorData[5]);
    Serial.print(" m/s^2, Accel Z: ");
    Serial.print(sensorData[6]);
    Serial.println(" m/s^2");
    Serial.print(" Gyro X: ");
    Serial.print(sensorData[7]);
    Serial.print(" m/s^2");
    Serial.print(" Gyro Y: ");
    Serial.print(sensorData[8]);
    Serial.print(" m/s^2");
    Serial.print(" Gyro Z: ");
    Serial.print(sensorData[9]);
    Serial.println(" m/s^2");

    Serial.print(" Mag X: ");
    Serial.print(sensorData[10]);
    Serial.print(" m/s^2");
    Serial.print(" Mag Y: ");
    Serial.print(sensorData[11]);
    Serial.print(" m/s^2");
    Serial.print(" Mag Z: ");
    Serial.print(sensorData[12]);
    Serial.println(" m/s^2");
    
    

    delay(3000);
  }
}