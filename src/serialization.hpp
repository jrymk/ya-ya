#ifndef _SERIALIZATION_HPP_
#define _SERIALIZATION_HPP_

#include <tuple>
#include <string>
using namespace std;  // sorry too many stds xD

namespace SaveUtilities{
    template<typename Class, typename MemberType>
    struct PropertyObj{
        MemberType Class::*member;
        const int ID;
        constexpr PropertyObj(MemberType Class::*_member, int _ID) : member(_member), ID(_ID){};
    };

    template<typename Class, typename MemberType>
    constexpr auto property(MemberType Class::*member, int ID){
        return PropertyObj<Class, MemberType>{member, ID};
    }

    template<typename T, T... IdxSeq, typename FuncType>
    constexpr void forSequence(integer_sequence<T, IdxSeq...>, FuncType&& f){
        (static_cast<void>(f(std::integral_constant<T, IdxSeq>{})), ...);
    }
}

namespace Serialization{
    /* BEGIN serialize */
    template<typename T>
    string serialize(const T& obj){
        string str, className = T::className;
        constexpr auto propertyCnt = tuple_size<decltype(T::properties)>::value;
        str.append("<" + className + ">");
        SaveUtilities::forSequence(make_index_sequence<propertyCnt>{}, [&](auto i){
            constexpr auto property = get<i>(T::properties);
            str.append("<" + to_string(property.ID) + ">");
            str.append(serialize(obj.*(property.member)));
            str.append("</" + to_string(property.ID) + ">");
        });
        str.append("</" + className + ">");

        return str;
    }

    template<>
    string serialize<int>(const int& val){
        return to_string(val);
    }
    template<>
    string serialize<float>(const float& val){
        return to_string(val);
    }
    template<>
    string serialize<double>(const double& val){
        return to_string(val);
    }
    template<>
    string serialize<string>(const string& str){
        return str;
    }
    /* END serialize*/

    /* BEGIN unserialize */
    template<typename T>
    void unserialize(T& obj, const string& str){
        constexpr auto propertyCnt = tuple_size<decltype(T::properties)>::value;
        SaveUtilities::forSequence(make_index_sequence<propertyCnt>{}, [&](auto i){
            constexpr auto property = get<i>(T::properties);
            
            int tagStart = str.find("<" + to_string(property.ID) + ">"),
                tagEnd = str.find("</" + to_string(property.ID) + ">"),
                tagLen = to_string(property.ID).size() + 2;
            unserialize(obj.*(property.member), str.substr(tagStart + tagLen, tagEnd - tagStart - tagLen));
        });
    }

    template<>
    void unserialize<int>(int& val, const string& data){
        val = stoi(data);
    }
    template<>
    void unserialize<float>(float& val, const string& data){
        val = stof(data);
    }
    template<>
    void unserialize<double>(double& val, const string& data){
        val = stod(data);
    }
    template<>
    void unserialize<string>(string& str, const string& data){
        str = data;
    }
    /* END unserialize */
}

#endif