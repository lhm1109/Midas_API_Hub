#if !defined(__CMWINDITEMIBC2012TOPOGRAPHY_H__)
#define __CMWINDITEMIBC2012TOPOGRAPHY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemIBC2012Topography.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2012Topography dialog

class CCMWindItemIBC2012Topography : public CInternationalDlg
{
// Construction
public:
	CCMWindItemIBC2012Topography(T_WIND_IBC2012* pData, CWnd* pParent = NULL);   // standard constructor
	CCMWindItemIBC2012Topography(T_WIND_NSR2010* pData, CWnd* pParent = NULL);   // standard constructor
	CCMWindItemIBC2012Topography(T_WIND_ASCE7_16* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemIBC2012Topography)
	enum { IDD = IDD_ETC_WIND_ITEM_IBC2012_TOPOGRAPY };
	MButton   m_chkTopographic_X;
	MComboBox	m_cobxHillShape_X;
	MComboBox	m_cobxBLocation_X;
	CEditUnit	m_editHHeight_X;  
	CEditUnit	m_editHillLeng_X;
	CEditUnit	m_editCBDistance_X;

	MButton   m_chkTopographic_Y;  
	MComboBox	m_cobxHillShape_Y;
	MComboBox	m_cobxBLocation_Y;
	CEditUnit	m_editHHeight_Y;  
	CEditUnit	m_editHillLeng_Y;
	CEditUnit	m_editCBDistance_Y;

	CTextUnit	m_unitHHeight;
	CTextUnit	m_unitHillLeng;
	CTextUnit	m_unitCBDistance;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemIBC2012Topography)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void InitHillShapeCombo();
	void InitBLocationCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	void ChangeComboWidth();

protected:
	T_WIND_IBC2012* m_pDataIBC;
	T_WIND_NSR2010* m_pDataNSR;
	T_WIND_ASCE7_16* m_pDataASCE7;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemIBC2012Topography)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdAnalTopoCheck_X();
	afx_msg void OnCmdAnalTopoCheck_Y();
	afx_msg void OnSelchangeCmdAnalHillshapeCombo_X();
	afx_msg void OnSelchangeCmdAnalHillshapeCombo_Y();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMIBC2012TOPOGRAPHY_H__)
