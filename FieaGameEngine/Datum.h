#pragma once
#pragma warning(disable : 4172) 
#define GLM_ENABLE_EXPERIMENTAL
#include "framework.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "DatumTypeHelper.h"
namespace Fiea::GameEngine {
    enum DatumType {
        Unknown,
        Int,
        Float,
        String,
        Vec4,
        Mat4,
        Table,
        Pointer
    };
    class RTTI;
    class Scope;
    class Datum {
        friend class Scope;
    private:

       DatumType _datatype = DatumType::Unknown;
        union ContainerData {
            int* intData;
            float* floatData;
            std::string* stringData;
            glm::vec4* vecData;
            glm::mat4x4* matData;
            Scope** scopeData;
            RTTI** pointerData;
        }_data = {};
        size_t Size = 0;
        size_t Capacity = 0;
        bool isExtern;
        DatumTypeHelper THelper;
    public:
#pragma region === constructors ===
        Datum();
        Datum(const Datum& other);
        Datum(Datum&& other) noexcept;
        ~Datum();
#pragma endregion === constructors ===


#pragma region === Push pop get set ===
        template<typename T>
        void PushItem(T& item);
        void PushItem(Scope* item);
        void PushItem(RTTI* item);
        template<typename T>
        void PushItem(T&& item);
        template<typename T>
        T& GetItemAsValue(size_t idx) const;
        void PopBack();
        template<typename T>
        void SetItemWithValue(T& item, size_t idx = 0);
        template<typename T>
        void SetItemWithValue(T&& item, size_t idx = 0);
        std::string GetItemAsString(size_t idx);

        void RemoveAt(size_t idx);
#pragma endregion === Push pop get set ===


#pragma region === properties getters ===    
        DatumType GetType() const;
        size_t GetSize() const;
        size_t GetCapacity() const;
#pragma endregion === properties getters ===


#pragma region Memory Management
        void shrink_to_fit();
        void Resize(size_t NewSize);
        void Reserve(size_t NewCap);
        void clear();
#pragma endregion Memory Management


#pragma region Assignment Operators
        bool operator==(const Datum& rhs) const;
        bool operator!=(const Datum& rhs) const;
        Datum& operator=(const Datum& other);
        Datum& operator=(Datum&& other) noexcept;
        Datum& operator=(int&& scalar);
        Datum& operator=(float&& other);
        Datum& operator=(std::string&& other);
        Datum& operator=(glm::vec4&& other);
        Datum& operator=(glm::mat4x4&& other);
        Scope& operator[](uint32_t index) { return *GetItemAsValue<Scope*>(index); }

#pragma endregion Assignment Operators

        bool findScope(Scope& ScopeToFind);
        template<typename T>
        void SetStorage(T* Extern,size_t size);

    };
}
#include "Datum.inl"