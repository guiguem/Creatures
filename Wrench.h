#include "Tool.h"

class Wrench : public Tool::Registrar<Wrench>
{
  public:
    Wrench(std::string x) : m_x(x) {}

    void makeNoise();// { std::cerr << "Cat: " << m_x << "\n"; }

  private:
    std::string m_x;
};