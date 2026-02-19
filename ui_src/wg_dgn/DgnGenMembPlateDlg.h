#if !defined(__DGNGENMEMBPLATEDLG_H__)
#define __DGNGENMEMBPLATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenMembPlateDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembPlateDlg dialog

class CDgnGenMembPlateDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenMembPlateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CDgnGenMembPlateDlg)
	enum { IDD = IDD_DGN_GEN_MEMB_PLATE_DLG };

	int		    m_nOption;
	CEditUnit m_edtName;
	int       m_nPlateMemb;
	int       m_nLocalUCS;
	MComboBox m_cmbUCS;
	int       m_nElemAvg;
	int       m_nXYDir;
	MButton	m_chkAvgNodal;

 	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenMembPlateDlg)
 	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	virtual ~CDgnGenMembPlateDlg();
	
	void InitUCSCombo();
 	void InitSelectedItem();
	void ControlsEnableDisable();
	void Data2Dlg();
	BOOL Dlg2Data();
	
	
	// Generated message map functions
	//{{AFX_MSG(CDgnGenMembPlateDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnOptionRdo();
	afx_msg void OnDgnLocalUCSRdo();	
	afx_msg void OnDgnRadioElemAvg();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
protected:
	T_RCPL_D m_Data;

protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	void OnDelKeyPressedInList();    
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void DisplaySelElemList();  
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void InsertItem(T_RCPL_K Key);
	void DeleteItem(T_RCPL_K Key);
	void SetListFocus(CArray<int, int>& aSelItem);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNGENMEMBPLATEDLG_H__)
