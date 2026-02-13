#if !defined(AFX_DGNSTLSPCSECTJTABDLG_H__)
#define AFX_DGNSTLSPCSECTJTABDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSPCSectJTabDlg.h : header file
//

#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\DB_ST_DT_SECT.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectJTabDlg dialog
class CDgnStlSPCSectDlg;
class CDgnSteelSPCSectJGrid;

class CDgnStlSPCSectJTabDlg : public CChildDialog
{
// Construction
public:
	CDgnStlSPCSectJTabDlg(CWnd* pParent = NULL);   // standard constructor  


// Dialog Data
	//{{AFX_DATA(CDgnStlSPCSectJTabDlg)
	enum { IDD = IDD_DGN_STL_SPC_SECT_J_DLG };	

	CEditUnit	m_editCent_Y;
	CEditUnit	m_editCent_Z;
	CEditUnit	m_edit1_Y;
	CEditUnit	m_edit1_Z;
	CEditUnit	m_edit2_Y;
	CEditUnit	m_edit2_Z;
	CEditUnit	m_edit3_Y;
	CEditUnit	m_edit3_Z;
	CEditUnit	m_edit4_Y;
	CEditUnit	m_edit4_Z;
	MButton	m_btnRedraw;
	CBCGPStatic m_textUnit;

	MComboBox	m_cboPoint1; 
	MComboBox	m_cboPoint2; 
	MComboBox	m_cboPoint3; 
	MComboBox	m_cboPoint4; 

	MButton	m_RadioPoint1;
	MButton	m_RadioPoint2;
	MButton	m_RadioPoint3;
	MButton	m_RadioPoint4;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlSPCSectJTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation

public:
	T_SECT_SECTBASE_D* m_sectData;
	BOOL bExistJ;	

	void SelectLineRedraw(ROWCOL nRow);
	
	void DataToDlg();
	BOOL DlgToData();
	void DataToGrid();
	void GridToData();

	void SetPointComboItem();
	void SetPointEdit(int nPointMode, double dPositionX, double dPositionY);
	void SetPointModeNon();
	
protected:
	CDBDoc*						m_pDoc;

	int               m_nRowCount;

	CDgnSteelSPCSectJGrid*  m_Grid;  
	CDgnStlSPCSectDlg*        m_pParent;

	CArray<int, int&> m_aGroupIndex;
	CArray<int, int&> m_aSubGroupIndex;

	void SetInitUnit();
	void InitLineData();

	void InitPoint1Combo();
	void InitPoint2Combo();
	void InitPoint3Combo();
	void InitPoint4Combo();
	void PointRedraw(); 

	CArray<int, int&> m_arPoint1Group;
	CArray<int, int&> m_arPoint2Group;
	CArray<int, int&> m_arPoint3Group;
	CArray<int, int&> m_arPoint4Group;
	CArray<int, int&> m_arPoint1SubGroup;
	CArray<int, int&> m_arPoint2SubGroup;
	CArray<int, int&> m_arPoint3SubGroup;
	CArray<int, int&> m_arPoint4SubGroup;
	
	void SetControlEnable();
	  
	// Generated message map functions
	//{{AFX_MSG(CDgnStlSPCSectJTabDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickRedrawBtn();
	afx_msg void OnKillfocusPoint1YEdit();
	afx_msg void OnKillfocusPoint2YEdit();
	afx_msg void OnKillfocusPoint3YEdit();
	afx_msg void OnKillfocusPoint4YEdit();
	afx_msg void OnKillfocusPoint1ZEdit();
	afx_msg void OnKillfocusPoint2ZEdit();
	afx_msg void OnKillfocusPoint3ZEdit();
	afx_msg void OnKillfocusPoint4ZEdit();
	afx_msg void OnClickRadioPoint1();
	afx_msg void OnClickRadioPoint2();
	afx_msg void OnClickRadioPoint3();
	afx_msg void OnClickRadioPoint4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSPCSECTJTABDLG_H__)
