#include "Tool.h"

class ScrewDriver : public Tool::Registrar<ScrewDriver>
{
  public:
    ScrewDriver(std::string x) : m_x(x) {}

    void makeNoise();// { std::cerr << "Cat: " << m_x << "\n"; }

  private:
    std::string m_x;
};