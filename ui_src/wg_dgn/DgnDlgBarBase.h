#if !defined(AFX_DGNDLGBARBASE_H__BE8B36E1_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_DGNDLGBARBASE_H__BE8B36E1_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnDlgBarBase.h : header file
//
#include "..\wg_base\wg_base_FormViewInBar.h"

class I_GENModelBase;

/////////////////////////////////////////////////////////////////////////////
// CDgnDlgBarBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnDlgBarBase : public CFormViewInBar
{
protected:
	CDgnDlgBarBase();           // protected constructor used by dynamic creation
	CDgnDlgBarBase(UINT nID);           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnDlgBarBase)

	virtual ~CDgnDlgBarBase();

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

#endif // !defined(AFX_DGNDLGBARBASE_H__BE8B36E1_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
