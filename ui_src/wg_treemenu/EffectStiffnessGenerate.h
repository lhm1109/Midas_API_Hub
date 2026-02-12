#pragma once

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

class CEffectStiffnessGenerate : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CEffectStiffnessGenerate(CWnd* pParent = NULL);
	enum { IDD = IDD_TM_STFF_GENA };

public:
	virtual BOOL ExternalInit(UINT key);

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnInitDialog();
	virtual void Execute();
	virtual void OnTmClose();
	DECLARE_MESSAGE_MAP()

public:
	BOOL Dlg2Data();

private:
	int TypeByDefaultSecfValue(T_SECF_K key);
	int TypeByDefaultSecfValue(T_SECT_K key);

	BOOL IsBeam(T_ELEM_K Key);
	BOOL IsColumn(T_ELEM_K Key);
	BOOL Save_Stiffness_BC();
	BOOL Save_Stiffness_WALL();

	BOOL CheckElemConcMatl(T_ELEM_K ElemK);
	void ChangeSecf(BOOL bBeam, BOOL bColumn, T_SECF_D &rData);
	void ChangeWssf(UINT ElemD, T_WSSF_D &rData);

private:
	CEditUnit m_edtBM;
	CEditUnit m_edtBS;
	CEditUnit m_edtCM;
	CEditUnit m_edtCS;
	CEditUnit m_edtCA;
	CEditUnit m_edtWM;
	CEditUnit m_edtWS;
	CEditUnit m_edtWA;

private:
	CDBDoc* m_pDoc;
};
