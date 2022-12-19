#include "audio.h"
#include "debugger.h"

void Audio::loadSound(std::string fileName) {
    try{
        bufferVec.push_back(buffer);
        soundVec.push_back(sound);
        loop.push_back(false);
        bufferVec[samples].loadFromFile(fileName);
        samples++;
    }
    catch(...){
        debug << "Error occured, could not load file\n";
    }
}

void Audio::playSound(int id) {
    try{
        soundVec[id].setBuffer(bufferVec[id]);
        soundVec[id].setLoop(loop[id]);
        soundVec[id].play();
    }
    catch(...){
        debug << "Error occured, could not play file or the file isn't loaded yet : " << id << "\n";
    }
    
}

void Audio::playBGM() {
    try{
        soundVec[BGM_ID].setBuffer(bufferVec[BGM_ID]);
        loop[BGM_ID] = true;
        soundVec[BGM_ID].setLoop(loop[BGM_ID]);
        soundVec[BGM_ID].setVolume(5);
        soundVec[BGM_ID].play();
    }
    catch(...){
        debug << "Error occured, could not play file or the file isn't loaded yet : " << BGM_ID << "\n";
    }
    
}

void Audio::muteSound() {
    for(auto i : index(soundVec)) {
        if(i.second.getVolume() != 0.)
            i.second.setVolume(0);
        else if(i.second.getVolume() == 0. && i.first != BGM_ID)
            i.second.setVolume(100);
        else if(i.second.getVolume() == 0. && i.first == BGM_ID)
            i.second.setVolume(5);
    }
}

void Audio::setLoop(int id, bool set) {
    if(set)
        loop[id] = true;
    else
        loop[id] = false;
}