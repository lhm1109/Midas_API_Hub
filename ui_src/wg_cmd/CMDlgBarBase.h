#if !defined(AFX_CMDLGBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMDLGBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDlgBarBase.h : header file
//

#include "..\wg_base\MChildFormView.h"

class I_GENModelBase;

/////////////////////////////////////////////////////////////////////////////
// CCMDlgBarBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDlgBarBase : public MChildFormView
{
protected:
	CCMDlgBarBase();           // protected constructor used by dynamic creation
	CCMDlgBarBase(UINT nID);           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCMDlgBarBase)

	virtual ~CCMDlgBarBase();

// Attributes
public:

// Operations
public:
	BOOL GetSelectedNodeKeyList(CArray<UINT, UINT> &raNodeKeyList);
	BOOL GetSelectedElemKeyList(CArray<UINT, UINT> &raElemKeyList);

	virtual void LayoutAdjust() {}

protected:
	I_GENModelBase * GetIGenModelBase();

};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDLGBARBASE_H__35FE1381_8E39_11D3_92DE_0000C0B0E6B3__INCLUDED_)
