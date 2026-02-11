#if !defined(_LOADSTEPDLG_H__)
#define _LOADSTEPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "NXNlctLoadStepGrid.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__	CMNXNlctLoadStepDlg : public CDialogMove
{
public:
	CMNXNlctLoadStepDlg(CWnd* pParent = NULL);

	void SetData(T_NLCT_DATA& data, int m_nMethod);
	void GetData(T_NLCT_DATA& data);

	enum { IDD = IDD_CMD_LOADSTEP };

	CFormulaEditSpin	m_edtNum;
	CNXNlctLoadStepGrid m_wndGrid;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void OnDataChanged();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdGenerateLoadFactorBtn();
	afx_msg void OnChangeCmdGenerateLoadFactorEdt();

	BOOL CheckGridData();

	T_NLCT_DATA m_Data;

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif