#ifndef VREG_H
#define VREG_H

#include <bitset>
#include <cassert>
#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>

#include "../utils/macros.h"

// All types can me implemented via uint64_t and bit_cast (which is simpiler)
// I don't know why I've decided to do types in below way

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
    // static VReg *VRegBuilder(VRegType type, std::string name = NO_NAME, uint8_t num_bits = 0);

    ACCESSOR_MUTATOR(type_, Type, VRegType)
    ACCESSOR_MUTATOR(name_, Name, std::string)

    int32_t GetInt();
    int64_t GetCustomInt();
    double GetDouble();
    uintptr_t GetPointer();

    void SetInt(int32_t value);
    void SetCustomInt(int64_t value);
    void SetDouble(double value);
    void SetPointer(uintptr_t value);

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

    void Dump();

  protected:
    VReg(VRegType type, std::string name) : type_(type), name_(name)
    {
    }

  private:
    VRegType type_ = VRegType::UNINITIALIZED;
    std::variant<Function*, Class*, Layer*> owner_;
    std::string name_ = NO_NAME;
};

class VRegDefaultInt : public VReg
{
  public:
    VRegDefaultInt(int32_t value, std::string name) : VReg(VRegType::INT, name), value_(value)
    {
    }
    VRegDefaultInt(std::string name) : VReg(VRegType::INT, name)
    {
    }
    ACCESSOR_MUTATOR(value_, Value, int32_t)

  private:
    int32_t value_ = 0;
};

// do we need big (> 64 bit) int here?
class VRegCustomInt : public VReg
{
  public:
    VRegCustomInt(int64_t value, uint8_t bit_num, std::string name)
        : VReg(VRegType::CUSTOM_INT, name), bit_num_(bit_num)
    {
        assert(bit_num != 0);
        for (int i = 0; i < bit_num; ++i) {
            mask_ = mask_ << 1;
            mask_ = mask_ | 1;
        }
        value_ = value & mask_;
    }
    VRegCustomInt(uint8_t bit_num, std::string name)
        : VReg(VRegType::CUSTOM_INT, name), bit_num_(bit_num)
    {
        assert(bit_num != 0);
        for (int i = 0; i < bit_num; ++i) {
            mask_ = mask_ << 1;
            mask_ = mask_ | 1;
        }
    }

    ACCESSOR_MUTATOR(bit_num_, BitNum, uint8_t)

    int64_t GetValue()
    {
        return value_ & mask_;
    }

    void SetValue(int64_t value)
    {
        value_ = value & mask_;
    }

  private:
    int64_t value_ = 0;
    uint8_t bit_num_ = 0;
    uint8_t mask_ = 0;
};

class VRegDouble : public VReg
{
  public:
    VRegDouble(double value, std::string name) : VReg(VRegType::DOUBLE, name), value_(value)
    {
    }
    VRegDouble(std::string name) : VReg(VRegType::DOUBLE, name)
    {
    }
    ACCESSOR_MUTATOR(value_, Value, double)

  private:
    double value_ = 0;
};

class VRegObject : public VReg
{
  public:
    VRegObject(uintptr_t value, std::string name)
        : VReg(VRegType::OBJECT_TYPE, name), value_(value)
    {
    }
    VRegObject(std::string name) : VReg(VRegType::OBJECT_TYPE, name)
    {
    }
    ACCESSOR_MUTATOR(value_, Value, uintptr_t)

  private:
    uintptr_t value_ = 0;
};

template <typename type>
VReg* VReg::VRegBuilder(type value, std::string name, uint8_t num_bits)
{
    if (num_bits != 0 && std::is_integral<type>::value) {
        assert(num_bits <= 64);
        return static_cast<VReg*>(new VRegCustomInt(value, num_bits, name));
    }
    if constexpr (std::is_pointer<type>::value) {
        return static_cast<VReg*>(new VRegObject(value, name));
    }
    if constexpr (std::is_integral<type>::value) {
        return static_cast<VReg*>(new VRegDefaultInt(value, name));
    }
    if constexpr (std::is_floating_point<type>::value) {
        return static_cast<VReg*>(new VRegDouble(value, name));
    }
    UNREACHABLE("This line should be unreachable")
    return nullptr;
}

// IDK why do we need inline here
template <>
inline VReg* VReg::VRegBuilder<VRegType>(VRegType type, std::string name, uint8_t num_bits)
{
    // can be done via dispatch table
    switch (type) {
    case VRegType::INT:
        return static_cast<VReg*>(new VRegDefaultInt(name));
    case VRegType::CUSTOM_INT:
        return static_cast<VReg*>(new VRegCustomInt(num_bits, name));
    case VRegType::DOUBLE:
        return static_cast<VReg*>(new VRegDouble(name));
    case VRegType::OBJECT_TYPE:
        return static_cast<VReg*>(new VRegDefaultInt(name));
    case VRegType::UNINITIALIZED:
        return new VReg(type, name);
    }
    UNREACHABLE("This line should be unreachable")
    return nullptr;
}

#endif // VREG_H
