#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <cstdlib>
#include <cxxabi.h>

std::string demangle(const char *name)
{

    int status = -4; // some arbitrary value to eliminate the compiler warning

    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

    return (status == 0) ? res.get() : name;
}

template <class Base, class... Args>
class Factory
{
  public:
    template <class... T>
    static std::unique_ptr<Base> make(const std::string &s, T &&... args)
    {
        return data().at(s)(std::forward<T>(args)...);
    }

    template <class T>
    struct Registrar : Base
    {
        friend T;

        static bool registerT()
        {
            const auto name = demangle(typeid(T).name());
            Factory::data()[name] = [](Args... args) -> std::unique_ptr<Base> {
                return std::make_unique<T>(std::forward<Args>(args)...);
            };
            std::cout << "Registered " << name << " as " << typeid(T).name() << std::endl;
            return true;
        }
        static bool registered;

      private:
        Registrar() : Base(Key{}) { (void)registered; }
    };

    friend Base;

  private:
    class Key
    {
        Key(){};
        template <class T>
        friend struct Registrar;
    };
    using FuncType = std::unique_ptr<Base> (*)(Args...);
    Factory() = default;

    static auto &data()
    {
        static std::unordered_map<std::string, FuncType> s;
        return s;
    }
};

template <class Base, class... Args>
template <class T>
bool Factory<Base, Args...>::Registrar<T>::registered =
    Factory<Base, Args...>::Registrar<T>::registerT();

// struct Animal : Factory<Animal, int>
// {
//     Animal(Key) {}
//     virtual void makeNoise() = 0;
// };
struct DeadAnimal : Factory<DeadAnimal, std::string>
{
    DeadAnimal(Key) {}
    virtual void makeNoise() = 0;
};

// class Dog : public Animal::Registrar<Dog>
// {
//   public:
//     Dog(int x) : m_x(x) {}

//     void makeNoise() { std::cerr << "Dog: " << m_x << "\n"; }

//   private:
//     int m_x;
// };

// class Cat : public Animal::Registrar<Cat>
// {
//   public:
//     Cat(int x) : m_x(x) {}

//     void makeNoise() { std::cerr << "Cat: " << m_x << "\n"; }

//   private:
//     int m_x;
// };

class DeadCat : public DeadAnimal::Registrar<DeadCat>
{
  public:
    DeadCat(std::string x) : m_x(x) {}

    void makeNoise() { std::cerr << "Cat: " << m_x << "\n"; }

  private:
    std::string m_x;
};

struct Creature : Factory<Creature, std::unique_ptr<int>>
{
    Creature(Key) {}
    virtual void makeNoise() = 0;
};

class Ghost : public Creature::Registrar<Ghost>
{
  public:
    Ghost(std::unique_ptr<int> &&x) : m_x(*x) {}

    void makeNoise() { std::cerr << "Ghost: " << m_x << "\n"; }

  private:
    int m_x;
};

int main()
{
    // auto x = Animal::make("Dog", 2);
    // auto y = Animal::make("Cat", 2);
    auto zz = DeadAnimal::make("DeadCat", "splash");
    zz->makeNoise();
    // y->makeNoise();
    auto z = Creature::make("Ghost", std::make_unique<int>(4));
    z->makeNoise();
    return 0;
}
