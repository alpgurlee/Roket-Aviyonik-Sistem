# Roket-Aviyonik-Sistem
 BMP180,MPU-9250,GY-NEO6MV2 sensörleri roketin uçuş değerini alır  ve  NRF240l01 yardımı ile Ana aviyonik sistem ile  Yedek aviyonik sistem arasındaki bağlantıyı sağlar


Neden bu sensörleri seçtik


![](Havacılık/Screenshot_1.png)



Özgün uçuş kartının blok devre diyagramı

![](Havacılık/UCUS_BILGISAYARI_.png)


Yer istasyonu blok devre diyagramı

![](Havacılık/YER_ISTASYONU.png)






GPS modülümüzden konum verisi, barometrik sensörümüzden basınç ve irtifa verisi, ivme ölçer modülümüzden ivme ve  eğim bilgisi yer istasyonuna aktarılacaktır.

Veriler 433MHz bandında RF veri paketleriyle iletileceklerdir.


![image](https://user-images.githubusercontent.com/66573571/236504941-2cdbc144-6754-4669-a1c0-fcbbc1764025.png)

Roket istenilen irtifaya ulaştığı anda burun konisinin  açılmasıyla beraber, görev yükü paraşütüne bağlı şok  kordonu ile kurtarılması hedeflenmektedir.
Burun konisinin serbest kalmasıyla oluşan basınçla  roketten ayrılması hedeflenmektedir.
Görev yükünün kütlesi : 2500 gram

Görev yükü üzerindeki konum belirleme sisteminin elemanları özgün uçuş bilgisayarımızın devre elemanlarıyla oldukça  benzerdir. En önemli farkları görev yükü üzerinde bulunacak bilgisayarın yalnızca konum bildirmesi ve kurtarmayı  kolaylaştıracak elemanlar içermesidir.



AKIŞ DİYAGRAMI

![](Havacılık/Resim1.png)

