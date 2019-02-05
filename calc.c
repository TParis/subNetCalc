
/*
    
    I want a function that accepts two char * parameters. The first is an IP address, the second is a subnet.

    Without using the string library, determine the IP range that the IP address identified can communicate without
        the use of a gateway.

    use BitWise operands, pointer arithmetic, and return a struct containing two in_addr_t variables by utilizing 
        the netinet/in.h library. 

    Ensure to do proper input validation and return -1 on invalid inputs.

*/

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

struct Network {
    char* first;
    char* last;
};

struct Network *netAddr(char* addr, char* subNet);
int intAddr(char* string, int* first, int* second, int* third, int* fourth);

int main() {
    char* addr = "122.13.14.1";
    char* subNet = "255.128.0.0";
    struct Network *networks = netAddr(addr, subNet);
}

struct Network *netAddr(char* addr, char* subNet){
    struct Network *returnStruct = malloc(sizeof(struct Network));  //Allocate at compile time, not runtime
    int* oct0 = (int)malloc(sizeof(int));                           //You don't need to do this on the heap.  Allocate at compile time, not runtime
    int* oct1 = (int)malloc(sizeof(int));
    int* oct2 = (int)malloc(sizeof(int));
    int* oct3 = (int)malloc(sizeof(int));
    int* sub0 = (int)malloc(sizeof(int));
    int* sub1 = (int)malloc(sizeof(int));
    int* sub2 = (int)malloc(sizeof(int));
    int* sub3 = (int)malloc(sizeof(int));
    char firstAddr[51] = { 0 };
    char lastAddr[51] = { 0 };
    unsigned int bitShifter = 0xFF;
    if (intAddr(addr, oct0, oct1, oct2, oct3) == -1) {
        printf("INVALID INPUT");
        return -1;
    }
    if (intAddr(subNet, sub0, sub1, sub2, sub3) == -1) {
        printf("INVALID INPUT");                                    //Don't handle errors inside of the function, handle them outside either in Main() or in an error handler
        return -1;
    }
    if (*sub1 < 255) {                                                              //Lines 56 through 84 can be done in nearly 5 or 6 lines if you understand IP addressing.
        if (*sub3 != 0 || *sub2 !=0) {                                              //Don't try to solve each byte independently.  Make the IP and Subnet uint32s and then AND
            printf("INVALID SUBNET MASK");                                          //them together.  The output will be your low address.
            return -1;
        }
        snprintf(firstAddr, 50, "(%d.%d.%d.%d)", *oct0, (*oct1 & *sub1), 0, 1);
        returnStruct->first = &firstAddr;
        unsigned int wildcard = (bitShifter ^ *sub1);
        snprintf(lastAddr, 50, "(%d.%d.%d.%d)", *oct0, (*oct1 | wildcard),255 , 254);
        returnStruct->last = lastAddr;
    }else if (*sub2 < 255) {
        if (*sub3 != 0) {
            printf("INVALID SUBNET MASK");
            return -1;
        }
        snprintf(firstAddr, 50, "(%d.%d.%d.%d)", *oct0, *oct1, (*oct2 & *sub2), 1);
        returnStruct->first = &firstAddr;
        unsigned int wildcard = (bitShifter ^ *sub2);
        snprintf(lastAddr, 50, "(%d.%d.%d.%d)", *oct0, *oct1, (*oct2 | wildcard), 254);
        returnStruct->last = lastAddr;    
    }
    else if (*sub3 <= 255) {
        snprintf(firstAddr, 50, "(%d.%d.%d.%d)", *oct0, *oct1, *oct2, ((*oct3 & *sub3) + 1));
        returnStruct->first = &firstAddr;
        unsigned int wildcard = (bitShifter ^ *sub3);
        snprintf(lastAddr, 50, "(%d.%d.%d.%d)", *oct0, *oct1, *oct2, ((*oct3 | wildcard) - 1));
        returnStruct->last = lastAddr;
    }
    return returnStruct;
}

int intAddr(char* string, int* first, int* second, int* third, int* fourth) {   //You can put the entire IP address in 1 uint32.  Think binary
    int tmp = 0;
    double tensCount = 0;
    int dotPosition = 0;
    int lastDotPosition = 0;
    int placeCounter = 0;
    int count = 0;
    for (int i = 0; i <= strlen(string); i++) {
        if (string[i] != '.' && i != strlen(string)) {     //No string library functions.  What do you need to check for to determine the end of a string?
            count++;
        }
        else {
            tensCount = pow(10, (count - 1));
            for (int o = (i - count); o < i; o++) {
                tmp += ((string[o] - 48) * tensCount);      // Good job.  A char number - 48 equals the integer value.  Use pointer arithmatic, not array syntax
                tensCount /= 10;
            }
            if (tmp < 0 | tmp > 255) {
                printf("INVALID ADDRESS");
                return -1;
            }
            switch (dotPosition) {                          //You can add these values into a uint32 using shl and OR bitwise operators
            case 0:
                *first = tmp;
                dotPosition++;
                tmp = 0;
                break;
            case 1:
                *second = tmp;
                dotPosition++;
                tmp = 0;
                break;
            case 2:
                *third = tmp;
                dotPosition++;
                tmp = 0;
                break;
            case 3:
                *fourth = tmp;
                dotPosition++;
                tmp = 0;
                break;
            }
            tensCount = 0;
            lastDotPosition = count + 2;
            count = 0;
        }

    }
    return 0; 
}

