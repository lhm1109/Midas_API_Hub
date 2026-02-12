#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog
#include "CMLoadCombDefGrid.h"
#include "LoadCombDefineData.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefPage dialog


class CCMLoadCombDefDlg;
//#define CCMLoadCombDefPage_base CChildDialog

class CCMLoadCombDefPage : public CChildDialog
{
public:
	CCMLoadCombDefPage(CLoadCombDefineData* m_pData, CWnd* pParent =NULL);   // standard constructor

public:
	mit::frx::MComboBox		m_cmbCode;
	mit::frx::MButton		m_bChkCase;
	mit::frx::MButton		m_bChkWind;

	CDBDoc* m_pDoc;
	CCMLoadCombDefGrid m_wndGrid;
	CCMLoadCombDefDlg* m_pParent;
	CLoadCombDefineData *m_pData;
	enum { IDD = IDD_CMD_LOADCOMB_DEF_DEF_PAGE };

public:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT OpenNewTab(WPARAM wParam, LPARAM lParam);
	LRESULT CloseCurTab(WPARAM wParam, LPARAM lParam);
protected:
	
	afx_msg void OnResetButton();
	afx_msg void OnSelectCode();

	void InitSelectCode();
	void InitGrid(BOOL bInit);
	int GetSelectCodeIndex(CString strCode);
	BOOL m_bInit;

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
