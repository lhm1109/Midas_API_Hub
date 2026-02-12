#if !defined(__SEIS_CVL_JP_SET_TARGET_UPPER_DLG_H__)
#define __SEIS_CVL_JP_SET_TARGET_UPPER_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSeisCvlJpSetTargetUpperDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_db\wg_db_selectctrl.h"
#include "..\wg_db\SingleSelectEdit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
class CSeisCvlJpSetTargetDlg;
class CSingleSelectEdit;
/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUpperDlg dialog

class CSeisCvlJpSetTargetUpperDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
private:
	CDBDoc* m_pDoc;
	int				 m_nOption;		//옵션 라디오 버튼
	MEdit			 m_name;		//명칭
	CSelectEditColor m_elements;	//조사대상
	CSingleSelectEdit m_nodeL;		//응답변위 산출 절점위치(거더 좌단)
	CSingleSelectEdit m_nodeR;		//응답변위 산출 절점위치(거더 우단)
	MComboBox		 m_shearAxisDir;//교축방향 전단력
	CListCtrl		 m_sigrUpperList;
	CSeisCvlJpSetTargetDlg* m_pParent;

	// Construction
public:
	CSeisCvlJpSetTargetUpperDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCvlJpSetTargetUpperDlg();

// Dialog Data
	//{{AFX_DATA(CSeisCvlJpSetTargetUpperDlg)
	enum { IDD = IDD_TM_SIGR_UPPER };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetTargetUpperDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetTargetUpperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocusElemEdt();
	afx_msg void OnSetFocusNodeLEdt();
	afx_msg void OnSetFocusNodeREdt();
	afx_msg void OnNMClickSigrUpperList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	//OnInitDialog() 관련함수
	void InitControl();
	void InitSigrUpperListCtrl();
	void SetHeaderTitleListCtrl();
	BOOL InsertItemListCtrl(const T_SIGR_UPPER_K& Key, T_SIGR_UPPER_D& Data);
	CString DataToStr(const int& index, const T_SIGR_UPPER_D& data);
	//Execute() 관련함수
	BOOL AddData();
	BOOL ModData(const T_SIGR_UPPER_K& key);
	BOOL DelData(const T_SIGR_UPPER_K& key);
	BOOL DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_UPPER_D& rUpperD);
	BOOL CheckData(IN T_SIGR_D& sigrD, const T_SIGR_UPPER_D& upperD, BOOL bCheckName = TRUE);
	BOOL IsOnlyBeam(const T_ELEM_K_LIST& elemKeys);
	void ResetControls();
	void ResetViewControls();
	//OnUpdate() 관련함수
	void UpdateBuffer();
	BOOL InsertItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data);
	BOOL DeleteItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data);
	BOOL ModifyItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data);

	void UpdataDialog(const T_SIGR_UPPER_K& key);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_TARGET_UPPER_DLG_H__)