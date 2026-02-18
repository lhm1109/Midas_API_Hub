#if !defined(__DGNCONCOLMNEWDLG_H__)
#define __DGNCONCOLMNEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmNewDlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\FoldGroupBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDlgBase.h"
#include "DgnDrawWnd.h"
#include "DgnConColmRebarGrid.h"
#include "wg_dgnRes2.h"

#include "HeaderPre.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnConColmNewDlg dialog

class __MY_EXT_CLASS__ CDgnConColmNewDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnConColmNewDlg(CWnd* pParent = NULL);
	virtual ~CDgnConColmNewDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConColmNewDlg)
	enum { IDD = IDD_DGN_CON_COLM_DLG_NEW };
	CSelectEdit m_edtMemb;
	CListCtrl m_SectList;
	mit::frx::MButton m_chkSubSect;
	mit::frx::MEdit m_edtSectK;
	mit::frx::MEdit m_edtSubSectK;

	CEditUnit m_edtD0;
	CTextUnit m_untD0;
	mit::frx::MEdit m_edtBcjr;
	CSpinButtonCtrl m_spnBcjr;

	mit::frx::MButton m_chkSameRebarMidEndt;

	CFoldGroupBox_Cross m_grbDetail;

	CDgnDrawWnd m_INodeWnd;
	CDgnDrawWnd m_CenterWnd;

	mit::frx::MillustViewer m_wndRect;
	mit::frx::MillustViewer m_wndCircle;
	mit::frx::MillustViewer m_wndPipe;
	//}}AFX_DATA

protected:
	BOOL m_bInit;
	T_REBC_D m_RebcD;

	CDgnConColmRebarGrid m_RebarGrid;
	CMap<int,int,T_REBC_KEY,T_REBC_KEY> m_mIndex2RebcK;
	CMap<T_REBC_KEY,T_REBC_KEY,int,int> m_mRebcK2Index;

	CArray<UINT,UINT> m_aMrebK;
public:
	void ShowSelectedRebc(T_REBC_KEY RebcKey, BOOL bInit/*=TRUE*/);

	void DrawSectionWnd(T_REBC_D* pRebcD);
	
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
	BOOL Dlg2Data(T_REBC_D* pRebcD);
	
	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_REBC_KEY& RebcKey);
	int GetSelectedItems(CArray<T_REBC_KEY,T_REBC_KEY>& aRebcKey);
	void MakeItemEx();
	void SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar);
	
	UINT GetNextSubSectionKey(UINT SectK);
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmNewDlg)
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
	//{{AFX_MSG(CDgnConColmNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkSubSection();
	afx_msg void OnChangeDZero();
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSameEndNCenterRebar();
	afx_msg void OnDgnAddReplace();
	afx_msg void OnDgnDelete();
	afx_msg void OnDgnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCOLMNEWDLG_H__)
