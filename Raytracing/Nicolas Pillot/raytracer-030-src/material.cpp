
#include "material.h"

Material::Material(const std::string& _materialname)
: materialname(_materialname)
{
}

Material::~Material()
{
}

std::string Material::getMaterialName() const
{
    return materialname;
}
