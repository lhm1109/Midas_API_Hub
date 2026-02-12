// CMThrdNllk.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdNllk.h"
#include "CMThMainDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\Qsort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define THIS_MODE_METHOD  1
#define THIS_MSPT_METHOD  2
#define THIS_DIRE_METHOD  3
/////////////////////////////////////////////////////////////////////////////
// CCMThrdNllk dialog


CCMThrdNllk::CCMThrdNllk(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdNllk::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdNllk)
	m_sName = _T("");
	m_nType = 0;
	//}}AFX_DATA_INIT
}

void CCMThrdNllk::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdNllk)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THNLLK_CBX, m_cbxNllk);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cbxComp);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_sName);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_DEFORM, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdNllk, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdNllk)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_DEFORM, OnGpsBarThtypeRadioPress)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THCOMP, OnSelchangeGpsBarThcomp)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_INODEFORC, OnGpsBarThtypeRadioPress)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_JNODEFORC, OnGpsBarThtypeRadioPress)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THLOADCASE, OnSelchangeGpsBarThloadcase)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdNllk message handlers

BOOL CCMThrdNllk::OnExecute()
{
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

void CCMThrdNllk::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 7);
}

void CCMThrdNllk::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 7);
	}
}

void CCMThrdNllk::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdNllk::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nComponent = 1;
	m_pDlgData.bAllMode = TRUE;

	m_bModify = FALSE;
	strOldName = _T("");
}

BOOL CCMThrdNllk::Dlg2Data()
{
	int nSelIdx;
	UpdateData(TRUE);

	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 8;
	m_pDlgData.Name = m_sName;
	//sscanf(m_sNllk.operator LPCTSTR(), _T("%d"), &m_pDlgData.nEntity);
	m_pDlgData.nSubType  = m_nType+1;
	nSelIdx=m_cbxNllk.GetCurSel();

	if(nSelIdx!=CB_ERR && nSelIdx<m_arNllk.GetSize())
	{
		m_pDlgData.nEntity = m_cbxNllk.GetItemData(nSelIdx);
	}
	nSelIdx=m_cbxComp.GetCurSel();
	m_pDlgData.nComponent= nSelIdx+1;

	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis     = nLKey; 
	return TRUE;
}

void CCMThrdNllk::Data2Dlg()
{
	m_sName = m_pDlgData.Name;
	m_nType = m_pDlgData.nSubType - 1;
	ChangeCompCbx(m_nType);
	if(m_bModify) m_cbxComp.SetCurSel(m_pDlgData.nComponent-1);

	int nSize = m_cbxNllk.GetCount();
	if (nSize > 0)
		m_cbxNllk.SetCurSel(0);
	
	for (int i=0; i<nSize; i++)
	{
		if (m_cbxNllk.GetItemData(i) == m_pDlgData.nEntity)
		{
			m_cbxNllk.SetCurSel(i);
			break;
		}
	}
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY,m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdNllk::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_bModify=FALSE;
	m_nComboType = -1;
	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY ); 

	if (!m_bModify)
		DataInit();

	Data2Dlg();
	MakeNllkKeyList();  

	//m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY ); 
	UpdateModeNumCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//KYE-HONG-20020520
//Nllk Key List를 만든후 이를 ComboBox에 반영 
void  CCMThrdNllk::MakeNllkKeyList(void)
{
	CDBDoc *pDoc=CDBDoc::GetDocPoint();

	pDoc->m_pAttrCtrl->GetNlnkKeyList(m_arNllk);
	if(!m_arNllk.GetSize()) return;

	CString   sNllk;
	T_NLNK_D  Data;
	m_cbxNllk.ResetContent();
	for (int i=0; i<m_arNllk.GetSize(); i++)
	{
		if(pDoc->m_pAttrCtrl->GetNlnk(m_arNllk[i],Data))
		{
			sNllk.Format(_T("%d (n1:%d,n2:%d)"),m_arNllk[i],Data.Node1,Data.Node2);
			m_cbxNllk.AddString(sNllk);
			m_cbxNllk.SetItemData(i, m_arNllk[i]);
		}
	}
	
	if (m_cbxNllk.GetCount() > 0)
		m_cbxNllk.SetCurSel(0);
}

void  CCMThrdNllk::ChangeCompCbx(int nSubType)
{
	switch(nSubType)
	{
	case 0:
		{
			if (m_nComboType == 0)
				return;
			m_nComboType = 0;
			m_cbxComp.ResetContent(); 
			/**
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_AXIAL));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_TRAN_Y));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_TRAN_Z));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_TORSIONAL));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_ROTATION_Y));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_ROTATION_Z));
			**/
			m_cbxComp.AddString(_LSX(Dx));
			m_cbxComp.AddString(_LSX(Dy));
			m_cbxComp.AddString(_LSX(Dz));
			m_cbxComp.AddString(_LSX(Rx));
			m_cbxComp.AddString(_LSX(Ry));
			m_cbxComp.AddString(_LSX(Rz));
		}
		break;
	case 1:
	case 2:
		{
			if (m_nComboType == 1)
				return;
			m_nComboType = 1;
			m_cbxComp.ResetContent(); 
			/**
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_AXIAL_F));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_SHEAR_Y));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_SHEAR_Z));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_TORSION));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_MOMENT_Y));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_NLLINK_MOMENT_Z));
			**/
			m_cbxComp.AddString(_LSX(Fx));
			m_cbxComp.AddString(_LSX(Fy));
			m_cbxComp.AddString(_LSX(Fz));
			m_cbxComp.AddString(_LSX(Mx));
			m_cbxComp.AddString(_LSX(My));
			m_cbxComp.AddString(_LSX(Mz));
		}
		break;
	}
	if(m_bModify) m_cbxComp.SetCurSel(m_pDlgData.nComponent-1); 
	else          m_cbxComp.SetCurSel(0); 
}

void CCMThrdNllk::OnGpsBarThtypeRadioPress() 
{
	UpdateData(TRUE);
	ChangeCompCbx(m_nType); 
}

void CCMThrdNllk::OnSelchangeGpsBarThcomp() 
{
}

void CCMThrdNllk::OnSelchangeGpsBarThloadcase() 
{
	UpdateModeNumCtrl();
}


void CCMThrdNllk::UpdateModeNumCtrl(void)
{
	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		EnableModeNumCtrl(FALSE);
		return;
	}
	switch(GetThisLdcType(nLKey))
	{
	case 1: EnableModeNumCtrl(TRUE);    break;  // Mode Super
	case 2: EnableModeNumCtrl(FALSE);   break;  // Direct Intergration
	case 3: EnableModeNumCtrl(FALSE);   break;  // Direct Intergration
	}
}

// 1:Mode Superposition  2:Direct Intergration
int  CCMThrdNllk::GetThisLdcType(UINT ThisK)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();

	int nRtnVal;
	T_THIS_D ThisD;
	pDoc->m_pAttrCtrl->GetThis(ThisK,ThisD);

	// Mode Superposition
	if(ThisD.nAnalMethod==1)
	{
		T_THMS_K ThmsK;
		T_THMS_D ThmsD;
		BOOL bDirect = FALSE;
		POSITION posThms=pDoc->m_pAttrCtrl->GetStartThms();
		while(posThms)
		{
			pDoc->m_pAttrCtrl->GetNextThms(posThms, ThmsK, ThmsD);
			if(ThmsD.ThisKey == ThisK)
			{
				bDirect=TRUE;
				break;
			}
		}

		if (bDirect)
			nRtnVal = THIS_MSPT_METHOD;
		else
			nRtnVal = THIS_MODE_METHOD;
	}
	// Direct Intergration
	else if(ThisD.nAnalMethod==2)
	{
		nRtnVal = THIS_DIRE_METHOD;
	}
	else if(ThisD.nAnalMethod==3)
	{
		nRtnVal = THIS_DIRE_METHOD;  
	}
	else
		ASSERT(0);
	return nRtnVal;
}

void  CCMThrdNllk::EnableModeNumCtrl(BOOL bEnable)
{
	//GetDlgItem(IDC_CMD_BAR_THMODNUM_ALL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_CMD_BAR_THMODNUM_ONE)->EnableWindow(bEnable);
	//GetDlgItem(IDC_CMD_BAR_THMODNUM)->EnableWindow(bEnable);
}

BOOL CCMThrdNllk::ExternalInit(UINT key)
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
