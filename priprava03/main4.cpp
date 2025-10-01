#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <vector>
#include <string>



typedef struct {
    char name[256];
    struct stat st;
} mystat;

typedef struct {
    char name[256];
    long prev_size;
} file_monitor;

// Porovnávací funkce pro qsort - řazení podle velikosti
int compare_by_size(const void *a, const void *b) {
    mystat *fa = (mystat *)a;
    mystat *fb = (mystat *)b;
    
    if (fa->st.st_size < fb->st.st_size) return -1;
    if (fa->st.st_size > fb->st.st_size) return 1;
    return 0;
}

// Funkce pro výpis práv souboru
void print_mode(mode_t mode) {
    char str[11];
    
    // Typ souboru
    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else str[0] = '-';
    
    // Práva vlastníka
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    
    // Práva skupiny
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    
    // Práva ostatních
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    
    str[10] = '\0';
    
    printf("%10s", str);
}

// Funkce pro kontrolu, zda je soubor spustitelný
int is_executable(mode_t mode) {
    return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

// Funkce pro výpis prvních N řádků ze souboru
void print_first_lines(const char *filename, int num_lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    
    char line[1024];
    int count = 0;
    
    while (count < num_lines && fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        count++;
    }
    
    fclose(file);
}

// Funkce pro výpis posledních N řádků ze souboru - jako tail -N
void print_last_lines(const char *filename, int num_lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    
    // Zjistíme velikost souboru
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    
    if (file_size == 0) {
        fclose(file);
        return;
    }
    
    // Začneme od konce souboru a půjdeme pozpátku
    long pos = file_size - 1;
    int newlines_found = 0;
    
    // Hledáme num_lines odřádkování pozpátku
    while (pos >= 0 && newlines_found < num_lines) {
        fseek(file, pos, SEEK_SET);
        if (fgetc(file) == '\n') {
            newlines_found++;
        }
        pos--;
    }
    
    // Pokud jsme našli dostatek řádků, začneme od dalšího znaku
    // Jinak začneme od začátku souboru
    long start_pos;
    if (newlines_found == num_lines) {
        start_pos = pos + 2; // pos + 1 (protože pos-- na konci) + 1 (přeskočit \n)
    } else {
        start_pos = 0; // soubor má méně řádků než požadujeme
    }
    
    // Nastavíme pozici a vypíšeme zbytek
    fseek(file, start_pos, SEEK_SET);
    
    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    
    fclose(file);
}

// Funkce pro výpis nových dat ze souboru
void print_new_data(const char *filename, off_t start_pos) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    
    fseek(file, start_pos, SEEK_SET);
    
    printf("Nová data:\n");
    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    printf("\n");
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použití: %s <soubor1> <soubor2> ...\n", argv[0]);
        return 1;
    }
    
    // Alokace pole pro struktury mystat
    mystat *files = (mystat *)malloc((argc - 1) * sizeof(mystat));

    if (files == NULL) {
        perror("malloc");
        return 1;
    }
    
    int count = 0;
    
    // Projdeme všechny zadané soubory
    for (int i = 1; i < argc; i++) {
        struct stat st;
        
        // Získání informací o souboru
        if (stat(argv[i], &st) < 0) {
            // Soubor neexistuje nebo nelze přistupovat
            continue;
        }
        
        // Vynecháme adresáře
        if (S_ISDIR(st.st_mode)) {
            continue;
        }
        
        // Vynecháme spustitelné soubory
        if (is_executable(st.st_mode)) {
            continue;
        }
        
        // Kontrola, zda lze soubor otevřít pro čtení
        if (access(argv[i], R_OK) != 0) {
            continue;
        }
        
        // Přidáme soubor do seznamu
        strncpy(files[count].name, argv[i], sizeof(files[count].name) - 1);
        files[count].name[sizeof(files[count].name) - 1] = '\0';
        files[count].st = st;
        count++;
    }
    
    // Seřadíme soubory podle velikosti
    qsort(files, count, sizeof(mystat), compare_by_size);
    
    // Vypíšeme soubory
    printf("Počáteční stav souborů:\n");
    for (int i = 0; i < count; i++) {
        // mode (šířka 10)
        print_mode(files[i].st.st_mode);
        printf(" ");
        
        // size (šířka 10)
        printf("%10ld", files[i].st.st_size);
        printf(" ");
        
        // time (formát)
        char timestr[64];
        struct tm *tm_info = localtime(&files[i].st.st_mtime);
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s", timestr);
        printf(" ");
        
        // name (zarovnané zleva)
        printf("%s", files[i].name);
        printf("\n");
        
        // Oddělovač, prvních 5 řádků, oddělovač, posledních 5 řádků, oddělovač
        printf("---------\n");
        print_first_lines(files[i].name, 5);
        printf("---------\n");
        print_last_lines(files[i].name, 5);
        printf("\n---------\n");
    }
    
    // Vytvoříme pole pro monitorování velikostí
    file_monitor *monitors = (file_monitor *)malloc(count * sizeof(file_monitor));
    if (monitors == NULL) {
        perror("malloc");
        free(files);
        return 1;
    }
    
    // Inicializujeme monitory s aktuálními velikostmi
    for (int i = 0; i < count; i++) {
        strncpy(monitors[i].name, files[i].name, sizeof(monitors[i].name) - 1);
        monitors[i].name[sizeof(monitors[i].name) - 1] = '\0';
        monitors[i].prev_size = files[i].st.st_size;
    }
    
    printf("\nZačínám monitorování souborů...\n");
    
    // Nekonečná smyčka monitorování
    while (1) {
        sleep(1);
        
        for (int i = 0; i < count; i++) {
            struct stat current_st;
            
            // Získání aktuálních informací o souboru
            if (stat(monitors[i].name, &current_st) < 0) {
                continue;
            }
            
            // Kontrola změny velikosti
            if (current_st.st_size != monitors[i].prev_size) {
                printf("\nZměna velikosti souboru: %s\n", monitors[i].name);
                printf("Předchozí velikost: %ld bytes\n", monitors[i].prev_size);
                printf("Nová velikost: %ld bytes\n", current_st.st_size);
                
                // Pokud se soubor zvětšil, vypíšeme nová data
                if (current_st.st_size > monitors[i].prev_size) {
                    print_new_data(monitors[i].name, monitors[i].prev_size);
                }
                
                // Aktualizujeme uloženou velikost
                monitors[i].prev_size = current_st.st_size;
            }
        }
    }
    
    // Tento kód se nikdy nespustí kvůli nekonečné smyčce
    free(monitors);
    free(files);
    return 0;
}