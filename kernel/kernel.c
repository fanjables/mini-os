
char *const vbuffer = (char*)0xb8000;
unsigned int vbufferValue = 0;

char version[] = "FANJ OS v0.5.2|";
void printChar(unsigned char character);
void print(char *string);
void printCharExtended(unsigned char character, unsigned int setting);
void printExtended(char *string, unsigned int setting);
void clearTerminal();
int coordsToValue(unsigned int x, unsigned int y);
struct vec2 valueToCoords(unsigned int value);
unsigned int checkIfKey();
unsigned char readScanCode();
unsigned char readScanCodeBlock();
unsigned char readCharBlock(unsigned int Doesprint);
unsigned int readString(char *output_buf, unsigned int buf_len);
void commandHandler(char string[]);
int fStrcmp(char *stringa, char* stringb);
int intToString(unsigned int number, char *outputBuf, unsigned int bufLen);
int readDisk(unsigned int lbaSector, unsigned char numSectors, unsigned short int *dataBuf);


void main(){    //void main beacuse there is no os to return a value to
    clearTerminal();
    print(version);
    print("For a list of commands, type \"help\"|");
    unsigned short int loadTest[256];
    readDisk(1, 1, loadTest);
    if(loadTest[0] == 24929){
        print("diskreading worked|");
    }
    else{
        print("didnt work|");
    }
    while(1){
        char string[40];
        unsigned int string_len;
        string_len = readString(string, 40);
        commandHandler(string);
    }
    return;
}

void printChar(unsigned char character){
    if(vbufferValue > 4000){
        clearTerminal();
    }
    if(character == '|'){ //handles newlines
        int charOnLine = vbufferValue % 160; //finds how many char are on current line, remainder of vbufferValue / 80 (the max amount of chars on a line)
        vbufferValue += (160 - charOnLine); //moves vbufferValue to firs cell on next line
    }
    else{   //prints characters
        vbuffer[vbufferValue] = character;
        vbufferValue += 2;
    }
}

void print(char *string){
    if(vbufferValue > 4000){    //clears the screen if vbufferValue is greater than 4000
        clearTerminal();
    }
    while(*string != 0){
        printChar(*string);
        string ++;  //moves string to the next character
    }
    return;
}

void printCharExtended(unsigned char character, unsigned int setting){
    if(vbufferValue > 4000){    //clears the screen if vbufferValue is greater than 4000
        clearTerminal();
    }
    if(character == '|'){ //handles newlines
            int charOnLine = vbufferValue % 160; //finds how many char are on current line, remainder of vbufferValue / 80 (the max amount of chars on a line)
            vbufferValue += (160 - charOnLine); //moves vbufferValue to firs cell on next line
        }
    else{   //prints characters
        vbuffer[vbufferValue] = character;
         vbufferValue ++;
        vbuffer[vbufferValue] = setting;
        vbufferValue ++;
    }
}

void printExtended(char *string, unsigned int setting){
    
    while(*string != 0){
        printCharExtended(*string, setting);
        string ++;  //moves string to the next character
    }
    return;
}

void clearTerminal(){
    vbufferValue = 0;
    while(vbufferValue <= 4000){    //resets every cell to have no character and be white on black
        vbuffer[vbufferValue] = 0;
        vbufferValue ++;
        vbuffer[vbufferValue] = 15;
        vbufferValue ++;
    }
    vbufferValue = 0;
    return;
}

int coordsToValue(unsigned int x, unsigned int y){
    int value = (y * 160) + (x * 2);
    return value;
}

struct vec2{
    int x;
    int y;
};

struct vec2 valueToCoords(unsigned int value){
    struct vec2 coords;
    coords.x = value % 160;
    coords.y = (value - coords.x) / 160;
    coords.x /= 2;
    return coords;
}

unsigned int checkIfKey(){
    unsigned char status;
    __asm__ volatile (
        "inb $0x64, %%al\n\t"   //putting what is in io port 0x64 (keyboard controller) into al
        : "=a"(status)  //copys ax into variable status
        :
        : "cc"  //tells compiler which registers we use
    );
    return status & 0x01;   //returns 1 if if last bit in status and 1 are equal to eachother, if not, return 0
}

unsigned char readScanCode(){
    unsigned char scanCode;
    __asm__ volatile (
        "inb $0x60, %%al"   //putting what is in io port 0x60 (keyboard input) into al
        : "=a"(scanCode)
        :
        : "cc"
    );
    return scanCode;
}

unsigned char readScanCodeBlock(){
    unsigned char scanCode;
    while(checkIfKey() == 0){}
    scanCode = readScanCode();
    return scanCode;
}

// Index 0x00 is unused. 0x01 is Escape. 0x1C is Enter, etc.
const unsigned char keymap_lowercase[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 0x00 - 0x09 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 0x10 - 0x13 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '|',	/* Enter key */
    0,			/* 0x1D - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 0x1E - 0x27 */
 '\'', '#',   0,		/* 0x28 - 0x2A Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',	/* 0x2B - 0x34 */
  '/',   0,			/* 0x35 - Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 0x3B - F1 key */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* <--- F10 */
    0,	/* 0x45 - Num lock*/
    0,	/* 0x46 - Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 0x4F - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

const unsigned char keymap_uppercase[128] = {
    0,  27, '!', '"', 0, '$', '%', '^', '&', '*',	/* 0x00 - 0x09 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 0x10 - 0x13 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',	/* Enter key */
    0,			/* 0x1D - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 0x1E - 0x27 */
 '@', '~',   0,		/* 0x28 - 0x2A Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>',	/* 0x2B - 0x34 */
  '?',   0,			/* 0x35 - Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 0x3B - F1 key */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* <--- F10 */
    0,	/* 0x45 - Num lock*/
    0,	/* 0x46 - Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 0x4F - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned int shift_pressed = 0;

unsigned char readCharBlock(unsigned int Doesprint){
    unsigned char scanCode = readScanCodeBlock();
    unsigned char character = 0;
    if(scanCode & 0x80){    //checks last bit
        unsigned char releasedCode = scanCode & ~0x80;  //removes last bit
        if(releasedCode == 0x2A || releasedCode == 0x36){   //0x2A lshift 0x36 rshift
            shift_pressed = 0;
        }
        return character;
    }
    if(scanCode == 0x2A || scanCode == 0x36){
        shift_pressed = 1;
        return character;
    }
    if(shift_pressed == 1){
        character = keymap_uppercase[scanCode]; //sets character to the ascii character in the table using index scancode
    }
    else{
        character = keymap_lowercase[scanCode]; //sets character to the ascii character in the table using index scancode
    }
    if(Doesprint == 1){
        printChar(character);
    }
    return character;
}

unsigned int readString(char *output_buf, unsigned int buf_len){
    unsigned int numChars = 0;
    unsigned char currentChar = 0;
    while(currentChar != '|' && numChars < buf_len){    //stops reading string when the character is | (newline)
        currentChar = readCharBlock(1);
        if(currentChar != 0){
            *output_buf = currentChar;
            output_buf ++;
            numChars ++;
        }
    }
    *output_buf = 0;
    return numChars;
}

char commands[] = "help, clear, version|";

void commandHandler(char string[]){
    if(!fStrcmp(string, "clear|")){
        clearTerminal();
    }
    else if(!fStrcmp(string, "version|")){
        print(version);
    }
    else if(!fStrcmp(string, "help|")){
        print(commands);
    }
    else{
        print("Not a command|");
    }
    return;
}

int fStrcmp(char *stringa, char* stringb){
    while(*stringa == *stringb){
        if (*stringa == 0){
            return 0;
        }
        stringa++;
        stringb++;
    }
    return (*stringa - *stringb);
}

int intToString(unsigned int number, char *outputBuf, unsigned int bufLen){
    unsigned int numChars = 0;
    unsigned int temp = number;
    while(temp > 0){    //gets the number of characters the number is
        temp /= 10;
        numChars ++;
    }
    if(numChars > (bufLen - 1)){    //stops mem overflow
        print("bufLen too small");
        return -1;
    }
    char stringInverted[40];    //max of 40 character number
    char string[40];
    temp = 0;
    while(number > 0){    //makes the inverted string
        stringInverted[temp++] = (number % 10) + 48;  //+48 to make it ascii for the number
        number /= 10;
    }
    temp = 0;
    unsigned int temp2 = numChars - 1;
    while(temp <= (numChars - 1)){    //makes the non inverted string
        string[temp] = stringInverted[temp2];
        temp2 --;
        temp ++;
    }
    temp = 0;
    while(temp < numChars){   //puts string into the outputBuf
        *outputBuf = string[temp];
        temp ++;
        outputBuf ++;
    }
    *outputBuf = 0; //adds null terminator

    return 0;
}

int readDisk(unsigned int lbaSector, unsigned char numSectors, unsigned short int *dataBuf){
    if(((lbaSector >> 28) & 0x0f) != 0){  //Return early if lbaSector is >= 2^28
        return 1;
    }

    unsigned char busybit = 1;
    unsigned char driveReadyBit = 0;
    unsigned short int port = 0;

    port = 0x1F7;
    while(busybit != 0 && driveReadyBit == 0){   //checks weather the drive is busy
        unsigned char unprocessedBusyBit = 0;
        __asm__ volatile(
            "inb %%dx, %%al"
            : "=a" (unprocessedBusyBit)
            : "d" (port)
            : "cc"
        );
        busybit = unprocessedBusyBit & 0x80;
        driveReadyBit = unprocessedBusyBit & 0x40;
    }

    unsigned int num = 0xE0 | (lbaSector >> 24 & 0x0F);
    
    port = 0x1F6;
    __asm__ volatile(   //drive select, first 4 bits of lba
        "outb %%al, %%dx"
        :
        :"a" (num), "d" (port)
        :"cc"
    );

    port = 0x1F7;
    for (int d = 0; d < 4; d++) {   //4 dummy reads
        __asm__ volatile(
            "inb %%dx, %%al"
            :
            : "d" (port)
            : "ax", "cc"
        );
    }

    unsigned char lba_low = lbaSector & 0xFF;
    unsigned char lba_mid = (lbaSector >> 8) & 0xFF;
    unsigned char lba_high = (lbaSector >> 16) & 0xFF;

    port = 0x1F2;
    __asm__ volatile(   //tells how many sectors to read
        "outb %%al, %%dx"
        :
        :"a" (numSectors), "d" (port)
        :"cc"
    );

    port = 0x1F3;
    __asm__ volatile(   //smallest byte
        "outb %%al, %%dx"
        :   //output variables
        :"a" (lba_low), "d" (port) //input variables
        :"cc"   //registers we used
    );

    port = 0x1F4;
    __asm__ volatile(
        "outb %%al, %%dx"
        :
        :"a" (lba_mid), "d" (port)
        :"cc"
    );

    port = 0x1F5;
    __asm__ volatile(
        "outb %%al, %%dx"
        :
        :"a" (lba_high), "d" (port)
        :"cc"
    );

    __asm__ volatile(   //tells it to read
        "movb $0x20, %%al\n\t"
        "movw $0x1F7, %%dx\n\t"
        "outb %%al, %%dx"
        :
        :
        :"ax", "dx", "cc"
    );

    for(int i = 0; i < numSectors; i++){
        unsigned short int currentData = 0;
        unsigned char bsy = 0;
        unsigned char drdy = 0;
        unsigned char drq = 0;
        unsigned char err = 0;
        unsigned char df = 0;
        port = 0x1F7;

        while(bsy != 0 || drdy == 0 || drq == 0 || err != 0 || df != 0){
            __asm__ volatile(
                "inb %%dx, %%al"
                : "=a" (bsy)
                : "d" (port)
                : "cc"
            );
            drdy = bsy & 0x40;  //bit 6
            drq = bsy & 0x08;   //bit 3
            err = bsy & 0x01;   //bit 0
            df = bsy & 0x20;    //bit 5
            bsy = bsy & 0x80;   //bit 7

            if (err != 0 || df != 0) {
                return 2;
            }

            if (bsy == 0 && drq != 0) {
                break;
            }
        }   //do nothing until bsy is not set, drdy and drq are set and err and df are not set

        for (int w = 0; w < 256; w++){
            __asm__ volatile(
                "movw $0x1F0, %%dx\n\t"   //reads a 16 bit word into ax
                "inw %%dx, %%ax"
                :"=a" (currentData)
                :
                :"dx", "cc"
            );

            *dataBuf = currentData;
            dataBuf ++;
        }
        
    }

    return 0;
}
