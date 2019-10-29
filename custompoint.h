#ifndef CUSTOMPOINT_H
#define CUSTOMPOINT_H

#include <string>
class CustomPoint
{
public:
    CustomPoint(double x, double y, std::string id);
    double x;
    double y;
    std::string id;
    bool operator< (CustomPoint & other);
};

#endif // CUSTOMPOINT_H
