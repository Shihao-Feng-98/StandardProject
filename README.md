# 这是一个标准的项目框架
希望通过制作一个项目案例来学习大型项目的规范开发流程，在编写的过程中会添加足够多的注释；在这个案例中会包含`markdown`、 `cpp`、`python`、`git`、`cmake`、`pybind11`和`Doctest`等相关工具的使用。

[Markdown基本语法](https://markdown.com.cn/basic-syntax/)

[CMake Tutorial](https://github.com/BrightXiaoHan/CMakeTutorial)


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

### 配置和构建

按默认构建选项编译：
```shell
mkdir Build && cd Build
cmake ..  # -DOPTION=ON/OFF 
cmake --build . -j8 
cmake --install .   
```
编译和运行单元测试：
```shell
cmake .. -DBUILD_TEST=ON
cmake --build . -j8
ctest
```

说明：

* `cmake ..`: 通过外部构建的方式，在构建目录下生成项目文件(build tree)，如Makefile等，".." 表示CMakeList的路径；
* `cmake --build . -j8`: "cmake --build" 表示对生成好的项目文件(build tree)进行编译，"." 表示项目文件(build tree)的路径，"-j8" 表示采用8个线程同时编译，最大值取决于cpu的线程数；效果相当于`make -j8`，主要为了跨平台使用；
* `cmake --install .`: 将项目生成的库文件、头文件和可执行文件等相关文件安装到指定目录；效果相当于`make install`； 

---
Author: Shihao Feng

Email: 13247344844@163.com

Update date: 2023-07-26