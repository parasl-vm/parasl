#include "vreg.h"
#include "function.h"
#include "layer.h"

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
        std::cout << "DEFAULT INT, " << GetInt();
        break;
    case VRegType::CUSTOM_INT:
        std::cout << "CUSTOM INT, " << GetCustomInt();
        break;
    case VRegType::DOUBLE:
        std::cout << "DOUBLE, " << GetDouble();
        break;
    case VRegType::OBJECT_TYPE:
        // TODO(itrubachev) change to dumping object
        std::cout << "OBJECT, " /* << GetPointer() */;
        break;
    default:
        std::cout << "UNINITIALIZED";
    }
    std::cout << "}";
}
