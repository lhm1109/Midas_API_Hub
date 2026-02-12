#pragma once

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonSewaDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CSeisCommonSewaDlg(CWnd* pParent = nullptr);
	~CSeisCommonSewaDlg();
	enum { IDD = IDD_TM_SEIS_COMMON_SEWA };

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
	bool Dlg2Data(T_SEWA_D& rSewaD);
	bool Data2Dlg(const T_SEWA_D& SewaD);

private:
	MenuBarRunMode GetRunMode();
	void SetRunMode(MenuBarRunMode nMode);

	void SetDetailWall(UINT nDetail);
	UINT GetDetailWall();

private:
	MButton m_rdoExecAdd;
	MButton m_rdoExecDel;

	MButton m_rdoDetlSeis;
	MButton m_rdoDetlMiti;
	MButton m_rdoDetlNone;
};
