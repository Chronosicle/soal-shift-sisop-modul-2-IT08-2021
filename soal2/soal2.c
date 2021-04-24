#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>

void copy(char *folder, char *asal, char *tujuan);
void buat_folder(char *nama);
void keterangan (char *folder, char *nama_hewan, char *umur_hewan);

int main(){
    pid_t id;
    id = fork();

    char asal[10] = "pets.zip";
    char tujuan[10] = "petshop";

    chdir("/home/kali/SISOP/modul_2/shift");

    if(id == 0){
        char *argv[5] = {"unzip", asal, "-d", tujuan};
        execv("/usr/bin/unzip", argv);
        exit(EXIT_SUCCESS);
    }else if(id > 0){
        wait(NULL);
    }

    struct dirent *cek;
    DIR *folder = opendir(tujuan);

    if(!folder){
        exit(EXIT_FAILURE);
    }

    chdir("petshop");

    while (cek = readdir(folder)){
        if(strcmp(cek->d_name, ".") != 0 && strcmp(cek->d_name, "..") != 0){
            char *source = cek->d_name;
            char nama_file[40];
            sprintf(nama_file, "%s", source);

            DIR *folder2 = opendir(source);

            char ga_penting[40], nama_asal[40];
            sprintf(ga_penting, "%s", source);
            sprintf(nama_asal, "%s", source);

            if(folder2){
                pid_t id2;
                int kondisi;

                id = fork();

                if(id == 0){
                    char *argv[4] = {"rm", "-rf", ga_penting};
                    execv("/bin/rm", argv);
                    exit(EXIT_SUCCESS);
                }else if(id2 > 0){
                    while(wait(&kondisi) > 0);
                }
            }else{
                char *nama_folder, *nama_hewan, *umur_hewan, *token, *jenis_nama_umur[3];
                token = strtok(source, ";_");
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
            closedir(folder2);
        }
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