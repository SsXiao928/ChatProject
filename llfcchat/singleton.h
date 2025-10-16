#ifndef SINGLETON_H
#define SINGLETON_H
#include "global.h"
template <typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            //这里只能用shared_ptr<T>(new T), 不能用make_shared
            //继承模版单例类的子类的构造会设置为private，make_shared无法访问私有的构造函数
            //基类声明为派生类的友元，可以访问派生类的构造函数
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress(){
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton(){
        std::cout << "This is singleton destruct" << std::endl;
    }
};

//private和protected变量在类外不能通过对象访问，但可以通过类名访问
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
