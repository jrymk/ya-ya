#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <cstring>

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
    constexpr PropertyObj<Class, MemberType> property(MemberType Class::*member, const char* ID);

    template<typename T, T... IdxSeq, typename FuncType>
    constexpr void forSequence(std::integer_sequence<T, IdxSeq...>, FuncType&& f);
}

template<typename T>
std::string rserialize(const T& obj);  // recursive serialize helper
template<typename T>
void runserialize(T& obj, const std::string& str);  // recursive deserialize helper

/// @brief converting objects and vectors to string, vice versa
namespace Serialization{
    /* BEGIN serialize */
    template<typename T>
    std::string serialize(const T& obj);

    template<typename T, typename U, typename A>  // vector<object>
    std::string serialize(const std::vector<U, A>& obj);

    template<typename T, typename U, typename A>  // vector<object*>
    std::string serialize(const std::vector<U*, A>& obj);

    template<typename T, typename K, typename V, typename C, typename A>
    std::string serialize(const std::map<K, V, C, A>& obj);  // map<key, object>

    template<typename T, typename K, typename V, typename C, typename A>
    std::string serialize(const std::map<K, V*, C, A>& obj);  // map<key, object*>

    template<>
    std::string serialize(const int& val);
    template<>
    std::string serialize(const float& val);
    template<>
    std::string serialize(const double& val);
    template<>
    std::string serialize(const std::string& str);
    /* END serialize*/

    /* BEGIN deserialize */
    template<typename T>
    void deserialize(T& obj, const std::string& str);
    
    template<typename T, typename U, typename A>  // vector<object>
    void deserialize(std::vector<U, A>& obj, const std::string& str);

    template<typename T, typename U, typename A>  // vector<object*>
    void deserialize(std::vector<U*, A>& obj, const std::string& str);
    
    template<typename T, typename K, typename V, typename C, typename A>  // map<key, object>
    void deserialize(std::map<K, V, C, A>& obj, const std::string& str);

    template<typename T, typename K, typename V, typename C, typename A>  // map<key, object*>
    void deserialize(std::map<K, V*, C, A>& obj, const std::string& str);
    
    template<>
    void deserialize(int& val, const std::string& data);
    template<>
    void deserialize(float& val, const std::string& data);
    template<>
    void deserialize(double& val, const std::string& data);
    template<>
    void deserialize(std::string& str, const std::string& data);
    /* END deserialize */
}

template<typename T>
std::string rserialize(const T& obj);
template<typename T>
void runserialize(T& obj, const std::string& str);
#endif