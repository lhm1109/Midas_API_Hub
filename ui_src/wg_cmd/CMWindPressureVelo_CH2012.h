#if !defined(__CM_WIND_VELO_CH2012_H__)
#define __CM_WIND_VELO_CH2012_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureVelo_KBC2009.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_CH2012 dialog
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_CH2012 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_CH2012(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();

	void SetData2Dlg(T_WVEP_CH2012& data);
	BOOL SetDlg2Data(T_WVEP_CH2012& data);
	
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_CHINA2012 };
	
	MComboBox	m_cobxCategory;
	CEditUnit	m_editWindPressure;
	CEditUnit	m_editTopoCoef;
	
private:
	CCMWindPressureVelocityAddMod* m_pParent;

	T_WVEP_CH2012 m_Data;

protected:	
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


#endif
