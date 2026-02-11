// CMLoadCaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadFireLoadAnalysDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadFireLoadAnalysDlg dialog


CCMLoadFireLoadAnalysDlg::CCMLoadFireLoadAnalysDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadFireLoadAnalysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadFireLoadAnalysDlg)
	m_bChk = TRUE;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMLoadFireLoadAnalysDlg::~CCMLoadFireLoadAnalysDlg()
{
}

void CCMLoadFireLoadAnalysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadFireLoadAnalysDlg)
	DDX_Check(pDX, IDC_CMD_LOAD_FIRE_CHECK,     m_bChk);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCMLoadFireLoadAnalysDlg, CDialogMove)
#undef CDialog
	//{{AFX_MSG_MAP(CCMLoadFireLoadAnalysDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnDlgOK)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCLE, OnDlgClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadFireLoadAnalysDlg message handlers

BOOL CCMLoadFireLoadAnalysDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	m_bChk = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadFireLoadAnalysDlg::OnDlgOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	T_STLD_K StldK = m_pDoc->m_pAttrCtrl->GetStldKey(FIRE_LOAD_CASE_NAME);
	if (m_bChk == FALSE)
	{
		CString csErrMsg = _LS(IDS_CMD_ERROR_MODS_FIRE_ANALYS_NO_REDUCTION_CHK);
		AfxMessageBox(csErrMsg);
		return;
		
	}
	else if (StldK < 1)
	{
		CString csErrMsg = _LS(IDS_CMD_ERROR_MODS_GB51249_2017_NO_TEMPERATURE_CASE);
		AfxMessageBox(csErrMsg);
		return;
	}
	else
	{
		T_ELEM_K_LIST lstElem;
		m_pDoc->m_pAttrCtrl->GetElemFire(lstElem);

		if (lstElem.GetCount() > 0)
			AddElemStiffCoef();
		UpdateData(FALSE);
		CDialogMove::OnOK();
	}	
}

void CCMLoadFireLoadAnalysDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCMLoadFireLoadAnalysDlg::AddNewSection()
{
	CMap<T_SECT_K, T_SECT_K, T_KEY_LIST_EX, T_KEY_LIST_EX&> mapElemPSect;
	CMap<T_SECT_K, T_SECT_K, T_KEY_LIST_EX, T_KEY_LIST_EX&> mapElemPFireCase;
	
	T_ELEM_K_LIST lstElem;
	m_pDoc->m_pAttrCtrl->GetElemFire(lstElem);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K&> mapFire;
	mapFire.InitHashTable(lstElem.GetCount()+1);
	for (int i=0; i<lstElem.GetCount(); i++)
	{
		mapFire[lstElem[i]] = 1;
	}
	//
	T_ELEM_K_LIST lstAll;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(lstAll);
	T_SECT_D SectD;
	T_ELEM_D ElemD;
	for (int j=0; j<lstAll.GetCount(); j++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(lstAll[j], ElemD);
		if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			continue;
		mapElemPSect[ElemD.elpro].Add(lstAll[j]);
		if (mapFire.PLookup(lstAll[j]) != nullptr)
			mapElemPFireCase[ElemD.elpro].Add(lstAll[j]);
	}
	POSITION pos = mapElemPFireCase.GetStartPosition();
	T_KEY nSectKey;
	T_KEY_LIST_EX lstFireElemK;

	while (pos)
	{
		mapElemPFireCase.GetNextAssoc(pos, nSectKey, lstFireElemK);
		T_KEY newSectKey = nSectKey;
		if(lstFireElemK.GetCount() != mapElemPSect[nSectKey].GetCount())
		{
			T_SECT_D newSectD;
			m_pDoc->m_pAttrCtrl->GetSect(nSectKey, newSectD);
			newSectKey = m_pDoc->m_pAttrCtrl->GetLastNumSect()+1;
			m_pDoc->m_pDataCtrl->AddSect(newSectKey, newSectD);
			for (int i=0; i<lstFireElemK.GetCount(); i++)
			{
				T_ELEM_D elemD;
				m_pDoc->m_pAttrCtrl->GetElem(lstFireElemK[i], elemD);
				elemD.elpro = newSectKey;
				m_pDoc->m_pDataCtrl->ChangeElem(lstFireElemK[i], elemD);
			}
		}
		//
		double dCoef = GetCoef(newSectKey, lstFireElemK);
		T_SECF_D SecfD;
		SecfD.Initialize();

		for (int i=0; i<6; i++)
			SecfD.dScaleFactor[i] = dCoef;
		SecfD.dScaleFactor[6] = 1.0;
		T_SECF_K SecfK;
		if(!m_pDoc->m_pAttrCtrl->MakeKeySecf(nSectKey, 0, SecfK)) { ASSERT(0); return;}
		SecfD.bBefore = TRUE;  
		SecfD.bAfter = TRUE; 
		if (!m_pDoc->m_pDataCtrl->AddSecf(SecfK, SecfD))  { ASSERT(0); return; }
		
	}

}

BOOL CCMLoadFireLoadAnalysDlg::AddElemStiffCoef()
{
	T_ELEM_K_LIST lstFireElem, lstSelElem, lstElem;
	m_pDoc->m_pAttrCtrl->GetElemFire(lstElem);
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(lstSelElem);

	CArray<T_ETMP_K, T_ETMP_K> arEtmpK;
	CArray<T_ETMP_D, T_ETMP_D&> arEtmpD;
	T_STLD_K FireStlK = m_pDoc->m_pAttrCtrl->GetStldKey(FIRE_LOAD_CASE_NAME);
	T_MHTP_D MhtpD;
	T_MHAT_D MhatD;
	T_ESSF_D EssfD;
	T_ELEM_D ElemD;

	if (lstSelElem.GetCount() > 0)
	{
		for (int i=0; i<lstSelElem.GetCount(); i++)
		{
			for (int j = 0; j < lstElem.GetCount(); j++)
			{
				if (lstElem[j] == lstSelElem[i])
				{
					lstFireElem.Add(lstSelElem[i]);
				}
				else
					continue;
			}
		}
	}
	else
	{
		lstFireElem.Copy(lstElem);
	}

	CArray<T_ESSF_D, T_ESSF_D&> rData;
	CArray<T_ESSF_K, T_ESSF_K> rKey;
	T_ESSF_K EssfK;
	for (int i = 0; i < lstFireElem.GetCount(); i++)
	{
		arEtmpK.RemoveAll();
		arEtmpD.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetEtmp(lstFireElem[i], arEtmpK, arEtmpD);
		int nElemT = 0.0;
		int nCount = arEtmpK.GetCount();
		for (int j = 0; j < nCount; j++)
		{
			if (FireStlK == arEtmpD[j].LoadCaseKey)
			{
				nElemT = arEtmpD[j].dblElemTemp;
				break;
			}
		}
		ElemD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetElem(lstFireElem[i], ElemD))
			return FALSE;
		MhatD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMhat(ElemD.elmat, MhatD))
			return FALSE;
		MhtpD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMhtp(MhatD.HighMatlTypeKey, MhtpD);
		if (MhtpD.nMatlType == 1)
		{
			nElemT = nElemT > 1000 ? 1000 : nElemT;
			nElemT = nElemT < 0 ? 0 : nElemT;
		}
		else
		{
			int nCount = MhtpD.aFuncData.GetCount();
			nElemT = nElemT > MhtpD.aFuncData[nCount - 1].nC0 ? MhtpD.aFuncData[nCount - 1].nC0 : nElemT;
			nElemT = nElemT < MhtpD.aFuncData[0].nC0 ? MhtpD.aFuncData[0].nC0 : nElemT;
		}
		double dCoef = m_pDoc->m_pAttrCtrl->GetElastCoef(nElemT, MhtpD);

		EssfD.Initialize();
		for (int i = 0; i < 6; i++)
			EssfD.dScaleFactor[i] = dCoef;
		EssfD.dScaleFactor[6] = 1.0;
		EssfD.bBefore = TRUE;
		EssfD.bAfter = TRUE;
		EssfK.key.entity = lstFireElem[i];
		rData.Add(EssfD);
		rKey.Add(EssfK);
	}

	if (!m_pDoc->m_pDataCtrl->AddEssf(rKey,rData)) { ASSERT(0); return FALSE; }
	return TRUE;
}

double CCMLoadFireLoadAnalysDlg::GetCoef(T_SECT_K nKey, T_KEY_LIST &lstFireElem)
{
	double dCoefMin = 1000;
	T_MHTP_D MhtpD;
	T_MHAT_D MhatD;	
	CArray<T_ETMP_K, T_ETMP_K> arEtmpK;
	CArray<T_ETMP_D, T_ETMP_D&> arEtmpD;
	T_STLD_K FireStlK = m_pDoc->m_pAttrCtrl->GetStldKey(FIRE_LOAD_CASE_NAME);
	T_ELEM_D ElemD;

	for (int i=0; i<lstFireElem.GetCount(); i++)
	{
		arEtmpK.RemoveAll();
		arEtmpD.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetEtmp(lstFireElem[i], arEtmpK, arEtmpD);
		int nElemT = 0.0;
		int nCount = arEtmpK.GetCount();
		for (int j=0; j<nCount; j++)
		{
			if (FireStlK == arEtmpD[j].LoadCaseKey)
			{
				nElemT = arEtmpD[j].dblElemTemp;
				break;
			}
		}
		ElemD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(lstFireElem[i], ElemD))
			return FALSE;
		MhatD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetMhat(ElemD.elmat, MhatD))
			return FALSE;	
		MhtpD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMhtp(MhatD.HighMatlTypeKey, MhtpD);
		if (MhtpD.nMatlType == 1)
		{
			nElemT = nElemT > 1000 ? 1000 : nElemT;
			nElemT = nElemT < 0 ? 0 : nElemT;			
		}
		else
		{
			int nCount = MhtpD.aFuncData.GetCount();
			nElemT = nElemT > MhtpD.aFuncData[nCount-1].nC0 ? MhtpD.aFuncData[nCount-1].nC0 : nElemT;
			nElemT = nElemT < MhtpD.aFuncData[0].nC0 ? MhtpD.aFuncData[0].nC0 : nElemT;		
		}
		double dCoefTemp = m_pDoc->m_pAttrCtrl->GetElastCoef(nElemT, MhtpD);
		dCoefMin = dCoefTemp < dCoefMin ? dCoefTemp : dCoefMin;
	}
	return dCoefMin;
}
