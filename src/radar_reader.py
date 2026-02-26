import serial
import time
from datetime import datetime

# --- AYARLAR ---
arduino_port = "COM3"  # Kendi portunla (COM4 vb.) değiştir
baud_rate = 9600

def start_security_system():
    try:
        # Bağlantıyı kur
        ser = serial.Serial(arduino_port, baud_rate, timeout=1)
        time.sleep(2) # Kalibrasyon için bekleme
        print("--- GÜVENLİK SİSTEMİ AKTİF ---")
        print(f"Bağlantı: {arduino_port}\n")

        while True:
            if ser.in_waiting > 0:
                # Arduino'dan gelen satırı oku ve temizle
                raw_line = ser.readline().decode('utf-8').strip()
                
                if not raw_line:
                    continue

                # Zaman damgası ekle
                now = datetime.now().strftime("%H:%M:%S")

                # Eğer Arduino "HAREKET ALGILANDI" mesajı gönderirse
                if "HAREKET ALGILANDI" in raw_line:
                    print(f"[{now}] 🚨 UYARI: Hareket Tespit Edildi! Güvenlik ihlali olabilir.")
                
                # Sadece mesafe bilgisini ekrana yazdır (Loglama)
                elif "Mesafe:" in raw_line:
                    print(f"[{now}] {raw_line} cm")

    except serial.SerialException:
        print("HATA: Arduino bağlantısı kesildi veya port bulunamadı.")
    except KeyboardInterrupt:
        print("\nSistem kullanıcı tarafından güvenli bir şekilde kapatıldı.")
    finally:
        if 'ser' in locals():
            ser.close()

if __name__ == "__main__":
    start_security_system()
