#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

/**
 * @brief Checks if port number is in bounds
 * @note if port number is wrong, will exit the program
 * @param portnum 
 */
void checkportnum(long int portnum){
    if( portnum < 1 || portnum > 65535){
        fprintf(stderr,"Error: Port must be in range 1 to 65535\n");   
        exit(1);
    }
}

/**
 * @brief Get the current cpu load 
 * @note Contains sleep(1)
 * @return load of cpu
 */
double get_cpu_load(){
    char* word;
    long long prev_user,prev_nice,prev_system,prev_idle,prev_iowait,prev_irq,prev_softirq,prev_steal;
    long long user,nice,system,idle,iowait,irq,softirq,steal;
    
    
    FILE *prev_file,*file;
    prev_file = fopen("/proc/stat","r");
    char prev_str[1024];
    fgets(prev_str,1023,prev_file);

    
    // fprintf(stderr,"prev_:\n");
    // fprintf(stderr,"%s",prev_str);

    word = strtok(prev_str, " ");


    if(strcmp("cpu",word) != 0){
        fprintf(stderr,"Error while getting CPU load");
        exit(1);
    }
    
    // user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
    prev_user = atoll(strtok(NULL," "));
    prev_nice = atoll(strtok(NULL," "));
    prev_system = atoll(strtok(NULL," "));
    prev_idle = atoll(strtok(NULL," "));
    prev_iowait = atoll(strtok(NULL," "));
    prev_irq = atoll(strtok(NULL," "));
    prev_softirq = atoll(strtok(NULL," "));
    prev_steal = atoll(strtok(NULL," "));
    fclose(prev_file);

    

    sleep(1);

    file = fopen("/proc/stat","r");
    char str[1024];
    fgets(str,1023,file);
    
    // fprintf(stderr,"act:\n");
    // fprintf(stderr,"%s",str);

    word = strtok(str, " ");

    if(strcmp("cpu",word) != 0){
        fprintf(stderr,"Error while getting CPU load");
        exit(1);
    }

    user = atoll(strtok(NULL," "));
    nice = atoll(strtok(NULL," "));
    system = atoll(strtok(NULL," "));
    idle = atoll(strtok(NULL," "));
    iowait = atoll(strtok(NULL," "));
    irq = atoll(strtok(NULL," "));
    softirq = atoll(strtok(NULL," "));
    steal = atoll(strtok(NULL," "));


    // fprintf(stderr,"prev_:\n");
    // fprintf(stderr,"CPU %lld %lld %lld %lld %lld %lld %lld %lld\n",prev_user,prev_nice,prev_system,prev_idle,prev_iowait,prev_irq,prev_softirq,prev_steal);

    // fprintf(stderr,"act:\n");
    // fprintf(stderr,"CPU %lld %lld %lld %lld %lld %lld %lld %lld\n",user,nice,system,idle,iowait,irq,softirq,steal);


    long long Prev_Idle = prev_idle + prev_iowait;
    long long Idle = idle + iowait;

    long long Prev_NonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    long long NonIdle = user + nice + system + irq + softirq + steal;

    long long Prev_Total = Prev_Idle + Prev_NonIdle;
    long long Total = Idle + NonIdle;

    long long totald = Total - Prev_Total;
    long long idled = Idle - Prev_Idle;
    // fprintf(stderr,"totald %lld idled %lld\n",totald,idled);

    double CPU_Percentage = (double) (totald - idled)/totald;
    fclose(file);
    // printf("CPU Usage: (%lld-%lld)/%lld = %lf\n",totald,idled,totald,CPU_Percentage);
    return CPU_Percentage;
}

/**
 * @brief Get hostname actual machine
 * 
 * @param str string into which the hostname will be saved
 */
void get_hostname(char *str){
    FILE *file;
    file = fopen("/proc/sys/kernel/hostname","r");

    str = fgets(str,255,file);
}

/**
 * @brief Get the cpu model
 *
 * @param str string into which the cpu model name will be saved
 */
void get_cpu_model(char *str){
    FILE *file;
    file = popen("cat /proc/cpuinfo | awk -F ': ' '$1 ~ /^model name/ {print $2; exit 1}'","r");
    fgets(str,255,file);
    pclose(file);
}

/**
 * @brief Checks if request is containing the path specified
 * 
 * @param request HTTP request
 * @param path the path which will be checked (should be without '/' at the end, ie.: 'cpu-name/' is wrong, 'cpu-name' is correct)
 * @return true if path in request matches the specified path
 * @return false otherwise
 */
// bool is_URL_path(char *request, char *path){
    
//     printf("The specified path is %s\n",path_r);
//     return strcmp(path, path_r) == 0;
// }

/**
 * @brief removes redundant '/' symbol at the end of a path
 * @param path to be fixed
 */
void path_unify(char *path){
    int path_len = strlen(path);
    // fprintf(stderr,"Last character of path is %c len:%d\n",,path_len);
    if(path_len > 1 && path[path_len-1] == '/'){
        path[path_len-1] = '\0';
    }
}

/**
 * @brief closes the socket
 * @param socket 
 */
void close_connection(int *socket){
    close(*socket);
    (*socket) = -1;
    printf("Closed connection...\n");
    exit(0);
}

char response_prefix[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain; charset=UTF-8\r\n\r\n";

char wrong_req[] = 
    "HTTP/1.1 400 Bad request\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "Bad request!\n";

char wrong_path[] = 
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/plain\r\n\r\n"
    "Not Found!\n";

int main(int argc, char const *argv[])
{

    if(argc < 2){
        fprintf(stderr,"Error: Not enough arguments\n");
        fprintf(stderr,"Usage:\n");
        fprintf(stderr,".\\hinfosvc [port]\n");
        exit(1);
    }
    
    const char* port = argv[1];

    long portnum = strtol(port, NULL, 10);
    checkportnum(portnum);
    

    /**
     *  GET SYSTEM INFO 
     */
    // fprintf(stderr,"CPU load is %.6lf\n",get_cpu_load());
    
    char hostname[255];
    get_hostname(hostname);
    // fprintf(stderr,"hostname is %s",hostname);
    
    char cpu_model[255];
    get_cpu_model(cpu_model);
    // fprintf(stderr,"CPU model is %s",cpu_model);


    /**
     * SERVER SETUP
     */
    int on = 1;
    int sct = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sct,SOL_SOCKET, SO_REUSEADDR, &on,sizeof(int));

    char buffer[2048];

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portnum);

    struct sockaddr_in client_address;
    socklen_t client_addr_s = sizeof(client_address);

    
    if( bind(sct, (struct sockaddr*) &address, sizeof(address) ) == -1){
        fprintf(stderr,"Error at BIND, port might be in use!\n");
        exit(1);
    }
    if( listen(sct, 10) == -1){
        fprintf(stderr,"Error at LISTEN!\n");
        exit(1);
    }
    fprintf(stderr,"Listening on port %s ...\n",port);
    
    while (1)
    {
        int sct_client = accept(sct, (struct sockaddr*) &client_address, &client_addr_s );

        if(sct_client == -1){
            perror("Connection failed...");
            continue;
        }

        printf("Got connection!\n");

        if(!fork()){

            close(sct);
            sct = -1;
            
            
            memset(buffer, 0, 2048);
            read(sct_client,buffer,2047);

            
            char * method = strtok(buffer," ");
            
            /**
             * CHECK HTTP METHOD
             */
            if(strcmp(method,"GET")){
                write(sct_client, wrong_req, sizeof(wrong_req) - 1);
                close_connection(&sct_client);
            }

            char * path = strtok(NULL," ");
            
            path_unify(path);

            /**
             *      /hostname
             */
            if( !strcmp(path,"/hostname") ){
                char *message = hostname;
                char res[strlen(response_prefix)+strlen(message)+1];
                
                sprintf(res,"%s%s",response_prefix,message);
                write(sct_client, res, sizeof(res) - 1);
            }
            /**
             *      /cpu-name
             */
            else if(!strcmp(path,"/cpu-name")){
                char *message = cpu_model;
                char res[strlen(response_prefix)+strlen(message)+1];
                
                sprintf(res,"%s%s",response_prefix,message);
                write(sct_client, res, sizeof(res) - 1);
            }
            /**
             *      /load
             */
            else if(!strcmp(path,"/load")){
                char message[32];
                double load = get_cpu_load();
                sprintf(message,"%lf%%",load*100);

                char res[strlen(response_prefix)+strlen(message)+1];
                
                sprintf(res,"%s%s\n",response_prefix,message);
                write(sct_client, res, sizeof(res) - 0);
            }
            /**
             *      WRONG PATH
             */
            else{
                write(sct_client, wrong_path, sizeof(wrong_path) - 1);
            }

            close_connection(&sct_client);
        }

        close(sct_client);
        sct_client = -1;

    }
    

    return 0;
}