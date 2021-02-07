#ifndef PERSON_H
#define PERSON_H

#include "dsl.h"
#include <string>

namespace person
{

struct Record
{
    std::string name{};
    std::string surname{};
    unsigned int age{};
};

// Note: fields are global variables but are not bounded to any instance of a record. The bonding comes together with
// the Update functor.
Field name{&Record::name};    //< Fields a are bounded to a member variable.
Field surname{&Record::surname};
Field age{&Record::age};

} // namespace person

#endif // PERSON_H
