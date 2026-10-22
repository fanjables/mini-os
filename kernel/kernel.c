char *const vbuffer = (char*)0xb8000;
unsigned int vbufferValue = 0;

void print(char *string);
void printExtended(char *string, unsigned int setting);
void clearTerminal();
int coordsToValue(unsigned int x, unsigned int y);
struct vec2 valueToCoords(unsigned int value);

void main(){    //void main beacuse there is no os to return a value to
    print("test");
    clearTerminal();
    print("FANJ OS v0.2|Newline test|");
    printExtended("Settings test", 219);
    return;
}

void print(char *string){
    if(vbufferValue > 4000){    //clears the screen if vbufferValue is greater than 4000
        vbufferValue = 0;
        while(vbufferValue <= 4000){
            vbuffer[vbufferValue] = 0;
            vbufferValue += 2;
        }
        vbufferValue = 0;
    }
    while(*string != 0){
        if(*string == '|'){ //handles newlines
            int charOnLine = vbufferValue % 160; //finds how many char are on current line, remainder of vbufferValue / 80 (the max amount of chars on a line)
            vbufferValue += (160 - charOnLine); //moves vbufferValue to firs cell on next line
        }
        else{   //prints characters
            vbuffer[vbufferValue] = *string;
            vbufferValue += 2;
        }
        string ++;  //moves string to the next character
    }
    return;
}

void printExtended(char *string, unsigned int setting){
    if(vbufferValue > 4000){    //clears the screen if vbufferValue is greater than 4000
        vbufferValue = 0;
        while(vbufferValue <= 4000){
            vbuffer[vbufferValue] = 0;
            vbufferValue += 2;
        }
        vbufferValue = 0;
    }
    while(*string != 0){
        if(*string == '|'){ //handles newlines
            int charOnLine = vbufferValue % 160; //finds how many char are on current line, remainder of vbufferValue / 80 (the max amount of chars on a line)
            vbufferValue += (160 - charOnLine); //moves vbufferValue to firs cell on next line
        }
        else{   //prints characters
            vbuffer[vbufferValue] = *string;
            vbufferValue ++;
            vbuffer[vbufferValue] = setting;
            vbufferValue ++;
        }
        string ++;  //moves string to the next character
    }
    return;
}

void clearTerminal(){
    vbufferValue = 0;
    while(vbufferValue <= 4000){
        vbuffer[vbufferValue] = 0;
        vbufferValue += 2;
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
