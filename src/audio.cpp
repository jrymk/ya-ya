#include "audio.h"
#include "debugger.h"

void Audio::loadSound(std::string fileName){
    try{
        bufferVec.push_back(buffer);
        bufferVec[samples].loadFromFile(fileName);
        samples++;
    }
    catch(...){
        debug << "Error occured, could not load file\n";
    }
}

void Audio::playSound(int id){
    try{
        sound.setBuffer(bufferVec[id]);
        sound.play();
    }
    catch(...){
        debug << "Error occured, could not play file : " << id << "\n";
    }
    
}