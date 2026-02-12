#if !defined(AFX_CMTHGADLG_H__1B659D81_E90C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMTHGADLG_H__1B659D81_E90C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThgaDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

using namespace mit::frx;

class CCMThgaDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMThgaDlg();           // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CCMThgaDlg)

// Form Data
public:
	//{{AFX_DATA(CCMThgaDlg)
	enum { IDD = IDD_DBAR_THGA_DLG };
	CFormulaEditSpin	m_wndHGA;
	CListCtrl	m_List;
	CSelectLC	m_wndLoadCase;
	//}}AFX_DATA
	CTextUnit	m_wndArTimeUnit[3];
	CEditUnit	m_wndArTime[3];
	CFormulaEdit m_wndScaleFactor[3];
	CCobxThfc	m_wndFuncName[3];

// Attributes
public:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlDir[3];

// Operations
public:
	virtual BOOL ExternalInit(UINT key);
	void Execute(){};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThgaDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlRadio;
	CArray<UINT, UINT> m_aCtrlAngle;

protected:
	virtual ~CCMThgaDlg();

	void SetListCtrlHeader();
	BOOL GetSelectedData(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_THGA_K key, T_THGA_D &data);
	void SetItem(int nIndex, T_THGA_K key, T_THGA_D &data);
	BOOL FindItem(T_THGA_K key, int &nIndex);
	BOOL ValidData(T_THGA_D &data);
	BOOL Dlg2Data(T_THGA_K &key, T_THGA_D &data);
	BOOL Data2Dlg(T_THGA_K key, T_THGA_D &data);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMThgaDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdFuncBtn();
	afx_msg void OnItemchangedThgaList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmdFuncName();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdAngleOfHgaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHGADLG_H__1B659D81_E90C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
