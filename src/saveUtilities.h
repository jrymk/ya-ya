#pragma once
#ifndef YAYA_SAVEUTILITIES_H
#define YAYA_SAVEUTILITIES_H

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>
#include <string>
#include <cstring>
#include <type_traits>

/// @brief tools for serialization
namespace SaveUtilities {
    template<typename Class, typename MemberType>
    struct PropertyObj {
        MemberType Class::*member;
        const char* ID;

        inline constexpr PropertyObj(MemberType Class::*_member, const char* _ID) : member(_member), ID(_ID) {};
    };

    template<typename Class, typename MemberType>
    inline constexpr PropertyObj<Class, MemberType> property(MemberType Class::*member, const char* ID) {
        return SaveUtilities::PropertyObj<Class, MemberType>{member, ID};
    }

    template<typename T, T... IdxSeq, typename FuncType>
    inline constexpr void forSequence(std::integer_sequence<T, IdxSeq...>, FuncType &&f) {
        (static_cast<void>(f(std::integral_constant<T, IdxSeq>{})), ...);
    }

    inline std::unordered_map<std::uintptr_t, bool> checkSerialized;                       // check serialization state of object
    inline std::unordered_map<std::uintptr_t, void*> objectTracker;                        // track deserialized objects
    inline std::unordered_map<std::uintptr_t, std::shared_ptr<void> > smartObjectTracker;  // track deserialized objects

    template<typename T>
    inline std::string getAddressStr(const T* ptr) {  // get pointer address string
        std::string nameStr = typeid(std::remove_const<T>).name();
        std::uintptr_t typeCode = nameStr.size();
        for (char &c: nameStr) typeCode += c;
        return "<Addr>" + std::to_string(reinterpret_cast<std::uintptr_t>(ptr) + typeCode * 1000000000) + "</Addr>";
    }

    template<typename T>
    inline std::uintptr_t getAddressNum(const T* ptr) {  // get pointer address number
        std::string nameStr = typeid(std::remove_const<T>).name();
        std::uintptr_t typeCode = nameStr.size();
        for (char &c: nameStr) typeCode += c;
        return reinterpret_cast<std::uintptr_t>(ptr) + typeCode * 1000000000;
    }

    inline void clearObjTracker() {  // VITAL: clear temporary pointers for deserialization
        objectTracker.clear();
        smartObjectTracker.clear();
    }
}

#endif