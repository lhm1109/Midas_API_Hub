#if !defined(AFX_WINDCLASSIFYKBC2015DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_WINDCLASSIFYKBC2015DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindClassifyKBC2015Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CWindClassifyKBC2015Dlg dialog

class CWindClassifyKBC2015Dlg : public CDialogMove
{
// Construction
public:
	CWindClassifyKBC2015Dlg(BOOL bShowBuildingType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindClassifyKBC2015Dlg)
	enum { IDD = IDD_ETC_WIND_CLASSIFY_KBC2015_DLG };	
	CEditUnit	m_edtHeight;
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtArea;
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndHeightUnit;
	CTextUnit	m_wndAreaUnit;
	//}}AFX_DATA

// Attribute
public:
	CArray<UINT, UINT> m_aCtrlStructure;
	CArray<UINT, UINT> m_aCtrlPlane;
protected:
	BOOL m_bShowBuildingType;
	WIND_CODETYPE  m_nWindCodeType;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindClassifyKBC2015Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nStructure;
	int m_nPlane;
	double m_dHeight;
	double m_dWidth;
	double m_dBreadth;
	double m_dArea;
	void SetWindLoadCode(WIND_CODETYPE nWindCodeType) { m_nWindCodeType = nWindCodeType; }
protected:
	void InitialData();
	void CalcArea();

	void AlignCtrl();
	// Generated message map functions
	//{{AFX_MSG(CWindClassifyKBC2015Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdStructRadio();
	afx_msg void OnCmdShapeRadio();
	afx_msg void OnCmdClassify();
	afx_msg void OnCmdCancel();
	afx_msg void OnChangeBreadthEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDCLASSIFYKBC2015DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
