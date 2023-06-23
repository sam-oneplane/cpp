//
//  templateMeta.h
//  TemplateMetaProgramming
//
//  Created by Samuel on 2/26/16.
//  Copyright (c) 2016 Samuel. All rights reserved.
//

#ifndef TemplateMetaProgramming_templateMeta_h
#define TemplateMetaProgramming_templateMeta_h



#include <iostream>
using namespace std ;

// Templates that calculate a value  :
template <unsigned char byte>
class BITS_SET {
public:
	enum {
		B0 = (byte & 0x1) ? 1 : 0 ,
		B1 = (byte & 0x2) ? 1 : 0 ,
		B2 = (byte & 0x4) ? 1 : 0 ,
		B3 = (byte & 0x8) ? 1 : 0 ,
		B4 = (byte & 0x10) ? 1 : 0 ,
		B5 = (byte & 0x20) ? 1 : 0 ,
		B6 = (byte & 0x40) ? 1 : 0 ,
		B7 = (byte & 0x80) ? 1 : 0
	};
	enum  {
		RESULT = B0 + B1 + B2 + B3 + B4 + B5 + B6 + B7
	};
};

// Template that calc a value using recursion
template <int i>
class FACTOR {
public:
	enum {
		RESULT = i * FACTOR<i-1>::RESULT
	};
};

template <>
class FACTOR<1> {
public:
	enum {
		RESULT = 1
	};
};
//**************************************
// or using static const var  for accumulation

template <int i>
class ACC {
public:
	// const calculated at compile time
	// static object because template applied to Classes not object
	static const int RESULT = i + ACC<i-1>::RESULT ;
};

template <> class ACC<1> {
public:
	static const int RESULT = 1 ;
};
//**************************************
// Template that unroll loops :
template <int i>
class LOOP {
public:
	static inline void EXEC() {
		cout << "A-" << i << " " ;
		LOOP<i-1>::EXEC() ;
		static const int RESULT = i   ;
		cout << " " << RESULT  ;
	}
};

template <> class LOOP<0> {
public:
	static inline void EXEC() {
		cout << "A-" << 0  ;
		cout << "\n" ;
		static const int RESULT = 0 ;
		cout << RESULT  ;
	}
};
//**************************************
// template to perform IF statment
template <bool expr>
class IF {
public:
	static inline void EXEC() {
		cout << "TRUE" << endl ;
	}
};

template <> class IF<false> {
public:
	static inline void EXEC() {
		cout << "FALSE" << endl ;
	}
};
//**************************************
// define fractions
template <long N , long D> class FRAK {
public:
	static const long NUM = N ;
	static const long DEN = D ;
};
//**************************************
template <int N , typename F> class SCALAR_MUL {
public:
	// use typedef for like return value RESULT
	typedef FRAK<N*F::NUM, F::DEN> RESULT ;
};

//**************************************
// most common devidor : MCD
template <long X , long Y> class MCD {
public:
	// static const types is used in this case for compile time template meta programing
	static const long RESULT = MCD<Y,X%Y>::RESULT ;
};

template <long X> class MCD<X,0> {
public:
	static const long RESULT = X ;
};
//**************************************
// using MCD :
// F is a FRAK class ; MCD receives F::NUM and F::DEN as template args and return the result
template <typename F> class  SIMPL {
public:
	static const long MY_MCD = MCD<F::NUM,F::DEN>::RESULT ;
	typedef  FRAK<F::NUM/MY_MCD , F::DEN/MY_MCD> S_RESULT ;
};
//**************************************
// CALC e = SUM(1/n!)  = 1/0! + 1/1! + 1/2! + 1/3! ....
template <int N> class FACT {
public:
    static const long RESULT = N*FACT<N-1>::RESULT ;
};
template <> class FACT<0> {
public:
    static const long RESULT = 1 ;
};
//**************************************
template <typename X1 , typename Y1> class SAME_BASE {
public:
    typedef typename SCALAR_MUL<X1::DEN, Y1>::RESULT Y ;
    typedef typename SCALAR_MUL<Y1::DEN, X1>::RESULT X ;
};
//**************************************
template <typename X , typename Y> class SUM {
public:
    typedef SAME_BASE<X, Y> B ;
    static const long NUM = B::X::NUM + B::Y::NUM ;
    static const long DEN = (long)(B::X::DEN * B::Y::DEN) ;
    typedef SIMPL<FRAK<NUM,DEN>> SIMPLE_FRAK ;
};
//**************************************
template <int N> class E {
public:
    static const long DEN = FACT<N>::RESULT ;
    typedef FRAK<1, DEN> TERM ;
    typedef typename E<N-1>::TERM NEXT_TERM ;
    typedef typename SUM<TERM, NEXT_TERM>::RESULT RESULT ;
};

#endif
