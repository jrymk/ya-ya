#pragma once
#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <memory>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <cstring>
#include <type_traits>

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

/// @brief tools for serialization
namespace SaveUtilities{
    template<typename Class, typename MemberType>
    struct PropertyObj{
        MemberType Class::*member;
        const char* ID;
        inline constexpr PropertyObj(MemberType Class::*_member, const char* _ID): member(_member), ID(_ID){};
    };

    template<typename Class, typename MemberType>
    inline constexpr PropertyObj<Class, MemberType> property(MemberType Class::*member, const char* ID){
        return SaveUtilities::PropertyObj<Class, MemberType>{member, ID};
    }

    template<typename T, T... IdxSeq, typename FuncType>
    inline constexpr void forSequence(std::integer_sequence<T, IdxSeq...>, FuncType&& f){
        (static_cast<void>(f(std::integral_constant<T, IdxSeq>{})), ...);
    }
}


template<typename T>
inline std::string rserialize(const T& obj);  // recursive serialize helper
template<typename T>
inline void rdeserialize(T& obj, const std::string& str);  // recursive unserialize helper

/// @brief converting objects and vectors to string, vice versa
namespace Serialization{
    /* BEGIN serialize */
    template<typename T,
            typename std::enable_if_t<!std::is_pointer<T>::value>* = nullptr,
            typename std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
    inline std::string serialize(const T& obj){  // object
        std::string str;
        constexpr auto propertyCnt = std::tuple_size<decltype(T::properties)>::value;
        SaveUtilities::forSequence(std::make_index_sequence<propertyCnt>{}, [&](auto i){
            constexpr auto property = std::get<i>(T::properties);
            str.append("<" + std::string(property.ID) + ">");
            str.append(rserialize(obj.*(property.member)));
            str.append("</" + std::string(property.ID) + ">");
        });

        return str;
    }

    template<typename T,
            typename std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
    inline std::string serialize(const T ptr){  // object* and normal type*
        return rserialize(*ptr);
    }

    template<typename T,
            typename std::enable_if_t<std::is_enum<T>::value>* = nullptr>
    inline std::string serialize(const T& etp){  // enum
        return std::to_string(static_cast<unsigned>(etp));
    }

    template<typename T, typename U>  // shared_ptr
    inline std::string serialize(const std::shared_ptr<U>& ptr){
        return rserialize(*ptr);
    }

    template<typename T, typename U, typename A>  // vector<object>
    inline std::string serialize(const std::vector<U, A>& obj){
        std::string str;
        str.append("<VecSize>" + std::to_string(obj.size()) + "</VecSize>[");
        for(const U& element : obj)
            str.append(rserialize(element) + ",");
       
        if(obj.size()) str.pop_back();
        str.append("]");
        return str;
    }

    template<typename T, typename U, typename A>  // vector<object*>
    inline std::string serialize(const std::vector<U*, A>& obj){
        std::string str;
        str.append("<VecSize>" + std::to_string(obj.size()) + "</VecSize>[");
        for(const U* element : obj)
            str.append(rserialize(*element) + ",");
    
        if(obj.size()) str.pop_back();
        str.append("]");

        return str;
    }

    template<typename T, typename K, typename V, typename C, typename A>
    inline std::string serialize(const std::map<K, V, C, A>& obj){  // map<key, object>
        std::string str;
        str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
        for(const std::pair<const K, V>& kvpair : obj)
            str.append(rserialize(kvpair.first) + ":" + rserialize(kvpair.second) + ";");

        if(obj.size()) str.pop_back();
            str.append("}");

        return str;
    }

    template<typename T, typename K, typename V, typename C, typename A>
    inline std::string serialize(const std::map<K, V*, C, A>& obj){  // map<key, object*>
        std::string str;
        str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
        for(const std::pair<const K, V*>& kvpair : obj)
            str.append(rserialize(kvpair.first) + ":" + rserialize(*kvpair.second) + ";");

        if(obj.size()) str.pop_back();
        str.append("}");

        return str;
    }

    template<>
    inline std::string serialize(const int& val){
        return std::to_string(val);
    }
    template<>
    inline std::string serialize(const bool& val){
        return std::to_string(val);
    }
    template<>
    inline std::string serialize(const float& val){
        return std::to_string(val);
    }
    template<>
    inline std::string serialize(const double& val){
        return std::to_string(val);
    }
    template<>
    inline std::string serialize(const std::string& str){
        return str;
    }
    /* END serialize*/

    /* BEGIN deserialize */
    template<typename T,
            typename std::enable_if_t<!std::is_pointer<T>::value>* = nullptr,
            typename std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
    inline void deserialize(T& obj, const std::string& str){  // object
        constexpr auto propertyCnt = std::tuple_size<decltype(T::properties)>::value;
        SaveUtilities::forSequence(std::make_index_sequence<propertyCnt>{}, [&](auto i){
            constexpr auto property = std::get<i>(T::properties);

            int tagStart = str.find("<" + std::string(property.ID) + ">"),
                tagEnd = str.find("</" + std::string(property.ID) + ">"),
                tagLen = strlen(property.ID) + 2;
            rdeserialize(obj.*(property.member), str.substr(tagStart + tagLen, tagEnd - tagStart - tagLen));
        });
    }

    template<typename T,
            typename std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
    inline void deserialize(T& ptr, const std::string& str){  // object* and normal type*
        typedef typename std::remove_pointer<T>::type U;
        ptr = new U;
        rdeserialize(*ptr, str);
    }

    template<typename T,
            typename std::enable_if_t<std::is_enum<T>::value>* = nullptr>
    inline void deserialize(T& etp, const std::string& str){  // enum
        etp = static_cast<T>(std::stoi(str));
    }

    template<typename T, typename U>
    inline void deserialize(std::shared_ptr<U>& obj, const std::string& str){  // shared_ptr
        obj = std::make_shared<U>();
        rdeserialize(*obj, str);
    }
    
    template<typename T, typename U, typename A>  // vector<object>
    inline void deserialize(std::vector<U, A>& obj, const std::string& str){
        int sizeTagStart = str.find("<VecSize>"),
            sizeTagEnd = str.find("</VecSize>");
        int vecSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
            searchIdx = str.find("[") + 1;

        obj.resize(vecSize);
        for(size_t i = 0; i < vecSize; i++){
            int nxtIdx = searchIdx;
            if(i == vecSize - 1) nxtIdx = str.size() - 1;
            else{
                int brackets = 0;
                while(brackets || str[nxtIdx] != ','){
                    if(str[nxtIdx] == '[') brackets++;
                    if(str[nxtIdx] == ']') brackets--;
                    nxtIdx++;
                }
            }
            rdeserialize(obj[i], str.substr(searchIdx, nxtIdx - searchIdx));
            searchIdx = nxtIdx + 1;
        }
    }

    template<typename T, typename U, typename A>  // vector<object*>
    inline void deserialize(std::vector<U*, A>& obj, const std::string& str){
        int sizeTagStart = str.find("<VecSize>"),
            sizeTagEnd = str.find("</VecSize>");
        int vecSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
            searchIdx = str.find("[") + 1;

        obj.resize(vecSize);
        for(size_t i = 0; i < vecSize; i++){
            int nxtIdx = searchIdx;
            if(i == vecSize - 1) nxtIdx = str.size() - 1;
            else{
                int brackets = 0;
                while(brackets || str[nxtIdx] != ','){
                    if(str[nxtIdx] == '[') brackets++;
                    if(str[nxtIdx] == ']') brackets--;
                    nxtIdx++;
                }
            }
            obj[i] = new U;
            rdeserialize(*obj[i], str.substr(searchIdx, nxtIdx - searchIdx));
            searchIdx = nxtIdx + 1;
        }
    }
    
    template<typename T, typename K, typename V, typename C, typename A>  // map<key, object>
    inline void deserialize(std::map<K, V, C, A>& obj, const std::string& str){
        int sizeTagStart = str.find("<MapSize>"),
            sizeTagEnd = str.find("</MapSize>");
        int mapSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
            searchIdx = str.find("{") + 1;

        obj.clear();
        for(size_t i = 0; i < mapSize; i++){
            int nxtIdx = searchIdx, sepIdx = 0, brackets = 0;
            if(i == mapSize - 1){
                nxtIdx = str.size() - 1;
                sepIdx = searchIdx;
                while(brackets || str[sepIdx] != ':'){
                    if(str[sepIdx] == '{') brackets++;
                    if(str[sepIdx] == '}') brackets--;
                    sepIdx++;
                }
            }
            else while(brackets || str[nxtIdx] != ';'){
                if(str[nxtIdx] == '{') brackets++;
                if(str[nxtIdx] == '}') brackets--;
                if((!brackets) && str[nxtIdx] == ':') sepIdx = nxtIdx;
                nxtIdx++;
            }
            K key; V value;
            rdeserialize(key, str.substr(searchIdx, sepIdx - searchIdx));
            rdeserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
            obj[key] = value;
            searchIdx = nxtIdx + 1;
        }
    }


    template<typename T, typename K, typename V, typename C, typename A>  // map<key, object*>
    inline void deserialize(std::map<K, V*, C, A>& obj, const std::string& str){
        int sizeTagStart = str.find("<MapSize>"),
            sizeTagEnd = str.find("</MapSize>");
        int mapSize = std::stoi(str.substr(sizeTagStart + 9, sizeTagEnd - sizeTagStart - 9)),
            searchIdx = str.find("{") + 1;

        obj.clear();
        for(size_t i = 0; i < mapSize; i++){
            int nxtIdx = searchIdx, sepIdx = 0, brackets = 0;
            if(i == mapSize - 1){
                nxtIdx = str.size() - 1;
                sepIdx = searchIdx;
                while(brackets || str[sepIdx] != ':'){
                    if(str[sepIdx] == '{') brackets++;
                    if(str[sepIdx] == '}') brackets--;
                    sepIdx++;
                }
            }
            else while(brackets || str[nxtIdx] != ';'){
                if(str[nxtIdx] == '{') brackets++;
                if(str[nxtIdx] == '}') brackets--;
                if((!brackets) && str[nxtIdx] == ':') sepIdx = nxtIdx;
                nxtIdx++;
            }
            K key; V* value = new V;
            rdeserialize(key, str.substr(searchIdx, sepIdx - searchIdx));
            rdeserialize(*value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
            obj[key] = value;
            searchIdx = nxtIdx + 1;
        }
    }
    
    template<>
    inline void deserialize(int& val, const std::string& data){
        val = std::stoi(data);
    }
    template<>
    inline void deserialize(bool& val, const std::string& data){
        val = std::stoi(data);
    }
    template<>
    inline void deserialize(float& val, const std::string& data){
        val = std::stof(data);
    }
    template<>
    inline void deserialize(double& val, const std::string& data){
        val = std::stod(data);
    }
    template<>
    inline void deserialize(std::string& str, const std::string& data){
        str = data;
    }
    /* END deserialize */
}

template<typename T>
inline std::string rserialize(const T& obj){
    return Serialization::serialize<T>(obj);
}
template<typename T>
inline void rdeserialize(T& obj, const std::string& str){
    Serialization::deserialize<T>(obj, str);
}

#endif