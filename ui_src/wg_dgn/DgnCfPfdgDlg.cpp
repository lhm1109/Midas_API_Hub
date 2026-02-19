#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfPfdgDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\CompFunc.h"

#include "../wg_db/PostCtrl.h"
#include "../wg_db/DesignResult.h"
#include "../wg_db/FileDesignMgrBase_CFS.h"

// CDgnCfPfdgDlg 对话框

IMPLEMENT_DYNAMIC(CDgnCfPfdgDlg, CDlgChild)

CDgnCfPfdgDlg::CDgnCfPfdgDlg(CWnd* pParent /*=NULL*/)
	: CDlgChild(CDgnCfPfdgDlg::IDD, pParent)
{
}

CDgnCfPfdgDlg::~CDgnCfPfdgDlg()
{
}

void CDgnCfPfdgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DGN_PFDN_LOAD_LEVEL, m_nPerforLevel);
    DDX_Control(pDX, IDC_DGN_PFDN_DUL_LEVEL, m_nDucLevel);
    DDX_Text(pDX, IDC_DGN_PFDN_NP_COE, m_dNoPlaCoe);
    DDX_Text(pDX, IDC_DGN_PFDN_MIN_PL, m_dMinCoe);
    DDX_Control(pDX, IDC_DGN_PFDN_GRAVITY_COM, m_nGravityCom);
    DDX_Control(pDX, IDC_DGN_PFDN_HSEIS_COM, m_aHSeismicCom);
    DDX_Control(pDX, IDC_DGN_PFDN_VSEIS_COM, m_nVSeismicCom);
    DDX_Control(pDX, IDC_DGN_PFDN_FRAME_COM, m_FrameType);
    
}


BEGIN_MESSAGE_MAP(CDgnCfPfdgDlg, CDlgChild)
	ON_CBN_SELCHANGE(IDC_DGN_PFDN_LOAD_LEVEL,     OnSelectPfdl)
END_MESSAGE_MAP()


// CDgnCfPfdgDlg 消息处理程序


BOOL CDgnCfPfdgDlg::OnInitDialog()
{
    CDlgChild::OnInitDialog();
    m_pDoc = CDBDoc::GetDocPoint();
    BOOL bDefaut = FALSE;
    T_PFDN_D PfdnD;
	GetPfdnData(PfdnD);
   
	GetDlgItem(IDC_DGN_PFDN_FRAME_COM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STRUCTURE_TYPE_STC6)->ShowWindow(SW_HIDE);

    //LoadLevel Combox
    CString strLoadLevel = _LS(IDS_DGN_PERFORMANCE_NAME);
    CString strTemp;
    for(int i = 0;i < 4; i++)
    {
        strTemp.Format(_T("%s%d"),strLoadLevel,i+1);
        m_nPerforLevel.AddString(strTemp);
    }
    m_nPerforLevel.SetCurSel(PfdnD.nPerforLevel);
    //Ductility Level Combox
    m_nDucLevel.AddString(_LS(IDS_TB_PFDL_1));
    m_nDucLevel.AddString(_LS(IDS_TB_PFDL_2));
    m_nDucLevel.AddString(_LS(IDS_TB_PFDL_3));
    m_nDucLevel.SetCurSel(PfdnD.nDucLevel);
	
    //No-Plastic Coe && Plastic Min Coe 
    m_dNoPlaCoe = PfdnD.dNoPlaCoe;
    m_dMinCoe = PfdnD.dMinCoe;
    //Gravity Combox
	CArray<T_LCOM_K,T_LCOM_K> aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,aLcomK);
    T_LCOM_D Data;
    int nIndex = 0;
    int nExistGravity = 0;
    for(int i = 0;i < aLcomK.GetSize();i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL,aLcomK[i],Data))
        {
            nIndex = m_nGravityCom.AddString(Data.LoadCombName);
            m_nGravityCom.SetItemData(nIndex,aLcomK[i]);
            if(aLcomK[i] == PfdnD.nGravityCom)
                nExistGravity = nIndex;
        }
    }
     m_nGravityCom.SetCurSel(nExistGravity);
    //H Seis
    //V Seis
    CArray<T_SPLC_K, T_SPLC_K> aSplcK;
    CArray<T_THIS_K, T_THIS_K> aThisK;
    CArray<T_POLC_K, T_POLC_K> aPolcK;
    T_SPLC_D SplcD;
    T_THIS_D ThisD;
    T_POLC_D PolcD;
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
    m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisK);
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    T_LCOM_BASE LcomK;
    LcomK.AnalType = D_LOADCASE_SPECTRUM;
    for(int i = 0;i < aSplcK.GetSize();i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i],SplcD))
        {
			if(SplcD.nDirection == 1)
			{
				nIndex = m_aHSeismicCom.AddString(SplcD.LoadCaseName);
	            m_aHSeismicCom.SetItemData(nIndex,aSplcK[i] + 100000);
				LcomK.LoadCaseKey = aSplcK[i];
				if(ExistKey(PfdnD.aHSeismicCom,LcomK))
					m_aHSeismicCom.SetCheck(nIndex);
			}
			else if(SplcD.nDirection == 2)
			{
				nIndex = m_nVSeismicCom.AddString(SplcD.LoadCaseName);
	            m_nVSeismicCom.SetItemData(nIndex,aSplcK[i] + 100000);
			}
        }
    }
    LcomK.AnalType = D_LOADCASE_HISTORY;
    for(int i = 0;i < aThisK.GetSize();i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetThis(aThisK[i],ThisD))
        {
			//
			nIndex = m_aHSeismicCom.AddString(_T("Time_") + ThisD.LoadCaseName);
			m_aHSeismicCom.SetItemData(nIndex, aThisK[i] + 200000);
			LcomK.LoadCaseKey = aThisK[i];
			if (ExistKey(PfdnD.aHSeismicCom, LcomK))
				m_aHSeismicCom.SetCheck(nIndex);
			//
			nIndex = m_nVSeismicCom.AddString(_T("Time_") + ThisD.LoadCaseName);
            m_nVSeismicCom.SetItemData(nIndex, aThisK[i] + 200000);
        }
    }
    LcomK.AnalType = D_LOADCASE_PUSHOVER;
    for(int i = 0;i < aPolcK.GetSize();i++)
    {
		//if(m_pDoc->m_pPostCtrl->ExistPushoverAnalysisResultFile(aPolcK[i]))
		//{
			if(m_pDoc->m_pAttrCtrl->GetPolc(aPolcK[i],PolcD))
			{
				nIndex = m_aHSeismicCom.AddString(_T("Push_") + PolcD.strName);
				m_aHSeismicCom.SetItemData(nIndex,aPolcK[i] + 300000);
				LcomK.LoadCaseKey = aSplcK[i];
				if(ExistKey(PfdnD.aHSeismicCom,LcomK))
					m_aHSeismicCom.SetCheck(nIndex);
			}
		//}
    }
	LcomK.AnalType = D_LCOMTYPE_GENERAL;
	for(int i = 0;i < aLcomK.GetSize();i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL,aLcomK[i],Data) && Data.LoadCombType == 3)//SRSS
		{
			nIndex = m_aHSeismicCom.AddString(Data.LoadCombName);
			m_aHSeismicCom.SetItemData(nIndex,aLcomK[i]+400000);
			LcomK.LoadCaseKey = aLcomK[i];
			if(ExistKey(PfdnD.aHSeismicCom,LcomK))
				m_aHSeismicCom.SetCheck(nIndex);
		}
	}
	nIndex = m_nVSeismicCom.AddString(_LS(IDS_DGN__NONE));
	m_nVSeismicCom.SetItemData(nIndex,-1);
    if(bDefaut)
        m_aHSeismicCom.SetCheck(0);
	else
	{
		for(int i = 0;i < m_nVSeismicCom.GetCount();i++)
		{
			UINT nSerisID = m_nVSeismicCom.GetItemData(i);
			if(PfdnD.nVSeismicCom.AnalType == D_LOADCASE_PUSHOVER && nSerisID > 300000 && nSerisID - 300000 == PfdnD.nVSeismicCom.LoadCaseKey)
			{
				nIndex = i;
				break;
			}
			if(PfdnD.nVSeismicCom.AnalType == D_LOADCASE_HISTORY && nSerisID > 200000 && nSerisID - 200000 == PfdnD.nVSeismicCom.LoadCaseKey)
			{
				nIndex = i;
				break;
			}
			if(PfdnD.nVSeismicCom.AnalType == D_LOADCASE_SPECTRUM && nSerisID > 100000 && nSerisID - 100000 == PfdnD.nVSeismicCom.LoadCaseKey)
			{
				nIndex = i;
				break;
			}
		}
	}
    m_nVSeismicCom.SetCurSel(nIndex);
    m_aHSeismicCom.SetCurSel(0);
    //UpdateData(FALSE);
    //Frame Type
    CString strFrameType[] = {_LS(IDS_DGN_PERFORMANCE_PARAM_FRAME),_LS(IDS_DGN_PERFORMANCE_PARAM_SUPPORT),
        _LS(IDS_DGN_PERFORMANCE_PARAM_FRAME_CENTER_SUPPORT),_LS(IDS_DGN_PERFORMANCE_PARAM_FRAME_ECCENTRIC_SUPPORT)};
    int nExistType = 0;
    for(int i = 0;i < 4;i++)
    {
        m_FrameType.SetItemData(m_FrameType.AddString(strFrameType[i]),i);
        if(PfdnD.nStructureType == i)
            nExistType = i;
    }
    m_FrameType.SetCurSel(nExistType);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDgnCfPfdgDlg::OnOK() 
{
    UpdateData(TRUE);
    T_PFDN_D PfdnD;
    PfdnD.Initialize();
    PfdnD.nPerforLevel = m_nPerforLevel.GetCurSel();
    PfdnD.nDucLevel = m_nDucLevel.GetCurSel();
    PfdnD.dNoPlaCoe = m_dNoPlaCoe;
    PfdnD.dMinCoe   = m_dMinCoe;
	PfdnD.nStructureType = m_FrameType.GetItemData(m_FrameType.GetCurSel());
    int nGravityCom = m_nGravityCom.GetCurSel();
    if(nGravityCom == -1)
    {
        GSaveHistoryNF(_LS(IDS_DGN_PFDN_ERROR_GRAVITY_COM));
        return;
    }
    if(m_dNoPlaCoe < 0.0)
    {
        AfxMessageBox(_LS(IDS_DGN_PFDN_ERROR_NP));
        return;
    }
    if(m_dMinCoe < 0.0)
    {
        AfxMessageBox(_LS(IDS_DGN_PFDN_ERROR_MC));
        return;
    }
    if((PfdnD.nPerforLevel == 6 && (PfdnD.nDucLevel == 2 || PfdnD.nDucLevel == 3 || PfdnD.nDucLevel == 4)) ||
        (PfdnD.nPerforLevel == 5 && (PfdnD.nDucLevel == 4 || PfdnD.nDucLevel == 5)) || (PfdnD.nPerforLevel == 4 && PfdnD.nDucLevel == 5))
    {
        GSaveHistoryNF(_LS(IDS_DGN_PFDN_WARN_DL));
    }
    PfdnD.nGravityCom = m_nGravityCom.GetItemData(m_nGravityCom.GetCurSel());
    int nLoadCase = 0;
    T_LCOM_BASE LoadCom;
	LoadCom.Initialize();
	LoadCom.Factor = 1.0;
    for(int i = 0;i < m_aHSeismicCom.GetCount();i++)
    {
        if(m_aHSeismicCom.GetCheck(i))
        {
            nLoadCase =  m_aHSeismicCom.GetItemData(i);
            if(nLoadCase < 200000)
            {
                nLoadCase -= 100000;
                LoadCom.AnalType = D_LOADCASE_SPECTRUM;
            }
            else if(nLoadCase < 300000)
            {
                nLoadCase -= 200000;
                LoadCom.AnalType = D_LOADCASE_HISTORY;
            }
            else if(nLoadCase < 400000)
            {
                nLoadCase -= 300000;
                LoadCom.AnalType = D_LOADCASE_PUSHOVER;
            }
			else if(nLoadCase < 500000)
			{
				nLoadCase -= 400000;
				LoadCom.AnalType = D_LCOMTYPE_GENERAL;
			}
            else
            {
                ASSERT(FALSE);
                continue;
            }
            LoadCom.LoadCaseKey = nLoadCase;
            PfdnD.aHSeismicCom.Add(LoadCom);
        }
    }
    if(PfdnD.aHSeismicCom.IsEmpty())
    {
        AfxMessageBox(_LS(IDS_DGN_PFDN_ERROR_HOR_SEIS_COM));
        return;
    }
    nLoadCase = m_nVSeismicCom.GetItemData(m_nVSeismicCom.GetCurSel());
    if(nLoadCase < 0)
    {
        nLoadCase = 0;
        LoadCom.Initialize();
    }
    else if(nLoadCase < 200000)
    {
        nLoadCase -= 100000;
        LoadCom.AnalType = D_LOADCASE_SPECTRUM;
    }
    else if(nLoadCase < 300000)
    {
        nLoadCase -= 200000;
        LoadCom.AnalType = D_LOADCASE_HISTORY;
    }
    else
    {
        ASSERT(FALSE);
    }

    LoadCom.LoadCaseKey = nLoadCase;
    PfdnD.nVSeismicCom = LoadCom;

	CCFSDesignMgr* pCfsDesignMgr = CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult()->GetCfsDgnMgr();
	BOOL bPfdnDgnEnable = pCfsDesignMgr->GetPfdnDgnMgr()->IsDesignResultEnable();
	if (bPfdnDgnEnable)
	{
		if (AfxMessageBox(_LS(IDS_DB_REMOVEDESIGN), MB_YESNO) != IDYES)
			return;
        pCfsDesignMgr->GetPfdnDgnMgr()->RemoveDesignResultFile();
	}

    BOOL bCheck = m_pDoc->m_pDataCtrl->AddPfdn(PfdnD);
    if(bCheck)  CDlgChild::OnOK();
}

BOOL CDgnCfPfdgDlg::ExistKey(const CArray<T_LCOM_BASE>& aSeis,T_LCOM_BASE nKey)
{
    for(int i = 0;i < aSeis.GetSize();i++)
    {
        if(nKey.AnalType == aSeis[i].AnalType && nKey.LoadCaseKey == aSeis[i].LoadCaseKey)
            return TRUE;
    }
    return FALSE;
}

BOOL CDgnCfPfdgDlg::GetPfdnData(T_PFDN_D& PfdnD)
{
	if(m_pDoc->m_pAttrCtrl->GetPfdn(PfdnD))
	{
		return TRUE;
	}
	else
	{
		PfdnD.Initialize();
		T_LCOM_D Data;
		T_LCOM_BASE LoadCom;
		LoadCom.Factor = 1.0;
		CArray<T_LCOM_K,T_LCOM_K> aLcomK;
		CArray<T_LCOM_K,T_LCOM_K> aStlLcomK;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,aLcomK);
		CArray<T_SPLC_K, T_SPLC_K> aSplcK;
		CArray<T_THIS_K, T_THIS_K> aThisK;
		CArray<T_POLC_K, T_POLC_K> aPolcK;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
		m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisK);
		m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
		if(aLcomK.IsEmpty() || (aSplcK.IsEmpty() && aThisK.IsEmpty() &&aPolcK.IsEmpty()))
			return FALSE;
		PfdnD.nGravityCom = aLcomK[0];
		if(!aSplcK.IsEmpty())
		{
			LoadCom.AnalType = D_LOADCASE_SPECTRUM;
			LoadCom.LoadCaseKey = aSplcK[0];
			PfdnD.aHSeismicCom.Add(LoadCom);
		}
		else if(!aThisK.IsEmpty())
		{
			LoadCom.AnalType = D_LOADCASE_HISTORY;
			LoadCom.LoadCaseKey = aThisK[0];
			PfdnD.aHSeismicCom.Add(LoadCom);
		}
		else if(!aPolcK.IsEmpty())
		{
			LoadCom.AnalType = D_LOADCASE_PUSHOVER;
			LoadCom.LoadCaseKey = aPolcK[0];
			PfdnD.aHSeismicCom.Add(LoadCom);
		}
		else
		{
            int i = 0;
			for(i = 0;i < aStlLcomK.GetSize();i++)
			{
				if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL,aStlLcomK[i],Data) && Data.LoadCombType == 3)//SRSS
				{
					LoadCom.AnalType = D_LCOMTYPE_GENERAL;
					LoadCom.LoadCaseKey = aStlLcomK[i];
					PfdnD.aHSeismicCom.Add(LoadCom);
					break;
				}
			}
			if(i == aStlLcomK.GetSize())
				return FALSE;
		}
		ASSERT(PfdnD.aHSeismicCom.GetSize() == 1);
		m_pDoc->m_pDataCtrl->AddPfdn(PfdnD);
		return TRUE;
	}
}

void CDgnCfPfdgDlg::OnSelectPfdl()
{
	UpdateData(TRUE);
    int nPfdl_cfs[] = { 2,2,1,0 };
	int nIndex = m_nPerforLevel.GetCurSel();
    m_nDucLevel.SetCurSel(nPfdl_cfs[nIndex]);
    double dMinPfmc_CFS[] = { 1.1,0.7,0.35,0.28 };
	m_dMinCoe = dMinPfmc_CFS[nIndex];

	UpdateData(FALSE);
}