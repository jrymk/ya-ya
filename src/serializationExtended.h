#pragma once
#ifndef YAYA_SERIALIZATIONEXTENDED_H
#define YAYA_SERIALIZATIONEXTENDED_H

#include "serialization.h"
#include "entity.h"
#include "duck.h"
#include "player.h"
#include "egg.h"
#include "eggcarton.h"
#include "truck.h"

namespace Serialization{
    template<>
    inline std::string serialize<std::shared_ptr<Entity>, Entity>(const std::shared_ptr<Entity> &ptr) {  // specialization for entities
        if(!ptr)
            return "<Addr>-1</Addr>";
        else switch(ptr -> type){
            case PLAYER: {
                return rserialize(std::dynamic_pointer_cast<Player>(ptr));
                break;
            }
            case DUCK: {
                return rserialize(std::dynamic_pointer_cast<Duck>(ptr));
                break;
            }
            case EGG: {
                return rserialize(std::dynamic_pointer_cast<Egg>(ptr));
                break;
            }
            case EGG_CARTON: {
                return rserialize(std::dynamic_pointer_cast<EggCarton>(ptr));
                break;
            }
            case TRUCK: {
                return rserialize(std::dynamic_pointer_cast<Truck>(ptr));
                break;
            }
            default: {  // from original definition
                if(SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr.get())])
                    return SaveUtilities::getAddressStr(ptr.get());
                else{
                    SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr.get())] = 1;
                    return SaveUtilities::getAddressStr(ptr.get()) + rserialize(*ptr);
                }
                break;
            }
        }
    }

    template<>  // from original definition
    inline void deserialize<std::shared_ptr<Entity>, Entity>(std::shared_ptr<Entity>& ptr, const std::string &str) {  // specialization for entities map
        int addrStart = str.find("<Addr>"),
                addrEnd = str.find("</Addr>");
        std::uintptr_t oldAddress = std::stoull(str.substr(addrStart + 6, addrEnd - addrStart - 6));

        if (oldAddress == -1) {
            ptr = nullptr;
            return;
        }
        else if (SaveUtilities::smartObjectTracker[oldAddress])
            ptr = std::static_pointer_cast<Entity>(SaveUtilities::smartObjectTracker[oldAddress]);
        else {
            int idStart = str.find("<Ey.id>"), idEnd = str.find("</Ey.id>");
            std::string id = str.substr(idStart + 7, idEnd - idStart - 7);

            if(id.find("player$") != std::string::npos){
                auto derivedPtr = SaveUtilities::make_derived<Player>(ptr);
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*derivedPtr, str.substr(addrEnd + 7));
            }
            else if(id.find("duck$") != std::string::npos){
                auto derivedPtr = SaveUtilities::make_derived<Duck>(ptr);
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*derivedPtr, str.substr(addrEnd + 7));
            }
            else if(id.find("egg$") != std::string::npos){
                auto derivedPtr = SaveUtilities::make_derived<Egg>(ptr);
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*derivedPtr, str.substr(addrEnd + 7));
            }
            else if(id.find("eggcarton$") != std::string::npos){
                auto derivedPtr = SaveUtilities::make_derived<EggCarton>(ptr);
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*derivedPtr, str.substr(addrEnd + 7));
            }
            else if(id.find("truck$") != std::string::npos){
                auto derivedPtr = SaveUtilities::make_derived<Truck>(ptr);
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*derivedPtr, str.substr(addrEnd + 7));
            }
            else{
                ptr = std::make_shared<Entity>();
                SaveUtilities::smartObjectTracker[oldAddress] = ptr;
                rdeserialize(*ptr, str.substr(addrEnd + 7));
            }
        }
    }
}

#endif