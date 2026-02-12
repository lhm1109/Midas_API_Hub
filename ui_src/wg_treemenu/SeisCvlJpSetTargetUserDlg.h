#if !defined(__SEIS_CVL_JP_SET_TARGET_USER_DLG_H__)
#define __SEIS_CVL_JP_SET_TARGET_USER_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetTargetUserDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_db\wg_db_selectctrl.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
class CSeisCvlJpSetTargetDlg;

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUserDlg dialog

class CSeisCvlJpSetTargetUserDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	int				 m_nOption;
	MEdit			 m_name;
	CSelectEditColor m_elements;
	MComboBox		 m_shearAxisDir;
	CListCtrl		 m_sigrUserList;
	//}}AFX_DATA

	CDBDoc* m_pDoc;
	CSeisCvlJpSetTargetDlg* m_pParent;

	// Construction
public:
	CSeisCvlJpSetTargetUserDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetTargetUserDlg();

	// Dialog Data
		//{{AFX_DATA(CSeisCvlJpSetTargetUserDlg)
	enum { IDD = IDD_TM_SIGR_USER };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetTargetUserDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetTargetUserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickSigrUserList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	void InitControl();
	void InitSigrUserListCtrl();
	void SetHeaderTitleListCtrl();
	BOOL InsertItemListCtrl(T_SIGR_USER_K Key, T_SIGR_USER_D& Data);
	CString DataToStr(const int& index, const T_SIGR_USER_D& data);

	BOOL AddData();
	BOOL ModData(const T_SIGR_USER_K& key);
	BOOL DelData(const T_SIGR_USER_K& key);
	void DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_USER_D& rUserD);
	BOOL CheckData(IN T_SIGR_D& sigrD, const T_SIGR_USER_D& userD, BOOL bCheckName = TRUE);
	void ResetControls();

	void UpdateBuffer();
	BOOL InsertItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data);
	BOOL DeleteItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data);
	BOOL ModifyItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data);

	void UpdataDialog(const T_SIGR_USER_K& key);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_TARGET_USER_DLG_H__)