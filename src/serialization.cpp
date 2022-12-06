#include "serialization.h"

template<typename Class, typename MemberType>
constexpr SaveUtilities::PropertyObj<Class, MemberType> SaveUtilities::property(MemberType Class::*member, const char* ID){
    return SaveUtilities::PropertyObj<Class, MemberType>{member, ID};
}

template<typename T, T... IdxSeq, typename FuncType>
constexpr void SaveUtilities::forSequence(std::integer_sequence<T, IdxSeq...>, FuncType&& f){
    (static_cast<void>(f(std::integral_constant<T, IdxSeq>{})), ...);
}

/* START serialize */
template<typename T>
std::string Serialization::serialize(const T& obj){
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

template<typename T, typename U, typename A>  // vector<object>
std::string Serialization::serialize(const std::vector<U, A>& obj){
    std::string str;
    str.append("<VecSize>" + std::to_string(obj.size()) + "</VecSize>[");
    for(const U& element : obj)
        str.append(rserialize(element) + ",");
       
    if(obj.size()) str.pop_back();
    str.append("]");
    return str;
}

template<typename T, typename U, typename A>  // vector<object*>
std::string Serialization::serialize(const std::vector<U*, A>& obj){
    std::string str;
    str.append("<VecSize>" + std::to_string(obj.size()) + "</VecSize>[");
    for(const U* element : obj)
        str.append(rserialize(*element) + ",");
    
    if(obj.size()) str.pop_back();
    str.append("]");

    return str;
}

template<typename T, typename K, typename V, typename C, typename A>
std::string Serialization::serialize(const std::map<K, V, C, A>& obj){  // map<key, object>
    std::string str;
    str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
    for(const std::pair<const K, V>& kvpair : obj)
        str.append(rserialize(kvpair.first) + ":" + rserialize(kvpair.second) + ";");
        
    if(obj.size()) str.pop_back();
    str.append("}");

    return str;
}

template<typename T, typename K, typename V, typename C, typename A>
std::string Serialization::serialize(const std::map<K, V*, C, A>& obj){  // map<key, object*>
    std::string str;
    str.append("<MapSize>" + std::to_string(obj.size()) + "</MapSize>{");
    for(const std::pair<const K, V*>& kvpair : obj)
        str.append(rserialize(kvpair.first) + ":" + rserialize(*kvpair.second) + ";");
    
    if(obj.size()) str.pop_back();
    str.append("}");

    return str;
}

template<>
std::string Serialization::serialize(const int& val){
    return std::to_string(val);
}
template<>
std::string Serialization::serialize(const float& val){
    return std::to_string(val);
}
template<>
std::string Serialization::serialize(const double& val){
    return std::to_string(val);
}
template<>
std::string Serialization::serialize(const std::string& str){
    return str;
}
/* END serialize */

/* START deserialize */
template<typename T>
void Serialization::deserialize(T& obj, const std::string& str){
    constexpr auto propertyCnt = std::tuple_size<decltype(T::properties)>::value;
    SaveUtilities::forSequence(std::make_index_sequence<propertyCnt>{}, [&](auto i){
        constexpr auto property = std::get<i>(T::properties);
        
        int tagStart = str.find("<" + std::string(property.ID) + ">"),
            tagEnd = str.find("</" + std::string(property.ID) + ">"),
            tagLen = strlen(property.ID) + 2;
        runserialize(obj.*(property.member), str.substr(tagStart + tagLen, tagEnd - tagStart - tagLen));
    });
}
    
template<typename T, typename U, typename A>  // vector<object>
void Serialization::deserialize(std::vector<U, A>& obj, const std::string& str){
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
        runserialize(obj[i], str.substr(searchIdx, nxtIdx - searchIdx));
        searchIdx = nxtIdx + 1;
    }
}

template<typename T, typename U, typename A>  // vector<object*>
void Serialization::deserialize(std::vector<U*, A>& obj, const std::string& str){
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
        runserialize(*obj[i], str.substr(searchIdx, nxtIdx - searchIdx));
        searchIdx = nxtIdx + 1;
    }
}

template<typename T, typename K, typename V, typename C, typename A>  // map<key, object>
void Serialization::deserialize(std::map<K, V, C, A>& obj, const std::string& str){
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
        runserialize(key, str.substr(searchIdx, sepIdx - searchIdx));
        runserialize(value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
        obj[key] = value;
        searchIdx = nxtIdx + 1;
    }
}

template<typename T, typename K, typename V, typename C, typename A>  // map<key, object*>
void Serialization::deserialize(std::map<K, V*, C, A>& obj, const std::string& str){
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
        runserialize(key, str.substr(searchIdx, sepIdx - searchIdx));
        runserialize(*value, str.substr(sepIdx + 1, nxtIdx - sepIdx - 1));
        obj[key] = value;
        searchIdx = nxtIdx + 1;
    }
}
    
template<>
void Serialization::deserialize(int& val, const std::string& data){
    val = std::stoi(data);
}
template<>
void Serialization::deserialize(float& val, const std::string& data){
    val = std::stof(data);
}
template<>
void Serialization::deserialize(double& val, const std::string& data){
    val = std::stod(data);
}
template<>
void Serialization::deserialize(std::string& str, const std::string& data){
    str = data;
}
/* END deserialize */

template<typename T>
std::string rserialize(const T& obj){
    return Serialization::serialize<T>(obj);
}
template<typename T>
void runserialize(T& obj, const std::string& str){
    Serialization::deserialize<T>(obj, str);
}