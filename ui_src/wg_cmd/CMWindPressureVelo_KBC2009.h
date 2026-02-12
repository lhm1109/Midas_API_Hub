#if !defined(__CM_WIND_VELO_KBC2009_H__)
#define __CM_WIND_VELO_KBC2009_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureVelo_KBC2009.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KBC2009 dialog
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_KBC2009 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_KBC2009(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WVEP_KBC2009& data);
	BOOL SetDlg2Data(T_WVEP_KBC2009& data);
	
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_KBC2009 };
	
	MComboBox	m_cobxCategory;
// 	CTextUnit m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	MComboBox m_cobxImportFactor;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	MButton	  m_chkTopoEffect;
	CEditUnit	m_editKZT;
	CTextUnit	m_unitVKZT;
	CEditUnit	m_editVKZT;
	
protected:
	void InitCategoryCombo();
	void InitImportFactorCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	CCMWindPressureVelocityAddMod* m_pParent;

protected:
	double m_dBXKBC2009 ;		
	double m_dBYKBC2009 ;		
	double m_dFreqXKBC2009;	
	double m_dFreqYKBC2009;	
	double m_dDampingKBC2009;
	
	T_WVEP_KBC2009  m_Data;
	
public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdTopoEffectCheck();

	DECLARE_MESSAGE_MAP()
};


#endif
