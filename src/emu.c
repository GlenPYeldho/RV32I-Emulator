#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<getopt.h>
#include<stdbool.h>

#define FALSE 0
#define TRUE  1

#define SUCESS TRUE
#define FAILED FALSE

#define DRAM_SIZE 1024*1024*1     // 1 MiB DRAM
#define DRAM_BASE 0x80000000

/*Configration structure for Emulator*/
typedef struct emuConfig_t {
    char *inputFile;
    bool elf;
    char *machine;
    uint8_t *bin;
    ssize_t binSize;
}emuConfig;

typedef struct emuState_t {
    uint64_t regs[32];     /* 32 64 Bit register(x0-x31) */
    uint64_t pc;           /* 64 Bit Program Counter*/
    uint8_t *mem;          /* Memeory*/
    
}emuState;

/*Print command line usage*/
void print_usage(char **argv) {
    printf("Usage: %s [-e elf] [-i] filename\n",argv[0]);
    exit(FAILED);
}
/*TODO find the valid conbination required in command line
  parser*/

/*Parse Command Line Argument for Emulator*/
void parse_options(int argc,char **argv,emuConfig *config) {
    int opt;
    if(argc < 2) {
        print_usage(argv);
    }
    while((opt = getopt(argc,argv,"ehi:")) != -1) {
        switch (opt) {
            case 'e':
                config->elf = TRUE;
                break;
            case 'i':
                config->inputFile = optarg;
                break;
            case 'h':
                print_usage(argv);
                exit(SUCESS);
                break;
            default:
            print_usage(argv);
        }
    }
}

void read_binary(emuConfig *config) {
    if(config->inputFile) {
    /*Reading the binary file*/
    FILE *fp = fopen(config->inputFile,"rb");
    if(!fp) {
    printf("[Error in read file] File does not exit\n");
    exit(FAILED); 
    }

    fseek(fp,0,SEEK_END);
    config->binSize = ftell(fp);
    fseek(fp,0,SEEK_SET);

    config->bin = malloc(sizeof(config->binSize));
    fread(config->bin,config->binSize,1,fp);
    fclose(fp);


    }else {
        printf("ERROR: Invalid Filename\n");
        exit(FAILED);
    }
}

/*Constructor for config*/
void config_init(emuConfig *config) {
    config->inputFile = NULL;
    config->elf = FALSE;
    config->machine = NULL;
    config->bin = NULL;
    config->binSize = 0;
}

void emulator_init(emuState *hart, emuConfig *config) {
    hart->regs[0] = 0;
    hart->regs[2] = DRAM_BASE + DRAM_SIZE; /*set Stack */
    hart->pc = DRAM_BASE;
    /*TODO check if its a elf or not if elf parse  populate mem      and hart-mem is equal to DRAM_SIZE memcpy elf parsed row
      data into hart->mem*/
    hart->mem = config->bin;
}

uint32_t load32_memory(emuState *hart) {
    return (uint32_t) hart->mem[hart->pc - DRAM_BASE]
        |  (uint32_t) hart->mem[hart->pc - DRAM_BASE] << 8
        |  (uint32_t) hart->mem[hart->pc - DRAM_BASE] << 16
        |  (uint32_t) hart->mem[hart->pc - DRAM_BASE] << 24;
}
uint32_t fetchInstruction(emuState *hart) {
   return load32_memory(hart);

}

int  execute(emuState *hart, uint32_t inst) {
   int opcode = inst & 0x7f;
   int fun3   = (inst>>12) &0x7;
   int fun7   = (inst>>25) &0x7f;
return 0;//TODO

}

void start_emulation(emuState *hart) {
    while(1) {
        uint32_t inst = fetchInstruction(hart);
        
        hart->pc +=4;

        if(!execute(hart,inst))
            break;
        /*dump register*/
        if(hart->pc = 0)
            break;
    }

}

int main(int argc, char **argv) {
    emuConfig config;
    emuState hart;
    config_init(&config);
    parse_options(argc,argv,&config);
    read_binary(&config);
    /*Emulation init*/
    emulator_init(&hart,&config);
    
    start_emulation(&hart);
    


    return SUCESS;
}
