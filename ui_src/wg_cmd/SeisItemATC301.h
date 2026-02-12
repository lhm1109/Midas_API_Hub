#if !defined(__SEISITEMATC301_H__)
#define __SEISITEMATC301_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemATC301.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemATC301 dialog

class CSeisItemDlgNew;
class CSeisItemATC301 : public CChildDialog
{
public:
	CSeisItemATC301(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_ATC306& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_ATC306& data);
	void SetFoldGroupCtrls();

	enum { IDD = IDD_ETC_SEIS_ITEM_ATC306 };

	CEditUnit m_editRMFX;
	CEditUnit m_editRMFY;
	CEditUnit m_editCodeY;
	CEditUnit m_editCodeX;
	CEditUnit m_editAnalY;
	CEditUnit m_editAnalX;
	CEditUnit m_editZF;
	CEditUnit m_editSF;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_ATC306 m_Data;

protected:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMATC301_H__)
