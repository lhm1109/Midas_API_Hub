#if !defined(__CMWINDITEMCHINA2012TOOL_H__)
#define __CMWINDITEMCHINA2012TOOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemChina2012.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\MathFunc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemChina2012 dialog
#include "HeaderPre.h"

class CCMWindItemChina2012Tool //: public CObject
{
public:
	//(8.4.4-2)
	static double GetWindCoff_KW (const T_WIND_CH2012 &data);
	//8.4.4
	static void CalWind_PulseMagnifyFactor    (T_WIND_CH2012 &data);
	
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMCHINA2012TOOL_H__)
