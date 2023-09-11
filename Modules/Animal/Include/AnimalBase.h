#pragma one

#include <string>
#include <iostream>

/**
 * @brief 简单的注释
 *
 *  详细的注释
 *
 * @bug 记录bug
 * @todo 记录将来要完成的工作
 * @note 记录注意事项
 */
class AnimalBase
{
private:
    std::string m_name;

public:
    /**
     * @brief 函数注释
     *
     * @param 参数1 参数1的解释
     * @param 参数2 参数2的解释
     * @return 返回值 返回值的解释
     */
    AnimalBase(/* args */)
    {
        // 语句注释
        std::cout << "Hello animal!\n";
    }

    /// @brief 简短函数注释
    virtual ~AnimalBase() = default;
};
