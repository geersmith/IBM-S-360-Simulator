#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


unsigned int ram[4096] = {0};
unsigned int addvalArray[4096] = {0};
unsigned int reg[32] = {0};

char charArray[];
int input_counter = 0;
int condition_code;
int inst_addr = 0;


unsigned int r1 = 0;
unsigned int r2 = 0;
int theTemp;
int hex;
int add_counter;
int verbose = 0;
int lr_counter =0,
    cr_counter=0,
    sub_counter=0,
    la_counter=0,
    bct_counter=0,
    bc_counter=0,
    st_counter=0,
    l_counter=0,
    bc_taken_count=0,
    c_counter=0;
int condition_code = 0;
int halt = 0;
unsigned int ramCounter = 0;
unsigned int loopCounter = 0;
int bct_taken_count = 0;
int data_read = 0;
int data_write = 0;
int tempForTest = 9;
int fetch_align;









void loader(){
    int i = 0;

    if(verbose) printf( "addr value\n" );
    while(scanf("%x", &addvalArray[i]) != EOF ){
        if(i >= 4096){
            printf("input file has overflow");
            exit(-1);
        }
        addvalArray[i] = addvalArray[i] & 0xfff;
        ram[i] = addvalArray[i];
        if(verbose) printf( "%03x: %02x\n", i, addvalArray[i] );
        //printf("ram i is %d \n", ram[i]);
        i++;
    }
    loopCounter = i;
}


void simple_add(int g){
    
    int temp;
    temp = ram[g+1];
    
    sprintf(charArray, "%02x", temp);
    
    char temp_r1 = charArray[0];
    char temp_r2 = charArray[1];
    
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    if(verbose){
    printf( "\nAR instruction, ");
    printf("operand 1 is R%x, ", r1);
    printf("operand 2 is R%x\n", r2);
}
    reg[r1] = reg[r1] + reg[r2];
    
    
    int i =0;
    inst_addr = inst_addr +2;
    if(reg[r1] > 0){
        condition_code = 2;
    }
    else if(reg[r1] < 0){
        condition_code = 1;
    }
    else{
        condition_code = 0;
    }
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
   
    }
}

void load_register(int y){
    unsigned int temp;
    temp = ram[y+1];
    
    unsigned int first = (temp >> 4) & 0xf;
    unsigned int second = (temp >> 0) & 0xf;
   
    sprintf(charArray, "%02x", temp);
    
    r1 = first;
    r2 = second;
   
    
    if(verbose){
    printf( "\nLR instruction, ");
    printf("operand 1 is R%x, ", r1);
    printf("operand 2 is R%x\n", r2);
}
    reg[r1] = reg[r2];
    int i = 0;
    inst_addr = inst_addr + 2;
    if(verbose){ 
    printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }
}

void load_address(int y){
   
    int temp; 
    int temp2;
    int temp3;
    int temp4;
    unsigned char output1[4096];
    int n = 1;
    unsigned int r3;
    unsigned int r4;
    unsigned int r5;
    unsigned int storeValue = 0;
    unsigned int maybe[64];
    
    temp = addvalArray[y+1];
    temp3 = addvalArray[y+2];
    temp4 = addvalArray[y+3];
    
    unsigned int storeArray[32];
    unsigned int lastElement;
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    char temp_r2 = charArray[1];
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    r3 = (temp3 >> 4) & 0xf;
    sprintf(charArray, "%02x", temp3);
    char temp_r3 = charArray[0];
    unsigned char *ptr = &output1[0];
    char firstElement = charArray[1];
    r3 = temp_r3 - '0';
    sprintf(charArray, "%02x", temp4);
    char temp_r4 = charArray[0];
    char temp_r5 = charArray[1];
    r4 = temp_r4 - '0';
    r5 = temp_r5 - '0';
    output1[0] = firstElement;
    output1[1] = temp_r4;
    output1[2] = temp_r5;
    unsigned int g;

    r4 = temp_r4 - '0';
    r5 = temp_r5 - '0';
    theTemp = ram[tempForTest];
    
    sscanf(output1, "%x", &g);
    unsigned int eff_addr;
    
    if(r2==0){
        if(r3==0){
            eff_addr = g;
        }
        else{
            eff_addr = reg[r3] + g;
        }
    }
    else if(r3==0){
        if(r2!=0){
            eff_addr = reg[r2] + g;
        }
        
    }
    else{
        eff_addr = reg[r2] + reg[r3] + g;
    }
    
   
storeValue = reg[r2] + reg[r3] + g;
    if(verbose){
        printf("\nLA instruction, ");
        printf("operand 1 is R%x,", r1);
        printf(" operand 2 at address %06x\n", eff_addr);
    }
    
    reg[r1] = eff_addr;
    inst_addr = inst_addr + 4;
    int i =0;
    if(verbose){
    
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    
    }
    
}

char * convert(int dec, char *theout) {
    theout[4] = '\0';
    theout[3] = (dec & 1) + '0';
    theout[2] = ((dec >> 1) & 1) + '0';
    theout[1] = ((dec >> 2) & 1) + '0';
    theout[0] = ((dec >> 3) & 1) + '0';
    return theout;
}

void branch_on_condition(int y){
    unsigned int temp;
    unsigned int temp2;
    unsigned int temp3;
    unsigned int r3;
    char theoutput[4];
    unsigned int needed;
    unsigned int eff_addr;
    temp = addvalArray[y+1];
    temp2 = addvalArray[y+2];
    unsigned int fourthElement = addvalArray[y+3];
    inst_addr = inst_addr + 4;
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    r3 = (temp2 >> 4) & 0xf;

    convert(r1, theoutput);

    if(condition_code == 2 && theoutput[2] == '1'){
        unsigned int idk = addvalArray[y+3];
        eff_addr = idk;
        ramCounter = idk;
        inst_addr = idk;
        bc_taken_count++;
    }
    if(condition_code == 1 && theoutput[1]=='1'){
        unsigned int yep = addvalArray[y+3];
        eff_addr = yep;
        ramCounter = yep;
        bc_taken_count++;
        inst_addr = yep;
    }
    if(condition_code == 0 && theoutput[0] == '1'){
        unsigned int yup = addvalArray[y+3];
        eff_addr = yup;
        bc_taken_count++;
        ramCounter = yup;
        inst_addr = yup;
    }

    temp3 = addvalArray[y+3];
    if(verbose){
    printf("\nBC instruction, ");
    printf("mask is %x, ", r1);
    printf("branch target is address %06x\n", temp3);

    }

    int i =0;
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }


    

}

void branch_on_count(int y){
    unsigned int temp; 
    unsigned int temp2;
    unsigned int temp3;
    unsigned int r3;
    int n = 0;
    unsigned int eff_addr;
    temp = addvalArray[y+1];
    temp2 = addvalArray[y+2];
    temp3 = addvalArray[y+3];
    unsigned char output[16];
    char storeArray[32];
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    sprintf(charArray, "%02x", temp2);
    r3 = (temp2 >> 4) & 0xf;
    char temp_r4 = charArray[1];

    sprintf(charArray, "%02x", temp3);
    char temp_r5 = charArray[0];
    char temp_r6 = charArray[1];

    output[0] = temp_r4;
    output[1] = temp_r5;
    output[2] = temp_r6;

    unsigned int g;

    sscanf(output, "%x", &g);

    if(r2==0){
        if(r3==0){
            eff_addr = g;
        }
        else{
            eff_addr = reg[r3] + g;
        }
    }
    else if(r3==0){
        if(r2!=0){
            eff_addr = reg[r2] + g;
        }
        
    }
    else{
        eff_addr = reg[r2] + reg[r3] + g;
    }

    char tempOutput2 = output[2];
    int pleasework = eff_addr;
    
    

    if(verbose){
    printf("\nBCT instruction, ");
    printf("operand 1 is R%x, ", r1);
    printf("branch target is address %06x\n", eff_addr);

    }
    unsigned int tempForBCT = eff_addr;

    reg[r1]--;
    if(reg[r1] != 0){
        ramCounter = pleasework;
        bct_taken_count++;
        
    }

   
    if(reg[r1]!= 0){
        inst_addr = eff_addr;
    }
    else{
        inst_addr = inst_addr + 4;
    }

    int i =0;
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }
}



void compare(int y){
    data_read++;
    inst_addr = inst_addr + 4;
    int temp; 
    int temp2;
    int n = 0;
    temp = addvalArray[y+1];
    temp2 = addvalArray[y+3];
    char storeArray[32];
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    r1 = temp_r1 - '0';
    r2 = temp2;

    int tempCompare;
    for(int j = 0; j <= r2; j++){
        
        if(j = r2){
            int tempj = addvalArray[r2];
            for(int i = 0; i < 4; i++){
                n+=sprintf(&storeArray[n], "%02x", addvalArray[r2 + i]);
            }
            tempCompare = (int)strtol(storeArray, NULL, 16);
        }
    }
    if(reg[r1] > tempCompare){
        condition_code = 2;
    }
    else if(reg[r1] < tempCompare){
        condition_code = 1;
    }
    else{
        condition_code = 0;
    }
    if(verbose){
        printf("\nC instruction, ");
        printf("operand 1 is R%d, ", r1);
        printf("operand 2 at address %06x\n", temp2);
    }
    int i =0;
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }
    
}

void store(int y){
    data_write++;
    inst_addr = inst_addr + 4;
    int temp; 
    int temp2;
    int temp3;
    int temp4;
    int r3;
    int r4;
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int storeValue;
    unsigned char output[4096];
    temp = addvalArray[y+1];
    temp3 = addvalArray[y+2];
    int n = 0;
    temp4 = addvalArray[y+3]; 
    char storeArray[32];
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    char temp_r2 = charArray[1];
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    sprintf(charArray, "%02x", temp3);
    char tempfirstforLast = charArray[1];
    r3 = (temp3 >> 4) & 0xf;
    sprintf(charArray, "%02x", temp4);
    char tempsecondforlast = charArray[0];
    char tempthirdforlast = charArray[1];
    output[0] = tempfirstforLast;
    output[1] = tempsecondforlast;
    output[2] = tempthirdforlast;
    unsigned int g;

    sscanf(output, "%x", &g);
    r4 = g;
    sprintf(charArray, "%08x", reg[r1]);
    char first2ofoutput = charArray[0];
    
    char second2ofoutput = charArray[1];
    output[0] = first2ofoutput;
    
    output[1] = second2ofoutput;
    sscanf(output, "%02x", &a);

    char thirdoutput = charArray[2];
    char fourthoutput = charArray[3];
    output[0] = thirdoutput;
    output[1] = fourthoutput;
    sscanf(output, "%02x", &b);

    char fifthoutput = charArray[4];
    char sixthoutput = charArray[5];
    output[0] = fifthoutput;
    output[1] = sixthoutput;
    sscanf(output, "%02x", &c);

    char seventhoutput = charArray[6];
    char eighthoutput = charArray[7];
    output[0] = seventhoutput;
    output[1] = eighthoutput;
    sscanf(output, "%02x", &d);

    if(r2==0){
        if(r3==0){
            storeValue = r4;
        }
        else{
            storeValue = reg[r3] + r4;
        }
    }
    else if(r3==0){
        if(r2!=0){
            storeValue = reg[r2] + r4;
        }
        
    }
    else{
        storeValue = reg[r2] + reg[r3] + r4;
    }


    ram[storeValue] = a;
    ram[storeValue + 1] = b;
    ram[storeValue + 2] = c;
    ram[storeValue + 3] = d;


    if(verbose){
printf("\nST instruction, ");
        printf("operand 1 is R%x, ", r1);
        printf("operand 2 at address %06x\n", storeValue);
    }
    int i =0;
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }
    
    

}


void load(int y){
    inst_addr = inst_addr + 4;
    unsigned int temp; 
    unsigned int temp2;
    data_read++;
    unsigned char output[4096];
    temp = addvalArray[y+1];
    int n = 0;
    temp2 = addvalArray[y+2];
    unsigned int temp3 = addvalArray[y+3]; 
    unsigned char storeArray[32];
    unsigned int effective;
   
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
   
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    sprintf(charArray, "%02x", temp2);
    char temp_r4 = charArray[1];
  
    unsigned int r3 = (temp2 >> 4) & 0xf;
    sprintf(charArray, "%02x", temp3);
    char temp_r5 = charArray[0];
    char temp_r6 = charArray[1];
   
    output[0] = temp_r4;
    output[1] = temp_r5;
    output[2] = temp_r6;

    unsigned int p;
    sscanf(output, "%x", &p);

    unsigned int eff_addr;
    if(r2==0){
        if(r3==0){
            eff_addr = p;
        } 
        else{
            eff_addr = reg[r3] + p;
        }
    }
    else if(r3==0){
        if(r2!=0){
            eff_addr = reg[r2] + p;
        }
        
    }
    else{
        eff_addr = reg[r2] + reg[r3] + p;
    }
  
    for(unsigned int j = 0; j <= eff_addr; j++){
     
        if(j = eff_addr){
            int tempj = addvalArray[eff_addr];
           
            for(int i = 0; i < 4; i++){
                n+=sprintf(&storeArray[n], "%02x", ram[eff_addr + i]);
                
            }
           
            reg[r1] = (int)strtol(storeArray, NULL, 16);
           
        }   
    }
    if(verbose){
        printf("\nL instruction, ");
        printf("operand 1 is R%x, ", r1);
        printf("operand 2 at address %06x\n", eff_addr);
  
    
    int i =0;

                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);

        
        
    }
   
}


void compare_register(int g){
    int temp;
    temp = ram[g+1];
    inst_addr = inst_addr + 2;
   
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    char temp_r2 = charArray[1];
  
    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    if(verbose){
    printf( "\nCR instruction, ");
    printf("operand 1 is R%x, ", r1);
    printf("operand 2 is R%x\n", r2);
}
    if(reg[r1] > reg[r2]){
        condition_code = 2;
    }
    else if(reg[r1] < reg[r2]){
        condition_code = 1;
    }
    else if(reg[r1] = reg[r2]){
        condition_code = 0;
    }
    int i = 0;
    if(verbose){
    printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
    }
}

void subtract(int g){
    int temp;
    temp = ram[g+1];
   
    sprintf(charArray, "%02x", temp);
    char temp_r1 = charArray[0];
    char temp_r2 = charArray[1];

    r1 = (temp >> 4) & 0xf;
    r2 = (temp >> 0) & 0xf;
    if(verbose){
    printf( "\nSR instruction, ");
    printf("operand 1 is R%x, ", r1);
    printf("operand 2 is R%x\n", r2);
}
    reg[r1] = reg[r1] - reg[r2];

    int i =0;
    inst_addr = inst_addr +2;
    if(reg[r1] > 0){
        condition_code = 2;
    }
    else if(reg[r1] < 0){
        condition_code = 1;
    }
    else{
        condition_code = 0;
    }
    if(verbose){
                printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);
                printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);

    }
}



void print_final_memory_words(){
   
    unsigned int temp = 0;
    unsigned int temp1;
    unsigned int temp2;
    unsigned int temp3;
    unsigned int temp4;
    unsigned char output[8];
    int n = 0;
    char *end;
    unsigned int g;
   
    
    for(int j = 0; j < loopCounter; j++){
        if(j == temp || j%4==0){
           
            temp1 = ram[j];
            temp2 = ram[j+1];
            temp3 = ram[j+2];
            temp4 = ram[j+3];

            sprintf(charArray, "%02x", temp1);
            char temp_r1 = charArray[0];
            char temp_r2 = charArray[1];
            sprintf(charArray, "%02x", temp2);
            char temp_r3 = charArray[0];
            char temp_r4 = charArray[1];

            sprintf(charArray, "%02x", temp3);
            char temp_r5 = charArray[0];
            char temp_r6 = charArray[1];

            sprintf(charArray, "%02x", temp4);
            char temp_r7 = charArray[0];
            char temp_r8 = charArray[1];
            output[0] = temp_r1;
            output[1] = temp_r2;
            output[2] = temp_r3;
            output[3] = temp_r4;
            output[4] = temp_r5;
            output[5] = temp_r6;
            output[6] = temp_r7;
            output[7] = temp_r8;
            sscanf(output, "%x", &g);
            
         ram[j] = g;
         if(verbose){
           printf("%03x: %08x\n", j, ram[j]);
           }
           temp = temp + 4;
        }
        
    }
    


}

void halt_now(){
    int i = 0;
    if(halt = 1){
        if(verbose){
        printf("\nHalt encountered\n");
        inst_addr = inst_addr + 2;
        printf("instruction address = %06x, condition code = %d\n", inst_addr, condition_code);

        printf("R0 = %08x, R4 = %08x, R8 = %08x, RC = %08x\n", reg[i], reg[i+4], reg[i+8], reg[i+12]);
                printf("R1 = %08x, R5 = %08x, R9 = %08x, RD = %08x\n", reg[i+1], reg[i+5], reg[i+9], reg[i+13]);
                printf("R2 = %08x, R6 = %08x, RA = %08x, RE = %08x\n", reg[i+2], reg[i+6], reg[i+10], reg[i+14]);
                printf("R3 = %08x, R7 = %08x, RB = %08x, RF = %08x\n", reg[i+3], reg[i+7], reg[i+11], reg[i+15]);
        }
    }
}

int main( int argc, char **argv ){
    printf("\nbehavioral simulation of S/360 subset\n");
    
    if( argc > 1 ){
    if( ( argv[1][0] == '-' ) &&
        (( argv[1][1] == 'v' ) || ( argv[1][1] == 'V' )) ){

      verbose = 1;

    }else{
      printf( "usage: either %s or %s -v with input taken from stdin\n",
        argv[0], argv[0] );
      exit( -1 );
    }
    if(verbose){
        printf("\n(memory is limited to 4096 bytes in this simulation)\n");
    printf("(addresses, register values, and memory values are shown in hexadecimal)\n\n");
        printf("initial contents of memory arranged by bytes\n");
    }
    }

    loader();
    if(verbose){
    printf("\ninitial pc, condition code, and register values are all zero\n\n");
    printf("updated pc, condition code, and register ");
    printf("values are shown after\n");
    printf(" each instruction has been executed\n");
    }
    for(ramCounter = 0; ramCounter < loopCounter; ramCounter++){
        if(ramCounter%2){

        }
        else{

        if(ram[ramCounter] == 26){
            simple_add(ramCounter);
            add_counter++;
            input_counter++;
        }
        if(ram[ramCounter]==65){
            la_counter++;
            load_address(ramCounter);
            input_counter++;
            
           
        }

        if(ram[ramCounter]==71){
            branch_on_condition(ramCounter);
            input_counter++;
            bc_counter++;
        }

        if(ram[ramCounter]==25){
            compare_register(ramCounter);
            input_counter++;
            cr_counter++;
        }
        if(ram[ramCounter]==70){
            branch_on_count(ramCounter);
            input_counter++;
            bct_counter++;
        }
        if(ram[ramCounter]==89){
            compare(ramCounter);
            c_counter++;
            input_counter++;
        }

        if(ram[ramCounter]==80){
            store(ramCounter);
            input_counter++;
            st_counter++;
        }

        
        
        if(ram[ramCounter]==88){
            load(ramCounter);
            l_counter++;
            input_counter++;
           
        }
        
        if(ram[ramCounter]==27){
            sub_counter++;
            subtract(ramCounter);
            input_counter++;
        }
        if(ram[ramCounter]==24){
            lr_counter++;
            load_register(ramCounter);
            input_counter++;
        }
        if(ram[ramCounter]==0 && ram[ramCounter+1]==0 && ram[ramCounter+2]==0 && ram[ramCounter+3]==0){
            halt = 1;
            halt_now();
            break;
        }
        }
        
        }
        if(verbose){

        
        printf("\nfinal contents of memory arranged by words\n");
        printf("addr value\n");
        print_final_memory_words();
        }
  

        printf("\nexecution statistics\n");
        printf("  instruction fetches = %d\n", input_counter);
        printf("    LR  instructions  = %d\n", lr_counter);
        printf("    CR  instructions  = %d\n", cr_counter);
        printf("    AR  instructions  = %d\n", add_counter);
        printf("    SR  instructions  = %d\n", sub_counter);
        printf("    LA  instructions  = %d\n", la_counter);
        printf("    BCT instructions  = %d", bct_counter);
if(bct_counter > 0){
            printf(", taken = %d (%.1f%%)\n", bct_taken_count,
      100.0*((float)bct_taken_count)/((float)bct_counter) );
  }
  else{
      printf("\n");
        }
        printf("    BC  instructions  = %d", bc_counter);
    if(bc_counter > 0){
            printf(", taken = %d (%.1f%%)\n", bc_taken_count,
      100.0*((float)bc_taken_count)/((float)bc_counter) );
  }
  else{
      printf("\n");
        }
        
        printf("    ST  instructions  = %d\n", st_counter);
        printf("    L   instructions  = %d\n", l_counter);
        printf("    C   instructions  = %d\n", c_counter);
        printf("  memory data reads   = %d\n", data_read);
        printf("  memory data writes  = %d\n", data_write);
  
}



