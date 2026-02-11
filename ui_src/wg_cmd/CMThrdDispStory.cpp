// CMThrdDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdDispStory.h"
#include "CMThMainDlg.h"

#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THIS_MODE_METHOD  1
#define THIS_MSPT_METHOD  2
#define THIS_DIRE_METHOD  3
/////////////////////////////////////////////////////////////////////////////
// CCMThrdDispStory dialog


CCMThrdDispStory::CCMThrdDispStory(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdDispStory::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdDispStory)
	m_Name = _T("");
	m_Type = 0;
	//}}AFX_DATA_INIT
}


void CCMThrdDispStory::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdDispStory)
	DDX_Control(pDX, IDC_CMD_COBX_THTYPE_REF, m_RefTypeCb);
	DDX_Control(pDX, IDC_CMD_BAR_LOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_CompCb);
	DDX_Control(pDX, IDC_CMD_COBX_STORY, m_cobxStor);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_Name);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_DISP, m_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdDispStory, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdDispStory)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_LOADCASE, OnSelchangeGpsBarThloadcase)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_ACCEL, OnGpsBarThtypeofResult)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_DISP, OnGpsBarThtypeofResult)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_VEL, OnGpsBarThtypeofResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdDispStory message handlers

BOOL CCMThrdDispStory::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam==13))
		pMsg->wParam = 9;
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

BOOL CCMThrdDispStory::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifyThrd(strOldName, m_pDlgData))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddThrd(m_pDlgData))
			return FALSE;
	}
	return TRUE;
}

void CCMThrdDispStory::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 10);
}

void CCMThrdDispStory::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 10);
	}
}

void CCMThrdDispStory::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

BOOL CCMThrdDispStory::OnInitDialog()
{
	CCMChildBarBase::OnInitDialog();
	
	m_bModify = FALSE;  
	if (!m_bModify)
		DataInit();

	CString sComp[]={ _T("DX"), _T("DY") };
	for(int i=0; i<2; i++)m_CompCb.AddString(sComp[i]);

	MakeRefTypeCmb(TRUE);
	
	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY); 
	InitCobxStor();
	
	Data2Dlg();

	return FALSE;
}

void CCMThrdDispStory::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nComponent = 1;
	m_pDlgData.nSubType = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdDispStory::Data2Dlg()
{
	m_Name = m_pDlgData.Name;
	if (m_pDlgData.nEntity != 0)
		CDlgUtil::CobxSetCurSelItemData(m_cobxStor, m_pDlgData.nEntity);
	if (m_cobxStor.GetCurSel() == -1 && m_cobxStor.GetCount() > 0)
		m_cobxStor.SetCurSel(0);

	m_Type = m_pDlgData.nSubType - 1;

	//m_RefTypeCb.SetCurSel(m_pDlgData.nRefPoint-1);
	CDlgUtil::CobxSetCurSelItemData(m_RefTypeCb, m_pDlgData.nRefPoint);
	m_CompCb.SetCurSel(m_pDlgData.nComponent-1);
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	UpdateRefNodeCtrl();

	UpdateData(FALSE);
}

BOOL CCMThrdDispStory::Dlg2Data()
{
	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 12;
	m_pDlgData.Name = m_Name;
	CDlgUtil::CobxGetItemDataByCurSel(m_cobxStor, (DWORD&)m_pDlgData.nEntity);
	m_pDlgData.nSubType    = m_Type + 1;
	//m_pDlgData.nRefPoint   = m_RefTypeCb.GetCurSel()+1;
	CDlgUtil::CobxGetItemDataByCurSel(m_RefTypeCb, (DWORD&)m_pDlgData.nRefPoint);
 
	m_pDlgData.nComponent  = m_CompCb.GetCurSel() + 1;

	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE; 
	}
	m_pDlgData.KeyThis     = nLKey; 
	return TRUE;
}

void CCMThrdDispStory::OnSelchangeGpsBarThloadcase() 
{
	UpdateData();
	UpdateRefNodeCtrl();
}

void CCMThrdDispStory::OnGpsBarThtypeofResult() 
{
	//UpdateRefPtCbx();
	UpdateData(TRUE);

	if (m_Type == 2)
		//m_RefTypeCb.SetCurSel(1);
		CDlgUtil::CobxSetCurSelItemData(m_RefTypeCb, 2);
	//else m_RefTypeCb.SetCurSel(0);
	else 
	{
		int nRefType;
		UINT nLType,nLKey,nMnMxType;
		if(m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType) &&
			IsMutiSupportThis(nLKey)) nRefType = 2;
		else nRefType = 1;

		CDlgUtil::CobxSetCurSelItemData(m_RefTypeCb, nRefType);
	}

	UpdateData(FALSE);
}

void CCMThrdDispStory::UpdateRefNodeCtrl(void)
{
	UINT nLType,nLKey,nMnMxType;
	
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		//GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(TRUE);
		MakeRefTypeCmb(TRUE);
	
		return; 
	}
	
	if(IsMutiSupportThis(nLKey)) 
	{
		int nType=0;
		CDlgUtil::CobxGetItemDataByCurSel(m_RefTypeCb, (DWORD&)nType);
		//if(m_RefTypeCb.GetCurSel()==0)
		if (nType == 1)
		{  
			//m_RefTypeCb.SetCurSel(1); 
			//UpdateData(FALSE);  
			CDlgUtil::CobxSetCurSelItemData(m_RefTypeCb, 2);
		}
		//GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(FALSE);
		MakeRefTypeCmb(FALSE);
	}
	else 
	{
		//GetDlgItem(IDC_CMD_BAR_THTYPE_REFGROUND)->EnableWindow(TRUE);
		MakeRefTypeCmb(TRUE);
	}
}

BOOL CCMThrdDispStory::IsMutiSupportThis(UINT nLKey)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_THMS_K ThmsK;
	T_THMS_D ThmsD;
	POSITION nPos = pDoc->m_pAttrCtrl->GetStartThms();
	while(nPos)
	{
		pDoc->m_pAttrCtrl->GetNextThms(nPos,ThmsK,ThmsD);
		if(ThmsD.ThisKey == nLKey)
			return TRUE;
	}
	return FALSE; 
}

BOOL CCMThrdDispStory::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THRD_D ThrdD;
	pDoc->m_pAttrCtrl->GetThrd(key, ThrdD);
	m_pDlgData = ThrdD;
	strOldName = ThrdD.Name;
	m_bModify = TRUE;
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	Data2Dlg();
	return TRUE;
}

void CCMThrdDispStory::InitCobxStor()
{
	m_cobxStor.ResetContent();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_STOR_K, T_STOR_K> aStorK;
	T_STOR_D StorD;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	for (int i = 0; i < aStorK.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
		CDlgUtil::CobxAddItem(m_cobxStor, StorD.StoryName, aStorK[i]);
	}
}

void CCMThrdDispStory::MakeRefTypeCmb(BOOL bAddRelative)
{
	int nType;
	if (m_RefTypeCb.GetCount() == 0) nType = 1;
	else CDlgUtil::CobxGetItemDataByCurSel(m_RefTypeCb, (DWORD&)nType);

	//CString sRefType[]={_T("Relative"),_T("Absolute"),_T("Story Drift")};
	CString sRefType[] = {
		_LS(IDS_CMD_THRD_DISP_RELATIVE),
		_LS(IDS_CMD_THRD_DISP_ABSOLUTE),
		_LS(IDS_CMD_THRD_DISP_Inter_Story),
	};

	m_RefTypeCb.ResetContent();
	for(int j=0;j<3;j++)
	{
		if (j == 0 && bAddRelative == FALSE) continue;
		CDlgUtil::CobxAddItem(m_RefTypeCb, sRefType[j], j+1);
	}
	if (bAddRelative == FALSE && nType == 1) nType = 2;
	CDlgUtil::CobxSetCurSelItemData(m_RefTypeCb, nType);
}