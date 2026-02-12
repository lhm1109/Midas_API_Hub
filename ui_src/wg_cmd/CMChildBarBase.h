#if !defined(__CCMCHILDBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define      __CCMCHILDBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDlgBarBase.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class I_GENModelBase;

/////////////////////////////////////////////////////////////////////////////
// CCMChildBarBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMChildBarBase : public CMenuBarChildDlg
{
protected:
	CCMChildBarBase();           // protected constructor used by dynamic creation
	CCMChildBarBase(UINT nID);           // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CCMChildBarBase)

	virtual ~CCMChildBarBase();

// Attributes
public:

// Operations
public:
	BOOL GetSelectedNodeKeyList(CArray<UINT, UINT> &raNodeKeyList);
	BOOL GetSelectedElemKeyList(CArray<UINT, UINT> &raElemKeyList);

protected:
	I_GENModelBase * GetIGenModelBase();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDLGBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_)
