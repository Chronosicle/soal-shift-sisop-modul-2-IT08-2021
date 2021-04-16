#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

void extract(char *asal, char *tujuan){
    pid_t id_1; int kondisi;
    id_1 = fork();

    //mengunzip pets.zip ke folder petshop(tujuan)
    if(id_1 == 0){
        char *argv[5] = {"unzip", asal, "-d", tujuan};
        execv("/usr/bin/unzip", argv);
        exit(EXIT_SUCCESS);
    }else if(id_1 > 0){
        while(wait(&kondisi) > 0);
    }
}

void hapus(char *nama){
    char tujuan[20];
    sprintf(tujuan, "%s/", nama);

    pid_t id_2; int kondisi;
    id_2 = fork();

    if(id_2 == 0){
        //menghapus folder yang tidak penting
        char *argv[4] = {"rm", "-rf", tujuan};
        execv("/bin/rm", argv);
        exit(EXIT_SUCCESS);
    }else if(id_2 > 0){
        while(wait(&kondisi) > 0);
    }
}

void buat_folder(char *nama){
    char tujuan[40];
    sprintf(tujuan, "%s", nama);
    pid_t id_3; int kondisi;

    DIR *folder = opendir(nama);
    if(folder){
        return;
    }

    id_3 = fork();

    if(id_3 == 0){
        //membuat folder baru berdasarkan jenis hewan
        char *argv[3] = {"mkdir", tujuan};
        execv("/bin/mkdir", argv);
        exit(EXIT_SUCCESS);
    }else if(id_3 > 0){
        while(wait(&kondisi) > 0);
    }
}

void copy(char *folder, char *asal, char *tujuan){
    char source[30];int kondisi;
    sprintf(source, "%s", asal);

    char destination[40];
    sprintf(destination, "%s/%s.jpg", folder, tujuan);

    pid_t id_4;
    id_4 = fork();
    //mengcopy file ke folder sesuai jenis hewan nya
    if(id_4 == 0){
        char *argv[4] = {"cp", source, destination};
        execv("/bin/cp", argv);
        exit(EXIT_SUCCESS);
    }else if(id_4 > 0){
        while(wait(&kondisi) > 0);
    }
}

void keterangan (char *folder, char *nama_hewan, char *umur_hewan){
    char keterangan[50];
    sprintf(keterangan, "%s/keterangan.txt", folder);

    pid_t id_5;int kondisi;
    id_5 = fork();

    if(id_5 == 0){
        FILE *tulis;
        tulis = fopen(keterangan, "a");
        fprintf(tulis, "nama: %s\numur: %s\n\n", nama_hewan, umur_hewan);
        fclose(tulis);
        exit(EXIT_SUCCESS);
    }else if(id_5 > 0){
        while(wait(&kondisi) > 0);
    }
}

void check(char *source){
    char *nama_asal[40];
    sprintf(nama_asal, "%s", source);

    DIR *folder = opendir(source);

    //menghapus folder yang tidak penting
    if(folder){
        hapus(nama_asal);
    }else{
        char *token, *jenis_nama_umur[3];
        token = strtok(source, ";_");
        //fungsi perulangan jika terdapat lebih dari 1 hewan berdasarkan nama filenya
        while(token != NULL){
            //mendapatkan jenis, nama, umur hewan dengan fungsi strtok 
            int i = 0;
            while(i<3){
                jenis_nama_umur[i] = token;
                token = strtok(NULL, ";_");
                i++;
            }
            
            char *nama_folder = jenis_nama_umur[0];
            char *nama_hewan = jenis_nama_umur[1];
            char *umur_hewan = jenis_nama_umur[2];

            char *jpg;
            jpg = strstr(umur_hewan, ".jpg");
            if(jpg != NULL){
                int counter = jpg - umur_hewan;
                sprintf(umur_hewan, "%.*s", counter, umur_hewan);
            }

            buat_folder(nama_folder);
            copy(nama_folder, nama_asal, nama_hewan);
            keterangan(nama_folder, nama_hewan, umur_hewan);
        }
    }
    closedir(folder);
}

int main(){
    char source[10] = "pets.zip";
    char tujuan[10] = "petshop";

    chdir("/home/kali/SISOP/modul_2/shift");
    extract(source, tujuan);

    struct dirent *cek;
    DIR *folder = opendir(tujuan);

    if(!folder){
        exit(EXIT_FAILURE);
    }

    chdir("petshop");

    while (cek = readdir(folder)){
        //mengecek apakah nama filenya valid
        if(strcmp(cek->d_name, ".") != 0 && strcmp(cek->d_name, "..") != 0){
            check(cek->d_name);
        }
    }
    return 0;
}