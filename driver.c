#include "utils.h"

int main(int argc, char const *argv[])
{
	printf(ANSI_COLOR_GREEN "--------------------------------------------------------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "                 Welcome to the packet capture and analysis utility\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "--------------------------------------------------------------------------------------------\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\nYou can select any interface , track conversations , filter packets based on protocol(s) and even view graphical analysis\nChoose an appropriate interface to get started and then select the functionality you want from the menu.\n\n" ANSI_COLOR_RESET);

    // DISPLAYS ALL INTERFACES FOR PACKET CAPTURE
    show_interfaces();

    // BINDING INTERFACE TO SOCKET
    char intf[50];

    printf(ANSI_COLOR_RED "Enter the interface for packet capturing : " ANSI_COLOR_RESET);
    scanf("%s" , intf);

    // GETTING RAW SOCKET IN PROMISCOUS MODE
    int sock_raw = raw_init(intf);

    // flog FILE FOR PACKET CAPTURE
    char fname[50];

    int addr_size , data_size;
    struct sockaddr saddr;         
    unsigned char *buf = (unsigned char *) malloc(10000); 


    while(true)
    {
        tcp = udp = http = dns = ip = total = k = 0;
        ip_count = tcp_count = udp_count = 0;
        cnt = 0;

        printf(ANSI_COLOR_YELLOW "Enter the number of packets you wish to capture (MAX - 2000) : " ANSI_COLOR_RESET);  
        int nop;
        scanf("%d",&nop);
        printf(ANSI_COLOR_YELLOW"Enter the name of log file (must have extension .txt) : "ANSI_COLOR_RESET);
        scanf("%s" , fname);
        flog = fopen(fname , "w");

        clock_t c0 = clock();
        
        while(cnt < nop)
        {
            addr_size = sizeof saddr;
            data_size = recvfrom(sock_raw , buf , 10000 , 0 , &saddr , (socklen_t*)&addr_size);
            if(data_size < 0 )
            {
                printf("No packet received\n");
                return 1;
            }
            printf("Packets Received : %5d\r" , cnt+1);
            clock_t c1 = clock();
            tm[cnt] = (double)(c1 - c0) * 1000./CLOCKS_PER_SEC;
            packlen[k++] = data_size;
            analyze(buf , data_size);
        }
        printf("Packets captured written in %s\n" , fname);

        int choice;

        while(true)
        {
            printf(ANSI_COLOR_GREEN "\nANALYSIS : \n1. Track conversations between pair of IP addresses\n2. Apply protocol filters to packets\n3. Display protocol hierarchy\n4. Network traffic graph\n5. Packet length analysis\n6. Quit analysis\nChoice : " ANSI_COLOR_RESET);
            scanf("%d",&choice);

            if(choice == 1)   
               converse();
            else if(choice==2)
               filter();                       
            else if(choice == 3)
               protocol_hierarchy();
            else if(choice == 4)
               display_traffic();
            else if(choice == 5)
               display_packlen();
            else if(choice == 6)
                break;               
        }

        char c;
        printf("Do you want to capture more packets (Y/N) : ");
        scanf(" %c" , &c);

        if(c == 'n' || c == 'N')
            break;
        
    }

    close(sock_raw);
    return 0;
}
