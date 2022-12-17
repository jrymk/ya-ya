#pragma once
#ifndef YAYA_SERIALIZATION_H
#define YAYA_SERIALIZATION_H

#include "saveUtilities.h"

/*  SERIALIZATION GUIDES
    1. to use vector serialization, template Class must implement default constructor
    2. every class must define properties like this:
        constexpr static auto properties = std::make_tuple(
            SaveUtilities::property(&Pos::x, "Ps.x"),
            SaveUtilities::property(&Pos::y, "Ps.y")s
        );
    3. NO ",:;[]{}" symbols in property ID (2nd arg)
    4. serialize usage:
        string = Serialization::serialize<Class><object>    (!!! Class cannot be omitted !!!)
    5. deserialize usage:
        Serialization::deserialize<Class>(object, string)   */

template<typename T>
inline std::string rserialize(const T &obj);  // recursive serialize helper
template<typename T>
inline void rdeserialize(T &obj, const std::string &str);  // recursive unserialize helper

/// @brief converting objects and vectors to string, vice versa
namespace Serialization {
    /* BEGIN serialize */
    template<typename T,
            typename std::enable_if_t<!std::is_pointer<T>::value>* = nullptr,
            typename std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
    inline std::string serialize(const T &obj) {  // object
        std::string str;
        constexpr auto propertyCnt = std::tuple_size<decltype(T::properties)>::value;
        SaveUtilities::forSequence(std::make_index_sequence<propertyCnt>{}, [&](auto i) {
            constexpr auto property = std::get<i>(T::properties);
            str.append("<" + std::string(property.ID) + ">");
            str.append(rserialize(obj.*(property.member)));
            str.append("</" + std::string(property.ID) + ">");
        });

        return str;
    }

    template<typename T,
            typename std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
    inline std::string serialize(const T ptr) {  // object* and normal type*
        if(!ptr)
            return "<Addr>-1</Addr>";
        else if(SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr)])
            return SaveUtilities::getAddressStr(ptr);
        else{
            SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr)] = 1;
            return SaveUtilities::getAddressStr(ptr) + rserialize(*ptr);
        }
    }

    template<typename T,
            typename std::enable_if_t<std::is_enum<T>::value>* = nullptr>
    inline std::string serialize(const T &etp) {  // enum
        return std::to_string(static_cast<unsigned>(etp));
    }

    template<typename T, typename U>  // reminder: modify extended
    inline std::string serialize(const std::shared_ptr<U> &ptr) {  // shared_ptr
        if(!ptr)
            return "<Addr>-1</Addr>";
        else if(SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr.get())])
            return SaveUtilities::getAddressStr(ptr.get());
        else{
            SaveUtilities::checkSerialized[SaveUtilities::getAddressNum(ptr.get())] = 1;
            return SaveUtilities::getAddressStr(ptr.get()) + rserialize(*ptr);
        }
    }

    template<typename T, typename U, typename A>
    // vector<object>
    inline std::string serialize(const std::vector<U, A> &obj) {
        std::string str;
        str.append("<VecSize>" + std::to_string(obj.size()) + "</VecSize>[");
        for (const U &element: obj)
            str.append(rserialize(element) + ",");

        if (obj.size()) str.pop_back();
        str.append("]");
        return str;
    }

    template<typename T, typename K, typename V, typename C, typename A>
    inline std::string serialize(const std::map<K, V, C, A> &obj) {  // map<key, object>
        std::string str;
        str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
        for (const std::pair<const K, V> &kvpair: obj)
            str.append(rserialize(kvpair.first) + ":" + rserialize(kvpair.second) + ";");

        if (obj.size()) str.pop_back();
        str.append("}");

        return str;
    }

    template<>
    inline std::string serialize(const int &val) {
        return std::to_string(val);
    }

    template<>
    inline std::string serialize(const bool &val) {
        return std::to_string(val);
    }

    template<>
    inline std::string serialize(const char &val) {
        return std::to_string(static_cast<int>(val));
    }

    template<>
    inline std::string serialize(const float &val) {
        return std::to_string(val);
    }

    template<>
    inline std::string serialize(const double &val) {
        return std::to_string(val);
    }

    template<>
    inline std::string serialize(const std::string &str) {
        return str;
    }
    /* END serialize*/

    /* BEGIN deserialize */
    template<typename T,
            typename std::enable_if_t<!std::is_pointer<T>::value>* = nullptr,
            typename std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
    inline void deserialize(T &obj, const std::string &str) {  // object
        if(!str.size()) return;  // avoid empty object
        constexpr auto propertyCnt = std::tuple_size<decltype(T::properties)>::value;

        int findTagStartStart = 0;  // to avoid <Ivt><zdo> a </zdo></Ivt> <zdo> b </zdo>
        SaveUtilities::forSequence(std::make_index_sequence<propertyCnt>{}, [&](auto i){
            constexpr auto property = std::get<i>(T::properties);
            // debug << typeid(T).name() << " finding " << "<" + std::string(property.ID) + ">" << '\n';

            int tagStart = str.find("<" + std::string(property.ID) + ">", findTagStartStart),
                tagLen = strlen(property.ID) + 2,
                tagEnd = tagStart;
            int tags = 0;
            while(true){  // to avoid <Ivt><Ivt> a </Ivt></Ivt> <Ivt> b </Ivt>
                if(str.substr(tagEnd, tagLen) == "<" + std::string(property.ID) + ">"){
                    tags++;
                    tagEnd += tagLen;
                }
                else if(str.substr(tagEnd, tagLen + 1) == "</" + std::string(property.ID) + ">"){
                    tags--;
                    if(!tags) break;
                    tagEnd += tagLen + 1;
                }
                else tagEnd++;
            }
            rdeserialize(obj.*(property.member), str.substr(tagStart + tagLen, tagEnd - tagStart - tagLen));
            findTagStartStart = tagEnd + tagLen + 1;
        });
    }

    template<typename T,
            typename std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
    inline void deserialize(T &ptr, const std::string &str) {  // object* and normal type*
        if(!str.size()) return;  // avoid empty object
        typedef typename std::remove_pointer<T>::type U;
        int addrStart = str.find("<Addr>"),
                addrEnd = str.find("</Addr>");
        std::uintptr_t oldAddress = std::stoull(str.substr(addrStart + 6, addrEnd - addrStart - 6));

        if (oldAddress == -1) {
            ptr = nullptr;
            return;
        }
        else if (SaveUtilities::objectTracker[oldAddress])
            ptr = static_cast<T>(SaveUtilities::objectTracker[oldAddress]);
        else {
            ptr = new U;
            SaveUtilities::objectTracker[oldAddress] = ptr;
            rdeserialize(*ptr, str.substr(addrEnd + 7));
        }
    }

    template<typename T,
            typename std::enable_if_t<std::is_enum<T>::value>* = nullptr>
    inline void deserialize(T &etp, const std::string &str) {  // enum
        if(!str.size()) return;  // avoid empty object
        etp = static_cast<T>(std::stoi(str));
    }

    template<typename T, typename U>  // reminder: modify extended
    inline void deserialize(std::shared_ptr<U> &ptr, const std::string &str) {  // shared_ptr
        if(!str.size()) return;  // avoid empty object
        int addrStart = str.find("<Addr>"),
                addrEnd = str.find("</Addr>");
        std::uintptr_t oldAddress = std::stoull(str.substr(addrStart + 6, addrEnd - addrStart - 6));

        if (oldAddress == -1) {
            ptr = nullptr;
            return;
        }
        else if (SaveUtilities::smartObjectTracker[oldAddress])
            ptr = std::static_pointer_cast<U>(SaveUtilities::smartObjectTracker[oldAddress]);
        else {
            ptr = std::make_shared<U>();
            SaveUtilities::smartObjectTracker[oldAddress] = ptr;
            rdeserialize(*ptr, str.substr(addrEnd + 7));
        }
    }

    template<typename T, typename U, typename A>
    // vector<object>
    inline void deserialize(std::vector<U, A> &obj, const std::string &str) {
        if(!str.size()) return;  // avoid empty object
        int sizeTagStart = str.find("<VecSize>"),
                sizeTagEnd = str.find("</VecSize>");
        int vecSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
                searchIdx = str.find("[") + 1;

        obj.resize(vecSize);
        for (size_t i = 0; i < vecSize; i++) {
            int nxtIdx = searchIdx;
            if (i == vecSize - 1) nxtIdx = str.size() - 1;
            else {
                int brackets = 0;
                while (brackets || str[nxtIdx] != ',') {
                    if (str[nxtIdx] == '[') brackets++;
                    if (str[nxtIdx] == ']') brackets--;
                    nxtIdx++;
                }
            }
            rdeserialize(obj[i], str.substr(searchIdx, nxtIdx - searchIdx));
            searchIdx = nxtIdx + 1;
        }
    }

    template<typename T, typename K, typename V, typename C, typename A>
    // map<key, object>
    inline void deserialize(std::map<K, V, C, A> &obj, const std::string &str) {
        if(!str.size()) return;  // avoid empty object
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
            K key;
            V value;
            rdeserialize(key, str.substr(searchIdx, sepIdx - searchIdx));
            rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
            obj[key] = value;
            searchIdx = nxtIdx + 1;
        }
    }

    template<>
    inline void deserialize(int &val, const std::string &data) {
        if(!data.size()) return;  // avoid empty object
        val = std::stoi(data);
    }

    template<>
    inline void deserialize(bool &val, const std::string &data) {
        if(!data.size()) return;  // avoid empty object
        val = std::stoi(data);
    }

    template<>
    inline void deserialize(char &val, const std::string &data) {
        if(!data.size()) return;  // avoid empty object
        val = static_cast<char>(std::stoi(data));
    }

    template<>
    inline void deserialize(float &val, const std::string &data) {
        if(!data.size()) return;  // avoid empty object
        val = std::stof(data);
    }

    template<>
    inline void deserialize(double &val, const std::string &data) {
        if(!data.size()) return;  // avoid empty object
        val = std::stod(data);
    }

    template<>
    inline void deserialize(std::string &str, const std::string &data) {
        str = data;
    }
    /* END deserialize */
}

template<typename T>
inline std::string rserialize(const T &obj) {
    return Serialization::serialize<T>(obj);
}

template<typename T>
inline void rdeserialize(T &obj, const std::string &str) {
    if(!str.size()) return;  // avoid empty object
    Serialization::deserialize<T>(obj, str);
}

#endif