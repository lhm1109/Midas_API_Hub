#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemCodeMgr.h"

// *^^* 코드 추가시 여기수정
// [SeisCode] 추가시 수정
#include "SeisItemDlgNew.h"
#include "SeisItemATC301.h"
#include "SeisItemChina2001.h"
#include "SeisItemChina2010.h"
#include "SeisItemDPT2018.h"
#include "SeisItemEURO1996.h"
#include "SeisItemEURO2004.h"
#include "SeisItemIBC2000.h"
#include "SeisItemIBC2012.h"
#include "SeisItemIS1893_2016.h"
#include "SeisItemIS2002.h"
#include "SeisItemJapan2000.h"
#include "SeisItemKBC2005.h"
#include "SeisItemKBC2008.h"
#include "SeisItemKBC2015.h"
#include "SeisItemKDS2019.h"
#include "SeisItemKorea1992.h"
#include "SeisItemKorea2000.h"
#include "SeisItemNBC1995.h"
#include "SeisItemNSR2010.h"
#include "SeisItemNTC2008.h"
#include "SeisItemNTC2012.h"
#include "SeisItemP100_2013.h"
#include "SeisItemShanghai.h"
#include "SeisItemTaiwan1999.h"
#include "SeisItemTaiwan2002.h"
#include "SeisItemTaiwan2011.h"
#include "SeisItemTaiwan2022.h"
#include "SeisItemUBC1991.h"
#include "SeisItemUBC1997.h"
#include "SeisItemIS1893_2025.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "SeisItemUser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum
{
    D_SEIS_CODE_DLG_KS1992 = KS_E_1992-1,
    D_SEIS_CODE_DLG_UBC1991,
    D_SEIS_CODE_DLG_UBC1997,
    D_SEIS_CODE_DLG_ATC306,
    D_SEIS_CODE_DLG_KS2000,
    D_SEIS_CODE_DLG_JIS,
    D_SEIS_CODE_DLG_IBC2000,
    D_SEIS_CODE_DLG_EURO1996,
    D_SEIS_CODE_DLG_CH2002,
    D_SEIS_CODE_DLG_NBC1995,
    D_SEIS_CODE_DLG_IS2002,
    D_SEIS_CODE_DLG_TAIWAN86,
    D_SEIS_CODE_DLG_KBC2005,
    D_SEIS_CODE_DLG_CHSH2003,
    D_SEIS_CODE_DLG_TAIWAN02,
    D_SEIS_CODE_DLG_EURO2004,
    D_SEIS_CODE_DLG_KBC2009,
    D_SEIS_CODE_DLG_TAIWAN11,
    D_SEIS_CODE_DLG_NTC2008,
    D_SEIS_CODE_DLG_CH2010,
    D_SEIS_CODE_DLG_IBC2009,
    D_SEIS_CODE_DLG_IBC2012,
    D_SEIS_CODE_DLG_NTC2012,
    D_SEIS_CODE_DLG_NSR2010,
    D_SEIS_CODE_DLG_KBC2015,
    D_SEIS_CODE_DLG_P100_2013,
    D_SEIS_CODE_DLG_IS1893_2016,
    D_SEIS_CODE_DLG_NTC2018,
    D_SEIS_CODE_DLG_KDS2019,
    D_SEIS_CODE_DLG_DPT2018,
    D_SEIS_CODE_DLG_TAIWAN22,
	D_SEIS_CODE_DLG_KDS2018,
	D_SEIS_CODE_DLG_USERTYPE,
    D_SEIS_CODE_DLG_NSCP2024,
	D_SEIS_CODE_DLG_IS1893_2025,
};

CSeisItemCodeMgr::CSeisItemCodeMgr(CWnd* pParent)
{
    m_pParent = NULL;
    ASSERT(pParent);
    m_pParent = ( CSeisItemDlgNew* ) pParent;

    m_nCurCodeIndex = -1;
    m_nInitCodeIndex = 0;
    m_aCodeName.RemoveAll();
    m_aCodeDlg.RemoveAll();

    CreateCodeDlg();
}

CSeisItemCodeMgr::~CSeisItemCodeMgr()
{
    DeleteCodeDlg();
}

// [SeisCode] 추가시 수정
int  CSeisItemCodeMgr::GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable/*=NULL*/)
{
    CArray<int, int> aSeisSeq;
    aSeisSeq.RemoveAll();
    CDBLib::GetSLCodeSeqList(aSeisSeq);

    BOOL bOnlyEnable = TRUE;
#if defined(_CH) && defined(_MGEN)
    bOnlyEnable = FALSE;
#endif
    if ( aCodeEnable==NULL ) bOnlyEnable= TRUE;
    if ( aCodeEnable!=NULL ) aCodeEnable->RemoveAll();

    CString csErrMsg(_T(""));
    for ( int i = 0; i < aSeisSeq.GetSize(); i++ )
    {
        BOOL bInclude = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadSeismic, aSeisSeq[i] + 1, csErrMsg);
        if ( bInclude==FALSE&&bOnlyEnable ) continue;
        aCodeName.Add(m_aCodeName[aSeisSeq[i]]);
        aCodeIndex.Add(aSeisSeq[i] + 1);
        if ( aCodeEnable!=NULL ) aCodeEnable->Add(bInclude);
    }
    return aCodeName.GetSize();
}

void CSeisItemCodeMgr::ChangeNationalAnnex(int nCodeIndex, int nNAIndex)
{
    if ( nCodeIndex!=15 ) return;
    ( ( CSeisItemEURO2004 * ) m_aCodeDlg[nCodeIndex] )->SetNationalAnnex(nNAIndex);
}

int CSeisItemCodeMgr::GetCurCodeType()
{
    return m_nCurCodeIndex+1;
}

void CSeisItemCodeMgr::ChangeCodeDlg(int nCodeIndex)
{
    HWND hWnd = NULL;
    hWnd = m_pParent->GetSafeHwnd();
    if ( hWnd == NULL || !( ::IsWindow(hWnd) ) ) return;

    if ( m_nCurCodeIndex == nCodeIndex ) return;

    if ( nCodeIndex > m_aCodeDlg.GetSize() - 1 )
    {
        //nCodeIndex = nCodeIndex;
        return;
    }

    if ( m_nCurCodeIndex >= 0 && m_aCodeDlg.GetSize()-1 >= m_nCurCodeIndex )
    {
        m_aCodeDlg[m_nCurCodeIndex]->End();
    }

    CRect WRect;
    m_pParent->GetDlgItem(IDC_CMD_CHILD_FRAME)->GetWindowRect(&WRect);
    m_pParent->ScreenToClient(WRect);
    if ( m_aCodeDlg[nCodeIndex]->CreateInit(m_pParent, WRect.left, WRect.top) )
    {
        if ( nCodeIndex == D_SEIS_CODE_DLG_IBC2009 || nCodeIndex == D_SEIS_CODE_DLG_IBC2012 || nCodeIndex == D_SEIS_CODE_DLG_NSCP2024)
        {
            ( ( CSeisItemIBC2012 * ) m_aCodeDlg[nCodeIndex] )->SetCode(nCodeIndex+1);
        }
    }
    m_nCurCodeIndex = nCodeIndex;

    CRect rcChild;
    m_aCodeDlg[nCodeIndex]->GetWindowRect(rcChild);
    m_pParent->AlignCtrl2Param(rcChild.bottom);
}

// [SeisCode] 추가시 수정
void CSeisItemCodeMgr::Data2CodeDlg(T_SEIS_D& data, int nIndex, BOOL bModify)
{    
    ( ( CSeisItemKorea1992  * ) m_aCodeDlg[D_SEIS_CODE_DLG_KS1992     ] )->SetData2Dlg(data.KS1992, bModify);
    ( ( CSeisItemUBC1991    * ) m_aCodeDlg[D_SEIS_CODE_DLG_UBC1991    ] )->SetData2Dlg(data.UBC1991, bModify);
    ( ( CSeisItemUBC1997    * ) m_aCodeDlg[D_SEIS_CODE_DLG_UBC1997    ] )->SetData2Dlg(data.UBC1997, bModify);
    ( ( CSeisItemATC301     * ) m_aCodeDlg[D_SEIS_CODE_DLG_ATC306     ] )->SetData2Dlg(data.ATC306, bModify);
    ( ( CSeisItemKorea2000  * ) m_aCodeDlg[D_SEIS_CODE_DLG_KS2000     ] )->SetData2Dlg(data.KS2000, bModify);
    ( ( CSeisItemJapan2000  * ) m_aCodeDlg[D_SEIS_CODE_DLG_JIS        ] )->SetData2Dlg(data.JP, bModify);
    ( ( CSeisItemIBC2000    * ) m_aCodeDlg[D_SEIS_CODE_DLG_IBC2000    ] )->SetData2Dlg(data.IBC2000, bModify);
    ( ( CSeisItemEURO1996   * ) m_aCodeDlg[D_SEIS_CODE_DLG_EURO1996   ] )->SetData2Dlg(data.EURO1996, bModify);
    ( ( CSeisItemChina2001  * ) m_aCodeDlg[D_SEIS_CODE_DLG_CH2002     ] )->SetData2Dlg(data.CH2002, bModify);
    ( ( CSeisItemNBC1995    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NBC1995    ] )->SetData2Dlg(data.NBC1995, bModify);
    ( ( CSeisItemIS2002     * ) m_aCodeDlg[D_SEIS_CODE_DLG_IS2002     ] )->SetData2Dlg(data.IS2002, bModify);
    ( ( CSeisItemTaiwan1999 * ) m_aCodeDlg[D_SEIS_CODE_DLG_TAIWAN86   ] )->SetData2Dlg(data.TAIWAN86, bModify);
    ( ( CSeisItemKBC2005    * ) m_aCodeDlg[D_SEIS_CODE_DLG_KBC2005    ] )->SetData2Dlg(data.KBC2005, bModify);
    ( ( CSeisItemShanghai   * ) m_aCodeDlg[D_SEIS_CODE_DLG_CHSH2003   ] )->SetData2Dlg(data.CHSH2003, bModify);
    ( ( CSeisItemTaiwan2022 * ) m_aCodeDlg[D_SEIS_CODE_DLG_TAIWAN22   ] )->SetData2Dlg(data.TAIWAN22, bModify);
    ( ( CSeisItemTaiwan2002 * ) m_aCodeDlg[D_SEIS_CODE_DLG_TAIWAN02   ] )->SetData2Dlg(data.TAIWAN02, bModify);
    ( ( CSeisItemEURO2004   * ) m_aCodeDlg[D_SEIS_CODE_DLG_EURO2004   ] )->SetData2Dlg(data.EURO2004, bModify, data.EURO2004.nNationalAnnex);
    ( ( CSeisItemKBC2009    * ) m_aCodeDlg[D_SEIS_CODE_DLG_KBC2009    ] )->SetData2Dlg(data.KBC2009, bModify);
    ( ( CSeisItemTaiwan2011 * ) m_aCodeDlg[D_SEIS_CODE_DLG_TAIWAN11   ] )->SetData2Dlg(data.TAIWAN11, bModify);
    ( ( CSeisItemNTC2008    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NTC2008    ] )->SetData2Dlg(data.NTC2008, bModify);
    ( ( CSeisItemChina2010  * ) m_aCodeDlg[D_SEIS_CODE_DLG_CH2010     ] )->SetData2Dlg(data.CH2010, bModify);
    ( ( CSeisItemIBC2012    * ) m_aCodeDlg[D_SEIS_CODE_DLG_IBC2009    ] )->SetData2Dlg(data.IBC2009, bModify);
    ( ( CSeisItemIBC2012    * ) m_aCodeDlg[D_SEIS_CODE_DLG_IBC2012    ] )->SetData2Dlg(data.IBC2012, bModify);
    ( ( CSeisItemNTC2012    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NTC2012    ] )->SetData2Dlg(data.NTC2012, bModify);
    ( ( CSeisItemNSR2010    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NSR2010    ] )->SetData2Dlg(data.NSR2010, bModify);
    ( ( CSeisItemKBC2015    * ) m_aCodeDlg[D_SEIS_CODE_DLG_KBC2015    ] )->SetData2Dlg(data.KBC2015, bModify);
    ( ( CSeisItemP100_2013  * ) m_aCodeDlg[D_SEIS_CODE_DLG_P100_2013  ] )->SetData2Dlg(data.P100_2013, bModify);
    ( ( CSeisItemIS1893_2016* ) m_aCodeDlg[D_SEIS_CODE_DLG_IS1893_2016] )->SetData2Dlg(data.IS1893_2016, bModify);
    ( ( CSeisItemNTC2012    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NTC2018    ] )->SetData2Dlg(data.NTC2018, bModify);
    ( ( CSeisItemKDS2019    * ) m_aCodeDlg[D_SEIS_CODE_DLG_KDS2019    ] )->SetData2Dlg(data.KDS2019, bModify);
    ( ( CSeisItemDPT2018    * ) m_aCodeDlg[D_SEIS_CODE_DLG_DPT2018    ] )->SetData2Dlg(data.DPT2018, bModify);
	( ( CSeisItemKDS2019    * ) m_aCodeDlg[D_SEIS_CODE_DLG_KDS2018	  ] )->SetData2Dlg(data.KDS2019, bModify);
	( ( CSeisItemUser		* ) m_aCodeDlg[D_SEIS_CODE_DLG_USERTYPE   ] )->SetData2Dlg(data, bModify);	
    ( ( CSeisItemIBC2012    * ) m_aCodeDlg[D_SEIS_CODE_DLG_NSCP2024   ] )->SetData2Dlg(data.NSCP2024, bModify);
    ( (CSeisItemIS1893_2025 * ) m_aCodeDlg[D_SEIS_CODE_DLG_IS1893_2025] )->SetData2Dlg(data.IS2025, bModify);

    ChangeCodeDlg(nIndex);
}

// [SeisCode] 추가시 수정
BOOL CSeisItemCodeMgr::CodeDlg2Data(T_SEIS_D& data)
{
    if ( m_nCurCodeIndex < 0 ) return FALSE;

    BOOL bOK = TRUE;
    switch ( m_nCurCodeIndex )
    {
    case D_SEIS_CODE_DLG_KS1992:      bOK = ( ( CSeisItemKorea1992*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KS1992); break;
    case D_SEIS_CODE_DLG_UBC1991:     bOK = ( ( CSeisItemUBC1991*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.UBC1991); break;
    case D_SEIS_CODE_DLG_UBC1997:     bOK = ( ( CSeisItemUBC1997*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.UBC1997); break;
    case D_SEIS_CODE_DLG_ATC306:      bOK = ( ( CSeisItemATC301*      ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.ATC306); break;
    case D_SEIS_CODE_DLG_KS2000:      bOK = ( ( CSeisItemKorea2000*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KS2000); break;
    case D_SEIS_CODE_DLG_JIS:         bOK = ( ( CSeisItemJapan2000*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.JP); break;
    case D_SEIS_CODE_DLG_IBC2000:     bOK = ( ( CSeisItemIBC2000*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IBC2000); break;
    case D_SEIS_CODE_DLG_EURO1996:    bOK = ( ( CSeisItemEURO1996*    ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.EURO1996); break;
    case D_SEIS_CODE_DLG_CH2002:      bOK = ( ( CSeisItemChina2001*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CH2002); break;
    case D_SEIS_CODE_DLG_NBC1995:     bOK = ( ( CSeisItemNBC1995*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NBC1995); break;
    case D_SEIS_CODE_DLG_IS2002:      bOK = ( ( CSeisItemIS2002*      ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IS2002); break;
    case D_SEIS_CODE_DLG_TAIWAN86:    bOK = ( ( CSeisItemTaiwan1999*  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.TAIWAN86); break;
    case D_SEIS_CODE_DLG_KBC2005:     bOK = ( ( CSeisItemKBC2005*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KBC2005); break;
    case D_SEIS_CODE_DLG_CHSH2003:    bOK = ( ( CSeisItemShanghai*    ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CHSH2003); break;
    case D_SEIS_CODE_DLG_TAIWAN22:    bOK = ( ( CSeisItemTaiwan2022*  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.TAIWAN22); break;
    case D_SEIS_CODE_DLG_TAIWAN02:    bOK = ( ( CSeisItemTaiwan2002*  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.TAIWAN02); break;
    case D_SEIS_CODE_DLG_EURO2004:    bOK = ( ( CSeisItemEURO2004*    ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.EURO2004); break;
    case D_SEIS_CODE_DLG_KBC2009:     bOK = ( ( CSeisItemKBC2009*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KBC2009); break;
    case D_SEIS_CODE_DLG_TAIWAN11:    bOK = ( ( CSeisItemTaiwan2011*  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.TAIWAN11); break;
    case D_SEIS_CODE_DLG_NTC2008:     bOK = ( ( CSeisItemNTC2008*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NTC2008); break;
    case D_SEIS_CODE_DLG_CH2010:      bOK = ( ( CSeisItemChina2010*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CH2010); break;
    case D_SEIS_CODE_DLG_IBC2009:     bOK = ( ( CSeisItemIBC2012*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IBC2009); break;
    case D_SEIS_CODE_DLG_IBC2012:     bOK = ( ( CSeisItemIBC2012*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IBC2012); break;
    case D_SEIS_CODE_DLG_NTC2012:     bOK = ( ( CSeisItemNTC2012*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NTC2012); break;
    case D_SEIS_CODE_DLG_NSR2010:     bOK = ( ( CSeisItemNSR2010*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NSR2010); break;
    case D_SEIS_CODE_DLG_KBC2015:     bOK = ( ( CSeisItemKBC2015*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KBC2015); break;
    case D_SEIS_CODE_DLG_P100_2013:   bOK = ( ( CSeisItemP100_2013*   ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.P100_2013); break;
    case D_SEIS_CODE_DLG_IS1893_2016: bOK = ( ( CSeisItemIS1893_2016* ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IS1893_2016); break;
    case D_SEIS_CODE_DLG_NTC2018:     bOK = ( ( CSeisItemNTC2012*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NTC2018); break;
    case D_SEIS_CODE_DLG_KDS2019:     bOK = ( ( CSeisItemKDS2019*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KDS2019); break;
    case D_SEIS_CODE_DLG_DPT2018:     bOK = ( ( CSeisItemDPT2018*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.DPT2018); break;
	case D_SEIS_CODE_DLG_KDS2018:     bOK = ( ( CSeisItemKDS2019*	  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.KDS2019); break;
	case D_SEIS_CODE_DLG_USERTYPE:    bOK = ( ( CSeisItemUser*		  ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data); break;
    case D_SEIS_CODE_DLG_NSCP2024:    bOK = ( ( CSeisItemIBC2012*     ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.NSCP2024); break;
    case D_SEIS_CODE_DLG_IS1893_2025: bOK = ( ( CSeisItemIS1893_2025* ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.IS2025); break;
    default: ASSERT(0);
    }

    return bOK;
}

// [SeisCode] 추가시 수정
void CSeisItemCodeMgr::CreateCodeDlg()
{
    auto L_AddCodeDlg = [ this ] (const CString& csCodeName, CChildDialog* pDialog)
    {
        m_aCodeName.Add(csCodeName);
        m_aCodeDlg.Add(pDialog);

    };
    L_AddCodeDlg(_T("Korea(Arch.1992)"), new CSeisItemKorea1992());
    L_AddCodeDlg(_T("UBC(1991)"), new CSeisItemUBC1991());
    L_AddCodeDlg(_T("UBC(1997)"), new CSeisItemUBC1997());
    L_AddCodeDlg(_T("ATC3-06"), new CSeisItemATC301());
    L_AddCodeDlg(_T("Korea(Arch.2000)"), new CSeisItemKorea2000());
    L_AddCodeDlg(_T("Japan(Arch.2000)"), new CSeisItemJapan2000());
    L_AddCodeDlg(_T("IBC2000(ASCE7-98)"), new CSeisItemIBC2000());
    L_AddCodeDlg(_T("Eurocode-8(1996)"), new CSeisItemEURO1996());
    L_AddCodeDlg(_T("China(GB50011-2001)"), new CSeisItemChina2001());
    L_AddCodeDlg(_T("NBC(1995)"), new CSeisItemNBC1995());
    L_AddCodeDlg(_T("IS1893(2002)"), new CSeisItemIS2002());
    L_AddCodeDlg(_T("Taiwan(1999)"), new CSeisItemTaiwan1999());
    L_AddCodeDlg(_T("KBC(2005)"), new CSeisItemKBC2005());
    L_AddCodeDlg(_T("China Shanghai(DGJ08-9-2003)"), new CSeisItemShanghai());
    L_AddCodeDlg(_T("Taiwan(2006)"), new CSeisItemTaiwan2002()); // 2002로 개발도중 2006으로 업그레이드.    
    L_AddCodeDlg(_T("Eurocode-8(2004)"), new CSeisItemEURO2004());
    L_AddCodeDlg(_T("KBC(2009)"), new CSeisItemKBC2009());
    L_AddCodeDlg(_T("Taiwan(2011)"), new CSeisItemTaiwan2011());
    L_AddCodeDlg(_T("NTC2008"), new CSeisItemNTC2008());
	L_AddCodeDlg(_T("China(GB/T50011-2010)"), new CSeisItemChina2010());
    L_AddCodeDlg(_T("IBC2009(ASCE7-05)"), new CSeisItemIBC2012());
    L_AddCodeDlg(_T("IBC2012(ASCE7-10)"), new CSeisItemIBC2012());
    L_AddCodeDlg(_T("NTC2012"), new CSeisItemNTC2012());
    L_AddCodeDlg(_T("NSR-10"), new CSeisItemNSR2010());
    L_AddCodeDlg(_T("KBC(2016)"), new CSeisItemKBC2015());
    L_AddCodeDlg(_T("P100-1(2013)"), new CSeisItemP100_2013());
    L_AddCodeDlg(_T("IS1893(2016)"), new CSeisItemIS1893_2016());
    L_AddCodeDlg(_T("NTC2018"), new CSeisItemNTC2012(TRUE));
    L_AddCodeDlg(_T("KDS(41-17-00:2019)"), new CSeisItemKDS2019());
    L_AddCodeDlg(_T("DPT.1301/1302-61:2018"), new CSeisItemDPT2018(m_pParent));
    L_AddCodeDlg(_T("Taiwan(2022)"), new CSeisItemTaiwan2022());
	L_AddCodeDlg(_T("KDS(2018)"), new CSeisItemKDS2019());
	L_AddCodeDlg(_T("User Type"), new CSeisItemUser());
    L_AddCodeDlg(_T("NSCP 2024"), new CSeisItemIBC2012());
    L_AddCodeDlg(_T("IS1893(2025)"), new CSeisItemIS1893_2025(m_pParent));
}

void CSeisItemCodeMgr::DeleteCodeDlg()
{
    int nCount = m_aCodeDlg.GetSize();
    for ( int i = 0; i < nCount; i++ )
    {
        delete m_aCodeDlg[i];
    }
}