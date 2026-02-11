// CMJapanRailmpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMJapanRailmpactFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMJapanRailmpactFactorDlg dialog


CCMJapanRailmpactFactorDlg::CCMJapanRailmpactFactorDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMJapanRailmpactFactorDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMJapanRailmpactFactorDlg)
	m_nOption = 0;
	m_nTargetType = 0;
	//}}AFX_DATA_INIT
}


void CCMJapanRailmpactFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMJapanRailmpactFactorDlg)
	DDX_Radio(pDX, IDC_CMD_CONS_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_CMD_TARGET_RDO1, m_nTargetType);
	DDX_Control(pDX, IDC_WG_CMD_IMPACT_FACTOR_EDT,  m_dImpactFactor1);
	DDX_Control(pDX, IDC_WG_CMD_IMPACT_FACTOR2_EDT, m_dImpactFactor2);
	DDX_Control(pDX, IDC_CMD_MLTT_NODE_EDT,         m_edtLink);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMJapanRailmpactFactorDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMJapanRailmpactFactorDlg)
	ON_BN_CLICKED(IDC_CMD_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_ADD, OnTmConsAddDelRdo)
	ON_BN_CLICKED(IDC_CMD_CONS_OPT_DELETE, OnTmConsAddDelRdo)

	ON_BN_CLICKED(IDC_CMD_TARGET_RDO1, OnTmJimpType)
	ON_BN_CLICKED(IDC_CMD_TARGET_RDO2, OnTmJimpType)
	ON_BN_CLICKED(IDC_CMD_TARGET_RDO3, OnTmJimpType)
	ON_BN_CLICKED(IDC_CMD_TARGET_RDO4, OnTmJimpType)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMJapanRailmpactFactorDlg message handlers

void CCMJapanRailmpactFactorDlg::RemoveNodeSelect()
{
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);

	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);

	m_edtLink.SetWindowText(strBlank);
	m_edtLink.Disconnect();
}

void CCMJapanRailmpactFactorDlg::Data2Dlg(T_JIMP_D& data)
{
	m_nTargetType = data.nTgtType;
	m_dImpactFactor1.SetEditUnit(data.dImpactFactor);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_JIMP_D JimpDtemp;
	if (pDoc->m_pAttrCtrl->GetJimp(T_JIMP_K_MAX, JimpDtemp))
	{
		data.dImpactFactor = JimpDtemp.dImpactFactor;
	}

	m_dImpactFactor2.SetEditUnit(JimpDtemp.dImpactFactor);

	UpdateData(FALSE);
}

void CCMJapanRailmpactFactorDlg::Dlg2Data(T_JIMP_D& data)
{
	UpdateData();

	data.nTgtType = m_nTargetType;
	data.dImpactFactor = m_dImpactFactor1.GetEditValue();
}

void CCMJapanRailmpactFactorDlg::OnTmExecute()
{  
	UpdateData();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	I_GENModelBase * pIGM = I_GENModelBase::GetCurMySelfST();
	CArray<UINT, UINT> aSelKey;
	
	if (m_nTargetType == D_JIMP_ELEM)
	{
		pIGM->GetSelectedElemKeyList(aSelKey);
	}
	else if (m_nTargetType == D_JIMP_NODE)
	{
		pIGM->GetSelectedNodeKeyList(aSelKey);
	}
	else if (m_nTargetType == D_JIMP_ELNK || m_nTargetType == D_JIMP_GLNK)
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKey;
		CArray<T_ELNK_K, T_ELNK_K> aELinkK;
		CArray<T_NLNK_K, T_NLNK_K> aNLinkK;

		CString str;
		m_edtLink.GetWindowText(str);
		GetNodeList(str, aNodeKey);
		pDoc->m_pAttrCtrl2->GetLinkList(aNodeKey, aELinkK, aNLinkK);

		if (m_nTargetType == D_JIMP_ELNK)
			aSelKey.Copy(aELinkK);
		else if (m_nTargetType == D_JIMP_GLNK)
			aSelKey.Copy(aNLinkK);
		else
			ASSERT(0);
	}
	else
		ASSERT(0);
	
	T_JIMP_D data;
	CArray<T_JIMP_D, T_JIMP_D&> aData;

	data.Initialize();
	Dlg2Data(data);

	int nSize = aSelKey.GetSize();
	if (nSize < 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___IMPACT_FACTOR_NO_SELECTED));
		return;
	}

	if (m_nOption == 0)
	{
		//Add
		for (int i=0; i<nSize; i++)
		{
			data.nTgtKey = aSelKey[i];
			aData.Add(data);
		}

// 		data.Initialize();
// 		data.nFactorType = 1;
// 		data.dImpactFactor = m_dImpactFactor2.GetEditValue();
// 		aData.Add(data);

		if (pDoc->m_pDataCtrl->AddJimp(aData))
			pIGM->UnselectAllElem();

	}
	else
	{
		//Delete
		int nFactorType = 0;
		T_JIMP_K key;
		CArray<T_JIMP_K, T_JIMP_K> raKey;
		for (int i=0; i<nSize; i++)
		{
			key = pDoc->m_pAttrCtrl->GetJimpKey(m_nTargetType, aSelKey[i], nFactorType);
			raKey.Add(key);
		}

// 		nFactorType = 1;
// 		key = pDoc->m_pAttrCtrl->GetJimpKey(m_nTargetType, 0/*юс╫ц*/, nFactorType);
// 		raKey.Add(key);

		if (pDoc->m_pDataCtrl->DelJimp(raKey))
			pIGM->UnselectAllElem();
	}
}

void CCMJapanRailmpactFactorDlg::OnTmClose()
{
	CloseDlg();
}

BOOL CCMJapanRailmpactFactorDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	GetDlgItem(IDC_CMD_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);

	m_dImpactFactor1.SetUnitType(0);
	m_dImpactFactor2.SetUnitType(0); 
	m_dImpactFactor1.SetWindowText(_T("1.0"));
	m_dImpactFactor2.SetWindowText(_T("1.0"));

	OnTmJimpType();
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMJapanRailmpactFactorDlg::Execute()
{
	OnTmExecute();
}

BOOL CCMJapanRailmpactFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	T_JIMP_D data;
	if (!pDoc->m_pAttrCtrl->GetJimp(key, data))
		data.Initialize();

	Data2Dlg(data);

	return TRUE;
}

void CCMJapanRailmpactFactorDlg::OnTmConsAddDelRdo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 1)
	{
		UpdateData(FALSE);
	}
}

void CCMJapanRailmpactFactorDlg::OnTmJimpType()
{
	UpdateData(TRUE);

	BOOL bLink = (m_nTargetType == D_JIMP_ELNK || m_nTargetType == D_JIMP_GLNK) ? TRUE : FALSE;
	m_edtLink.EnableWindow(bLink);

	if (bLink)
	{
		m_edtLink.Connect(SC_ID_NODE, &m_edtLink);
	}
	else
	{

	}
}
