#if !defined(__POSTTENSIONMODIFYTENDONDLG_H__)
#define __POSTTENSIONMODIFYTENDONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_ChildDialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include <afxcmn.h>

class CPostTensionModifyTendonVerDlg;
class CPostTensionModifyTendonHorDlg;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CPostTensionModifyTendonDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CPostTensionModifyTendonDlg(CWnd* pParent = NULL);
	virtual ~CPostTensionModifyTendonDlg();

// Dialog Data
	enum { IDD = IDD_CMD_PT_MODIFY_TENDON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Attributes
protected:
	CDBDoc* m_pDoc;
	CDlgTabCtrl m_wndTab;
	CPostTensionModifyTendonVerDlg* m_pVerPage;
	CPostTensionModifyTendonHorDlg* m_pHorPage;
	int m_nActivePage;
	T_PTTN_K m_SeclectedTendonKey;
	T_PTTN_D m_OriginalPttnData;

	// Controls
	CListCtrl m_wndTendonList;
	mit::frx::MEdit m_edtTendonName;
	mit::frx::MComboBox m_cmbTendonProperty;
	mit::frx::MComboBox m_cmbTendonStressing;

// Operations
public:
	void SetPttnK(T_PTTN_K PttnKey);

	void SetActivePage(int nPage);
	int GetSelectedTendonKey();  // 현재 선택된 텐던 키 반환

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void LayoutChildDlg();
	virtual void OnClose();

	void InitTendonList();
	void UpdateTendonList();
	void UpdateTendonPropertyList();
	void UpdateTendonStressingList();
	void UpdateControlsByPttnData(T_PTTN_K nKey);
	void Dlg2Data(T_PTTN_D& Data);
	BOOL IsDataChanged(const T_PTTN_D& NewData);

// Generated message map functions
protected:
	afx_msg void OnItemClickTendonList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApplyBtn();
	afx_msg void OnDeleteBtn();
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif // !defined(__POSTTENSIONMODIFYTENDONDLG_H__)
