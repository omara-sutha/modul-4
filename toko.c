#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struktur data barang
typedef struct {
    char kode_barang[20];
    int jumlah_stok;
    char nama_barang[50];
    char jenis_barang[30];
    int harga_barang;  // Tambahan field harga
} Barang;

// Struktur data transaksi
typedef struct {
    char kode_transaksi[20];
    char waktu[30];
    char kode_barang[20];
    char nama_barang[50];  // Tambahan nama barang
    int jumlah;
    int harga_satuan;      // Tambahan harga satuan
    int total_harga;
} Transaksi;

// Fungsi untuk membuat backup file
void backup_file(const char *filename) {
    char backup_name[100];
    char command[200];
    
    sprintf(backup_name, "backup_%s", filename);
    
    #ifdef _WIN32
        sprintf(command, "copy %s %s > nul", filename, backup_name);
    #else
        sprintf(command, "cp %s %s", filename, backup_name);
    #endif
    
    system(command);
    printf("Backup file '%s' berhasil dibuat.\n", backup_name);
}

// Fungsi untuk menyimpan data barang ke file CSV
void simpan_barang(Barang *barang_list, int jumlah) {
    FILE *file = fopen("barang.csv", "w");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file barang.csv\n");
        return;
    }
    
    // Backup file lama jika ada
    backup_file("barang.csv");
    
    fprintf(file, "Kode Barang,Jumlah Stok,Nama Barang,Jenis Barang,Harga Barang\n");
    for (int i = 0; i < jumlah; i++) {
        fprintf(file, "%s,%d,%s,%s,%d\n", 
                barang_list[i].kode_barang, 
                barang_list[i].jumlah_stok,
                barang_list[i].nama_barang,
                barang_list[i].jenis_barang,
                barang_list[i].harga_barang);  // Tambahan harga
    }
    
    fclose(file);
    printf("Data barang berhasil disimpan ke barang.csv\n");
}

// Fungsi untuk membaca data barang dari file CSV
int baca_barang(Barang *barang_list) {
    FILE *file = fopen("barang.csv", "r");
    if (file == NULL) {
        printf("File barang.csv tidak ditemukan. Membuat file baru.\n");
        return 0;
    }
    
    char line[300];
    int jumlah = 0;
    
    // Skip header
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) && jumlah < 100) {
        char *token = strtok(line, ",");
        if (token != NULL) strcpy(barang_list[jumlah].kode_barang, token);
        
        token = strtok(NULL, ",");
        if (token != NULL) barang_list[jumlah].jumlah_stok = atoi(token);
        
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(barang_list[jumlah].nama_barang, token);
        
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(barang_list[jumlah].jenis_barang, token);
        
        token = strtok(NULL, "\n");
        if (token != NULL) barang_list[jumlah].harga_barang = atoi(token);  // Tambahan baca harga
        
        jumlah++;
    }
    
    fclose(file);
    return jumlah;
}

// Fungsi untuk menambah barang baru
int tambah_barang(Barang *barang_list, int jumlah) {
    if (jumlah >= 100) {
        printf("Maaf, kapasitas maksimum barang tercapai (100 barang).\n");
        return jumlah;
    }
    
    Barang barang_baru;
    
    printf("\n=== TAMBAH BARANG BARU ===\n");
    printf("Masukkan kode barang: ");
    scanf("%s", barang_baru.kode_barang);
    
    // Cek duplikasi kode barang
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(barang_list[i].kode_barang, barang_baru.kode_barang) == 0) {
            printf("Error: Kode barang sudah ada!\n");
            return jumlah;
        }
    }
    
    printf("Masukkan nama barang: ");
    scanf(" %[^\n]", barang_baru.nama_barang);
    
    printf("Masukkan jenis barang: ");
    scanf(" %[^\n]", barang_baru.jenis_barang);
    
    printf("Masukkan harga barang: Rp ");
    scanf("%d", &barang_baru.harga_barang);  // Tambahan input harga
    
    printf("Masukkan jumlah stok: ");
    scanf("%d", &barang_baru.jumlah_stok);
    
    barang_list[jumlah] = barang_baru;
    jumlah++;
    
    simpan_barang(barang_list, jumlah);
    printf("Barang berhasil ditambahkan!\n");
    
    return jumlah;
}

// Fungsi untuk mengubah data barang
void ubah_barang(Barang *barang_list, int jumlah) {
    char kode[20];
    int found = 0;
    
    printf("\n=== UBAH DATA BARANG ===\n");
    printf("Masukkan kode barang yang akan diubah: ");
    scanf("%s", kode);
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(barang_list[i].kode_barang, kode) == 0) {
            found = 1;
            printf("Data barang ditemukan:\n");
            printf("Kode: %s\n", barang_list[i].kode_barang);
            printf("Nama: %s\n", barang_list[i].nama_barang);
            printf("Jenis: %s\n", barang_list[i].jenis_barang);
            printf("Harga: Rp %d\n", barang_list[i].harga_barang);  // Tambahan tampilan harga
            printf("Stok: %d\n", barang_list[i].jumlah_stok);
            
            printf("\nMasukkan data baru:\n");
            printf("Nama barang baru: ");
            scanf(" %[^\n]", barang_list[i].nama_barang);
            printf("Jenis barang baru: ");
            scanf(" %[^\n]", barang_list[i].jenis_barang);
            printf("Harga barang baru: Rp ");
            scanf("%d", &barang_list[i].harga_barang);  // Tambahan input harga
            printf("Jumlah stok baru: ");
            scanf("%d", &barang_list[i].jumlah_stok);
            
            simpan_barang(barang_list, jumlah);
            printf("Data barang berhasil diubah!\n");
            break;
        }
    }
    
    if (!found) {
        printf("Barang dengan kode %s tidak ditemukan.\n", kode);
    }
}

// Fungsi untuk menghapus barang
int hapus_barang(Barang *barang_list, int jumlah) {
    char kode[20];
    int found = 0;
    
    printf("\n=== HAPUS BARANG ===\n");
    printf("Masukkan kode barang yang akan dihapus: ");
    scanf("%s", kode);
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(barang_list[i].kode_barang, kode) == 0) {
            found = 1;
            printf("Data barang yang akan dihapus:\n");
            printf("Kode: %s, Nama: %s, Harga: Rp %d, Stok: %d\n",
                   barang_list[i].kode_barang, barang_list[i].nama_barang,
                   barang_list[i].harga_barang, barang_list[i].jumlah_stok);
            
            printf("Yakin ingin menghapus? (y/n): ");
            char konfirmasi;
            scanf(" %c", &konfirmasi);
            
            if (konfirmasi == 'y' || konfirmasi == 'Y') {
                // Geser data ke kiri
                for (int j = i; j < jumlah - 1; j++) {
                    barang_list[j] = barang_list[j + 1];
                }
                jumlah--;
                
                simpan_barang(barang_list, jumlah);
                printf("Barang berhasil dihapus!\n");
            } else {
                printf("Penghapusan dibatalkan.\n");
            }
            break;
        }
    }
    
    if (!found) {
        printf("Barang dengan kode %s tidak ditemukan.\n", kode);
    }
    
    return jumlah;
}

// Fungsi untuk menampilkan barang berdasarkan jenis
void tampilkan_berdasarkan_jenis(Barang *barang_list, int jumlah) {
    char jenis[30];
    
    printf("\n=== TAMPILKAN BARANG BERDASARKAN JENIS ===\n");
    printf("Masukkan jenis barang: ");
    scanf(" %[^\n]", jenis);
    
    printf("\n%-15s %-25s %-15s %-12s %-10s\n", "Kode", "Nama", "Jenis", "Harga", "Stok");
    printf("------------------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(barang_list[i].jenis_barang, jenis) == 0) {
            printf("%-15s %-25s %-15s Rp %-9d %-10d\n",
                   barang_list[i].kode_barang,
                   barang_list[i].nama_barang,
                   barang_list[i].jenis_barang,
                   barang_list[i].harga_barang,  // Tambahan tampilan harga
                   barang_list[i].jumlah_stok);
            found = 1;
        }
    }
    
    if (!found) {
        printf("Tidak ada barang dengan jenis '%s'.\n", jenis);
    }
}

// Fungsi untuk menampilkan barang dengan stok habis
void tampilkan_stok_habis(Barang *barang_list, int jumlah) {
    printf("\n=== BARANG DENGAN STOK HABIS ===\n");
    printf("%-15s %-25s %-15s %-12s %-10s\n", "Kode", "Nama", "Jenis", "Harga", "Stok");
    printf("------------------------------------------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < jumlah; i++) {
        if (barang_list[i].jumlah_stok == 0) {
            printf("%-15s %-25s %-15s Rp %-9d %-10d\n",
                   barang_list[i].kode_barang,
                   barang_list[i].nama_barang,
                   barang_list[i].jenis_barang,
                   barang_list[i].harga_barang,  // Tambahan tampilan harga
                   barang_list[i].jumlah_stok);
            found = 1;
        }
    }
    
    if (!found) {
        printf("Tidak ada barang dengan stok habis.\n");
    }
}

// Fungsi untuk menyimpan transaksi ke file
void simpan_transaksi(Transaksi transaksi) {
    FILE *file = fopen("transaksi.csv", "a");
    if (file == NULL) {
        printf("Error: Tidak dapat membuka file transaksi.csv\n");
        return;
    }
    
    // Backup file jika ini transaksi pertama
    static int first_transaction = 1;
    if (first_transaction) {
        backup_file("transaksi.csv");
        fprintf(file, "Kode Transaksi,Waktu,Kode Barang,Nama Barang,Jumlah,Harga Satuan,Total Harga\n");
        first_transaction = 0;
    }
    
    fprintf(file, "%s,%s,%s,%s,%d,%d,%d\n",
            transaksi.kode_transaksi,
            transaksi.waktu,
            transaksi.kode_barang,
            transaksi.nama_barang,
            transaksi.jumlah,
            transaksi.harga_satuan,
            transaksi.total_harga);
    
    fclose(file);
}

// Fungsi untuk melakukan transaksi penjualan
void transaksi_jual(Barang *barang_list, int jumlah) {
    char kode_barang[20];
    int jumlah_beli;
    int found = 0;
    
    printf("\n=== TRANSAKSI PENJUALAN ===\n");
    printf("Masukkan kode barang: ");
    scanf("%s", kode_barang);
    
    for (int i = 0; i < jumlah; i++) {
        if (strcmp(barang_list[i].kode_barang, kode_barang) == 0) {
            found = 1;
            
            if (barang_list[i].jumlah_stok == 0) {
                printf("Maaf, stok barang ini habis!\n");
                return;
            }
            
            printf("\nDetail Barang:\n");
            printf("Nama: %s\n", barang_list[i].nama_barang);
            printf("Harga: Rp %d\n", barang_list[i].harga_barang);
            printf("Stok tersedia: %d\n", barang_list[i].jumlah_stok);
            
            printf("\nMasukkan jumlah yang dibeli: ");
            scanf("%d", &jumlah_beli);
            
            if (jumlah_beli > barang_list[i].jumlah_stok) {
                printf("Maaf, stok tidak mencukupi!\n");
                return;
            }
            
            if (jumlah_beli <= 0) {
                printf("Jumlah pembelian tidak valid!\n");
                return;
            }
            
            // Hitung total harga otomatis
            int total_harga = jumlah_beli * barang_list[i].harga_barang;
            
            printf("\nRincian Pembelian:\n");
            printf("%d x Rp %d = Rp %d\n", jumlah_beli, barang_list[i].harga_barang, total_harga);
            
            // Konfirmasi transaksi
            printf("\nKonfirmasi transaksi? (y/n): ");
            char konfirmasi;
            scanf(" %c", &konfirmasi);
            
            if (konfirmasi == 'y' || konfirmasi == 'Y') {
                // Update stok
                barang_list[i].jumlah_stok -= jumlah_beli;
                
                // Buat transaksi
                Transaksi transaksi;
                
                // Generate kode transaksi
                time_t now = time(NULL);
                sprintf(transaksi.kode_transaksi, "TRX%d", (int)now);
                
                // Format waktu
                strftime(transaksi.waktu, sizeof(transaksi.waktu), "%Y-%m-%d %H:%M:%S", localtime(&now));
                
                strcpy(transaksi.kode_barang, kode_barang);
                strcpy(transaksi.nama_barang, barang_list[i].nama_barang);
                transaksi.jumlah = jumlah_beli;
                transaksi.harga_satuan = barang_list[i].harga_barang;
                transaksi.total_harga = total_harga;
                
                // Simpan transaksi
                simpan_transaksi(transaksi);
                
                // Update file barang
                simpan_barang(barang_list, jumlah);
                
                printf("\n✅ Transaksi berhasil!\n");
                printf("========================================\n");
                printf("Kode Transaksi : %s\n", transaksi.kode_transaksi);
                printf("Waktu          : %s\n", transaksi.waktu);
                printf("Barang         : %s\n", barang_list[i].nama_barang);
                printf("Jumlah         : %d\n", jumlah_beli);
                printf("Harga Satuan   : Rp %d\n", barang_list[i].harga_barang);
                printf("Total Harga    : Rp %d\n", total_harga);
                printf("Sisa Stok      : %d\n", barang_list[i].jumlah_stok);
                printf("========================================\n");
            } else {
                printf("Transaksi dibatalkan.\n");
            }
            
            break;
        }
    }
    
    if (!found) {
        printf("Barang dengan kode %s tidak ditemukan.\n", kode_barang);
    }
}

// Fungsi untuk menampilkan transaksi berdasarkan kode
void tampilkan_transaksi() {
    char kode[20];
    
    printf("\n=== CARI TRANSAKSI ===\n");
    printf("Masukkan kode transaksi: ");
    scanf("%s", kode);
    
    FILE *file = fopen("transaksi.csv", "r");
    if (file == NULL) {
        printf("Belum ada data transaksi.\n");
        return;
    }
    
    char line[300];
    int found = 0;
    
    // Skip header
    fgets(line, sizeof(line), file);
    
    printf("\n%-15s %-20s %-15s %-20s %-10s %-15s %-15s\n", 
           "Kode", "Waktu", "Kode Barang", "Nama Barang", "Jumlah", "Harga Satuan", "Total Harga");
    printf("------------------------------------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), file)) {
        char kode_trx[20], waktu[30], kode_brg[20], nama_brg[50];
        int jumlah, harga_satuan, total;
        
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d,%d", 
               kode_trx, waktu, kode_brg, nama_brg, &jumlah, &harga_satuan, &total);
        
        if (strcmp(kode_trx, kode) == 0) {
            printf("%-15s %-20s %-15s %-20s %-10d Rp %-12d Rp %-12d\n", 
                   kode_trx, waktu, kode_brg, nama_brg, jumlah, harga_satuan, total);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("Transaksi dengan kode %s tidak ditemukan.\n", kode);
    }
}

// Fungsi untuk menampilkan semua transaksi
void tampilkan_semua_transaksi() {
    FILE *file = fopen("transaksi.csv", "r");
    if (file == NULL) {
        printf("Belum ada data transaksi.\n");
        return;
    }
    
    char line[300];
    int total_transaksi = 0;
    long total_pendapatan = 0;
    
    // Skip header
    fgets(line, sizeof(line), file);
    
    printf("\n=== RIWAYAT SEMUA TRANSAKSI ===\n");
    printf("%-15s %-20s %-15s %-20s %-10s %-15s %-15s\n", 
           "Kode", "Waktu", "Kode Barang", "Nama Barang", "Jumlah", "Harga Satuan", "Total Harga");
    printf("------------------------------------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), file)) {
        char kode_trx[20], waktu[30], kode_brg[20], nama_brg[50];
        int jumlah, harga_satuan, total;
        
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d,%d", 
               kode_trx, waktu, kode_brg, nama_brg, &jumlah, &harga_satuan, &total);
        
        printf("%-15s %-20s %-15s %-20s %-10d Rp %-12d Rp %-12d\n", 
               kode_trx, waktu, kode_brg, nama_brg, jumlah, harga_satuan, total);
        
        total_transaksi++;
        total_pendapatan += total;
    }
    
    fclose(file);
    
    printf("------------------------------------------------------------------------------------------------\n");
    printf("Total Transaksi : %d\n", total_transaksi);
    printf("Total Pendapatan: Rp %ld\n", total_pendapatan);
}

// Fungsi utama
int main() {
    Barang barang_list[100];
    int jumlah_barang = 0;
    int pilihan;
    
    // Backup file saat program dibuka
    printf("Membuat backup file...\n");
    backup_file("barang.csv");
    backup_file("transaksi.csv");
    
    // Baca data barang yang ada
    jumlah_barang = baca_barang(barang_list);
    printf("Data barang berhasil dimuat. Jumlah barang: %d\n", jumlah_barang);
    
    do {
        printf("\n========================================\n");
        printf("       SISTEM MANAJEMEN TOKO\n");
        printf("========================================\n");
        printf("1. Tampilkan Semua Barang\n");
        printf("2. Tambah Barang Baru\n");
        printf("3. Ubah Data Barang\n");
        printf("4. Hapus Barang\n");
        printf("5. Tampilkan Berdasarkan Jenis\n");
        printf("6. Tampilkan Stok Habis\n");
        printf("7. Transaksi Penjualan\n");
        printf("8. Cari Transaksi\n");
        printf("9. Tampilkan Semua Transaksi\n");
        printf("10. Keluar\n");
        printf("========================================\n");
        printf("Pilih menu (1-10): ");
        scanf("%d", &pilihan);
        
        switch(pilihan) {
            case 1:
                printf("\n=== DAFTAR SEMUA BARANG ===\n");
                printf("%-15s %-25s %-15s %-12s %-10s\n", "Kode", "Nama", "Jenis", "Harga", "Stok");
                printf("------------------------------------------------------------------------\n");
                for (int i = 0; i < jumlah_barang; i++) {
                    printf("%-15s %-25s %-15s Rp %-9d %-10d\n",
                           barang_list[i].kode_barang,
                           barang_list[i].nama_barang,
                           barang_list[i].jenis_barang,
                           barang_list[i].harga_barang,  // Tambahan tampilan harga
                           barang_list[i].jumlah_stok);
                }
                if (jumlah_barang == 0) {
                    printf("Belum ada data barang.\n");
                }
                break;
                
            case 2:
                jumlah_barang = tambah_barang(barang_list, jumlah_barang);
                break;
                
            case 3:
                ubah_barang(barang_list, jumlah_barang);
                break;
                
            case 4:
                jumlah_barang = hapus_barang(barang_list, jumlah_barang);
                break;
                
            case 5:
                tampilkan_berdasarkan_jenis(barang_list, jumlah_barang);
                break;
                
            case 6:
                tampilkan_stok_habis(barang_list, jumlah_barang);
                break;
                
            case 7:
                transaksi_jual(barang_list, jumlah_barang);
                break;
                
            case 8:
                tampilkan_transaksi();
                break;
                
            case 9:
                tampilkan_semua_transaksi();
                break;
                
            case 10:
                printf("\nTerima kasih telah menggunakan sistem ini!\n");
                simpan_barang(barang_list, jumlah_barang);
                break;
                
            default:
                printf("\nPilihan tidak valid! Silakan pilih 1-10.\n");
        }
        
    } while (pilihan != 10);
    
    return 0;
}