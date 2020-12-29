#pragma once
#include <vector>
#include <string>
class ObjShape{
  public:
    std::vector<float> vertices;
};
std::vector<ObjShape> loadFromFile(std::string fileName);
