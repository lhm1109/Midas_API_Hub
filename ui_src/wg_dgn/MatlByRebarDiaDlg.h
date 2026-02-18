#if !defined(__MatlByRebarDiaDlg_H__)
#define __MatlByRebarDiaDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MatlByRebarDiaDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class CDBDoc;
class CMatlDB;
class CMatlByRebarDiaGrid;
struct T_DCRB_D;

class __MY_EXT_CLASS__ CMatlByRebarDiaDlg : public CDialogMove
{
// Construction
public:
	CMatlByRebarDiaDlg(T_DCRB_D* pData, CWnd* pParent = NULL);
	~CMatlByRebarDiaDlg();

// Dialog Data
	//{{AFX_DATA(CCMLoadCaseDlg)
	enum { IDD = IDD_DGN_CON_BAR_MATL_DLG };
	CComboBox m_cmbConcMatCode;

	CComboBox m_cmdVertFromCmb;
	CComboBox m_cmdVertToedCmb;
	CComboBox m_cmdVertMatlCmb;

	CComboBox m_cmdHorzFromCmb;
	CComboBox m_cmdHorzToedCmb;
	CComboBox m_cmdHorzMatlCmb;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadCaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
 //public:
	 //void SetData(T_DCRB_D* pData){ mp_Data = pData; }
protected:
	CDBDoc* m_pDoc;
	CMatlDB* m_pMatlDB;
	T_DCRB_D* m_pData;
	T_DCRB_D* m_pBufData;

	void InitCodeCombo();
	void InitDiaCombo();
	void InitMatlCombo();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CCMLoadCaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnCodeCmb();

	afx_msg void OnVertFromCmb();
	afx_msg void OnVertToedCmb();
	afx_msg void OnHorzFromCmb();
	afx_msg void OnHorzToedCmb();

	afx_msg void OnVertAddRplBtn();
	afx_msg void OnVertAddBtn();
	afx_msg void OnVertInsertBtn();
	afx_msg void OnVertDelBtn();

	afx_msg void OnHorzAddRplBtn();
	afx_msg void OnHorzAddBtn();
	afx_msg void OnHorzInsertBtn();
	afx_msg void OnHorzDelBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMatlByRebarDiaGrid *m_wndVertGrid;
	CMatlByRebarDiaGrid *m_wndHorzGrid;

public:
	static BOOL GetSelectedString(const CComboBox* pCombo, CString* const pStrCur);
};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MatlByRebarDiaDlg_H__)
