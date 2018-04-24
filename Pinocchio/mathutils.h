//基本定义：常见的数学操作
#ifndef MATHUTILS_H_INCLUDED
#define MATHUTILS_H_INCLUDED

#include <math.h>
#include <functional>
#include <string>
#include "Pinocchio.h"

#ifdef _WIN32
#pragma warning(disable:4355)
#pragma warning(disable:4996)
#pragma warning(disable:4251)
#endif //_WIN32

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//五入
inline int ROUND(double x) { return (int)(x + 0.5); }
//四舍
inline int SIGN(double x) { return (x > 0.) ? 1 : -1; }

//平方
template<class T> T SQR(const T & x) { return x * x; }
//立方
template<class T> T CUBE(const T & x) { return x * x * x; }
//四次方
template<class T> T QUAD(const T & x) { return SQR(SQR(x)); }

//两者之间取最大值
template <class T> class maximum : public binary_function<T, T, T>
{
public:
    T operator()(const T &a1, const T &a2) const { return max(a1, a2); }
};
//两者之间取最小值
template <class T> class minimum : public binary_function<T, T, T>
{
public:
    T operator()(const T &a1, const T &a2) const { return min(a1, a2); }
};
//乘法：a1*a2
template <class A1, class A2, class R> class myMult : public binary_function<A1, A2, R>
{
public:
    R operator()(const A1 &a1, const A2 &a2) const { return a1 * a2; }
};
//除法
template <class A1, class A2, class R> class myDiv : public binary_function<A1, A2, R>
{
public:
    R operator()(const A1 &a1, const A2 &a2) const { return a1 / a2; }
};
//识别
template <class T> class ident : public unary_function<T, T>
{
public:
    T operator()(const T &v) const { return v; }
};
//
template<class T> class construct
{
public:
    typedef T result_type;
    template<class From> T operator()(const From &x) const { return T(x); }
};

#ifdef _WIN32
inline bool operator==(const string &s1, const string &s2) { return s1.compare(s2) == 0; }
inline bool operator<(const string &s1, const string &s2) { return s1.compare(s2) < 0; }
#endif

#endif //MATHUTILS_H_INCLUDED
