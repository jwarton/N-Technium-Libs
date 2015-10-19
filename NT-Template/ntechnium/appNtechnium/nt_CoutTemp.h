///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// nt_CoutTemp.h
// openGl scratch libs							///////////////////
// Console Out Template Functionality			///////////////////
// created by James Warton on 02/23/2014		///////////////////
///////////////////////////////////////////////////////////////////
#ifndef CORE_TEMPLATE_JPW_NTECHNIUM
#define CORE_TEMPLATE_JPW_NTECHNIUM

#include <iostream>

namespace jpw {
	template <typename T>
	void print(T t){
		std::cout << t;
	}
	template <typename T, typename U>
	void print(T t, U u){
		std::cout << t << u;
	}
	template <typename T, typename U, typename V>
	void print(T t, U u, V v){
		std::cout << t << u << v;
	}
	template <typename T, typename U, typename V, typename W>
	void print(T t, U u, V v, W w){
		std::cout << t << u << v << w;
	}
	template <typename T, typename U, typename V, typename W, typename X>
	void print(T t, U u, V v, W w, X x){
		std::cout << t << u << v << w << x;
	}
	template <typename T, typename U, typename V, typename W, typename X, typename Y>
	void print(T t, U u, V v, W w, X x, Y y){
		std::cout << t << u << v << w << x << y;
	}
	template <typename T, typename U, typename V, typename W, typename X, typename Y, typename Z>
	void print(T t, U u, V v, W w, X x, Y y, Z z){
		std::cout << t << u << v << w << x << y << z;
	}
	//////////////////////////////////// VARIADIC TEMPLATE APPROACH
	////////////// http://www.devx.com/cplus/Article/41533/0/page/2 
	template<typename T>
	void println(T value)
	{
		std::cout << value << std::endl;
	}

	template<typename First, typename ... Rest>
	void println(First first, Rest ... rest)
	{
		std::cout << first << " ";
		println(rest...);
	}
}
#define trace println

#endif//////////////////////////////////////// HOMAGE TO MACROMEDIA
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////