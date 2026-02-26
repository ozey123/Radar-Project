const int trigPin = A1;
const int echoPin = A0;
const int buzPin = 3;

int eskiMesafe = 0; // Bir önceki ölçümü saklar
int esikDegeri = 10; // Kaç cm değişimde hareket sayılsın? (Hassasiyet ayarı)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzPin, OUTPUT);
  Serial.begin(9600);
  
  // İlk ölçümü yapıp hafızaya alalım
  eskiMesafe = mesafeOlcu();
  Serial.println("Sistem Kalibre Edildi. Hareket Bekleniyor...");
}

void loop() {
  int suankiMesafe = mesafeOlcu();

  // Seri monitörden takip et
  Serial.print("Mesafe: "); Serial.println(suankiMesafe);

  // EĞER mesafe 0 değilse (hata değilse) VE mesafe aniden değişmişse
  if (suankiMesafe > 0 && abs(suankiMesafe - eskiMesafe) > esikDegeri) {
    // Sadece 350cm içindeki hareketleri ciddiye al
    if (suankiMesafe < 350) {
      Serial.println("!!! HAREKET ALGILANDI !!!");
      
      // 3 Bip sesi
      for(int i = 0; i < 3; i++) {
        digitalWrite(buzPin, HIGH);
        delay(100);
        digitalWrite(buzPin, LOW);
        delay(100);
      }
      
      // Hareketten sonra yeni mesafeyi kaydet ve biraz bekle
      eskiMesafe = suankiMesafe;
      delay(1000); 
    }
  } else {
    // Hareket yoksa, arka planda hafızayı yavaşça güncelle (Yavaş kaymaları eler)
    eskiMesafe = suankiMesafe;
  }

  delay(200); // Gözlem sıklığı
}

// Mesafe ölçen yardımcı fonksiyon
int mesafeOlcu() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 26000);
  return duration * 0.034 / 2;
}
