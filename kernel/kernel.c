char *const vbuffer = (char*)0xb8000;
unsigned int vbufferValue = 0;

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

void main(){    //void main beacuse there is no os to return a value to
    print("test");
    clearTerminal();
    print("FANJ OS v0.4|");
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
    while(vbufferValue <= 4000){
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

// A standard US-QWERTY Scan Code Set 1 translation table
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
        character = keymap_uppercase[scanCode];
    }
    else{
        character = keymap_lowercase[scanCode];
    }
    if(Doesprint == 1){
        printChar(character);
    }
    return character;
}

unsigned int readString(char *output_buf, unsigned int buf_len){
    unsigned int numChars = 0;
    unsigned char currentChar = 0;
    while(currentChar != '|' && numChars < buf_len){
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

void commandHandler(char string[]){
//    print(string);
    if(!fStrcmp(string, "clear|")){
        clearTerminal();
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
