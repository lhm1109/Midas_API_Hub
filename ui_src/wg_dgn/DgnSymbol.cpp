#include "StdAfx.h"
#include "DgnSymbol.h"
#include "DgnStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnSymbol::CDgnSymbol()
{
}

CDgnSymbol::~CDgnSymbol()
{
}

CString CDgnSymbol::GetFc(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
	case KCI_USD99:
		{
			strChar = _T("fck");
		}
		break;
	case JTJ023_85:
		{
			strChar = _T("R");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("fcu");
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case AASHTO_LC96:
	case ACI318_02:
	case AS5100_5_2017:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
	case TWN_BRG_LC90:
		{
			strChar = _T("fc'");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Rb");
		}
		break;
	default:
		{
			strChar = _T("fc");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetFy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("fy");

	switch ( nDgnCode )
	{
	case JTJ023_85:
		{
			strChar = _T("Rg");
		}
		break;
	case Eurocode2_2_05:
	case IRS_RC:
		{
			strChar = _T("fyk");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("fsy");
		}
		break;
	case BS5400_4_90:
	case IRC_112_11:
	case IRC_112_20:
	case IS456_2000:
	case TMH07_89:
		{
			strChar = _T("fy");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Rsn");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetFys(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("fys");

	switch ( nDgnCode )
	{
	case JTJ023_85:
		{
			strChar = _T("Rgk");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("fyw");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("fsy.f");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("fyv");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Rswn");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetPu(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Pu");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case IRC_21_00:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("N");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Pf");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("N_Ed");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("N*");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}

	return strText;
}

CString CDgnSymbol::GetPuy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Puy");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case IRC_21_00:
	case JTJ023_85:
	case TMH07_89:
		{
			strChar = _T("Ny");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Pfy");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("N_Edy");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("N*y");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetPuz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Puz");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case IRC_21_00:
	case JTJ023_85:
	case TMH07_89:
		{
			strChar = _T("Nz");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Pfz");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("N_Edz");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("N*z");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMu(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Mu");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case IRC_21_00:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("M");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Mf");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Ed");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("M*");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMc(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Mc");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case IRC_21_00:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("M");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Ed");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("M*");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMcy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Mcy");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("My");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Edy");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("M*y");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMcz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Mcz");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("Mz");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Edz");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("M*z");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetVu(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _T("Su");
		}
		break;
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Q");
		}
		break;
	case IRC_21_00:
		{
			strChar = _T("V");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Vf");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("v");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("V_Ed");
		}
		break;
	case AREMA23:
		{
			strChar = _T("vu");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("V*");
		}
		break;
	default:
		{
			strChar = _T("Vu");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetVuy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _T("Suy");
		}
		break;
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Qy");
		}
		break;
	case IRC_21_00:
		{
			strChar = _T("Vy");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Vfy");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("V_Edy");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("v_y");
		}
		break;
	case AREMA23:
		{
			strChar = _T("vuy");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("V*y");
		}
		break;
	default:
		{
			strChar = _T("Vuy");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetVuz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _T("Suz");
		}
		break;
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Qz");
		}
		break;
	case IRC_21_00:
		{
			strChar = _T("Vz");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Vfz");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("V_Edz");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("v_z");
		}
		break;
	case AREMA23:
		{
			strChar = _T("vuz");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("V*z");
		}
		break;
	default:
		{
			strChar = _T("Vuz");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetTu(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case AREMA23:
		{
			strChar = _T("Tu");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("T");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("T*");
		}
		break;
	default:
		{
			strChar = _T("Tu");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::Getxd(const int nDgnCode, bool bFixWidth)
{
	CString strChar = _T("x/d");
	switch ( nDgnCode )
	{
	case AS5100_5_2017:
		{
			strChar = _T("kuo");
		}
		break;
	}

	CString strText;
	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpPnmax(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("pPn-") + _LS(IDS_RCS_GRID_max);

	switch ( nDgnCode )
	{
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Nr-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case IRC_21_00:
		{
			strChar = _T("N-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Pr-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("N_Rd") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("N") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pNu") + _LS(IDS_RCS_GRID_max);
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%7s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpPn(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("pPn");

	switch ( nDgnCode )
	{
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Nr");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("N_Rd");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("Nu");
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Pr");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pNu");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpMn(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("pMn");

	switch ( nDgnCode )
	{
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Mr");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Rd");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("Mu");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pMu");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpMny(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = GetpMn(nDgnCode) + _T("y");

	switch ( nDgnCode )
	{
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Rdy");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("M_uy");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pMuy");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpMnz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = GetpMn(nDgnCode) + _T("z");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("M_u");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("M_Rdz");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pMuz");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpV(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _T("pSc");
		}
		break;
	case JTJ023_85:
		{
			strChar = _T("Qhk");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("Vc");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _T("V_Rdc");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _T("v_u");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Qr");
		}
		break;
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
		{
			strChar = _T("pV");
		}
		break;
	case AREMA23:
		{
			strChar = _T("vn");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pVu");
		}
		break;
	default:
		{
			strChar = _T("pVc");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpTn(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Tr");
		}
		break;
	case AREMA23:
		{
			strChar = _T("¥õTn");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("pTu");
		}
		break;
	default:
		{
			strChar = _T("Tn");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatP(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-P");

	switch ( nDgnCode )
	{
	case AS5100_5_2017:
	case BS5400_4_90:
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-N");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatPy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = GetRatP(nDgnCode) + _T("y");
	if ( bFixWidth )
	{
		strText.Format(_T("%6s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatPz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = GetRatP(nDgnCode) + _T("z");
	if ( bFixWidth )
	{
		strText.Format(_T("%6s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatV(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-S");
		}
		break;
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-Q");
		}
		break;
	case AS5100_5_2017:
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-V");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-v");
		}
		break;
	default:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-V");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatT(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case AREMA23:
	case AS5100_5_2017:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-T");
		}
		break;
	default:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-T");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetRatF(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-F");
		}
		break;
	default:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-F");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

// Service
CString CDgnSymbol::GetRatS(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case BS5400_4_90:
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
	case TMH07_89:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-S");
		}
		break;
	default:
		{
			strChar = _LS(IDS_RCS_GRID_TEXT_Rat) + _T("-S");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetAst(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("Ast");

	switch ( nDgnCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("As");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetAsV(const int nDgnCode, bool bFixWidth)
{
	// for Beam shear reinforcement.
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strChar = _T("AvS");
		}
		break;
	case JTJ023_85:
		{
			strChar = _T("Ak");
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
		{
			strChar = _T("Av");
		}
		break;
	case BS5400_4_90:
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
	case TMH07_89:
		{
			strChar = _T("Asw");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("Asv/s");
		}
		break;
	default:
		{
			strChar = _T("AsV");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetAsH(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("As-H");

	switch ( nDgnCode )
	{
	case JTJ023_85:
		{
			strChar = _T("Ag");
		}
		break;
	case BS5400_4_90:
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
	case TMH07_89:
		{
			strChar = _T("Asw-H");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetAsy(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Aswy");
		}
		break;
	default:
		{
			strChar = _T("Asvy");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetAsz(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("Aswz");
		}
		break;
	default:
		{
			strChar = _T("Asvz");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%3s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMFy(const int nDgnCode, bool bFixWidth)
{
	// Moment magnification factor.
	CString strText = _T("");
	CString strChar = _T("MF_y");
	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetMFz(const int nDgnCode, bool bFixWidth)
{
	// Moment magnification factor.
	CString strText = _T("");
	CString strChar = _T("MF_z");
	if ( bFixWidth )
	{
		strText.Format(_T("%5s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetCrack(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");
	switch ( nDgnCode )
	{
	case AASHTO_LC02:
		{
			strChar = _T("Z");
		}
		break;
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case ACI318_02:
		{
			strChar = _T("S");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("B2");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _T("a,cr");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("w");
		}
		break;
	default:
		{
			strChar = _T("w");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetCracka(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");

	switch ( nDgnCode )
	{
	case AASHTO_LC02:
		{
			strChar = _T("Za");
		}
		break;
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case ACI318_02:
		{
			strChar = _T("Sa");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strChar = _T("B2a");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strChar = _LS(IDS_RCS_GRID_Del_cr);
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("w") + _LS(IDS_RCS_GRID_max);
		}
		break;
	default:
		{
			strChar = _T("wa");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetStress(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");
	switch ( nDgnCode )
	{
	case Eurocode2_2_05:
		{
			strChar = _T("S");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("sscr");
		}
		break;
	default:
		{
			strChar = _T("S");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetStressa(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("");
	switch ( nDgnCode )
	{
	case Eurocode2_2_05:
		{
			strChar = _T("Sa");
		}
		break;
	case AS5100_5_2017:
		{
			strChar = _T("fscr");
		}
		break;
	default:
		{
			strChar = _T("Sa");
		}
		break;
	}

	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::Getffs(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("ffs");
	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::Getfrs(const int nDgnCode, bool bFixWidth)
{
	CString strText = _T("");
	CString strChar = _T("frs");
	if ( bFixWidth )
	{
		strText.Format(_T("%4s"), strChar);
	}
	else
	{
		strText = strChar;
	}
	return strText;
}

CString CDgnSymbol::GetpPnmax(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = strPi + _T("Pn-") + _LS(IDS_RCS_GRID_max);

	switch ( nDgnCode )
	{
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Nr-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strText = _T("Pr-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case BS5400_4_90:
	case IRC_21_00:
	case TMH07_89:
		{
			strText = _T("N-") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("N_Rd") + _LS(IDS_RCS_GRID_max);
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Nu") + _LS(IDS_RCS_GRID_max);
		}
		break;
	}

	return strText;
}

CString CDgnSymbol::GetpPn(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = strPi + _T("Pn");

	switch ( nDgnCode )
	{
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Nr");
		}
		break;
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strText = _T("Pr");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strText = _T("N_u");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("N_Rd");
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Nu");
		}
		break;
	}
	return strText;
}

CString CDgnSymbol::GetpPny(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = GetpPn(nDgnCode, strPi) + _T("y");
	return strText;
}

CString CDgnSymbol::GetpPnz(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = GetpPn(nDgnCode, strPi) + _T("z");
	return strText;
}

CString CDgnSymbol::GetpMn(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = strPi + _T("Mn");
	switch ( nDgnCode )
	{
	case AASHTO_LC02:
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Mr");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strText = _T("Mu");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("M_Rd");
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Mu");
		}
		break;
	}

	return strText;
}

CString CDgnSymbol::GetpMny(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = GetpMn(nDgnCode, strPi) + _T("y");
	return strText;
}

CString CDgnSymbol::GetpMnz(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = GetpMn(nDgnCode, strPi) + _T("z");
	return strText;
}

CString CDgnSymbol::GetpVn(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = _T("");
	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strText = strPi + _T("Sn");
		}
		break;
	case JTJ023_85:
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Qr");
		}
		break;
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
		{
			strText = _T("Vr");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRC_21_00:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("V_Rd");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strText = _T("v_u");
		}
		break;
	case AREMA23:
		{
			strText = _T("vn");
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Vu");
		}
		break;
	default:
		{
			strText = strPi + _T("Vn");
		}
		break;
	}

	return strText;
}

CString CDgnSymbol::GetpVc(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = _T("");
	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strText = strPi + _T("Sc");
		}
		break;
	case JTJ023_85:
		{
			strText = _T("Qhk");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
	case IRC_21_00:
		{
			strText = _T("Vc");
		}
		break;
	case AREMA23:
		{
			strText = _T("vc");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("V_Rdc");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strText = _T("v_c");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Qrc");
		}
		break;
	case AASHTO_LC07:
	case AASHTO_LC12:
	case AASHTO_LC16:
	case AASHTO_LC17:
	case AASHTO_LC20:
	case AASHTO_LC24:
		{
			strText = strPi + _T("V");
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Vuc");
		}
		break;
	default:
		{
			strText = strPi + _T("Vc");
		}
		break;
	}

	return strText;
}

CString CDgnSymbol::GetpVcy(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	return ( GetpVc(nDgnCode, strPi) + _T("y") );
}

CString CDgnSymbol::GetpVcz(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	return ( GetpVc(nDgnCode, strPi) + _T("z") );
}

CString CDgnSymbol::GetpVs(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	CString strText = _T("");
	switch ( nDgnCode )
	{
	case KSCE_USD96:
		{
			strText = strPi + _T("Ss");
		}
		break;
	case JTJ023_85:
		{
			strText = _T("Qw");
		}
		break;
	case CSA_S6_00:
	case CSA_S6_14_RC:
	case CSA_S6_19_RC:
	case IRC_21_00:
		{
			strText = _T("Vs");
		}
		break;
	case AREMA23:
		{
			strText = _T("vs");
		}
		break;
	case Eurocode2_2_05:
	case IRC_112_11:
	case IRC_112_20:
	case IRS_RC:
	case IS456_2000:
		{
			strText = _T("V_Rds");
		}
		break;
	case BS5400_4_90:
	case TMH07_89:
		{
			strText = _T("v_s");
		}
		break;
	case SNiP_20503_84RC:
	case SNiP_20503_84RC_MKS:
	case SP_35_13330_11RC:
	case SP_35_13330_11RC_MKS:
		{
			strText = _T("Qrs");
		}
		break;
	case AS5100_5_2017:
		{
			strText = strPi + _T("Vus");
		}
		break;
	default:
		{
			strText = strPi + _T("Vs");
		}
		break;
	}

	return strText;
}

CString CDgnSymbol::GetpVsy(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	return GetpVs(nDgnCode, strPi) + _T("y");
}

CString CDgnSymbol::GetpVsz(const int nDgnCode, CString strPi)
{
	// for Graphic Output.
	return GetpVs(nDgnCode, strPi) + _T("z");
}

CString CDgnSymbol::GetStar(const long nCount)
{
	CString csStar;
	for ( long i = 0; i < nCount; ++i )
	{
		csStar += _T("*");
	}
	return csStar;
}

CString CDgnSymbol::JoinString(const std::vector<CString>& vStr)
{
	CString csJoin;
	for ( const CString& cs : vStr )
	{
		csJoin += cs;
	}
	return csJoin;
}