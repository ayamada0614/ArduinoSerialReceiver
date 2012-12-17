#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>  
#include <termios.h>  
#include <time.h>  
#include <sys/wait.h>  

#include <sys/time.h>
  
#define BAUDRATE B9600 // fix 900bps to connect to Arduino 
  
#define MODEMDEVICE "/dev/cu.usbmodemfa131"  
  
#define BUFFSIZE 256  
#define COULDNOTFORK -1  
  
#define FALSE 0  
#define TRUE 1  
  
volatile int STOP = FALSE;  
static int fd = -1;  
  
/* Functions */  
void serial_init(int fd);  
void child_process();  
void parent_process(pid_t result_pid);  
  
/* Main */  
int main(void);  
  
// initiate serial port  
void serial_init(int fd) {  
    struct termios tio;  
    memset(&tio, 0, sizeof(tio));  
    tio.c_cflag = CS8 | CLOCAL | CREAD; // Baudrate: CS8: 8n1 (8 bits, nonparity, stop bit 1), CLOCAL: local connection, no modem control, CREAD: receiving characters on   
    tio.c_cc[VTIME] = 0; // don't use timer between characters  
    tio.c_lflag = ICANON; // Canonical input on 
    tio.c_iflag = IGNPAR | ICRNL; // IGNPAR: ignore parity error data, ICRNL: CR corresponds to NL 
  
    // Configuration for Boudrate  
    cfsetispeed(&tio, BAUDRATE);  
    cfsetospeed(&tio, BAUDRATE);  
    // Configuration for devices  
    tcsetattr(fd, TCSANOW, &tio);  
}  
  
void child_process() {  
  /*
    char buf[BUFFSIZE];  
    char input[BUFFSIZE];  
    int count;  
    int i;  
    int writecount = 0;  
  
    fprintf(stdout, "CHILD:RCV Start\n");  
  
    // infinite loop until receiving STOP signal 
    while (FALSE == STOP) {  
        memset(&buf, 0, sizeof(buf));  
        count = read(fd, &buf, BUFFSIZE);  
        if (count < 0) {  
            fprintf(stdout, "CHILD:Could not read from serial port\n");  
            STOP = TRUE;  
        } else {  
            fprintf(stdout, "CHILD:RCVD CHAR %s %d", buf, count);  
  
        }  
    }  
  
    fprintf(stdout, "CHILD:BYE!\n");  
   */
}  
  
void parent_process(pid_t result_pid) {  
  
    char input[BUFFSIZE];  
    int writecount = 0;  
    fprintf(stdout, "Parent:Waiting for Input\n");  
    int i = 0;  
    int inputcount = 0;  
  
    int data = 0;
    int j = 0;
  
    while (1) {  

        //sprintf(input2, "%d", channel);
        memset(&input, 13, sizeof(input));  
  
        //fgets(input, sizeof(input), stdin);  
        //fflush(stdin);  
        data++;     
        sprintf(input, "%d", data);
  
        for (i = 0; i < BUFFSIZE; i++) {
    
            if (input[i] == 0) {  
                inputcount = i;  
                input[i] = 13;  
                input[i + 1] = 10;  
                break;  
            }
           
        }
       
        writecount = write(fd, &input, inputcount);  
        if (writecount < 0) {  
            fprintf(stdout, "Could not write to serial port %d\n", writecount);  
            break;  
        } else {  
            fprintf(stdout, "Send %d bytes\n", writecount);  
  
        }  

      usleep(15*1000); // usec

      if(j >= 1000 - 1)
      {
        j = 0;
        data = 0;
      }else{
        j++;
      }
      
      
    }  
}  
  
int main(void) {  
    pid_t result_pid;  
  
    struct termios oldtio, newtio;  
    char buf[255];  
  
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);  
    if (fd < 0) {  
        perror(MODEMDEVICE);  
        return (-1);  
    }  
  
    tcgetattr(fd, &oldtio);
    memset(&newtio, 0, sizeof(newtio));  
  
    // ready serial port  
    serial_init(fd);  
  
    //fork  
    result_pid = fork();  
  
    if (result_pid == -1) {  
        fprintf(stderr, "fork failed.\n");  
        return COULDNOTFORK;  
    }  
  
    if (result_pid == 0) {  
        child_process();  
    } else {  
        fprintf(stderr, "fork completed");  
  
        parent_process(result_pid);  
    }  
    STOP = TRUE;  
  
    return 0;  
}  
