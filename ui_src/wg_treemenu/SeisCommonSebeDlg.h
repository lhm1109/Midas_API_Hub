#pragma once

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonSebeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CSeisCommonSebeDlg(CWnd* pParent = nullptr);
	~CSeisCommonSebeDlg();
	enum { IDD = IDD_TM_SEIS_COMMON_SEBE };

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
	DECLARE_MESSAGE_MAP()

private:
	bool Dlg2Data(T_SEBE_D& rSebeD);
	bool Data2Dlg(const T_SEBE_D& SebeD);

private:
	MenuBarRunMode GetRunMode();
	void SetRunMode(MenuBarRunMode nMode);

	void SetRebarDetail(UINT nDetail);
	UINT GetRebarDetail();

private:
	MButton m_rdoExecAdd;
	MButton m_rdoExecDel;

	MButton m_rdoDetlSeis;
	MButton m_rdoDetlNone;
	MButton m_rdoDetlAuto;
};
