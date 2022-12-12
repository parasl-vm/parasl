#include "vreg.h"
#include "function.h"
#include "layer.h"

int32_t VReg::GetDefaultInt()
{
    assert(type_ == VRegType::INT);
    return static_cast<VRegDefaultInt*>(this)->GetValue();
}

int64_t VReg::GetCustomInt()
{
    assert(type_ == VRegType::CUSTOM_INT);
    return static_cast<VRegCustomInt*>(this)->GetValue();
}

double VReg::GetDouble()
{
    assert(type_ == VRegType::DOUBLE);
    return static_cast<VRegDouble*>(this)->GetValue();
}

uintptr_t VReg::GetPointer()
{
    assert(type_ == VRegType::OBJECT_TYPE);
    return static_cast<VRegObject*>(this)->GetValue();
}

void VReg::SetDefaultInt(int32_t value)
{
    assert(type_ == VRegType::INT);
    static_cast<VRegDefaultInt*>(this)->SetValue(value);
}

void VReg::SetCustomInt(int64_t value)
{
    assert(type_ == VRegType::CUSTOM_INT);
    static_cast<VRegCustomInt*>(this)->SetValue(value);
}

void VReg::SetDouble(double value)
{
    assert(type_ == VRegType::DOUBLE);
    static_cast<VRegDouble*>(this)->SetValue(value);
}

void VReg::SetPointer(uintptr_t value)
{
    assert(type_ == VRegType::OBJECT_TYPE);
    static_cast<VRegObject*>(this)->SetValue(value);
}

void VReg::Dump()
{
    std::cout << "{" << name_ << ", owner ";
    if (GetClass() != nullptr) {
        // std::cout << GetClass()->GetName();
    } else if (GetFunction() != nullptr) {
        std::cout << GetFunction()->GetName();
    } else if (GetLayer() != nullptr) {
        std::cout << GetLayer()->GetName();
    }
    std::cout << ", ";

    switch (type_)
    {
    case VRegType::INT:
        std::cout << "DEFAULT INT, " << GetDefaultInt();
        break;
    case VRegType::CUSTOM_INT:
        std::cout << "CUSTOM INT, " << GetCustomInt();
        break;
    case VRegType::DOUBLE:
        std::cout << "DOUBLE, " << GetDouble();
        break;
    case VRegType::OBJECT_TYPE:
        // TODO(itrubachev) change to dumping object
        std::cout << "OBJECT, " << GetPointer();
        break;
    default:
        std::cout << "UNINITIALIZED";
    }
    std::cout << "}";
}
