#pragma once

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonSespDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CSeisCommonSespDlg(CWnd* pParent = nullptr);
	~CSeisCommonSespDlg();
	enum { IDD = IDD_TM_SEIS_COMMON_SESP };

public:
	virtual void Execute();

private:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	void UpdateBuffer();
	bool GetElemKeyList(T_ELEM_K_LIST& raElemK);

private:
	afx_msg void OnChangeMembType();
	afx_msg void OnChangeLengType();
	DECLARE_MESSAGE_MAP()

private:
	bool Dlg2Data(T_SESP_D& rSespD);
	bool Data2Dlg(const T_SESP_D& SespD);

	void InitUnitByLengType(BOOL bReal);
	void ShowHideByLengType(BOOL bReal);
	void EnableByMembType(UINT nMembType);

private:
	MenuBarRunMode GetRunMode();
	void SetRunMode(MenuBarRunMode nMode);

	void SetMembType(UINT nMembType);
	UINT GetMembType();

private:
	MButton m_rdoExecAdd;
	MButton m_rdoExecDel;

	MButton m_rdoMembBeam;
	MButton m_rdoMembColm;

	CBCGPStatic m_lblSpanLsyi; CEditUnit m_edtSpanLsyi; CTextUnit m_untSpanLsyi;
	CBCGPStatic m_lblSpanLsyj; CEditUnit m_edtSpanLsyj; CTextUnit m_untSpanLsyj;
	CBCGPStatic m_lblSpanLszi; CEditUnit m_edtSpanLszi; CTextUnit m_untSpanLszi;
	CBCGPStatic m_lblSpanLszj; CEditUnit m_edtSpanLszj; CTextUnit m_untSpanLszj;

	MButton m_chkRelative;
};
