#if !defined(__CM_WIND_VELO_ADDMOD_H__)
#define __CM_WIND_VELO_ADDMOD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "CMDlgBase.h"
#include "HeaderPre.h"      /////////
using namespace mit::frx;
class CCMWindPressureVelocityCodeMgr;
class __MY_EXT_CLASS__ CCMWindPressureVelocityAddMod : public CChildDialogMove
{
// Construction
public:
	CCMWindPressureVelocityAddMod(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMWindPressureVelocityAddMod();

	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_NEW };

	T_WVEP_D m_Data;
	T_WVEP_K m_Key;
	
	void SetWvepKey(T_WVEP_K Key);
	void GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID);
	BOOL GetHolderRect(CRect* pRect);

protected:
	MEdit     m_edtName;
	CBCGPStatic	  m_wndHolder;
	MComboBox	m_cobxCodeName;

	CCMWindPressureVelocityCodeMgr* m_pMgr;
	CArray<BOOL, BOOL> m_aWlCodeEnable;
	int m_nOldSel;

	double m_dVortexDM;
	double m_dVortexDB;
	double m_dRoofHeight;
	double m_dTotalMass;
protected: 
	void OnSelchangeCmdCodeName();
	void InitCodeNameCobx();
	void InitRoofHeight();
	void InitMassData();
	void InitDmDB4KDS2021();
	void GetInitDefaultValue(int nCodeType);
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      /////////

#endif
