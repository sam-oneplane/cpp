//
//  main.cpp
//  TemplateMetaProgramming
//
//  Created by Samuel on 2/26/16.
//  Copyright (c) 2016 Samuel. All rights reserved.
//

#include "templateMeta.h"
#include "templateMetaAdv1.h"

int main(int argc, const char * argv[])
{

	cout << "BITS_NUM = " << BITS_SET<15>::RESULT << endl ;
	cout << "FACTOR<6> = " << FACTOR<6>::RESULT << endl ;
	cout << "ACC<12> = " << ACC<12>::RESULT << endl ;
	//***************
	LOOP<6>::EXEC() ;
	cout << endl ;
	IF<0>::EXEC() ;
	//***************
	typedef FRAC<1,2> HALF ;
	typedef FRAC<2,3> TWO_BY3 ;
    typedef FRAC<3,12> ONE_Q ;
    typedef FRAC<1,6> ONE_BY6 ;
#if 1
	typedef SCALAR_MUL<5,HALF>::RESULT MUL_RESULT ;
	cout << "1/2*5 RESULT IS : " << MUL_RESULT::NUM << "/" << MUL_RESULT::DEN << endl ;
	//** this will cause an error : WHY
    cout << "2/3*7 RESULT IS : " << SCALAR_MUL<7,TWO_BY3>::RESULT::NUM << "/" << SCALAR_MUL<7,TWO_BY3>::RESULT::DEN << endl ;
	//***************
	
	typedef SIMPL<ONE_Q> MY_FREC ;
	cout << "3/12 MCD IS : " << MY_FREC::MY_MCD << " THE NEW FREC IS : " ;
	//** this will cause an error : WHY
	cout << SIMPL<ONE_Q>::S_RESULT::NUM << " / "  <<  SIMPL<ONE_Q>::S_RESULT::DEN << endl ;
    //***************
    cout << "SAME BASE 1/2 & 1/6  : " ;
#endif
    cout << SAME_BASE<HALF, ONE_BY6>::X::NUM << "/" << SAME_BASE<HALF, ONE_BY6>::X::DEN  << "  " ;
    cout << SAME_BASE<HALF, ONE_BY6>::Y::NUM << "/" << SAME_BASE<HALF, ONE_BY6>::Y::DEN << endl  ;
    cout << SUM<HALF, ONE_BY6>::NUM << "/" << SUM<HALF, ONE_BY6>::DEN << " == "  ;
    cout << SUM<HALF, ONE_BY6>::SIMPLE_FRAC::MY_MCD  << endl ;
    // e = S(1/n!) = 1/0! + 1/1! + 1/2! ....
    
    return 0;
}

