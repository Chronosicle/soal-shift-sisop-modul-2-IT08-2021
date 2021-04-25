#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <zip.h>
#include <wait.h>
#include <dirent.h>

void download(char *link, char *tujuan){
  pid_t id;
  id = fork();

  if(id == 0){
    //child process
    char *argv[6] = {"wget", "--no-check-certificate", "-q", link, "-O", tujuan};
    execv("/bin/wget", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    wait(NULL);
  }
}

void extract(char *tujuan){
  pid_t id;int kondisi;
  id = fork();

  if(id == 0){
    char *argv[4] = {"unzip", "-q", tujuan};
    execv("/usr/bin/unzip", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    while((wait(&kondisi)) > 0);
  }
}

void zip(char *source1, char *source2, char *source3){
  pid_t id; int kondisi;
  id = fork();

  if(id == 0){
    char *argv[8] = {"zip", "-r", "-q", "Lopyu_Stevany.zip", source1, source2, source3};
    execv("/usr/bin/zip", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    while((wait(&kondisi)) > 0 );
  }
}

void hapus(){
  pid_t id;
  id = fork();

  if (id == 0){
    char *argv[8] = {"rm", "-rf", "FOTO", "MUSIK", "FILM", "Musyik", "Fylm", "Pyoto"};
    execv("/bin/rm", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    wait(NULL);
  }
}

void buat_folder(char *tujuan){
  pid_t id;
  id = fork();
  if(id == 0){
    char *argv[] = {"mkdir", "-p", tujuan, NULL};
    execv("/bin/mkdir", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    wait(NULL);
  }
}

void copy(char *asal, char *tujuan){
  pid_t id;
  id = fork();
  if(id == 0){
    char *argv[] = {"cp", "-R", asal, tujuan, NULL};
    execv("/bin/cp", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    wait(NULL);
  }
}

void hapus_file(char *source){
  char nama_file[40]; int kondisi;
  sprintf(nama_file, "%s", source);
  
  pid_t id;
  id = fork();
  if(id == 0){
    char *argv[] = {"rm", nama_file};
    execv("/bin/rm", argv);
    exit(EXIT_SUCCESS);
  }else if(id > 0){
    while(wait(&kondisi) > 0);
  }
}

void check(char *source){
  char nama_file[40];
  sprintf(nama_file, "%s", source);

  DIR *folder = opendir(source);
  if(folder){
    printf("ini folder");
  }else{
    int count = strlen(nama_file);
    if(nama_file[count-3] != 'j'){
      hapus_file(nama_file);
    }else{
      printf("ini jpg");
    }
  }
  closedir(folder);
} 

int main() {
    struct tm ultah = {0, 22, 22, 9, 3, 121};
    time_t ulangtahun = mktime(&ultah);
    pid_t pid, sid;
    char musik[10] = "Musyik";
    char film[10] = "Fylm";
    char foto[10] = "Pyoto";
    char link_film[150] = "https://drive.google.com/u/0/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download";
    char link_musik[200] = "https://drive.google.com/u/0/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download";
    char link_foto[150] = "https://drive.google.com/u/0/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download";
    char hasil1[10] = "MUSIK.zip";
    char hasil2[10] = "FILM.zip";
    char hasil3[10] = "FOTO.zip";

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  chdir("/home/kali/SISOP/modul_2/shift");
  while(1){
    time_t now = time(NULL);
    double beda = difftime(ulangtahun, now);
    if(beda > 0 && beda == 21600){
      buat_folder(musik);
      download(link_musik, hasil1);
      extract(hasil1);
      copy("MUSIK/.", musik);

      buat_folder(film);
      download(link_film, hasil2);
      extract(hasil2);
      copy("FILM/.", film);

      buat_folder(foto);
      download(link_foto, hasil3);
      extract(hasil3);
      struct dirent *cek;
      DIR *folder = opendir("FOTO");
      if(!folder) exit(EXIT_FAILURE);
      chdir("FOTO");
      while(cek = readdir(folder)){
        if(strcmp(cek->d_name, ".") != 0 && strcmp(cek->d_name, "..") != 0){
          check(cek->d_name);
        }
      }
      chdir("..");
      copy("FOTO/.", foto);
      break;
    }
  }
    
  while(1){
    time_t now = time(NULL);
    double beda = difftime(ulangtahun, now);
    if(beda < 1 && beda > -1){
      zip(musik, film, foto);
      hapus();
      break;
    }
  }
}