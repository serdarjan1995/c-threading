/** Sardor Hazratov
    compile code with -pthread flag
    ```
        gcc -Wall -pthread code.c
    ```
**/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define DIR_NAME "thread_requests"


/* get random number between 1-10  */
int get_random_10(){
    return (rand() % 10) + 1;
}


/* create directory with the name defined in DIR_NAME */
void create_dir(){
    // create directory if it does not exists
    struct stat st = {0};

    if (stat(DIR_NAME, &st) == -1) {
        mkdir(DIR_NAME, 0700);
    }
}


/* create file with the filename formatted as TXXX-YYY.txt  */
void create_file(int thread_id, int request_id){
    char filename[20];
    char filepath[50];
    FILE *fptr;
    // format filename string with thread_id and request_id
    snprintf(filename, sizeof(filename), "T%03d-%03d.txt", thread_id, request_id);
    // concat filename and dir path
    snprintf(filepath, sizeof(filepath), "%s/%s", DIR_NAME, filename);
    //create file and close
    fptr = fopen(filepath, "w");
    fclose(fptr);
    printf("[CLNT-%d] %s created\n", thread_id, filename);
}


/* delete file */
void delete_file(char *filename){
    char filepath[50];
    snprintf(filepath, sizeof(filepath), "%s/%s", DIR_NAME, filename);
    if (remove(filepath) == 0) {
        printf("[SRV] %s deleted\n", filename);
    } else {
        printf("[SRV] !!!!The file is not deleted. %s\n", filename);
    }
}


/* creates random X files in the beginning of every minute */
void *create_file_every_minute(void *args){
    int number_of_requests = get_random_10();
    int thread_id = *((int *) args);
    int i;
    int request_id = 0;
    pthread_detach(pthread_self());
    
    while (1) {
        for (i=1; i<=number_of_requests; i++) {
            request_id += 1;
            create_file(thread_id, request_id);
        }
        sleep(60);  // wait 1 minute
    }
    free(args);
    pthread_exit(NULL);
}


void *delete_file_every_minute(void *args){
    DIR *directory;
    struct dirent *dir_content_item;
    int deleted_files_count;
    int files_to_delete = *((int *) args);
    time_t rawtime;
    struct tm * timeinfo;
    
    pthread_detach(pthread_self());

    // run indefinitely
    while (1) {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf("\n-----\n");
        printf( "[SRV] Current local datetime: %s", asctime(timeinfo));
        printf("\n-----\n");
        deleted_files_count = 0;
        directory = opendir(DIR_NAME);
        while ((dir_content_item = readdir(directory)) != NULL && deleted_files_count < files_to_delete) {
            // ensure item type is FILE; not direcory, symlinks ...
            if (dir_content_item->d_type == DT_REG) {
                delete_file(dir_content_item->d_name);
                deleted_files_count += 1;
            }
        }
        closedir(directory);
        
        // sleep 1 minute (60 seconds)
        sleep(60);
    }
    
    pthread_exit(NULL);
}


int main(){
    int client_thread_count = 5, server_capacity = 2;
    srand(time(NULL));
    pthread_t *t;
    pthread_t ptid_srv;
    int i;
    
    // get inputs from user
    printf("Client thread count N=");
    scanf("%d", &client_thread_count);
    printf("Server thread per minute capacity M=");
    scanf("%d", &server_capacity);
    printf("\n-----\n");

    // create directory in which the thread request files will be created
    create_dir();

    // start server thread
    pthread_create(&ptid_srv, NULL, delete_file_every_minute, &server_capacity);

    // allocate N threads
    t=(pthread_t *)malloc(client_thread_count * sizeof(pthread_t));
    for (i=0; i<client_thread_count; i++) {
        int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }
        *arg = i+1;
        pthread_create(&t[i], NULL, create_file_every_minute, arg);
    }
    
    // join threads | wait for termination
    pthread_join(ptid_srv, NULL); // server thread
    for (i=0; i<client_thread_count; i++) {  // client threads
        pthread_join(t[i], NULL);
    }
    
    printf("exit\n");
    exit(0);
}

