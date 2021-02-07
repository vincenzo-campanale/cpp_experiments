#ifndef VEHICLE_H
#define VEHICLE_H

#include "dsl.h"
#include <string>

namespace vehicle
{

class Record
{
  public:
    Record() = default;

    Record(std::string model, std::string color)
        : model_(model)
        , color_(color)
    {
    }

    bool SetModel(const std::string& model)
    {
        if (model.empty())
        {
            return false;
        }

        model_ = model;
        return true;
    }

    std::string Model() const
    {
        return model_;
    }

    bool SetColor(const std::string& color)
    {
        if (color.empty())
        {
            return false;
        }

        color_ = color;
        return true;
    }

    std::string Color() const
    {
        return color_;
    }

  private:
    std::string model_;
    std::string color_;
};

Field model{&Record::SetModel};  //< Fields a are bounded to a member function.
Field color{&Record::SetColor};

} // namespace vehicle

#endif  // VEHICLE_H
