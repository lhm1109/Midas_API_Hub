#if !defined(AFX_CMSECPAGESTIFFDLG_H__E4FDBEC1_2D73_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECPAGESTIFFDLG_H__E4FDBEC1_2D73_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSecPageStiffDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

// CGXGridWnd Drived class
class CCMSecPageStiffDlgGrid : public CGXGridWnd
{
// Construction and Destruction
public:
	CCMSecPageStiffDlgGrid() { }

// Attributes
public:

// Operations
public:
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL CanPaste() { return FALSE; }

// Implementations
	BOOL TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);

	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
};

/////////////////////////////////////////////////////////////////////////////
// CCMStiffDlg dialog

class CCMStiffDlg : public CDialogMove
{
// Construction
public:
	CCMStiffDlg(CDBDoc* pDoc,  T_SECT_D* pSectData, 
		CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>* pOrgStiff, CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	T_SECT_D* m_pSectData;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>* m_paOrgStiff;
	CCMSecPageStiffDlgGrid m_wndGrid;
	BOOL m_bSectPSC;
	BOOL m_bCmpTap;
	BOOL m_bCmpTapUseMulti;

// Dialog Data
	//{{AFX_DATA(CCMStiffDlg)
	enum { IDD = IDD_CMD_SP_VIEW_STIFF };
	mit::frx::MButton	m_wndUserCheck;
	mit::frx::MButton	m_wndSaveBtn;
	mit::frx::MButton	m_wndCloseBtn;
	// CButton	m_wndIBtn;
	// CButton	m_wndJBtn;
	//}}AFX_DATA

protected:
	CArray<UINT, UINT> m_aCtrlIJ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetTitleInGrid();
	void SetUnitInGrid();
	void SetStiffnessData();
	void SetStiffnessDataCmpTap(); // MNET:XXXX 20070302 mylee
	BOOL GetStiffnessData();
	void SetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness, double PeriIn, double PeriOut, double cy, double cz);
	void SetStiffnessDataSub(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness);
	BOOL GetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness, double PeriIn, double PeriOut, double cy, double cz);
	void SetStiffnessDataNull(int nCol); // added by mylee
	int  GetSectCount();
	void GetStiffAndPeri(int nIndex, T_SECT_STIFFNESS* &ppStiff, double &PeriIn, double &PeriOut, double& cy, double& cz);
	void GetColumnTitle(CStringArray &aTitle);
	ROWCOL GetUnitCol();
	void SetColumnStyle();
	void SetColumnStyleCmpTap(int n); // MNET:XXXX 20070302 mylee
	void SetDialogSize();
	void SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol, BOOL bReadOnly=TRUE);
	void ChangeInputType(BOOL bUserInput);

	// Generated message map functions
	//{{AFX_MSG(CCMStiffDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSpSdBtnSave();
	afx_msg void OnCmdUserCheck();
	afx_msg void OnCmdIBtn();
	afx_msg void OnCmdJBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECPAGESTIFFDLG_H__E4FDBEC1_2D73_11D3_92DE_0000C0B0E6B3__INCLUDED_)
