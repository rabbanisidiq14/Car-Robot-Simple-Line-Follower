/*
Potensi pengembangan : 
- Menggunakan pin ENA dan ENB untuk mengatur kecepatan mobil
- Menggunakan konsep ingatan untuk melakukan undo gerakan
- Menambah logik pengambilan keputusan terutama pada pengambilan keputusan kecepatan
- Menambah logik agar tidak masuk ke jalur yang salah
- Menambah logik gerakan yang lebih bagus, dan tidak instan, gerakannya direncanakan atau memorical, dapat dibantu menggunakan kalibrasi
- Menambah logik agar tidak terlalu memusingkan jalur yang rumit, dan mengambil jalur yang termudah namun tetap tepat
- Menambah logik negatif detection (0: putih, 1: hitam), jika ada kasus jalur yang warna nya tetiba dibalik

Tips pengembangan : 
- Membuat simulasi pergerakan mobil, sehingga sebelum dicode terlihat apakah masuk akal atau tidak kodenya, lalu kemudian implementasi di ril nya
*/
  
class Roda{
  public:
  byte pinMaju;
  byte pinMundur;
  Roda(byte _pinMaju, byte _pinMundur){
    pinMaju = _pinMaju;
    pinMundur = _pinMundur;
  }

  // penetapan pinMode untuk maju dan mundur, dipanggil di setup
  void init(){
    pinMode(pinMaju, OUTPUT);
    pinMode(pinMundur, OUTPUT);
  }

  //diam
  void diam(){
    digitalWrite(pinMaju, LOW);
    digitalWrite(pinMundur, LOW);
  }

  //maju
  void maju(){
    digitalWrite(pinMaju, HIGH);
    digitalWrite(pinMundur, LOW);
  }

  //mundur
  void mundur(){
    digitalWrite(pinMaju, LOW);
    digitalWrite(pinMundur, HIGH);
  }
};

// deklarasi pin
byte pinLineKiri = 4;
byte pinLineTengah1 = 34;
byte pinLineTengah2 = 35;
byte pinLineKanan = 15;

Roda rodaKiri(12, 13);
Roda rodaKanan(14, 27);

enum STATES {
  MAJU,
  MUNDUR,
  DIAM,
  BELOKKANANMAJU,
  BELOKKIRIMAJU,
  BELOKKANANMUNDUR,
  BELOKKIRIMUNDUR
};

STATES state;

void setup() {
  Serial.begin(115200);

  // Set mode pin
  pinMode(pinLineKiri, INPUT);
  pinMode(pinLineTengah1, INPUT);
  pinMode(pinLineTengah2, INPUT);
  pinMode(pinLineKanan, INPUT);

  state = DIAM;
  rodaKiri.init();
  rodaKanan.init();
}

void loop() {
  // pembacaan sensor line
  byte lineKiri = digitalRead(pinLineKiri);
  byte lineTengah1 = digitalRead(pinLineTengah1);
  byte lineTengah2 = digitalRead(pinLineTengah2);
  byte lineKanan = digitalRead(pinLineKanan);
  
  // Pemilihan state berdasarkan bacaan sensor (simple)
  if(lineTengah1 == LOW || lineTengah2 == LOW){
    // kanan dan kirinya kosong, jadi maju lurus aja
    state = MAJU;
  }
  else if(lineKiri == LOW){
    state = BELOKKIRIMAJU;
  }
  else if(lineKanan == LOW){
    state = BELOKKANANMAJU;
  }
  else{
    state = MAJU;
  }
  
  // jika kondisi maju, maka pinMaju di high kan
  if(state == MAJU){
    rodaKiri.maju();
    rodaKanan.maju();  
  }
  // jika kondisi mundur, maka pinMundur di high kan
  else if (state == MUNDUR){
    rodaKiri.maju();
    rodaKanan.mundur();
  }
  else if (state == BELOKKANANMAJU){
    rodaKiri.maju();
    rodaKanan.diam();
  }
  else if (state == BELOKKIRIMAJU){
    rodaKiri.diam();
    rodaKanan.maju();
  }
  else if (state == BELOKKANANMUNDUR){
    rodaKiri.mundur();
    rodaKanan.diam();
  }
  else if (state == BELOKKIRIMUNDUR){
    rodaKiri.diam();
    rodaKanan.mundur();
  }
  else{
    rodaKiri.diam();
    rodaKanan.diam();
  }
}
