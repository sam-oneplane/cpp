//
//  templateMetaAdv1.h
//  TemplateMetaProgramming
//
//  Created by Samuel on 2/26/16.
//  Copyright (c) 2016 Samuel. All rights reserved.
//

#ifndef TemplateMetaProgramming_templateMetaAdv1_h
#define TemplateMetaProgramming_templateMetaAdv1_h


#include <iostream>
using namespace std ;

// define integer type
template <int N> class INTEGER {
	static const int VALUE = N ;
};

// define end of list element
class NIL {
	typedef NIL HEAD ;
	typedef NIL TAIL ;
};
// start defining a list : List(1,List(2,List(3,NIL))
template <typename H , typename T=NIL> class LST {
	typedef H HEAD ;
	typedef T TAIL ;
};




#endif
