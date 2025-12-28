#include <stdio.h>  // Standart giris-çıkıs islemleri için gereken kütüphane
#include <stdlib.h>
#include <string.h> // String islemleri için gereken kütüphane

#define MAX_SEATS 10  // uçakta maksimum koltuk sayısını tanımlar
#define MAX_FLIGHTS 2 // Maksimum uçuş sayısını tanımlar
#define FILENAME "flight_reservations.txt" // Rezervasyon verilerinin saklanacağı dosya adı

// Flight structure
typedef struct {
    char flightCode[10];         // Uçus kodu
    int seats[MAX_SEATS];        // Koltuk durumu (1 = bos, 0 = dolu)
} Flight;

// Booking structure
typedef struct {
    char name[50];               // Yolcu adı
    char flightCode[10];         // Uçuþ kodu
    int seatNumber;              // Koltuk numarasi
} Booking;

// Global variables
Flight flights[MAX_FLIGHTS];      // Uçus bilgilerini tutan dizi
Booking bookings[MAX_SEATS * MAX_FLIGHTS]; // Tüm rezervasyonları tutan dizi
int bookingCount = 0;            // Toplam rezervasyon sayısı

// Fonksiyon prototipleri
void initialize_flights();
void load_data();
void display_menu();
void view_seats();
void book_ticket();
void view_bookings();
void cancel_reservation();
void save_data();

// Initialize flights with default data
void initialize_flights() {                   // flights dizisine FL001 ve FL002 kodlu uçuşları tanımlar
    strcpy(flights[0].flightCode, "FL001");   // İlk uçuşun kodunu "FL001" olarak ayarlar
    strcpy(flights[1].flightCode, "FL002");   // İkinci uçusun kodunu "FL002" olarak ayarlar
    for (int i = 0; i < MAX_FLIGHTS; i++) {   // MAX_FLI  GHTS adedinde uçuş tanımlar
        for (int j = 0; j < MAX_SEATS; j++) { //
            flights[i].seats[j] = 1;          // Tüm koltukları bos olarak işaretledi
        }
    }
}

// Load data from file
void load_data() {
    FILE *file = fopen(FILENAME, "r"); // Dosyayı okuma modunda açar
    if (file == NULL) {                // Eğer dosya yoksa
        printf("Data file not found. Initializing default flights.\n");
        initialize_flights();         // Varsayılan uçusları baslatır
        bookingCount = 0;             // Rezervasyon sayısını sıfırlar
        return;
    }

    // Load flights
    for (int i = 0; i < MAX_FLIGHTS; i++) {   // Tüm uçuslar için döngü
        fscanf(file, "Flight %s\n", flights[i].flightCode); // Uçus kodunu okur
        fscanf(file, "Seats: ");
        for (int j = 0; j < MAX_SEATS; j++) { // Her uçusun koltuklarını okur
            fscanf(file, "%d", &flights[i].seats[j]);
        }
        fscanf(file, "\n");  // Yeni satırı okur
    }

    // Load bookings
    fscanf(file, "Bookings (%d):\n", &bookingCount);  // Rezervasyon sayısını okur
    for (int i = 0; i < bookingCount; i++) {   // Her rezervasyon için döngü
        fscanf(file, "Name: %[^,], Flight: %[^,], Seat: %d\n",
               bookings[i].name, bookings[i].flightCode, &bookings[i].seatNumber);
    }

    fclose(file);  // Dosyayı kapatır
    printf("Data loaded successfully.\n");
}



// View available seats for each flight
void view_seats() {
    printf("\nAvailable Seats:\n");
    for (int i = 0; i < MAX_FLIGHTS; i++) {  // Tüm uçuslar için döngü
        printf("Flight %s: ", flights[i].flightCode); // Uçus kodunu yazdırır.
        for (int j = 0; j < MAX_SEATS; j++) {        // Her uçus için tüm koltukları döner
            printf("[%s] ", flights[i].seats[j] ? "Available" : "Booked");
        }    // Eğer koltuk bossa "Available", doluysa "Booked" yazdırır
        printf("\n");  // Yeni bir satır ile uçus bilgilerini ayırır
    }
}

// Book a ticket
void book_ticket() {
    if (bookingCount >= MAX_SEATS * MAX_FLIGHTS) {
        printf("Maximum booking limit reached.\n");
        return;
    }

    char flightCode[10], name[50];
    printf("Enter flight code: ");
    scanf("%s", flightCode); // Kullanıcıdan uçus kodunu alır

    int flightIndex = -1;
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        if (strcmp(flights[i].flightCode, flightCode) == 0) { //strcmp (string compare), iki karakter dizisini (C string) karşılaştırır.
            //str1 ve str2 aynı ise 0 döndürür. Bu kodun amacı, kullanıcı tarafından belirtilen uçuş kodunun flights dizisinde olup
            //olmadığını kontrol etmek ve yerini belirlemektir.

            flightIndex = i;// Uçuş kodu bulunduğunda, ilgili indeks kaydedilir.
            break;
        }
    }

    if (flightIndex == -1) {
        printf("Invalid flight code.\n");
        return;  // Eger uçus kodu bulunamazsa hata mesajı gösterilir ve islem sonlanır
    }

    int seatIndex = -1;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (flights[flightIndex].seats[i] == 1) {
            seatIndex = i; // İlk bos koltugun indeksini bulur.
            break;
        }
    }

    if (seatIndex == -1) {
        printf("No available seats on this flight.\n");
        return;  // Eğer bos koltuk bulunamazsa islem sonlanır
    }

    printf("Enter passenger name: ");
    scanf("%s", name); // Kullanıcıdan yolcu adını alır

    flights[flightIndex].seats[seatIndex] = 0; // Koltugu dolu olarak isaretler
    strcpy(bookings[bookingCount].name, name); // Rezervasyon bilgilerini kaydeder
    strcpy(bookings[bookingCount].flightCode, flightCode);
    bookings[bookingCount].seatNumber = seatIndex + 1;
    bookingCount++; // Rezervasyon sayısını artırır


    printf("Ticket booked successfully for %s on Flight %s, Seat %d.\n", name, flightCode, seatIndex + 1);
}

// View all bookings
void view_bookings() {
    if (bookingCount == 0) {  // kayıtlı rezervasyon yoksa No bookings found yazısını döndürür.
        printf("\nNo bookings found.\n");
        return;
    }

    printf("\nCurrent Bookings:\n");
    for (int i = 0; i < bookingCount; i++) { // tüm rezervasyonları tutan bookings dizisinden verileri bookingCount indexine kadar yazdırır.
        printf("Passenger Name: %s, Flight Code: %s, Seat Number: %d\n",
               bookings[i].name, bookings[i].flightCode, bookings[i].seatNumber);
    }
}



// Cancel a reservation
void cancel_reservation() {
    char name[50];
    printf("Enter passenger name to cancel reservation: ");
    scanf("%s", name); // Kullanıcıdan iptal edilecek yolcunun adını alır


    int bookingIndex = -1;
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].name, name) == 0) {  // İsmi girilen kişi dizide bulunuyorsa string compare 0 döndürür
                                                    // ve if döngüsüne girilir ve ismin bulundugu index degeri olan i bookingIndex değişkenine atanır.
            bookingIndex = i;
            break;
        }
    }

    if (bookingIndex == -1) {
        printf("No reservation found for this passenger.\n");
        return; // Eger rezervasyon bulunamazsa işlem sonlanır
    }

    int flightIndex = -1;
    for (int i = 0; i < MAX_FLIGHTS; i++) {
        if (strcmp(flights[i].flightCode, bookings[bookingIndex].flightCode) == 0) {
            flightIndex = i;  // Uçuş kodu bulunur
            break;
        }
    }

    int seatIndex = bookings[bookingIndex].seatNumber - 1;
    flights[flightIndex].seats[seatIndex] = 1; // Koltugu "bos" yapar

    for (int i = bookingIndex; i < bookingCount - 1; i++) {
        bookings[i] = bookings[i + 1]; // Rezervasyonlar arasındaki bosluğu kapatır
    }
    bookingCount--;  // Rezervasyon sayısını azaltır

    printf("Reservation for %s has been canceled.\n", name);
}

// datayı dosyaya yazar
void save_data() {
    FILE *file = fopen(FILENAME, "w");  // Dosyayı yazma modunda açar
    if (file == NULL) {                 // Eger dosya açılamazsa
        printf("Error: Could not save data.\n");
        return;
    }

    // Save flights
    for (int i = 0; i < MAX_FLIGHTS; i++) { // Tüm uçuslar için döngü
        fprintf(file, "Flight %s\n", flights[i].flightCode); // Uçus kodunu yazar
        fprintf(file, "Seats: ");
        for (int j = 0; j < MAX_SEATS; j++) {  // Her uçusun koltuklarını yazar

            fprintf(file, "%d ", flights[i].seats[j]);
        }
        fprintf(file, "\n");
    }

    // Save bookings
    fprintf(file, "Bookings (%d):\n", bookingCount); // Rezervasyon sayısını yazar
    for (int i = 0; i < bookingCount; i++) {     // Her rezervasyon için döngü
        fprintf(file, "Name: %s, Flight: %s, Seat: %d\n",
                bookings[i].name, bookings[i].flightCode, bookings[i].seatNumber);
    }

    fclose(file);
    printf("Data saved successfully.\n");
}

// Display user menu
void display_menu() {
    printf("\n--- Flight Reservation System ---\n");
    printf("1. View Available Seats\n");
    printf("2. Book a Ticket\n");
    printf("3. View all bookings\n");
    printf("4. Cancel a Reservation\n");
    printf("5. Exit\n");
    printf("----------------------------------\n");
}


// Main function
int main() {
    load_data();  // Program başlatıldığında veriler dosyadan yüklenir

    int choice;
    while (1) {  // Sürekli döngü, kullanıcı menüden çıkış yapana kadar devam eder
        display_menu(); // Kullanıcıya seçenekler gösterilir
        printf("Enter your choice: ");
        scanf("%d", &choice); // Kullanıcının seçim yapmasını sağlar

        switch (choice) {
            case 1:
                view_seats();  // Koltukları görüntüler
                break;
            case 2:
                book_ticket();  // Rezervasyon yapar
                break;
            case 3:
                view_bookings(); // tüm rezervasyonları komut ekranınd gösterir
                break;
            case 4:
                cancel_reservation(); // Rezervasyonu iptal eder
                break;
            case 5:
                save_data();  // Verileri dosyaya kaydeder
                printf("Exiting the system. Goodbye!\n");
                return 0; // Programdan çıkış yapar
            default:
                printf("Invalid choice. Please try again.\n"); // Geçersiz seçim iin uyarı
        }
    }

    return 0;
}
