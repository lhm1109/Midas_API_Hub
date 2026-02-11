#if !defined(AFX_CMSTORAUTOGENDLG_H__25EA85A1_269B_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSTORAUTOGENDLG_H__25EA85A1_269B_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStorAutoGenDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_FlagCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStorAutoGenDlg dialog
struct T_SEIS_WIDTH
{
	double dSeisWidthX;
	double dSeisWidthY;
	void Initialize()
	{
		dSeisWidthX = 0.;
		dSeisWidthY = 0.;
	}
};

class CCMStorAutoGenDlg : public CDialogMove
{
// Construction
public:
	CCMStorAutoGenDlg(CWnd* pParent = NULL);   // standard constructor
	CFlagCtrl* m_pFlagCtrl;

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle(BOOL bUnsel);

	void MakeItemEx(BOOL bUnsel);
	BOOL SetItem(BOOL bUnsel, int iItem, int nNo, T_STOR_D &Data, double dHeight=0.0);
	CString DataToStr(BOOL bUnsel, int i, int nNo, T_STOR_D &Data, double dHeight);

	void MakeStoryName();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

	// Auto Generate Function
	BOOL GenCandidate();
	void BuildStoryData(CArray<T_STOR_D, T_STOR_D&> &aStory, 
											CString &Name, double dblLevel, BOOL bDiaph);
	double GetStoryTolerance(CArray<T_NODE_D, T_NODE_D&>& aNode);
	static int CompAscNodeZ(const void* a1, const void* a2);
	void BuildSeisWidth(CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&> &aSeisWidth, 
											double dblMinX, double dblMaxX, double dblMinY, double dblMaxY);
	void BuildWindWidth(T_STOR_D& StorData,                       
											double dblMinXC, double dblMaxXC, double dblMinYC, double dblMaxYC,
											double dblMinXU, double dblMaxXU, double dblMinYU, double dblMaxYU);
protected:
	CDBDoc* m_pDoc;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<T_STOR_D, T_STOR_D&> m_aStory;
	CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&> m_aSeisWidth;
	CArray<int, int> m_aStoryKind;        // -2:base, -1:B??, 1:??F, 2:Roof
	BOOL m_bBaseFloorExist;

public:
	// Dialog Data
	//{{AFX_DATA(CCMStorAutoGenDlg)
	enum { IDD = IDD_ETC_STOR_AUTOGEN_DLG };
	CEditUnit	m_wndEccenEdit;
	CListCtrl	m_wndUnselList;
	CListCtrl	m_wndSelList;
	BOOL	m_bEccenChk;
	CEditUnit	m_wndWindEccenEdit;  
	BOOL	    m_bWindEccenChk;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStorAutoGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetSeisWidth(CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&>& Data);

protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStorAutoGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdSelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdUnselList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCmdEccenChk();
	afx_msg void OnCmdWindEccenChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTORAUTOGENDLG_H__25EA85A1_269B_11D4_92DE_0000C0B0E6B3__INCLUDED_)
