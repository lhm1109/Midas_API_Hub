#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\LateralLoad.h"
#include "CMWindItemUserGrid.h"

class CCMWindItemDlg;
class CCMWindItemUser : public CChildDialog
{
public:
	CCMWindItemUser(CWnd* pParent = NULL);

	enum { IDD = IDD_ETC_WIND_ITEM_USER };

	void SetData2Dlg(T_WIND_D& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_D& data);

public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	void SetControl();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	BOOL m_bInit;
	CCMWindItemDlg *m_pParent;
	CLateralLoad m_LateralLoad;
	CCMWindItemUserGrid m_Grid;
	T_WIND_D m_Data;

	CArray<UINT, UINT> m_aCtrlExRadio;
	CArray<UINT, UINT> m_aCtrlEyRadio;

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};