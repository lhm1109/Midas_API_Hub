#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\LateralLoad.h"
#include "SeisItemUserGrid.h"

class CSeisItemUser : public CChildDialog
{
public:
	CSeisItemUser(CWnd* pParent = NULL);
	
	enum { IDD = IDD_ETC_SEIS_ITEM_USER };

	void SetData2Dlg(T_SEIS_D& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_D& data);

public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	void SetControl();
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bInit;
	BOOL m_bModify;

	CLateralLoad m_LateralLoad;
	CSeisItemUserGrid m_Grid;
	CArray<T_SEIS_USER, T_SEIS_USER&> m_aData;

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};