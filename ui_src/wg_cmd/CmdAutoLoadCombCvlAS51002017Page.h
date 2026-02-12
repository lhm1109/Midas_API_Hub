#if !defined(__CMDAUTOLOADCOMBCVLAS51002017PAGE_H__)
#define __CMDAUTOLOADCOMBCVLAS51002017PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAS51002017Page dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlAS51002017Page : public CChildDialog
{
	// Construction
public:
	CCmdAutoLoadCombCvlAS51002017Page(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_CMD_LCOMBDEFAULT_AS_5100_2017_PAGE };

	MComboBox m_cmbBridgeType;
	int       m_nAS5100D;
	CEditUnit m_edtAS5100D_Max;
	CEditUnit m_edtAS5100D_Min;
	int       m_nAS5100DW;
	CEditUnit m_edtAS5100DW_Max;
	CEditUnit m_edtAS5100DW_Min;
	int				m_nAS5100SL;
	CEditUnit m_edtAS5100SL_Max;
	CEditUnit m_edtAS5100SL_Min;
	int       m_nAS5100GL;
	CEditUnit m_edtAS5100GL_Max;
	CEditUnit m_edtAS5100GL_Min;

	//CComboBox	m_cmbRTCase;
	MComboBox m_MoveLcase;
	CListCtrl	m_MovingList;

	CFoldGroupBox_Cross	m_chkLoadFactor;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlAS51002017Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	//}}AFX_VIRTUAL
//
//	// Implementation
public:  
	//CMap<int,int,int,int> m_MovingData;  

public:
	void InitCombo();
	void SetDefault();
	//2018-07-09 Fatigue
	void InitRTCombo();
	//2018-07-09 Fatigue

	void InitRTListBox();
	BOOL CheckMovingCaseName(CString str,int& Index);
	void GetAS5100LoadFactor();
	//2018-07-09 Fatigue
	void Show_MoveRTComboBox(BOOL bCheck);
	//2018-07-09 Fatigue

	//2018-07-09 Fatigue
	void Initial_RTListBox();
	void OnCmdAS5100Fatigue();
	//2018-07-09 Fatigue
	void OnCmdAS5100EnableDisable();


	int m_nConstLoad;
//
protected:
	CDBDoc* m_pDoc;

	CCmdAutoLoadCombCvlDlg* m_pParent;
//
//	// Generated message map functions
//	//{{AFX_MSG(CCmdAutoLoadCombCvlSP35133302011Page)
	virtual BOOL OnInitDialog();

	//2018-07-09 Fatigue
	afx_msg void OnCmdAS5100RTAddBtn();
	afx_msg void OnCmdAS5100RTDelBtn();
	//2018-07-09 Fatigue

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
//	//}}AFX_MSG
//
	DECLARE_MESSAGE_MAP()
public:
	int m_nASBridgeType;

	int m_nAS5100DNum;
	int m_nAS5100DWNum;
	int m_nAS5100SLNum;
	int m_nAS5100GLNum;

	double m_dAS5100DMax;
	double m_dAS5100DMin;
	double m_dAS5100DWMax;
	double m_dAS5100DWMin;
	double m_dAS5100SLMax;
	double m_dAS5100SLMin;
	double m_dAS5100GLMax;
	double m_dAS5100GLMin;

	CArray<UINT, UINT> m_aCtrlAS5100;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLAS51002017PAGE_H__)
