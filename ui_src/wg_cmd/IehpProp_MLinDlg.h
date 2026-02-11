#if !defined(AFX_IEHPPROP_MLINDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_)
#define AFX_IEHPPROP_MLINDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpPropDlg.h : header file
//
// #include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

//#include "..\wg_base\wg_base_MySRGraph.h"
#include "IehpYieldPropGrid.h"
#include "IehpProp_MLinGrid.h"

class CIehpProp_PivotPicDlg;
/////////////////////////////////////////////////////////////////////////////
// CIehpProp_MLinDlg dialog

class CIehpProp_MLinDlg : public CDialogMove
{
// Construction
public:
	CIehpProp_MLinDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIehpProp_MLinDlg();   

// Dialog Data
	//{{AFX_DATA(CIehpProp_MLinDlg)
	enum { IDD = IDD_ETC_IEHP_PROP_MLIN };

	CEditUnit	m_edtPlusStiff;
	CEditUnit	m_edtMnusStiff;
	CTextUnit	m_untPlusStiff;
	CTextUnit	m_untMnusStiff;

	CEditUnit m_edtAlpha1;
	CEditUnit m_edtAlpha2;
	CEditUnit m_edtBeta1;
	CEditUnit m_edtBeta2;
	CEditUnit m_edtEta;
	CEditUnit m_edtUnBeta;
	CEditUnit	m_edtSFactDispl;
	CEditUnit	m_edtSFactForce;
	
	int		m_nType;
	int		m_nDeformDefineType;
	//}}AFX_DATA
	CEditUnit m_edtDeformCapacity[2][5];
public:
	T_IEHP_D m_Data;
	T_IEHP_MULT m_MultiData;
	//void SetIehpPropData(T_IEHP_D& data, int nFg1, int nFg2);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_MLIN data, int nFg1, int nFg2);
	void OnDataChanged();
	bool SyncDialogData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpProp_MLinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nModelType;
	int m_nHingeType;
	int m_nDof;
	double m_dZero;

	CArray<UINT, UINT> m_aHysParam;
	CArray<UINT, UINT> m_aUnParam;
	CArray<UINT, UINT> m_aOK;
	CArray<UINT, UINT> m_aTypeRdo;
	CArray<UINT, UINT> m_aCtrlPlus;
	CArray<UINT, UINT> m_aCtrlMinus;

	CIehpProp_PivotPicDlg* m_pPivotPic;

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg);
	BOOL CheckData(BOOL bMsg=TRUE);

	void AlignCtrl();
	void SetUnitAndTitle();  
	void CreatePivotPic();
	void CtrlEnableDisable();

	// Generated message map functions
	//{{AFX_MSG(CIehpProp_MLinDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelectTypeRdo();
	afx_msg void OnChangeDefomCapacity();
	afx_msg void OnScaleFactorChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	/////////////////////////////////////////////////////
	// Chart

	CIehpYieldPropGraph m_Draw;
	void InitGraph();
	void UpdateGraph();
	void UpdateInitStiff();

	// Grid
	CIehpProp_MLinGrid m_wndGrid;
	void InitGrid();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPPROP_MLINDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_)
