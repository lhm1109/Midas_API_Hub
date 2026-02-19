// DgnSectUtil.cpp: implementation of the CDgnSectUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnSectUtil.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_base\wg_base_Product.h"

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDgnSectUtil::CDgnSectUtil()
{
	m_dZero_Size = 1.0E-07; 
	m_dZero_Area = 1.0E-07; 
	m_dZero_I	= 1.0E-07; 
}

CDgnSectUtil::~CDgnSectUtil()
{

}

BOOL CDgnSectUtil::Is_TaperedSect(const T_SECT_D *pSectD) const
{
	return (pSectD->nStype == D_SECT_TYPE_TAPERED);
}

bool CDgnSectUtil::Is_StlBoxSect(const T_SECT_D *pSectD) const
{
	switch (Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		return true;
	default:
		return false;
	}	
}

int	 CDgnSectUtil::Get_RealSectType(const T_SECT_D *pSectD) const
{
	return Is_TaperedSect(pSectD) ? pSectD->SectBefore.nStype : pSectD->nStype;
}

int CDgnSectUtil::Get_WebPartNum(const T_SECT_D *pSectD)
{
	switch (Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_STLG_I: 
	case D_SECT_TYPE_COMPO_STLG_I: 
	case D_SECT_TYPE_COMPO_I: 
		{
			return 1;
		}
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB: 
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB: 
		{
			return 2;
		}
	default:
		{
			ASSERT(0);
			return 0;
		}
	}	
}

double CDgnSectUtil::Get_Size_bft(const T_SECT_D *pSectD, bool bPosiI)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_COMPO_STLG_TUB: 
		{
			return Get_Size_bft_l(pSectD, bPosiI) + Get_Size_bft_r(pSectD, bPosiI);
		}
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_STLG_B:	
		{
			return Get_Size_bft_l(pSectD, bPosiI) + Get_Size_bft_m(pSectD, bPosiI) + Get_Size_bft_r(pSectD, bPosiI);
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_bfb(const T_SECT_D *pSectD, bool bPosiI)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_STLG_I:	
		{
			return Get_Size_bfb_l(pSectD, bPosiI) + Get_Size_bfb_r(pSectD, bPosiI);
		}
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return Get_Size_bfb_l(pSectD, bPosiI) + Get_Size_bfb_m(pSectD, bPosiI) + Get_Size_bfb_r(pSectD, bPosiI);
		}
	default:						
		{
			return 0.0;
		}
	}
}


double CDgnSectUtil::Get_Size_bft_l(const T_SECT_D *pSectD, bool bPosiI)		
{
	const T_SECT_SECTBASE_D& CurSect = GetCurrentSect(pSectD, bPosiI);

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[2]/2.0;
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[3];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[0];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_bft_m(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{	
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_STLG_I:
		{
			return 0.0;
		}
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[2];
		}	
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[1];
		}	
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_bft_r(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = GetCurrentSect(pSectD, bPosiI);

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[2]/2.0;
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[3];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[1];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[2];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0; 
		}
	}
}		

double CDgnSectUtil::Get_Size_bfb_l(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = GetCurrentSect(pSectD, bPosiI);

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[4]/2.0;
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[6];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[2];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[3];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0; 
		}
	}
}

double CDgnSectUtil::Get_Size_bfb_m(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{	
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_STLG_I:
		{
			return 0.0;
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[5];
		}	
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[4];
		}	
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}		
}

double CDgnSectUtil::Get_Size_bfb_r(const T_SECT_D *pSectD, bool bPosiI)	
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[4]/2.0;
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[6];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[3];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[5];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_bft_ll(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_TUB:	
		{
			return CurSect.Size[8];
		}
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[11];
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_bft_rr(const T_SECT_D *pSectD, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_TUB:	
		{
			return CurSect.Size[8];
		}
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[12];
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_tw(const T_SECT_D *pSectD, EN_WEB_THK enThk, bool bPosiI)
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[1];
		}
	case D_SECT_TYPE_COMPO_B:	 	
		{
			const double dtw = CurSect.Size[1];
			switch (enThk)
			{
			case EN_WEB_THK_LEFT: 
			case EN_WEB_THK_RIGHT:
				{
					return dtw;	
				}
			case EN_WEB_THK_BOTH:
				{
					return 2.0 * dtw;
				}
			default:
				{
					ASSERT(0);
					return 0.0;	
				}
			}
		} 
	case D_SECT_TYPE_COMPO_TUB:	
		{
			const double dtw = CurSect.Size[1];
			switch (enThk)
			{
			case EN_WEB_THK_LEFT: 
			case EN_WEB_THK_RIGHT:
				{
					return dtw;
				}
			case EN_WEB_THK_BOTH:
				{
					return 2.0 * dtw;
				}
			default:
				{
					ASSERT(0);
					return 0.0;
				}
			}
		} 
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[7];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			switch (enThk)
			{
			case EN_WEB_THK_LEFT:
				{
					return CurSect.Size[9];
				}
			case EN_WEB_THK_RIGHT:
				{
					return CurSect.Size[10];
				}
			case EN_WEB_THK_BOTH:
				{
					return CurSect.Size[9] + CurSect.Size[10];
				}
			default:
				{
					ASSERT(0);
					return 0.0;
				}
			}
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	case D_SECT_TYPE_COMPO_G:
	default: 
		{
			ASSERT(0);
			return 0.0;
		}
	} 
}

double CDgnSectUtil::Get_Size_tft(const T_SECT_D *pSectD, bool bPosiI)		
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;
	
	switch(Get_RealSectType(pSectD))
	{
	case  D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[3];
		}
	case  D_SECT_TYPE_COMPO_B:
	case  D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[4];
		}	
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[5];
		}
	case D_SECT_TYPE_COMPO_STLG_B:		
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[7];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B: 		
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_tfb(const T_SECT_D *pSectD, bool bPosiI)	
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
		{
			return CurSect.Size[5];
		}
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[7];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[6];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[8];
		}	
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_hw(const T_SECT_D *pSectD, bool bPosiI)	
{
	const T_SECT_SECTBASE_D& CurSect = IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;

	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			return CurSect.Size[0];
		}
	case D_SECT_TYPE_COMPO_STLG_I:
		{
			return CurSect.Size[4];
		}
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return CurSect.Size[6];
		}
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:	
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_Inclind_hw(const T_SECT_D *pSectD, int nType/* = 0*/, bool bPosiI)
{
	double dhw = Get_Size_hw(pSectD, bPosiI);

	switch(Get_RealSectType(pSectD))
	{
	case  D_SECT_TYPE_COMPO_I:		{return dhw;}
	case  D_SECT_TYPE_COMPO_B:	 	
		{
			double dbft		= Get_Size_bft_m(pSectD, bPosiI);
			double dbfb		= Get_Size_bfb_m(pSectD, bPosiI);
			double dL		= fabs(dbft - dbfb) / 2.0;
			double dAngle	= dhw < m_dZero_Size ? 0.0 : atan(dL / dhw);
			double dhw_In	= cos(dAngle) < m_dZero_Size ? 0.0 : dhw / cos(dAngle);

			switch (nType)
			{
			case 0: 
			case 1: {return dhw_In;}
			default:{return 0.0;}
			}
		} 
	case  D_SECT_TYPE_COMPO_TUB:	
		{
			double dbft_m	= Get_Size_bft_m(pSectD, bPosiI);
			double dbfb_m	= Get_Size_bfb_m(pSectD, bPosiI);
			double dbft_ll	= Get_Size_bft_ll(pSectD, bPosiI);
			double dbft_l	= Get_Size_bft_l(pSectD, bPosiI);
			double dL		= fabs((dbft_m - dbfb_m) / 2.0 + dbft_l - dbft_ll);
			double dAngle	= dhw < m_dZero_Size ? 0.0 : atan(dL / dhw);
			double dhw_In	= cos(dAngle) < m_dZero_Size ? 0.0 : dhw / cos(dAngle);

			switch (nType)
			{
			case 0: 
			case 1:		{return dhw_In;}
			default:	{return 0.0;   }
			}
		} 
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_B:
	default:
		{
			ASSERT(0);
			return 0.0; 
		}
	} 
}

double CDgnSectUtil::Get_Size_WebSlope(const T_SECT_D *pSectD, int nType/* = 0*/, bool bPosiI)
{
	double dhw		= Get_Size_hw(pSectD, bPosiI);
	double dAngle	= 0.0;

	switch( Get_RealSectType(pSectD))
	{
	case  D_SECT_TYPE_COMPO_I:		
		{
			return acos(1.0);
		}
	case  D_SECT_TYPE_COMPO_B:	 	
		{
			double dbft		= Get_Size_bft_m(pSectD, bPosiI);
			double dbfb		= Get_Size_bfb_m(pSectD, bPosiI);
			double dL		= fabs(dbft - dbfb) / 2.0;
			dAngle	= dhw < m_dZero_Size ? 0.0 : atan(dL / dhw);

			switch (nType)
			{
			case 0: 
			case 1:		{return dAngle;}
			default:	{return 0.0;   }
			}
		} 
	case  D_SECT_TYPE_COMPO_TUB:	
		{
			double dbft_m	= Get_Size_bft_m(pSectD, bPosiI);
			double dbfb_m	= Get_Size_bfb_m(pSectD, bPosiI);
			double dbft_ll	= Get_Size_bft_ll(pSectD, bPosiI);
			double dbfb_l	= Get_Size_bfb_l(pSectD, bPosiI);
			double dL		= fabs(dbft_m+2.0*dbft_ll - dbfb_m) / 2.0;
			dAngle	= dhw < m_dZero_Size ? 0.0 : atan(dL / dhw);

			switch (nType)
			{
			case 0: 
			case 1:		{return dAngle;}
			default:	{return 0.0;   }
			}
		} 
	case D_SECT_TYPE_STLG_I:	{return 0.0;}
	case D_SECT_TYPE_STLG_B:	{return 0.0;}
	case D_SECT_TYPE_COMPO_STLG_I:	{return 0.0;}
	case D_SECT_TYPE_COMPO_STLG_B:	{return 0.0;}
	case D_SECT_TYPE_COMPO_STLG_TUB: {return 0.0;}
	case D_SECT_TYPE_COMPO_G: {return 0.0; }
	default: {ASSERT(0); return 0.0;}
	} 
}

double CDgnSectUtil::Get_Size_H(const T_SECT_D *pSectD, bool bPosiI)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:	
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:		
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return Get_Size_hw(pSectD, bPosiI) + Get_Size_tft(pSectD, bPosiI) + Get_Size_tfb(pSectD, bPosiI);
		}	
	default:	
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_Bc(const T_SECT_D *pSectD)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return pSectD->SectAfter.SectJ.Size[0];
		}
	default:						
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_tc(const T_SECT_D *pSectD)	
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return pSectD->SectAfter.SectJ.Size[1];
		}
	default:						
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_Hh(const T_SECT_D *pSectD)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return pSectD->SectAfter.SectJ.Size[2];
		}
	default:						
		{
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_CTC(const T_SECT_D *pSectD)
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return pSectD->SectAfter.SectI.Size[1];
		}
	default:						
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_Ht(const T_SECT_D *pSectD, bool bPosiI)		
{
	switch(Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_I:		
	case D_SECT_TYPE_COMPO_B:		
	case D_SECT_TYPE_COMPO_TUB:	
	case D_SECT_TYPE_STLG_I:	
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_I:	
	case D_SECT_TYPE_COMPO_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return (Get_Size_H(pSectD, bPosiI) + Get_Size_tc(pSectD) + Get_Size_Hh(pSectD));
		}
	default:						
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_DistTopFlange(const T_SECT_D* pSectD, bool bPosiI)
{
	switch (Get_RealSectType(pSectD))
	{
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_STLG_I:		
	case D_SECT_TYPE_STLG_B:	
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:		
		{			
			return pSectD->SectAfter.SectI.Size[4];
		}
	default:
		{ 
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Size_DistBotFlange(const T_SECT_D* pSectD, bool bPosiI)
{
	switch (Get_RealSectType(pSectD))
	{	
	case D_SECT_TYPE_STLG_I:		
	case D_SECT_TYPE_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{			
			return pSectD->SectAfter.SectI.Size[5];
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
}

double CDgnSectUtil::Get_Area_TopFlange(const T_SECT_D *pSectD, bool bPosiI)
{
	return (Get_Size_bft(pSectD, bPosiI) * Get_Size_tft(pSectD, bPosiI));
}

double CDgnSectUtil::Get_Area_BotFlange(const T_SECT_D *pSectD, bool bPosiI)
{
	return (Get_Size_bfb(pSectD, bPosiI) * Get_Size_tfb(pSectD, bPosiI));
}

double CDgnSectUtil::Get_Area_Web(const T_SECT_D *pSectD, EN_WEB_THK enThk, bool bPosiI)
{
	switch (enThk)
	{
	case EN_WEB_THK_LEFT:
	case EN_WEB_THK_RIGHT:
		{
			return Get_Size_Inclind_hw(pSectD, enThk, bPosiI) * Get_Size_tw(pSectD, enThk, bPosiI);
		}
	case EN_WEB_THK_BOTH:
		{
			return (Get_Size_Inclind_hw(pSectD, EN_WEB_THK_LEFT, bPosiI) * Get_Size_tw(pSectD, EN_WEB_THK_LEFT, bPosiI)
				+ Get_Size_Inclind_hw(pSectD, EN_WEB_THK_RIGHT, bPosiI) * Get_Size_tw(pSectD, EN_WEB_THK_RIGHT, bPosiI));
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}
	}
	return 0.0;
}

double CDgnSectUtil::Get_Area_ConcreteSlab(const T_SECT_D *pSectD)
{
	return (Get_Size_tc(pSectD) * Get_Size_Bc(pSectD));
}

bool CDgnSectUtil::Calc_Stiffener_Area(int nType, const T_STIFF_SHAPE_D& InD, double& dArea)
{
	dArea = 0.0;

	switch (nType)
	{
	case 0: // Flat
		{
			double db = InD.dSize[1];
			double dh = InD.dSize[0];

			dArea = db * dh;

			break;
		}
	case 1: // Tee
		{
			double dh    = InD.dSize[0];
			double dbft  = InD.dSize[1];
			double dtw   = InD.dSize[2];
			double dtft  = InD.dSize[3];
			double dhw   = dh - dtft;

			dArea = dhw * dtw + dtft * dbft;

			break;
		}
	case 2: // U-Rib
		{
			double H   = InD.dSize[0];
			double B1  = InD.dSize[1];
			double B2  = InD.dSize[2];
			double t   = InD.dSize[3];
			double R   = InD.dSize[4];

			double Pi	 = 4.*atan(1.);
			double dAng	 = atan((B1-B2)/(2.*H));
			double dAngR = Pi/2. - dAng;

			double L1 = (H - (R+t) - (t/2.)*sin(dAng) + (R+t/2.)*cos(dAngR))/cos(dAng);
			double L2 = B2 - 2.*(R+t)*tan(dAngR/2.);
			double L3 = (R+t/2.)*dAngR;

			double A1 = L1*t;	// Diagonal.
			double A2 = L2*t;	// Horizontal.
			double A3 = L3*t;	// Fillet.

			dArea = 2.*(A1+A3) + A2;
			
			break;
		}
	default:
		break;
	}

	return true;
}

bool CDgnSectUtil::Calc_Stiffener_Iyy(int nType, const T_STIFF_SHAPE_D& InD, double& dIyy, double& dCzp, double& dCzm)
{
	dIyy = 0.0;

	switch (nType)
	{
	case 0: // Flat
		{
			double db = InD.dSize[1];
			double dh = InD.dSize[0];
			dCzp = dh / 2.0;
			dCzm = dh / 2.0;
			dIyy = db * pow(dh, 3) / 12.0;

			break;
		}
	case 1: // Tee
		{
			double dh    = InD.dSize[0];
			double dbft  = InD.dSize[1];
			double dtw   = InD.dSize[2];
			double dtft  = InD.dSize[3];
			double dhw   = dh - dtft;

			double dArea = dhw * dtw + dtft * dbft;
			dCzm = dArea < m_dZero_Area ? 0.0 : (dtw*dhw*dhw/2.0 + dbft*dtft*(dhw+dtft/2.0))/dArea;
			dCzp = dh - dCzm;
			dIyy = dtw*pow(dhw, 3)/12.0 + dtw*dhw*pow(dCzm-dhw/2.0,2) + dbft*pow(dtft, 3)/12.0 + dbft*dtft*pow(dhw+dtft/2.0-dCzm,2);

			break;
		}
	case 2: // U-Rib
		{
			double H   = InD.dSize[0];
			double B1  = InD.dSize[1];
			double B2  = InD.dSize[2];
			double t   = InD.dSize[3];
			double R   = InD.dSize[4];

			double Pi	 = 4.*atan(1.);
			double dAng	 = atan((B1-B2)/(2.*H));
			double dAngR = Pi/2. - dAng;

			double L1 = (H - (R+t) - (t/2.)*sin(dAng) + (R+t/2.)*cos(dAngR))/cos(dAng);
			double L2 = B2 - 2.*(R+t)*tan(dAngR/2.);
			double L3 = (R+t/2.)*dAngR;

			double A1 = L1*t;	// Diagonal.
			double A2 = L2*t;	// Horizontal.
			double A3 = L3*t;	// Fillet.
			double dArea = 2.*(A1+A3) + A2;

			double dDistZ1 = H - (t/2.)*sin(dAng) - L1*cos(dAng)/2.;
			double dDistZ2 = t/2.;
			double dDistZ3 = (R+t) - (R+t/2.)*cos(dAngR/2.);

			double dDistY1 = B2/2. + dDistZ1*tan(dAng);
			double dDistY2 = 0.;
			double dDistY3 = B2/2. + (R+t/2.)*sin(dAngR/2.);

			double dRyz1 = 0.0;
			double dRyr1 = t*pow(L1,3)/12.;
			double dRzr1 = L1*pow(t,3)/12.;
			double dRy1  = dRyr1*pow(cos(dAng),2) + dRzr1*pow(sin(dAng),2) - dRyz1*sin(2.*dAng);
			double dRz1  = dRyr1*pow(sin(dAng),2) + dRzr1*pow(cos(dAng),2) + dRyz1*sin(2.*dAng);
			double dRy2  = L2*pow(t,3)/12.;
			double dRz2  = t*pow(L2,3)/12.;
			double dRy3  = t*((dAngR+sin(dAngR)*cos(dAngR))/2. - pow(sin(dAngR),2)/dAngR)*pow(R+t/2.,3);
			double dRz3  = t*((dAngR-sin(dAngR)*cos(dAngR))/2. - pow(1.-cos(dAngR),2)/dAngR)*pow(R+t/2.,3);

			dCzm = dArea < m_dZero_Area ? 0.0 : (2.*(A1*dDistZ1) + (A2*dDistZ2) + 2.*(A3*dDistZ3)) / dArea;
			dCzp = H - dCzm;

			dIyy = 2.*(dRy1+A1*pow(dCzm-dDistZ1,2)) + dRy2+A2*pow(dCzm-dDistZ2,2) + 2.*(dRy3+A3*pow(dCzm-dDistZ3,2));

			break;
		}
	default:
		break;
	}

	return true;
}

bool CDgnSectUtil::Calc_Stiffener_Izz(int nType, const T_STIFF_SHAPE_D& InD, double& dIzz, double& dCyp, double& dCym)
{
	dIzz = 0.0;

	switch (nType)
	{
	case 0: // Flat
		{
			double db = InD.dSize[1];
			double dh = InD.dSize[0];
			dCyp = db / 2.0;
			dCym = db / 2.0;
			dIzz = dh * pow(db, 3) / 12.0;

			break;
		}
	case 1: // Tee
		{
			// 차후 추가!
			double dh    = InD.dSize[0];
			double dbft  = InD.dSize[1];
			double dtw   = InD.dSize[2];
			double dtft  = InD.dSize[3];
			double dhw   = dh - dtft;

			dCyp = 0.0;
			dCym = 0.0;
			dIzz = 0.0;

			break;
		}
	case 2: // U-Rib
		{
			// 차후 추가!
			double H   = InD.dSize[0];
			double B1  = InD.dSize[1];
			double B2  = InD.dSize[2];
			double t   = InD.dSize[3];
			double R   = InD.dSize[4];

			double Pi	 = 4.*atan(1.);
			double dAng	 = atan((B1-B2)/(2.*H));
			double dAngR = Pi/2. - dAng;

			double L1 = (H - (R+t) - (t/2.)*sin(dAng) + (R+t/2.)*cos(dAngR))/cos(dAng);
			double L2 = B2 - 2.*(R+t)*tan(dAngR/2.);
			double L3 = (R+t/2.)*dAngR;

			double A1 = L1*t;	// Diagonal.
			double A2 = L2*t;	// Horizontal.
			double A3 = L3*t;	// Fillet.
			double dArea = 2.*(A1+A3) + A2;

			double dDistZ1 = H - (t/2.)*sin(dAng) - L1*cos(dAng)/2.;
			double dDistZ2 = t/2.;
			double dDistZ3 = (R+t) - (R+t/2.)*cos(dAngR/2.);

			double dDistY1 = B2/2. + dDistZ1*tan(dAng);
			double dDistY2 = 0.;
			double dDistY3 = B2/2. + (R+t/2.)*sin(dAngR/2.);

			double dRyz1 = 0.0;
			double dRyr1 = t*pow(L1,3)/12.;
			double dRzr1 = L1*pow(t,3)/12.;
			double dRy1  = dRyr1*pow(cos(dAng),2) + dRzr1*pow(sin(dAng),2) - dRyz1*sin(2.*dAng);
			double dRz1  = dRyr1*pow(sin(dAng),2) + dRzr1*pow(cos(dAng),2) + dRyz1*sin(2.*dAng);
			double dRy2  = L2*pow(t,3)/12.;
			double dRz2  = t*pow(L2,3)/12.;
			double dRy3  = t*((dAngR+sin(dAngR)*cos(dAngR))/2. - pow(sin(dAngR),2)/dAngR)*pow(R+t/2.,3);
			double dRz3  = t*((dAngR-sin(dAngR)*cos(dAngR))/2. - pow(1.-cos(dAngR),2)/dAngR)*pow(R+t/2.,3);

			dCyp = 0.0;
			dCym = 0.0;
			dIzz = 0.0;

			break;
		}
	default:
		break;
	}

	return true;
}

bool CDgnSectUtil::Get_StlSect_StiffenerD(const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePosFlg, bool bReservePosWeb)
{

	return true;
}

bool CDgnSectUtil::Get_Flange_StiffenerD(bool bIsTop, int nPart, const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos)
{
	rD.Initialize();
	const T_SECT_STIFF_D* pStiffOldType		= &pSectD->SectBefore.SectI.Stiffener;
	const T_SECT_SOD_STIFF_D* pStiffNewType = &pSectD->SectBefore.SectI.SODStiffener;

	int nSectT = Get_RealSectType(pSectD);

	if(bIsTop) // Top Flange Part
	{

	}
	else  // Bottom Flange Part
	{

	}


	return true;
}

bool CDgnSectUtil::Get_Web_StiffenerD(bool bIsLeft, const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos)
{
	rD.Initialize();
	const T_SECT_STIFF_D* pStiffOldType		= &pSectD->SectBefore.SectI.Stiffener;
	const T_SECT_SOD_STIFF_D* pStiffNewType = &pSectD->SectBefore.SectI.SODStiffener;

	int nSectT	= Get_RealSectType(pSectD);
	double dHw	= Get_Size_hw(pSectD);

	if(bIsLeft) // Left Web
	{
		switch (nSectT)
		{
		case D_SECT_TYPE_COMPO_I:
		case D_SECT_TYPE_COMPO_B:
		case D_SECT_TYPE_COMPO_TUB:
			{
				const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD = &pStiffOldType->aStiffLeft;
				int nSize = paStiffD->GetSize();
				if(pStiffOldType->nRefPos2==1) bReservePos = TRUE;
				Convert_WebStiffenerD_OldT(paStiffD, pStiffOldType, dHw, rD, bReservePos);

				break;
			}
		case D_SECT_TYPE_STLG_I:
		case D_SECT_TYPE_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_I:
		case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_TUB:
			{
				CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> *paStiffD = nullptr;
				break;
			}
		default:
			break;
		}
	}
	else  // Right Web
	{
		switch (nSectT)
		{
		case D_SECT_TYPE_COMPO_I:
		case D_SECT_TYPE_COMPO_B:
		case D_SECT_TYPE_COMPO_TUB:
			{
				const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD = &pStiffOldType->aStiffRight;
				if (pStiffOldType->nRefPos2 == 1) bReservePos = TRUE;
				Convert_WebStiffenerD_OldT(paStiffD, pStiffOldType, dHw, rD, bReservePos);
				break;
			}
		case D_SECT_TYPE_STLG_I:
		case D_SECT_TYPE_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_I:
		case D_SECT_TYPE_COMPO_STLG_B:
		case D_SECT_TYPE_COMPO_STLG_TUB:
			{
				CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> *paStiffD = nullptr;
				break;
			}
		default:
			break;
		}
	}

	return true;
}

bool Convert_FlangeStiffenerD_OldT(const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD, const T_SECT_STIFF_D* pStiffD,
								   double dLength, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos)
{
	double dDistCum = 0.0;
	int nSize = paStiffD->GetSize();

	if(!bReservePos) // Left -> Right
	{

	}
	else // Right - > Left
	{

	}


	return true;
}

bool CDgnSectUtil::Convert_WebStiffenerD_OldT(const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD, const T_SECT_STIFF_D* pStiffD,
												double dHw, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos)
{
	double dDistCum = 0.0;
	int nSize = paStiffD->GetSize();

	rD.arStiffD.SetSize(nSize);

	if(!bReservePos) // Top -> Bottom
	{
		for (int i = 0; i < nSize; ++i)
		{
			const T_STIFF_PROFILE_D& StiffD = paStiffD->GetAt(i);
			const T_STIFF_SHAPE_D& StiffShapeD = pStiffD->aStiffShape.GetAt(StiffD.nShape);
			T_DGN_STL_STIFFENER_UNIT& StiffUnit = rD.arStiffD[i];

			dDistCum			+= StiffD.dSpacing;
			StiffUnit.strName	= StiffShapeD.strName;
			StiffUnit.bCalc		= StiffD.bCalc;
			StiffUnit.nType		= StiffShapeD.nType;
			StiffUnit.nPosition	= pStiffD->nPosition;   
			StiffUnit.dSpace	= StiffD.dSpacing;
			StiffUnit.dSpace_L	= StiffD.dSpacing;
			StiffUnit.dSpace_R	= i < nSize - 1 ?  paStiffD->GetAt(i+1).dSpacing : dHw - StiffUnit.dDist;
			StiffUnit.dDist		= dDistCum;
			Calc_Stiffener_Area(StiffShapeD.nType, StiffShapeD, StiffUnit.dArea);
			Calc_Stiffener_Iyy(StiffShapeD.nType, StiffShapeD, StiffUnit.dIyy, StiffUnit.dCzp, StiffUnit.dCzm);
			Calc_Stiffener_Izz(StiffShapeD.nType, StiffShapeD, StiffUnit.dIzz, StiffUnit.dCyp, StiffUnit.dCym);
			for(int j = 0;  j < nStiffenerSizeIndex; ++j){StiffUnit.dSize[j] = StiffShapeD.dSize[j];}
		}
	}
	else // Bottom - > Top
	{
		for (int i = 0; i < nSize; ++i)
		{
			dDistCum += paStiffD->GetAt(i).dSpacing;
		}

		for (int i = 0; i < nSize; ++i)
		{
			const T_STIFF_PROFILE_D& StiffD = paStiffD->GetAt(nSize - i - 1);
			const T_STIFF_SHAPE_D& StiffShapeD = pStiffD->aStiffShape.GetAt(StiffD.nShape);
			T_DGN_STL_STIFFENER_UNIT& StiffUnit = rD.arStiffD[i];

			StiffUnit.strName	= StiffShapeD.strName;
			StiffUnit.bCalc		= StiffD.bCalc;
			StiffUnit.nType		= StiffShapeD.nType;
			StiffUnit.nPosition	= pStiffD->nPosition;  
			StiffUnit.dSpace	= (i == 0) ? dHw - dDistCum : paStiffD->GetAt(nSize - i).dSpacing;
			StiffUnit.dSpace_L	= rD.arStiffD[i].dSpace;
			StiffUnit.dSpace_R	= StiffD.dSpacing;
			StiffUnit.dDist		+= rD.arStiffD[i].dSpace;
			Calc_Stiffener_Area(StiffShapeD.nType, StiffShapeD, StiffUnit.dArea);
			Calc_Stiffener_Iyy(StiffShapeD.nType, StiffShapeD, StiffUnit.dIyy, StiffUnit.dCzp, StiffUnit.dCzm);
			Calc_Stiffener_Izz(StiffShapeD.nType, StiffShapeD, StiffUnit.dIzz, StiffUnit.dCyp, StiffUnit.dCym);
			for(int j = 0;  j < nStiffenerSizeIndex; ++j){StiffUnit.dSize[j] = StiffShapeD.dSize[j];}
		}
	}

	return true;
}

bool CDgnSectUtil::IsTaperedPosJ(const T_SECT_D* pSectD, bool bPosiI) const
{
	if (bPosiI) { return false; }
	return Is_TaperedSect(pSectD) ? true : false;
}

const T_SECT_SECTBASE_D& CDgnSectUtil::GetCurrentSect(const T_SECT_D* pSectD, bool bPosiI) const
{
	return IsTaperedPosJ(pSectD, bPosiI) ? pSectD->CmpTapJ : pSectD->SectBefore.SectI;
}