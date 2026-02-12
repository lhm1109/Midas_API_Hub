#if !defined(__CMSTAGECOMPSECTSTIFFDLG_H__)
#define __CMSTAGECOMPSECTSTIFFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageCompSectStiffDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffDlg Grid
// CGXGridWnd Drived class

class CCMStageSectStiffGrid : public CGXGridWnd
{
// Construction and Destruction
public:
	CCMStageSectStiffGrid() { }

// Attributes
public:

// Operations
public:
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	//virtual BOOL CanPaste() { return FALSE; } // 2002. 12. 03 Copy 되도록 수정 
	virtual BOOL CanCut() { return FALSE; }  
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	virtual BOOL Copy( );
	//virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

// Implementations
	BOOL TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);

	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
};

/////////////////////////////////////////////////////////////////////////////
//
// CCMStageCompSectStiffDlg dialog

class CCMStageCompSectStiffDlg : public CDialogMove
{
// Construction
public:
	// nType : (1) Scale (2) Stiffness (3) Tapered Stiff.
	CCMStageCompSectStiffDlg(CDBDoc* pDoc, int nType, T_SECT_STIFFNESS* pData1, 
													 T_SECT_STIFFNESS* pData2=NULL, BOOL bReadOnly=FALSE, CWnd* pParent=NULL);
// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CCMStageCompSectStiffDlg)
	enum { IDD = IDD_CMD_SP_STIFF_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	MButton	m_wndIxxCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageCompSectStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	CCMStageSectStiffGrid m_wndGrid; 

	BOOL m_bAutoScale;
	int m_nType;
	BOOL m_bReadOnly;
	BOOL m_bHideUnderQzb;
	BOOL m_bDumbAndStub;
	T_SECT_STIFFNESS* m_pData1;
	T_SECT_STIFFNESS* m_pData2;
	T_SECT_STIFFNESS* m_pData3; // for LongTerm
	T_SECT_STIFFNESS* m_pData4; // for Shrinkage
	T_SECT_STIFFNESS* m_pDataAuto; 
	T_SECT_STIFFNESS* m_pDataAutoJ;

	CString m_strTitle;
	BOOL m_bCompNormalPar2;

// Implementation
protected:
	void SetTitleInGrid();
	void SetUnitInGrid();
	void SetStiffnessData();
	BOOL GetStiffnessData();
	void SetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness);
	int  GetSectCount();
	void GetStiffAndPeri(int nIndex, T_SECT_STIFFNESS* &ppStiff);
	void GetColumnTitle(CStringArray &aTitle);
	ROWCOL GetUnitCol();
	void SetColumnStyle();
	void SetDialogSize();
	void SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol);
	void ChangeIxxType(BOOL bIxxChk);

public:
	void SetTitle(CString& str);
	void SetMultiData(int nType, T_SECT_STIFFNESS* pData3, T_SECT_STIFFNESS* pData4); // added by mylee
	void SetScaleAutoData(T_SECT_STIFFNESS* pDataAuto, T_SECT_STIFFNESS* pDataAutoJ);
	void HideUnderQzb(); // added by mylee
	void SetCompNormalPar2(BOOL bCompNormalPar2);
	void GridForDumbAndStub();

	// Generated message map functions
	//{{AFX_MSG(CCMStageCompSectStiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSpSdBtnOk();
	afx_msg void OnCmdSpSdBtnCancel();
	afx_msg void OnCmdSpSdBtnImport();
	afx_msg void OnCmdIxxCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSTAGECOMPSECTSTIFFDLG_H__)
