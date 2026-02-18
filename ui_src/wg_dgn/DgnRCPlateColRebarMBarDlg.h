#pragma once
#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "DgnBeamGridWnd.h"

class CDgnBeamGridWndCIVIL;
class CDgnRCPlateColRebarMBarDlg : public CChildDialog
{
public:
	CDgnRCPlateColRebarMBarDlg(T_RPCM_D* pData, CWnd* pParent = NULL);
	~CDgnRCPlateColRebarMBarDlg();
	enum { IDD = IDD_DGN_RC_PLATE_COL_REBAR_MBAR_CHILD_DLG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

public:
	void Data2Dlg();
	BOOL Dlg2Data();
	void ReCalcAs();

protected:
	void InitCombo();
	void InitGrid();
	void InitUnit();
	void SetDataToGrid();
	void GetDataFromGrid();
	void ControlsEnableDisable();
	void InitCtrl();
	void SetNumCTC(BOOL bClick=FALSE);

private:
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeDgnRcTopLayerCmb();
	afx_msg void OnSelchangeDgnRcBotLayerCmb();
	afx_msg void OnDestroy();
	afx_msg void OnClickedNumCTC();
	afx_msg  LRESULT OnBeforeEndEditing(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	mit::frx::MComboBox m_cmbTopLayer;
	CEditUnit	m_edtAsTop;	
	mit::frx::MComboBox m_cmbBotLayer;
	CEditUnit	m_edtAsBot;	
	CDgnBeamGridWndCIVIL*  m_TopGrid;
	CDgnBeamGridWndCIVIL*  m_BotGrid;
	CArray<UINT, UINT> m_aRdoNumCTC;

private:
	CDBDoc* m_pDoc;
	T_RPCM_D* m_pData; // I만 사용, Layer 최대갯수 3개
	int m_iThik;
};
