// GTL::Switch
//
// Copyright Louis J, 2019
//
// Distributed under the BSD License, Version 1.0.

#ifndef GTL_SWITCH_HPP
#define GTL_SWITCH_HPP

#include "Switch/SwitchCommon.hpp"
#include "Switch/LabmdaPeel.hpp"
#include "Switch/SwitchJ.hpp"
#include "Switch/SwitchB.hpp"
//#include "Switch/SwitchL.hpp"




//DONE:可读性：参照switch/case的格式
//TODO:可读性：case重复时assert标识位置
//TODO:可读性：优化使用格式
//TODO:可读性：结构化绑定
//DONE:功能：编译期case获取、排序
//DONE:功能：default选项
//DONE:功能：case重复时assert
//TODO:功能：可使用lambda
//DONE:功能：编译期跳转表
//DONE:功能：编译期查找表（二分查找）
//TODO:功能：编译期查找表性能优化（二分查找使用了循环，需要优化掉循环本身）
//TODO:功能：判断使用跳转表还是查找表
//TODO:功能：区分是否是基础类型、使用errcode
//TODO:功能：hash查找表、二次判断
//TODO:功能：hash查找表判断重复、重复时多次判断

#endif  // GTL_SWITCH_HPP 