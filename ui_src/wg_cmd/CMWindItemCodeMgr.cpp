// CMWindItemCodeMgr.cpp: implementation of the CCMWindItemCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemCodeMgr.h"

// [WindCode] 추가시 수정
#include "CMWindItemDlg.h"
#include "CMWindItemKorea1992.h"
#include "CMWindItemJapan1997.h"
#include "CMWindItemUBC1997.h"
#include "CMWindItemANSI1982.h"
#include "CMWindItemKorea2000.h"
#include "CMWindItemIBC2000.h"
#include "CMWindItemEuro1992.h"
#include "CMWindItemBS1997.h"
#include "CMWindItemChina2001.h"
#include "CMWindItemJapan2000.h"
#include "CMWindItemNBC1995.h"
#include "CMWindItemIS1987.h"
#include "CMWindItemTaiwan1986.h"
#include "CMWindItemJapan2004.h"
#include "CMWindItemEuro2005.h"
#include "CMWindItemKBC2008.h"
#include "CMWindItemKBC2015.h"
#include "CMWindItemKDS2021.h"
#include "CMWindItemIBC2012.h"
#include "CMWindItemChina2012.h"
#include "CMWindItemNSR2010.h"
#include "CMWindItemIS875_2015.h"
#include "CMWindItemChina2019.h"
#include "CMWindItemDPT2007.h"
#include "CMWindItemUser.h"
#include "CMWindItemASCE7_2016.h"
#include "CMWindItemRUS2016_User.h"

// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMWindItemCodeMgr::CCMWindItemCodeMgr(CWnd* pParent)
{
    m_pParent = NULL;
    ASSERT(pParent);
    m_pParent = ( CCMWindItemDlg * ) pParent;

    m_nCurCodeIndex = -1;
    m_nInitCodeIndex = 0;
    m_aCodeName.RemoveAll();
    m_aCodeDlg.RemoveAll();
    m_aCodeType.RemoveAll();

    CreateCodeDlg();
}

CCMWindItemCodeMgr::~CCMWindItemCodeMgr()
{
    DeleteCodeDlg();
}

//////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////

// [WindCode] 추가시 수정
int  CCMWindItemCodeMgr::GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable/*=NULL*/)
{
    CArray<int, int> aWindSeq;
    aWindSeq.RemoveAll();
    CDBLib::GetWLCodeSeqList(aWindSeq);

    BOOL bOnlyEnable = TRUE;
#if defined(_CH) && defined(_MGEN)
    bOnlyEnable = FALSE;
#endif
    if ( aCodeEnable==NULL ) bOnlyEnable= TRUE;
    if ( aCodeEnable!=NULL ) aCodeEnable->RemoveAll();

    CString csErrMsg(_T(""));
    for ( int i = 0; i < aWindSeq.GetSize(); i++ )
    {
        BOOL bInclude = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadWind, aWindSeq[i] + 1, csErrMsg);
        if ( bInclude==FALSE&&bOnlyEnable ) continue;
        aCodeName.Add(m_aCodeName[aWindSeq[i]]);
        aCodeIndex.Add(aWindSeq[i] + 1);
        if ( aCodeEnable!=NULL ) aCodeEnable->Add(bInclude);
    }
    return aCodeName.GetSize();
}

void CCMWindItemCodeMgr::ChangeNationalAnnex(int nCodeIndex, int nNAIndex)
{
    if ( ( nCodeIndex+1 )!=EURO_W_2005 ) return;
    ( ( CCMWindItemEuro2005 * ) m_aCodeDlg[nCodeIndex] )->SetNationalAnnex(nNAIndex==1);
}

int CCMWindItemCodeMgr::GetCurCodeType()
{
    return m_nCurCodeIndex+1;
}

void CCMWindItemCodeMgr::ChangeCodeDlg(int nCodeIndex)
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
        m_aCodeDlg[m_nCurCodeIndex]->End();

    CRect WRect;
    m_pParent->GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(&WRect);
    m_pParent->ScreenToClient(WRect);
    if ( m_aCodeDlg[nCodeIndex]->CreateInit(m_pParent, WRect.left, WRect.top) )
    {
        if ( ( nCodeIndex+1 ) == IBC_W_2009 || ( nCodeIndex+1 ) == IBC_W_2012 )
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[nCodeIndex] )->SetIBCType(nCodeIndex);
        }
    }
    m_nCurCodeIndex = nCodeIndex;

    CRect rcChild;
    m_aCodeDlg[nCodeIndex]->GetWindowRect(rcChild);
    m_pParent->AlignCtrl2Param(rcChild.bottom);
}

// [WindCode] 추가시 수정
void CCMWindItemCodeMgr::Data2CodeDlg(T_WIND_D& data, int nIndex, BOOL bModify, BOOL bSingapore, T_KEY KeyWAorWT/*=NULL*/, T_KEY KeyWind/* = NULL*/)
{
    switch ( nIndex+1 )
    {
    case KS_W_92:
        {
            ( ( CCMWindItemKorea1992 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KS1992, bModify);
        }
        break;
    case JP_W_87:
        {
            ( ( CCMWindItemJapan1997 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.JP1987, bModify);
        }
        break;
    case UBC_W_97:
        {
            ( ( CCMWindItemUBC1997 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.UBC1997, bModify);
        }
        break;
    case ANSI_W_82:
        {
            ( ( CCMWindItemANSI1982 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.ANSI1982, bModify);
        }
        break;
    case KS_W_2000:
        {
            ( ( CCMWindItemKorea2000 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KS2000, bModify);
        }
        break;
    case IBC_W_2000:
        {
            ( ( CCMWindItemIBC2000 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.IBC2000, bModify);
        }
        break;
    case EURO_W_1992:
        {
            ( ( CCMWindItemEuro1992 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.EURO1992, bModify);
        }
        break;
    case BS6399_W_1997:
        {
            ( ( CCMWindItemBS1997 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.BS6399, bModify);
        }
        break;
    case CH_W_2002:
        {
            ( ( CCMWindItemChina2001 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.CH2002, bModify);
        }
        break;
    case JPN_W_2000:
        {
            ( ( CCMWindItemJapan2000 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.JPN2000, bModify);
        }
        break;
    case NBC_W_1995:
        {
            ( ( CCMWindItemNBC1995 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.NBC1995, bModify);
        }
        break;
    case IS_W_1987:
        {
            ( ( CCMWindItemIS1987 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.IS1987, bModify);
        }
        break;
    case TAIWAN_W_1986:
        {
            ( ( CCMWindItemTaiwan1986 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.TAIWAN86, bModify);
        }
        break;
    case JP_W_2004:
        {
            ( ( CCMWindItemJapan2004 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.JP2004, bModify);
        }
        break;
    case EURO_W_2005:
        {
            ( ( CCMWindItemEuro2005 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.EURO2005, bModify, bSingapore);
        }
        break;
    case KBC_W_2009:
        {
            ( ( CCMWindItemKBC2009 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KBC2009, bModify);
        }
        break;
    case IBC_W_2009:
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.IBC2009, bModify);
        }
        break;
    case IBC_W_2012:
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.IBC2012, bModify);
        }
        break;
    case CH_W_2012:
        {
            ( ( CCMWindItemChina2012 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.CH2012, bModify);
        }
        break;
    case NSR_W_2010:
        {
            ( ( CCMWindItemNSR2010 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.NSR2010, bModify);
        }
        break;
    case KBC_W_2016:
        {
            ( ( CCMWindItemKBC2015 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KBC2015, bModify, KeyWAorWT);
        }
        break;
    case IS_W_875_2015:
        {
            ( ( CCMWindItemIS875_2015 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.IS875_2015, bModify);
        }
        break;
    case CH_W_2021:
        {
            ( ( CCMWindItemChina2019 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.CH2019, bModify);
        }
        break;
    case KDS_W_2019:
        {
            ( ( CCMWindItemKBC2015 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KDS2019, bModify, KeyWAorWT);
        }
        break;
    case KDS_W_2022:
        {
            ( ( CCMWindItemKDS2021 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.KDS2021, bModify, KeyWAorWT);
        }
        break;
    case DPT_W_2007:
        {
            ( ( CCMWindItemDPT2007 * ) m_aCodeDlg[nIndex] )->SetData2Dlg(data.CodeParam.DPT2007, bModify, KeyWAorWT);
        }
        break;
	case USER_W_TYPE:
		{
			((CCMWindItemUser *)m_aCodeDlg[nIndex])->SetData2Dlg(data, bModify);
		}
		break;
	case RUS_W_2016:
	    {
		    ((CCMWindItemRUS2016_User*)m_aCodeDlg[nIndex])->SetData2Dlg(data, bModify, KeyWind);
	    }
	    break;
    case ASCE7_W_2016:
		{
			((CCMWindItemASCE7_2016*)m_aCodeDlg[nIndex])->SetData2Dlg(data.CodeParam.ASCE7_16, bModify);
		}
		break;
    case ASCE7_W_2022:
	    {
		    ((CCMWindItemASCE7_2016*)m_aCodeDlg[nIndex])->SetData2Dlg(data.CodeParam.ASCE7_22, bModify);
	    }
	    break;
    case NSCP_W_2024:
        {
            ((CCMWindItemASCE7_2016*)m_aCodeDlg[nIndex])->SetData2Dlg(data.CodeParam.NSCP2024, bModify);
        }
        break;
    default:
        ASSERT(0);
        break;
    }

    ChangeCodeDlg(nIndex);
}

// [WindCode] 추가시 수정
BOOL CCMWindItemCodeMgr::CodeDlg2Data(T_WIND_D& data)
{
    if ( m_nCurCodeIndex < 0 ) return FALSE;

    switch ( m_aCodeType[m_nCurCodeIndex] )
    {
    case KS_W_92:
        {
            ( ( CCMWindItemKorea1992 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KS1992);
        }
        break;
    case JP_W_87:
        {
            ( ( CCMWindItemJapan1997 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.JP1987);
        }
        break;
    case UBC_W_97:
        {
            ( ( CCMWindItemUBC1997 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.UBC1997);
        }
        break;
    case ANSI_W_82:
        {
            ( ( CCMWindItemANSI1982 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.ANSI1982);
        }
        break;
    case KS_W_2000:
        {
            ( ( CCMWindItemKorea2000 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KS2000);
        }
        break;
    case IBC_W_2000:
        {
            ( ( CCMWindItemIBC2000 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.IBC2000);
        }
        break;
    case EURO_W_1992:
        {
            ( ( CCMWindItemEuro1992 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.EURO1992);
        }
        break;
    case BS6399_W_1997:
        {
            ( ( CCMWindItemBS1997 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.BS6399);
        }
        break;
    case CH_W_2002:
        {
            ( ( CCMWindItemChina2001 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.CH2002);
        }
        break;
    case JPN_W_2000:
        {
            ( ( CCMWindItemJapan2000 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.JPN2000);
        }
        break;
    case NBC_W_1995:
        {
            ( ( CCMWindItemNBC1995 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.NBC1995);
        }
        break;
    case IS_W_1987:
        {
            ( ( CCMWindItemIS1987 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.IS1987);
        }
        break;
    case TAIWAN_W_1986:
        {
            ( ( CCMWindItemTaiwan1986 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.TAIWAN86);
        }
        break;
    case JP_W_2004:
        {
            ( ( CCMWindItemJapan2004 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.JP2004);
        }
        break;
    case EURO_W_2005:
        {
            ( ( CCMWindItemEuro2005 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.EURO2005);
        }
        break;
    case KBC_W_2009:
        {
            ( ( CCMWindItemKBC2009 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KBC2009);
        }
        break;
    case IBC_W_2009:
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.IBC2009);
        }
        break;
    case IBC_W_2012:
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.IBC2012);
        }
        break;
    case CH_W_2012:
        {
            ( ( CCMWindItemChina2012 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.CH2012);
        }
        break;
    case NSR_W_2010:
        {
            ( ( CCMWindItemNSR2010* ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.NSR2010);
        }
        break;
    case KBC_W_2016:
        {
            if ( !( ( CCMWindItemKBC2015 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KBC2015) ) return FALSE;
            if ( data.CodeParam.KBC2015.bTorsionalWind )
            {
                data.nCodeType = m_aCodeType[m_nCurCodeIndex];
                if ( !CDBDoc::GetDocPoint()->m_pEditData->CheckWindData(data) ) return FALSE;

                CLateralLoad LateralLoad;
                if ( !LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE) ) return FALSE;
            }
        }
        break;
    case IS_W_875_2015:
        {
            ( ( CCMWindItemIS875_2015 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.IS875_2015);
        }
        break;
    case CH_W_2021:
        {
            if ( !( ( CCMWindItemChina2019 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.CH2019) )
                return FALSE;
        }
        break;
    case KDS_W_2019:
        {
            if ( !( ( CCMWindItemKBC2015 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KDS2019) ) return FALSE;
            if ( data.CodeParam.KDS2019.bTorsionalWind )
            {
                data.nCodeType = m_aCodeType[m_nCurCodeIndex];
                if ( !CDBDoc::GetDocPoint()->m_pEditData->CheckWindData(data) ) return FALSE;

                CLateralLoad LateralLoad;
                if ( !LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE) ) return FALSE;
            }
        }
        break;
    case KDS_W_2022:
        {
            if ( !( ( CCMWindItemKDS2021 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.KDS2021) ) return FALSE;
            if ( data.CodeParam.KDS2021.bTorsionalWind )
            {
                data.nCodeType = m_aCodeType[m_nCurCodeIndex];
                if ( !CDBDoc::GetDocPoint()->m_pEditData->CheckWindData(data) ) return FALSE;

                CLateralLoad LateralLoad;
                if ( !LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE) ) return FALSE;
            }
        }
        break;
    case DPT_W_2007:
        {
            if ( !( ( CCMWindItemDPT2007 * ) m_aCodeDlg[m_nCurCodeIndex] )->SetDlg2Data(data.CodeParam.DPT2007) ) return FALSE;
            if ( data.CodeParam.DPT2007.Vibr.bTorsion )
            {
                data.nCodeType = m_aCodeType[m_nCurCodeIndex];
                if ( !CDBDoc::GetDocPoint()->m_pEditData->CheckWindData(data) ) return FALSE;

                CLateralLoad LateralLoad;
                if ( !LateralLoad.MakeWindLoad(data, FALSE, _T(""), TRUE) ) return FALSE;
            }
        }
        break;
	case USER_W_TYPE:
		{
			if (!((CCMWindItemUser *)m_aCodeDlg[m_nCurCodeIndex])->SetDlg2Data(data)) return FALSE;
		}
		break;
	case RUS_W_2016:
	{
		if (!((CCMWindItemRUS2016_User*)m_aCodeDlg[m_nCurCodeIndex])->SetDlg2Data(data)) return FALSE;
	}
	break;
	case ASCE7_W_2016:
	    {
		    ((CCMWindItemASCE7_2016*)m_aCodeDlg[m_nCurCodeIndex])->SetDlg2Data(data.CodeParam.ASCE7_16);
	    }
	    break;
	case ASCE7_W_2022:
	    {
		    ((CCMWindItemASCE7_2016*)m_aCodeDlg[m_nCurCodeIndex])->SetDlg2Data(data.CodeParam.ASCE7_22);
	    }
	    break;
    case NSCP_W_2024:
        {
            ((CCMWindItemASCE7_2016*)m_aCodeDlg[m_nCurCodeIndex])->SetDlg2Data(data.CodeParam.NSCP2024);
        }
        break;
    default:
        ASSERT(0);
        break;
    }

    return TRUE;
}

void CCMWindItemCodeMgr::GetWnatData(T_WNAT_D& WnatD)
{
    switch ( m_aCodeType[m_nCurCodeIndex] )
    {
    case IBC_W_2000:
        {
            ( ( CCMWindItemIBC2000 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case KBC_W_2009:
        {
            ( ( CCMWindItemKBC2009 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case IBC_W_2009:
    case IBC_W_2012:
        {
            ( ( CCMWindItemIBC2012 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case NSR_W_2010:
        {
            ( ( CCMWindItemNSR2010 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case KBC_W_2016:
    case KDS_W_2019:
        {
            ( ( CCMWindItemKBC2015 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case KDS_W_2022:
        {
            ( ( CCMWindItemKDS2021 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
    case DPT_W_2007:
        {
            ( ( CCMWindItemDPT2007 * ) m_aCodeDlg[m_nCurCodeIndex] )->GetWnatData(WnatD);
        }
        break;
	case ASCE7_W_2016:
	case ASCE7_W_2022:
    case NSCP_W_2024:
		{
			((CCMWindItemASCE7_2016*)m_aCodeDlg[m_nCurCodeIndex])->GetWnatData(WnatD);
		}
		break;
	case USER_W_TYPE:
		{
			//((CCMWindItemUser *)m_aCodeDlg[m_nCurCodeIndex])->GetWnatData(WnatD);
		}
		break;
    }
}

BOOL CCMWindItemCodeMgr::GetWnpsData(T_WNPS_D& WnpsD)
{
    if (m_aCodeType[m_nCurCodeIndex] == RUS_W_2016)
    {
        return ((CCMWindItemRUS2016_User*)m_aCodeDlg[m_nCurCodeIndex])->GetWnpsData(WnpsD);
    }
    else return FALSE;
}
//////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////

// [WindCode] 추가시 수정
void CCMWindItemCodeMgr::CreateCodeDlg()
{
    m_aCodeName.Add(FULL_KS_W_92);
    m_aCodeDlg.Add(new CCMWindItemKorea1992(m_pParent));
    m_aCodeType.Add(KS_W_92);

    m_aCodeName.Add(FULL_JP_W_87);
    m_aCodeDlg.Add(new CCMWindItemJapan1997(m_pParent));
    m_aCodeType.Add(JP_W_87);

    m_aCodeName.Add(FULL_UBC_W_97);
    m_aCodeDlg.Add(new CCMWindItemUBC1997(m_pParent));
    m_aCodeType.Add(UBC_W_97);

    m_aCodeName.Add(FULL_ANSI_W_82);
    m_aCodeDlg.Add(new CCMWindItemANSI1982(m_pParent));
    m_aCodeType.Add(ANSI_W_82);

    m_aCodeName.Add(FULL_KS_W_2000);
    m_aCodeDlg.Add(new CCMWindItemKorea2000(m_pParent));
    m_aCodeType.Add(KS_W_2000);

    m_aCodeName.Add(FULL_IBC_W_2000);
    m_aCodeDlg.Add(new CCMWindItemIBC2000(m_pParent));
    m_aCodeType.Add(IBC_W_2000);

    m_aCodeName.Add(FULL_EURO_W_1992);
    m_aCodeDlg.Add(new CCMWindItemEuro1992(m_pParent));
    m_aCodeType.Add(EURO_W_1992);

    m_aCodeName.Add(FULL_BS6399_W_1997);
    m_aCodeDlg.Add(new CCMWindItemBS1997(m_pParent));
    m_aCodeType.Add(BS6399_W_1997);

    m_aCodeName.Add(FULL_CH_W_2002);
    m_aCodeDlg.Add(new CCMWindItemChina2001(m_pParent));
    m_aCodeType.Add(CH_W_2002);

    m_aCodeName.Add(FULL_JPN_W_2000);
    m_aCodeDlg.Add(new CCMWindItemJapan2000(m_pParent));
    m_aCodeType.Add(JPN_W_2000);

    m_aCodeName.Add(FULL_NBC_W_1995);
    m_aCodeDlg.Add(new CCMWindItemNBC1995(m_pParent));
    m_aCodeType.Add(NBC_W_1995);

    m_aCodeName.Add(FULL_IS_W_1987);
    m_aCodeDlg.Add(new CCMWindItemIS1987(m_pParent));
    m_aCodeType.Add(IS_W_1987);

    m_aCodeName.Add(FULL_TAIWAN_W_1986);
    m_aCodeDlg.Add(new CCMWindItemTaiwan1986(m_pParent));
    m_aCodeType.Add(TAIWAN_W_1986);

    m_aCodeName.Add(FULL_JP_W_2004);
    m_aCodeDlg.Add(new CCMWindItemJapan2004(m_pParent));
    m_aCodeType.Add(JP_W_2004);

    m_aCodeName.Add(FULL_EURO_W_2005);
    m_aCodeDlg.Add(new CCMWindItemEuro2005(m_pParent));
    m_aCodeType.Add(EURO_W_2005);

    m_aCodeName.Add(FULL_KBC_W_2009);
    m_aCodeDlg.Add(new CCMWindItemKBC2009(m_pParent));
    m_aCodeType.Add(KBC_W_2009);

    m_aCodeName.Add(FULL_IBC_W_2009);
    m_aCodeDlg.Add(new CCMWindItemIBC2012(m_pParent));
    m_aCodeType.Add(IBC_W_2009);

    m_aCodeName.Add(FULL_IBC_W_2012);
    m_aCodeDlg.Add(new CCMWindItemIBC2012(m_pParent));
    m_aCodeType.Add(IBC_W_2012);

    m_aCodeName.Add(FULL_CH_W_2012);
    m_aCodeDlg.Add(new CCMWindItemChina2012(m_pParent));
    m_aCodeType.Add(CH_W_2012);

    m_aCodeName.Add(FULL_NSR_W_2010);
    m_aCodeDlg.Add(new CCMWindItemNSR2010(m_pParent));
    m_aCodeType.Add(NSR_W_2010);

    m_aCodeName.Add(FULL_KBC_W_2016);
    m_aCodeDlg.Add(new CCMWindItemKBC2015(m_pParent));
    m_aCodeType.Add(KBC_W_2016);

    m_aCodeName.Add(FULL_IS_W_875_2015);
    m_aCodeDlg.Add(new CCMWindItemIS875_2015(m_pParent));
    m_aCodeType.Add(IS_W_875_2015);

    m_aCodeName.Add(FULL_CH_W_2021);
    m_aCodeDlg.Add(new CCMWindItemChina2019(m_pParent));
    m_aCodeType.Add(CH_W_2021);

    m_aCodeName.Add(FULL_KDS_W_2019);
    m_aCodeDlg.Add(new CCMWindItemKBC2015(m_pParent));
    m_aCodeType.Add(KDS_W_2019);

    m_aCodeName.Add(FULL_KDS_W_2022);
    m_aCodeDlg.Add(new CCMWindItemKDS2021(m_pParent));
    m_aCodeType.Add(KDS_W_2022);

    if ( CDBLib::IsSupportDPT() )
    {
        m_aCodeName.Add(FULL_DPT_W_2007);
        m_aCodeDlg.Add(new CCMWindItemDPT2007(m_pParent));
        m_aCodeType.Add(DPT_W_2007);
    }

	m_aCodeName.Add(FULL_USER_TYPE);
	m_aCodeDlg.Add(new CCMWindItemUser(m_pParent));
	m_aCodeType.Add(USER_W_TYPE);

	m_aCodeName.Add(FULL_ASCE7_W_2016);
	m_aCodeDlg.Add(new CCMWindItemASCE7_2016(m_pParent, ASCE7_W_2016));
	m_aCodeType.Add(ASCE7_W_2016);

	m_aCodeName.Add(FULL_ASCE7_W_2022);
	m_aCodeDlg.Add(new CCMWindItemASCE7_2016(m_pParent, ASCE7_W_2022));
	m_aCodeType.Add(ASCE7_W_2022);

	m_aCodeName.Add(FULL_NSCP_W_2024);
	m_aCodeDlg.Add(new CCMWindItemASCE7_2016(m_pParent, NSCP_W_2024));
	m_aCodeType.Add(NSCP_W_2024);

	m_aCodeName.Add(FULL_RUS_W_2016_USER);
	m_aCodeDlg.Add(new CCMWindItemRUS2016_User(m_pParent));
	m_aCodeType.Add(RUS_W_2016);
}

void CCMWindItemCodeMgr::DeleteCodeDlg()
{
    int nCount = m_aCodeDlg.GetSize();
    for ( int i = 0; i < nCount; i++ )
    {
        delete m_aCodeDlg[i];
    }
}