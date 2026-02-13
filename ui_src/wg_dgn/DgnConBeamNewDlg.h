#if !defined(__DGNCONBEAMNEWDLG_H__)
#define __DGNCONBEAMNEWDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamNewDlg.h : header file
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
#include "DgnConBeamRebarGrid.h"
#include "wg_dgnRes2.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamNewDlg dialog

class __MY_EXT_CLASS__ CDgnConBeamNewDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CDgnConBeamNewDlg(CWnd* pParent = NULL);
	virtual ~CDgnConBeamNewDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConBeamNewDlg)
	enum { IDD = IDD_DGN_CON_BEAM_DLG_NEW };
	CSelectEdit m_edtMemb;
	CListCtrl m_SectList;
	mit::frx::MButton m_chkSubSect;
	mit::frx::MEdit m_edtSectK;
	mit::frx::MEdit m_edtSubSectK;

	CEditUnit m_edtDT;
	CEditUnit m_edtDB;
	CTextUnit m_untDTDB;

	CFoldGroupBox_Cross m_grbDetail;

	CDgnDrawWnd m_INodeWnd;
	CDgnDrawWnd m_CenterWnd;
	CDgnDrawWnd m_JNodeWnd;

	mit::frx::MButton m_chkSameSizeTopBot;
	mit::frx::MButton m_chkSameSizeIMJ;
	mit::frx::MButton m_chkSameSizeLayer;
	//}}AFX_DATA

protected:
	BOOL m_bInit;
	CArray<UINT,UINT> m_aSameValRdo;
	T_REBB_D m_RebbD;

	CDgnConBeamRebarGrid m_RebarGrid;
	CMap<int,int,T_REBB_KEY,T_REBB_KEY> m_mIndex2RebbK;
	CMap<T_REBB_KEY,T_REBB_KEY,int,int> m_mRebbK2Index;

	mit::frx::MillustViewer m_wndRect;
	mit::frx::MillustViewer m_wndTee;

	CArray<UINT,UINT> m_aMrebK;
public:
	void ShowSelectedRebb(T_REBB_KEY RebbKey, BOOL bInit/*=TRUE*/);

	void DrawSectionWnd(T_REBB_D* pRebbD);

protected:
	void ShowBitmap(CString str);

	void Initial_SectionData(T_SECT_D& SectD);
	double Get_SectionDimension(T_SECT_D& SectD);
	double Get_ChangeUnitFactor();

	double m_DtNo,m_DbNo,m_Hc,m_Bc,m_Hf,m_Bf,m_Space;
	CString m_strShape;

protected:
	void UpdateSelectedMembEdit();
	int GetSelectedMember(CArray<UINT,UINT>& aMembK);

	void InitCtrls();
	void EnableDisableCtrls();

	void Data2Dlg();
	BOOL Dlg2Data(T_REBB_D* pRebbD);

	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_REBB_KEY& RebbKey);
	int GetSelectedItems(CArray<T_REBB_KEY,T_REBB_KEY>& aRebbKey);
	void MakeItemEx();
	void SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar);

	UINT GetNextSubSectionKey(UINT SectK);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamNewDlg)
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
	//{{AFX_MSG(CDgnConBeamNewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkSubSection();
	afx_msg void OnSectionRdo();
	afx_msg void OnChkSameRebarSize();
	afx_msg void OnChangeDtDb();
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

#endif // !defined(__DGNCONBEAMNEWDLG_H__)
