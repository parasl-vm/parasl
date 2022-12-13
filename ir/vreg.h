#ifndef VREG_H
#define VREG_H

#include <bitset>
#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>
#include <cstring>

#include "../utils/macros.h"

static const std::string NO_NAME{ "" };

enum class VRegType
{
    INT = 0,    // int32_t
    CUSTOM_INT, // custom_int
    DOUBLE,     // double
    OBJECT_TYPE,
    UNINITIALIZED
};

class Function;
class Class;
class Layer;

class VReg
{
  public:
    template <typename type>
    static VReg* VRegBuilder(type value, std::string name = NO_NAME, uint8_t num_bits = 0);

    ACCESSOR_MUTATOR(type_, Type, VRegType)
    ACCESSOR_MUTATOR(name_, Name, std::string)
    ACCESSOR_MUTATOR(value_, Value, int64_t)

    // local variable
    Function* GetFunction()
    {
        if (std::holds_alternative<Function*>(owner_))
            return std::get<Function*>(owner_);
        return nullptr;
    }

    // field in class
    Class* GetClass()
    {
        if (std::holds_alternative<Class*>(owner_))
            return std::get<Class*>(owner_);
        return nullptr;
    }

    // global variable
    Layer* GetLayer()
    {
        if (std::holds_alternative<Layer*>(owner_))
            return std::get<Layer*>(owner_);
        return nullptr;
    }

    void SetFunction(Function* function)
    {
        owner_ = function;
    }

    void SetClass(Class* class_)
    {
        owner_ = class_;
    }

    void SetLayer(Layer* layer)
    {
        owner_ = layer;
    }

    bool IsInt()
    {
        return (type_ == VRegType::INT) || (type_ == VRegType::CUSTOM_INT);
    }

    bool IsDouble()
    {
        return (type_ == VRegType::DOUBLE);
    }

    bool IsObj()
    {
        return (type_ == VRegType::OBJECT_TYPE);
    }

    bool IsInitialized()
    {
        return !(type_ == VRegType::UNINITIALIZED);
    }

    int32_t GetInt()
    {
        return GetAs<int32_t>();
    }

    int64_t GetCustomInt()
    {
        return GetValue() & mask_;
    }

    double GetDouble()
    {
        return GetAs<double>();
    }

    // uintptr_t GetPointer()
    // {
    //
    // }

    void SetInt(int32_t value)
    {
        value_ = static_cast<int64_t>(value);
    }

    void SetCustomInt(int64_t value)
    {
        value_ = value & mask_;
    }

    void SetDouble(double value)
    {
        value_ = bit_cast<int64_t>(value);
    }

    // void SetPointer(uintptr_t value)
    // {
    //
    // }

    template <typename T, std::enable_if_t<std::is_same_v<int8_t, T> || std::is_same_v<uint8_t, T> ||
                                           std::is_same_v<int16_t, T> || std::is_same_v<uint16_t, T> ||
                                           std::is_same_v<std::int32_t, T> || std::is_same_v<uint32_t, T> ||
                                           std::is_same_v<std::int64_t, T> || std::is_same_v<uint64_t, T>> * = nullptr>
    T GetAs()
    {
        return static_cast<T>(GetValue());
    }

    template <typename T, std::enable_if_t<std::is_same_v<double, T> || std::is_same_v<float, T>> * = nullptr>
    double GetAs()
    {
        return bit_cast<double>(GetValue());
    }

    void Dump();

  private:
    VReg(int64_t value, VRegType type, std::string name) : type_(type), name_(name), value_(value)
    {}

    VReg(int64_t value, VRegType type, std::string name, uint8_t num_bits) : type_(type), name_(name)
    {
        // assert(num_bits != 0);
        for (int i = 0; i < num_bits; ++i) {
            mask_ = mask_ << 1;
            mask_ = mask_ | 1;
        }
        value_ = value & mask_;
    }

    template <class To, class From>
    static To bit_cast(const From &src)
    {
        static_assert(sizeof(To) == sizeof(From), "size of the types must be equal");
        To dst;
        std::memcpy(&dst, &src, sizeof(To));
        return dst;
    }

    VRegType type_ = VRegType::UNINITIALIZED;
    std::variant<Function*, Class*, Layer*> owner_; // mhe, bad idea
    std::string name_ = NO_NAME;
    int64_t value_ = 0;

    // TODO change it to std::optional
    uint64_t mask_ = 0; // for custom int
};

template <typename type>
VReg* VReg::VRegBuilder(type value, std::string name, uint8_t num_bits)
{
    if constexpr (std::is_same<type, VRegType>::value) {
        return new VReg(0, value, name, num_bits);
    }
    if constexpr (std::is_integral<type>::value) {
        if (num_bits != 0) {
            return new VReg(value, VRegType::CUSTOM_INT, name, num_bits);
        }
        return new VReg(value, VRegType::INT, name);
    }
    if constexpr (std::is_floating_point<type>::value) {
        int64_t casted_value = VReg::bit_cast<int64_t>(value);
        return new VReg(casted_value, VRegType::DOUBLE, name);
    }
    if constexpr (std::is_pointer<type>::value) {
        int64_t casted_value = VReg::bit_cast<int64_t>(value);
        return new VReg(casted_value, VRegType::OBJECT_TYPE, name);
    }

    UNREACHABLE("This line should be unreachable")
    return nullptr;
}

#endif // VREG_H
