#if !defined(AFX_CMSECTIONPAGE_H__A58A2481_1FD7_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTIONPAGE_H__A58A2481_1FD7_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectionPage.h : header file
//

#include "CMSecViewWnd.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog
#include "..\MIT_frx\MButton.h"
#include "..\mit_frx\MEdit.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectionPage dialog
class CCMPropertiesDlg;


class CCMSectionPageBase
{
// Construction
public:
	CCMSectionPageBase(CDBDoc* pDoc, BOOL bDesign, CWnd* pParent = NULL);   // standard constructor

//---------------------------------------------------------------------
// Overridables
public:
	void OnUpdateDB(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UnitChanged();
	void SetVirtual(int nDesignTye);

//---------------------------------------------------------------------
public:
	static POSITION GetStartSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual);
	static void GetNextSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual, POSITION &pos, T_SECT_K &Key, T_SECT_D &Data);
	static BOOL GetSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual, T_SECT_K Key, T_SECT_D &Data);

//---------------------------------------------------------------------
// Implementation
public:
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);
	virtual void ShowSizeTip(T_SECT_D& rData) {};

	BOOL InsertItem(T_SECT_K Key, T_SECT_D &Data);
	BOOL DeleteItem(T_SECT_K Key, T_SECT_D &Data);
	BOOL ModifyItem(T_SECT_K KeyOld, T_SECT_K Key, T_SECT_D &Data);
	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);

	BOOL IsUnmodifiedDesignSection(T_SECT_K Key, T_SECT_D& rData);

public:
	CDBDoc* m_pDoc;
	CCMSecViewWnd m_wndSecView; // variable for section preview
	CCMPropertiesDlg* m_pParent;

	CListCtrl	m_List;

	BOOL OnInitDialog(CWnd* pwndPreview);
	void OnAdd();
	void OnModify();
	void OnDelete();
	void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCopy();

public:
	T_SECT_K m_Key;
	T_SECT_D m_Data;
	BOOL        m_bDesign;
	BOOL        m_bVirtual;
	BOOL        m_bHideUDS;

};

class CCMSectionPage : public CChildDialog, protected CCMSectionPageBase
{
// Construction
public:
	CCMSectionPage(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

//---------------------------------------------------------------------
// Overrides
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivatePage();
	virtual void ShowSizeTip(T_SECT_D& rData);
protected:
	void SetShowTip(CString& csTip);

	MEdit	m_editTip;
	CString m_csTip;

//---------------------------------------------------------------------
// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CCMSectionPage)
	enum { IDD = IDD_CMD_SECTION_PAGE };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual void OnActivatePage(int nItemOld, int nItemNew);

	// Generated message map functions
	//{{AFX_MSG(CCMSectionPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnImport();
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnCmdBtnRenum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCMSectPage4Design dialog

class CCMSectPage4Design : public CDialogMove, protected CCMSectionPageBase, public CDBUpdateConnector
{
// Construction
public:
	CCMSectPage4Design(CWnd* pParent = NULL);   // standard constructor
	void SetVirtual(BOOL bVitual);

//---------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Dialog Data
	//{{AFX_DATA(CCMSectPage4Design)
	enum { IDD = IDD_CMD_SECT4DESIGN };
	MButton	m_chkHideUDS;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectPage4Design)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void VirtualReset();
	void VirtualResetAll();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectPage4Design)
	virtual BOOL OnInitDialog();
	afx_msg void OnModify();
	afx_msg void OnReset();
	afx_msg void OnResetAll();	
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHideUDS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTIONPAGE_H__A58A2481_1FD7_11D3_92DE_0000C0B0E6B3__INCLUDED_)
