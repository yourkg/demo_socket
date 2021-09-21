/*udp demo ver0.1 yourkg 210913*/
/*udp demo ver0.2 final yourkg 210921*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int PORT=4001;
char* ip="127.0.0.1";


int main(int argh, char **argv)
{
    if(argh<3){
        printf("Please start in the following format :\n[1] ./rps listen ip(wg):port \n[2] ./rps send port your_str\n");
        return 0;
    }
    
    if(strcmp(*(argv+1),(char*)"send")==0){
        
        PORT=atoi(*(argv+2));
        
        printf("sending %s ...\nip(wg):%s port: %d\n",*(argv+2),ip,PORT);
        int brdcFd;
        if((brdcFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1){
            printf("socket fail\n");
            return -1;
        }
        
        int optval = 1;
        setsockopt(brdcFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(int));
        
        struct sockaddr_in theirAddr;
        memset(&theirAddr, 0, sizeof(struct sockaddr_in));
        
        theirAddr.sin_family = AF_INET;
        theirAddr.sin_addr.s_addr = inet_addr(ip);
        theirAddr.sin_port = htons(PORT);
        
        int sendBytes;
        
        char *msg="";
        msg=*(argv+3);
        if((sendBytes = sendto(brdcFd, msg, strlen(msg), 0,
                (struct sockaddr *)&theirAddr, sizeof(struct sockaddr))) == -1){
            printf("sendto fail, errno=%d\n", errno);
            return -1;
        }
        //printf("msg=%s, msgLen=%ld, sendBytes=%d\n", msg, strlen(msg), sendBytes);
        printf("You can continue sending\n");
        while (1) {
            printf(">>");
            char newmsg[128]="";
            scanf("%s",newmsg);
            msg=newmsg;
            if((sendBytes = sendto(brdcFd, msg, strlen(msg), 0,
                    (struct sockaddr *)&theirAddr, sizeof(struct sockaddr))) == -1){
                printf("sendto fail, errno=%d\n", errno);
                return -1;
            }
        }
        return 0;


    }else if(strcmp(*(argv+1),(char*)"listen")==0){
        int len = strlen(*(argv+2));
        int len_cut=0;
        while(len_cut++<len){
            if(*(*(argv+2)+len_cut)==':'){
                //printf("%s\n",&*(*(argv+3)+len_cut));
                char *port=&*(*(argv+2)+len_cut+1);
                //printf("%s\n",port);
                PORT=atoi(port);
                *(*(argv+2)+len_cut)='\0';
                ip=*(argv+2);
            }
        };
        
        printf("listening %s ...\nip:%s port: %d\n",*(argv+2),ip,PORT);
        
        int sockListen;
        if((sockListen = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            printf("socket fail\n");
            return -1;
        }
        int set = 1;
        setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
        struct sockaddr_in recvAddr;
        memset(&recvAddr, 0, sizeof(struct sockaddr_in));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(PORT);
        recvAddr.sin_addr.s_addr = INADDR_ANY;
        if(bind(sockListen, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){
            printf("bind fail\n");
            return -1;
        }
        int recvbytes;
        char recvbuf[128];
        int addrLen = sizeof(recvAddr);
        
        while (1) {
            
            if((recvbytes = recvfrom(sockListen, recvbuf, 128, 0,
                    (struct sockaddr *)&recvAddr, (socklen_t *)&addrLen)) != -1){
                    recvbuf[recvbytes] = '\0';
                    printf("<< %s\n",recvbuf);
                }else{
                    printf("recvfrom fail\n");
                }
        }
        
    }else{
        printf("Please start in the following format :\n[1] ./rps listen ip(wg):port \n[2] ./rps send port your_str\n");
        return 0;
        
    }
}


//int up(int hi,char* str1,char* str2){
//
//    printf("..-> %d , %s , %s",hi,str1,str2);
//
//};


/*    if (strcmp(str2,(char*)"player")==0) {
    ***
    printf("link player :\n");
    int playerNum=0;
        while(playerNum++<2){
        char *pn;
        *********
    }
    return 1;
    
}*/
