#if !defined(__CCMPostTensionTendonStressingDlg_H__)
#define __CCMPostTensionTendonStressingDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DlgBase.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "MyListCtrl.h"

#include "wg_cmdRes2.h"

#include "HeaderPre.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMPostTensionTendonStressingDlg dialog

class __MY_EXT_CLASS__ CCMPostTensionTendonStressingDlg : public CDialogMove
{
// Construction
public:
	CCMPostTensionTendonStressingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMPostTensionTendonStressingDlg();

// Dialog Data
	//{{AFX_DATA(CCMPostTensionTendonStressingDlg)
	enum { IDD = IDD_CMD_PT_TENDON_STRESSING };
	
	// Left side list control and filter controls
	CListCtrl	m_listStressing;
	mit::frx::MEdit m_editName;
	CString m_strName;

	// Apply the same to both ends checkbox
	MButton		m_checkApplySame;
	
	// First section controls
	MComboBox	m_comboFirstMethod;
	MEdit		m_editFirstJackingStage;
	MEdit		m_editFirstTransferStage;
	MEdit		m_editFirstServiceStage;
	MButton		m_checkFirstAutoTransfer;
	
	// Last section controls
	MComboBox	m_comboLastMethod;
	MEdit		m_editLastJackingStage;
	MEdit		m_editLastTransferStage;
	MEdit		m_editLastServiceStage;
	MButton		m_checkLastAutoTransfer;
	
	// Dead Load Balancing Ratio controls
	MEdit		m_editMaxRatio;
	MEdit		m_editMinRatio;
	
	// Pre-compression Level controls
	MEdit		m_editMaxPreComp;
	MEdit		m_editMinPreComp;
	
	// Dialog buttons
	MButton		m_btnClose;
	
	// Member variables for dialog data
	CString		m_strFirstJackingStage;
	CString		m_strFirstTransferStage;
	CString		m_strFirstServiceStage;
	CString		m_strLastJackingStage;
	CString		m_strLastTransferStage;
	CString		m_strLastServiceStage;
	CString		m_strMaxRatio;
	CString		m_strMinRatio;
	CString		m_strMaxPreComp;
	CString		m_strMinPreComp;
	
	BOOL		m_bApplySame;
	BOOL		m_bFirstAutoTransfer;
	BOOL		m_bLastAutoTransfer;
	int			m_nFirstMethod;
	int			m_nLastMethod;
	int			m_nSlabFilter;
	//}}AFX_DATA

	int GetSelectedStressingKey();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPostTensionTendonStressingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void MakeListHeader();
	void SetData2Dlg();
	BOOL ApplyDlg();
	void OnSlabFilterChange();
	void On2FFilterChange();
	void OnXDirFilterChange();
	void OnApplySameToEnds();
	void EnableLastSectionControls(BOOL bEnable);

protected:
	CDBDoc* m_pDoc;
	
	// Custom draw support for checkbox in Typical column
	std::vector<bool> m_vTypicalChecked; // 각 아이템의 체크 상태 저장

	// Generated message map functions
	//{{AFX_MSG(CCMPostTensionTendonStressingDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnAddBtn();
	afx_msg void OnModifyBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnCheckApplySame();
	afx_msg void OnSelChangeFirstMethod();
	afx_msg void OnSelChangeLastMethod();
	afx_msg void OnCheckFirstAutoTransfer();
	afx_msg void OnCheckLastAutoTransfer();
	afx_msg void OnClickStressingList(NMHDR* pNMHDR, LRESULT* pResult);

	void UpdateStressingList();
	void UpdateStressingData();
	void InitTendonStressingList();
	void AddTendonStressingList(const UINT& nID, const CString& strName);
	void Data2Dlg();
	void Dlg2Data();

private:
	T_TDST_D m_Data;

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(__CCMPostTensionTendonStressingDlg_H__)