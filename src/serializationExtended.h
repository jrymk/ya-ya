#pragma once
#ifndef YAYA_SERIALIZATIONEXTENDED_H
#define YAYA_SERIALIZATIONEXTENDED_H

#include "serialization.h"
#include "entity.h"
#include "duck.h"
#include "player.h"
#include "egg.h"

namespace Serialization{
    template<>
    inline std::string serialize<std::map<std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > >,
                                std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > >
                                (const std::map<std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > > &obj) {  // specialization for entities map
        std::string str;
        str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
        for (const auto &kvpair: obj){
            str.append(rserialize(kvpair.first) + ":");
            if(kvpair.first.find("player$") != std::string::npos){
                typedef Player T;
                str.append(rserialize<std::shared_ptr<T> >(std::dynamic_pointer_cast<T>(kvpair.second)) + ";");
            }
            else if(kvpair.first.find("duck$") != std::string::npos){
                typedef Duck T;
                str.append(rserialize<std::shared_ptr<T> >(std::dynamic_pointer_cast<T>(kvpair.second)) + ";");
            }
            else if(kvpair.first.find("egg$") != std::string::npos){
                typedef Egg T;
                str.append(rserialize<std::shared_ptr<T> >(std::dynamic_pointer_cast<T>(kvpair.second)) + ";");
            }
            else{
                typedef Entity T;
                str.append(rserialize<std::shared_ptr<T> >(std::dynamic_pointer_cast<T>(kvpair.second)) + ";");
            }    
        }
        if (obj.size()) str.pop_back();
        str.append("}");

        return str;
    }

    template<>
    inline void deserialize<std::map<std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > >,
                            std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > >
                            (std::map<std::string, std::shared_ptr<Entity>, std::less<std::string>, std::allocator<std::pair<const std::string, std::shared_ptr<Entity> > > > &obj, const std::string &str) { // specialization for entities map
        int sizeTagStart = str.find("<MapSize>"),
                sizeTagEnd = str.find("</MapSize>");
        int mapSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
                searchIdx = str.find("{") + 1;

        obj.clear();
        for (size_t i = 0; i < mapSize; i++) {
            int nxtIdx = searchIdx, sepIdx = 0, brackets = 0;
            if (i == mapSize - 1) {
                nxtIdx = str.size() - 1;
                sepIdx = searchIdx;
                while (brackets || str[sepIdx] != ':') {
                    if (str[sepIdx] == '{') brackets++;
                    if (str[sepIdx] == '}') brackets--;
                    sepIdx++;
                }
            }
            else while (brackets || str[nxtIdx] != ';') {
                if (str[nxtIdx] == '{') brackets++;
                if (str[nxtIdx] == '}') brackets--;
                if ((!brackets) && str[nxtIdx] == ':') sepIdx = nxtIdx;
                nxtIdx++;
            }
            std::string key;
            rdeserialize(key, str.substr(searchIdx, sepIdx - searchIdx));

            if(key.find("player$") != std::string::npos){
                typedef std::shared_ptr<Player> V;
                V value;
                rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
                obj[key] = value;
            }
            else if(key.find("duck$") != std::string::npos){
                typedef std::shared_ptr<Duck> V;
                V value;
                rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
                obj[key] = value;
            }
            else if(key.find("egg$") != std::string::npos){
                typedef std::shared_ptr<Egg> V;
                V value;
                rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
                obj[key] = value;
            }
            else{
                typedef std::shared_ptr<Entity> V;
                V value;
                rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
                obj[key] = value;
            }
            searchIdx = nxtIdx + 1;
        }
    }
}

#endif