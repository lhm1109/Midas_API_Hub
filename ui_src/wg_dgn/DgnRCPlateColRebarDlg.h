#pragma once
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "DgnBeamGridWnd.h"

#include "HeaderPre.h"

class CDgnBeamGridWndCIVIL;
class __MY_EXT_CLASS__ CDgnRCPlateColRebarDlg : public CDialogMove
{
public:
	CDgnRCPlateColRebarDlg(CWnd* pParent = NULL);
	~CDgnRCPlateColRebarDlg();
	enum { IDD = IDD_DGN_RC_PLATE_COL_REBAR_PARENT_DLG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();

public:
	void SetData(T_RIPC_K RipcK);

protected:
	void LayoutChildDlgs();
	void InitCombo();
	void InitUnit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ChildDlg2Data();
	void ChildData2Dlg();
	void InitLst();
	void RefreshList(BOOL bKeepSelection);
	void SetDataByCurListItem();
	void ControlsEnableDisable();

private:
	afx_msg void OnClickedShearChk();
	afx_msg void OnBtnClose();
	afx_msg void OnClickedAdd();
	afx_msg void OnClickedMod();
	afx_msg void OnClickedDel();
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	mit::frx::MComboBox m_cmbSSize;
	CEditUnit	m_edtSSpace;
	CTextUnit	m_untSSpace;
	CEditUnit	m_edtSNum;
	CDlgTabCtrl	m_wndTab;
	CListCtrl	m_LstRebar;
	mit::frx::MEdit m_edtName;

private:
	BOOL m_bSRebar;
	CDBDoc* m_pDoc;
	T_RIPC_D m_Data;
	T_RIPC_K m_KeyForModify;
};
#include "HeaderPost.h"
