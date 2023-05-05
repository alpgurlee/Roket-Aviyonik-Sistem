#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MPU9250_asukiaaa.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
SoftwareSerial gpsSerial(4, 3); // RX, TX
TinyGPSPlus gps;
RF24 radio(9, 8); // CE, CSN
const uint64_t pipeOut = 0xE8E8F0F0E1LL;
MPU9250_asukiaaa mpu;

const int chunkSize = 32;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Wire.begin();
  
  if (!bmp.begin()) {
    Serial.println("BMP180 bulunamadı, lütfen bağlantıları kontrol edin!");
    while (1);
  }
   // nRF24L01 başlatılıyor
 radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(pipeOut);
  radio.stopListening();
  // MPU9250 sensörünü başlatma
  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  mpu.beginMag();
}

void sendChunks(float *data, size_t dataSize) {
  size_t numChunks = (dataSize + chunkSize - 1) / chunkSize;   //gönderilecek toplam (chunk) sayısını hesaplar. Bu değer, dataSize'ı chunkSize'a böler ve bölümü yukarı yuvarlar (ör. 33 byte için 2 parça olacaktır).
  for (size_t i = 0; i < numChunks; i++) {  // numChunks sayısına göre bir döngü başlatılır ve her bir parça gönderilir.
    size_t currentChunkSize = min(chunkSize, dataSize - i * chunkSize);  //değişken oluşturulur ve gönderilecek mevcut parçanın boyutunu hesaplamak için kullanılır. 
    radio.write(((uint8_t*)data) + (i * chunkSize), currentChunkSize); //Gönderilecek verinin başlangıç adresi, data'nın adresine i * chunkSize byte ekleyerek belirlenir. Gönderilen verinin boyutu, currentChunkSize ile belirlenir.
    delay(100); // Kısa bir bekleme süresi  Bu, alıcının parçalar arasında veriyi doğru bir şekilde işlemesine olanak tanımak içindir
  }
}

void loop() {
  if (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    float sensorData[15];
    
    // BMP180 verilerini okuma
    sensors_event_t event;
    bmp.getEvent(&event);
    sensorData[0] = event.pressure;
    float temperature;
    bmp.getTemperature(&temperature);
    sensorData[1] = temperature;

   // GPS verilerini okuma
    sensorData[2] = gps.location.lat();
    sensorData[3] = gps.location.lng();
    sensorData[13] = gps.altitude.meters();
    sensorData[14] = gps.speed.kmph();

   // MPU9250 verilerini okuma
    mpu.accelUpdate();
    sensorData[4] = mpu.accelX();
    sensorData[5] = mpu.accelY();
    sensorData[6] = mpu.accelZ();
    // mpu gyroUpdate
    mpu.gyroUpdate();
    sensorData[7] = mpu.gyroX();
    sensorData[8] = mpu.gyroY();
    sensorData[9] = mpu.gyroZ();
    //mpu mag update
    mpu.magUpdate();
    sensorData[10] = mpu.magX();
        sensorData[11] = mpu.magY();
    sensorData[12] = mpu.magZ();

    // Verileri nRF24L01 ile gönderme
    sendChunks(sensorData, sizeof(sensorData));

    // Gönderilen verileri seri monitörde gösterme
    Serial.print("Basınç: "); Serial.print(sensorData[0]); Serial.print(" Pa, ");
    Serial.print("Sıcaklık: "); Serial.print(sensorData[1]); Serial.print(" C, ");
    Serial.print("Enlem: "); Serial.print(sensorData[2], 6); Serial.print(", ");
    Serial.print("Boylam: "); Serial.print(sensorData[3], 6); Serial.print(", ");
    Serial.print("Metre: "); Serial.print(sensorData[13]); Serial.print(" m, ");
    Serial.print("Hız: "); Serial.print(sensorData[14]); Serial.println(" Kmph, ");
    Serial.print("Accel X: "); Serial.print(sensorData[4]); Serial.print(" m/s^2 , ");
    Serial.print("Accel Y: "); Serial.print(sensorData[5]); Serial.print(" m/s^2 , ");
    Serial.print("Accel Z: "); Serial.print(sensorData[6]); Serial.println(" m/s^2 , ");
    Serial.print("Gyro X: "); Serial.print(sensorData[7]); Serial.print(" m/s^2 , ");
    Serial.print("Gyro Y : "); Serial.print(sensorData[8]); Serial.print(" m/s^2 , ");
    Serial.print("Gyro Z: "); Serial.print(sensorData[9]); Serial.println(" m/s^2");
    Serial.print("Mag X: "); Serial.print(sensorData[10]); Serial.print(" m/s^2  , ");
    Serial.print("Mag Y: "); Serial.print(sensorData[11]); Serial.print(" m/s^2 , ");
    Serial.print("Mag Z: "); Serial.print(sensorData[12]); Serial.println(" m/s^2");

    delay(3000);
  }
}

