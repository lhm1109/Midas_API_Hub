#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\LateralLoad.h"
#include "CMWindItemUserGrid.h"

class CCMWindItemDlg;
class CCMWindItemRUS2016_User : public CChildDialog
{
public:
	CCMWindItemRUS2016_User(CWnd* pParent = NULL);

	enum { IDD = IDD_ETC_WIND_ITEM_RUS2016_USER	};

	void SetData2Dlg(T_WIND_D& data, BOOL bModify, T_KEY KeyWind);
	BOOL SetDlg2Data(T_WIND_D& data);
	BOOL GetWnpsData(T_WNPS_D& WnpsD);
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
	BOOL m_bSaveWnps;
	CCMWindItemDlg *m_pParent;
	CLateralLoad m_LateralLoad;
	CCMWindItemUserGrid m_Grid;

	T_KEY m_Key;
	T_WIND_D m_Data;
	T_WNPS_D m_WnpsD;

	CArray<UINT, UINT> m_aCtrlExRadio;
	CArray<UINT, UINT> m_aCtrlEyRadio;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCreateWindPulsation();
	afx_msg void OnCmdChkWindPulsation();
	DECLARE_MESSAGE_MAP()
};