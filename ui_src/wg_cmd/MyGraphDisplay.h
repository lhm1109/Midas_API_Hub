#if !defined(AFX_MyGraphDisplay_H__DCC1C961_EAA4_11D3_AA8B_0080AD78AAC8__INCLUDED_)
#define AFX_MyGraphDisplay_H__DCC1C961_EAA4_11D3_AA8B_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGraphDisplay.h : header file
//

#include "LogAxis.h"	// Added by ClassView
class  MyGraphDisplay : public SRGraphDisplay
{
		
//@access public member functions
public:
	//@cmember
	/* Constructor*/
	MyGraphDisplay();
	void SetTripartite(BOOL bTripartite);
	void SetDiagGrid(BOOL bDiagGridFlag);
	BOOL IsTripartite();
	void PutTicks();
	void FindLogAxis();
	BOOL IsInside(CPoint p);
	int  GetFloatRank(double d);
	double GetDeformParms(double x, double y, double d);
	double GetAccelParms(double x, double y, double d);
	virtual void DrawClassicAxis();
	//@cmember
	/* Virtual destructor for this class*/
	virtual ~MyGraphDisplay();


private:
	BOOL m_bDiagGridFlag;
	CLogAxis m_LADeform;
	CLogAxis m_LAAccel;
	BOOL m_bTripartite;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyGraphDisplay_H__DCC1C961_EAA4_11D3_AA8B_0080AD78AAC8__INCLUDED_)
