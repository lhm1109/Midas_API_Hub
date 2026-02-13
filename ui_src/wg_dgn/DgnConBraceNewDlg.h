#if !defined(__DGNCONBRACENEWDLG_H__)
#define __DGNCONBRACENEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBraceNewDlg.h : header file
//
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\FoldGroupBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDlgBase.h"
#include "DgnDrawWnd.h"
#include "DgnConBraceRebarGrid.h"
#include "wg_dgnRes2.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceNewDlg dialog

class __MY_EXT_CLASS__ CDgnConBraceNewDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBraceNewDlg(CWnd* pParent = NULL);
	virtual ~CDgnConBraceNewDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConBraceNewDlg)
	enum { IDD = IDD_DGN_CON_BRACE_DLG_NEW };
	CSelectEdit m_edtMemb;
	CListCtrl m_SectList;
	mit::frx::MButton m_chkSubSect;
	mit::frx::MEdit m_edtSectK;
	mit::frx::MEdit m_edtSubSectK;

	CEditUnit m_edtD0;
	CTextUnit m_untD0;

	CFoldGroupBox_Cross m_grbDetail;

	CDgnDrawWnd m_INodeWnd;
	CDgnDrawWnd m_CenterWnd;

	mit::frx::MillustViewer m_wndRect;
	mit::frx::MillustViewer m_wndCircle;
	mit::frx::MillustViewer m_wndPipe;
	//}}AFX_DATA

protected:
	BOOL m_bInit;
	T_REBR_D m_RebrD;

	CDgnConBraceRebarGrid m_RebarGrid;
	CMap<int,int,T_REBR_KEY,T_REBR_KEY> m_mIndex2RebrK;
	CMap<T_REBR_KEY,T_REBR_KEY,int,int> m_mRebrK2Index;

	CArray<UINT,UINT> m_aMrebK;
public:
	void ShowSelectedRebr(T_REBR_KEY RebrKey, BOOL bInit/*=TRUE*/);

	void DrawSectionWnd(T_REBR_D* pRebrD);
	
protected:
	void ShowBitmap(CString str);
	
	void Initial_SectionData(T_SECT_D& SectD);
	double Get_SectionDimension(T_SECT_D& SectD);
	double Get_ChangeUnitFactor();
	
	double m_nD0,m_Hc,m_Bc,m_Hf,m_Bf;
	int m_nNqrb,m_nNrow;
	CString m_strShape;
	
protected:  
	void UpdateSelectedMembEdit();
	int GetSelectedMember(CArray<UINT,UINT>& aMembK);
	
	void InitCtrls();
	void AlignCtrls();
	void EnableDisableCtrls();
	
	void Data2Dlg();
	BOOL Dlg2Data(T_REBR_D* pRebrD);
	
	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_REBR_KEY& RebrKey);
	int GetSelectedItems(CArray<T_REBR_KEY,T_REBR_KEY>& aRebrKey);
	void MakeItemEx();
	void SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar);
	
	UINT GetNextSubSectionKey(UINT SectK);
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBraceNewDlg)
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd=NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConBraceNewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkSubSection();
	afx_msg void OnChangeDZero();
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnAddReplace();
	afx_msg void OnDgnDelete();
	afx_msg void OnDgnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONBRACENEWDLG_H__)
