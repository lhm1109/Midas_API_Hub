#if !defined(__CMWINDITEMDLG_H__)
#define __CMWINDITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

struct T_CMD_WIND_STORY_D
{
	double dLx;
	double dLy;
	double dMinLx;
	double dMinLy;
	double dMaxUp4Dm; // Dm : Diameter of 2H/3 Level
	double dMaxDn4Dm;
	double dMaxUp4DB; // DB : Diameter of Bottom
	double dMaxDn4DB;

	void Init()
	{
		dLx = 0.0;
		dLy = 0.0;
		dMinLx = 0.0;
		dMinLy = 0.0;
		dMaxUp4Dm = 0.0;
		dMaxDn4Dm = 0.0;
		dMaxUp4DB = 0.0;
		dMaxDn4DB = 0.0;
	}
};

class CCMWindItemCodeMgr;
class CWindSeisAddLoadGrid;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMWindItemDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMWindItemDlg(CWnd* pParent = NULL);
	virtual ~CCMWindItemDlg();

	enum { IDD = IDD_ETC_WIND_ITEM_NEW };

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	void GetWindData(T_WIND_D& WindD);
	void SetParamData(T_WIND_K Key, T_WIND_D& Data, T_KEY KeyWAorWT = NULL);
	void AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ);
	void AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblX2,
	                       double dblY2, double dblRZ, double dblRZ2);

public:
	void AlignCtrl2Param(int nChildBottom);
	BOOL IsSaveFoldedInfo() { return m_bSaveFoldedInfo; }
	void UpdateGrid(BOOL bWindKBC2015, BOOL bAcross, BOOL bTorsinal);
	bool GetStoryData(T_CMD_WIND_STORY_D& rData);
	bool GetStoryMass(double& rdMassX, double& rdMassY, double& rdMassXY, double& rdTotal);

protected:
	void InitCodeNameCobx();
	BOOL EnableNationalAnnex();
	void Data2Dlg();
	BOOL Dlg2Data();
	void MakeItemEx();
	BOOL AddToDB();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	virtual void OnCancel();
	afx_msg void OnCmdBtnProfile();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnSelchangeCmdCodeName();
	afx_msg void OnSelchangeCmdNationalAnnex();
	afx_msg void OnCmdBtnImport();
	DECLARE_MESSAGE_MAP()

private:
	int GetMoveDistNA();

public:
	//     CBCGPStatic	  m_wndHolder;
	MEdit m_editScaleGX;
	MEdit m_editScaleGY;
	MEdit m_editScaleGR;
	CSelectLC m_cobxLoadCase;
	MEdit m_editDiscription;
	MComboBox m_cobxCodeName;
	MComboBox m_cmbNationalAnnex;

protected:
	BOOL m_bModify;
	T_KEY m_KeyWAorWT;
	CDBDoc* m_pDoc;
	T_WIND_K m_Key, m_OldKey;
	T_WIND_D m_Data;
	CArray<UINT, UINT> m_aMoveCtrlID;
	CArray<BOOL, BOOL> m_aCodeEnable;

	CCMWindItemCodeMgr* m_pCodeMgr;
	CWindSeisAddLoadGrid* m_pGridWnd;
	BOOL m_bSaveFoldedInfo;
};

#include "HeaderPost.h"

#endif // !defined(__CMWINDITEMDLG_H__)
