# 这是一个标准的项目框架
希望通过制作一个项目案例来学习较大型项目的规范开发流程，在编写的过程中会添加足够多的注释。

在这个案例中会包含`cpp`、`cmake`、`pybind11`和`Doctest`等相关工具的使用。

## 目录结构
待补充

### Modules下各子模块的目录结构
    |-- CMakeList.txt
    |-- README.md
    |-- Include
    |-- Src
    |-- Test
    |-- Python
    |-- Example
        |-- Python

* .h头文件存放在`Include/`文件夹下
* .cpp源文件存放在`Src/`文件夹下
* 单元测试代码存放在`Test/`文件夹下，文件以Test开头
* pybind代码存放在`Python/`文件夹下，文件以Py开头
* cpp案例代码存放在`Example/`文件夹下，python案例代码存放在`Example/Python/`文件夹下，文件均以Use开头

---
Author: Shihao Feng

Email: 13247344844@163.com

Update date: 2023-07-26