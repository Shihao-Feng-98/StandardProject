/*
* @brief: Example to use shared_ptr
* @date: 2023-07-30
* @author: Shihao Feng
*/

#include <iostream>
#include <memory>

class SimpleClass
{
public:
    SimpleClass(){
        std::cout << "SimpleClass construct\n";
    } 
    ~SimpleClass(){
        std::cout << "SimpleClass deconstruct\n";
    }
};

void Example01()
{
    // 
    std::shared_ptr<SimpleClass> ptr1(new SimpleClass);
    // std::shared_ptr<SimpleClass> ptr1 = std::make_shared<SimpleClass>();

    std::cout << "ptr1 reference count = " << ptr1.use_count() << std::endl;

    {
        std::shared_ptr<SimpleClass> ptr2 = ptr1;
        std::cout << "ptr2 reference count = " << ptr2.use_count() << std::endl;
        std::cout << "ptr1 reference count = " << ptr1.use_count() << std::endl;
    }

    std::cout << "ptr1 reference count = " << ptr1.use_count() << std::endl;

    return;
}


int main(int argc, char const *argv[])
{
    Example01();

    return 0;
}
