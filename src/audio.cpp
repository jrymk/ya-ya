#include "audio.h"
#include "debugger.h"

void Audio::playSound(std::string fileName){
    try {
        buffer.loadFromFile(fileName);
        sound.setBuffer(buffer);
        sound.play();
    }
    catch(...){
        debug << "Error occured, could not load file\n";
    }
    
}