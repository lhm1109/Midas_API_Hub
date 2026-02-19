// DgnSectUtil.h: interface for the CDgnSectUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNSECTUTIL_DGN_H__)
#define __DGNSECTUTIL_DGN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT_SECT.h"
#include "..\wg_db\DB_ST_DT_UNIT.h"

#include "DGN_SECT_UTIL.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSectUtil  
{
public:
	enum EN_WEB_THK
	{
		EN_WEB_THK_LEFT = 0,
		EN_WEB_THK_RIGHT,
		EN_WEB_THK_BOTH
	};
	
	// Member Functions
public:
	CDgnSectUtil();
	virtual ~CDgnSectUtil();


	// For Concrete Section



	// For Steel Section


	// For PSC Section


	// For PSC Composite Section


	// For Steel Girder Section


	// For Steel Composite Section


	// About Section Type
	BOOL Is_TaperedSect(const T_SECT_D *pSectD) const;
	bool Is_StlBoxSect(const T_SECT_D *pSectD) const;
	int	 Get_RealSectType(const T_SECT_D *pSectD) const;

	// About Get Size Function
	int Get_WebPartNum(const T_SECT_D *pSectD);
	double Get_Size_bft(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : Width of Top Flange
	double Get_Size_bfb(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : Width of Bottom Flange
	double Get_Size_bft_l(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Top Flange Left
	double Get_Size_bft_m(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Top Flange Middle
	double Get_Size_bft_r(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Top Flange Right
	double Get_Size_bfb_l(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Bottom Flange Left
	double Get_Size_bfb_m(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Bottom Flange Middle
	double Get_Size_bfb_r(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Bottom Flange Right
	double Get_Size_bft_ll(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Top Flange Left : for Tub
	double Get_Size_bft_rr(const T_SECT_D *pSectD, bool bPosiI = true);					// Girder dimension : Width of Top Flange Right : for Tub
	double Get_Size_tw(const T_SECT_D *pSectD, EN_WEB_THK enThk = EN_WEB_THK_LEFT, bool bPosiI = true);		// Girder dimension : web thick, 0 = left , 1 = right , 2 = sum of both
	double Get_Size_tft(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : top flange thick.
	double Get_Size_tfb(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : bottom flange thick.
	double Get_Size_hw(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : web height
	double Get_Size_Inclind_hw(const T_SECT_D *pSectD, int nType = 0, bool bPosiI = true);// Girder dimension : web height considered slope
	double Get_Size_WebSlope(const T_SECT_D *pSectD, int nType = 0, bool bPosiI = true);	// Girder dimension : web slope [Unit : radians], 0 : Left, 1 : Right
	double Get_Size_H(const T_SECT_D *pSectD, bool bPosiI = true);						// Girder dimension : Total Height of girder
	double Get_Size_Bc(const T_SECT_D *pSectD);						// Slab dimension :   Slab Width
	double Get_Size_tc(const T_SECT_D *pSectD);						// Slab dimension :   Slab Thick.
	double Get_Size_Hh(const T_SECT_D *pSectD);						// Slab dimension :	  Hunch height
	double Get_Size_CTC(const T_SECT_D *pSectD);						// Composite dimension : Center to Center between each girder
	double Get_Size_Ht(const T_SECT_D *pSectD, bool bPosiI = true);						// Composite dimension : Total height of composite section
	double Get_Size_DistTopFlange(const T_SECT_D* pSectD, bool bPosiI = true); // composite dimension : distance from the left side of the top flange to the reference line.
	double Get_Size_DistBotFlange(const T_SECT_D* pSectD, bool bPosiI = true); // composite dimension : distance from the left side of the bottom flange to the reference line.

	// About Get Part Area Function
	double Get_Area_TopFlange(const T_SECT_D *pSectD, bool bPosiI = true);
	double Get_Area_BotFlange(const T_SECT_D *pSectD, bool bPosiI = true);
	double Get_Area_Web(const T_SECT_D *pSectD, EN_WEB_THK enThk = EN_WEB_THK_LEFT, bool bPosiI = true); // nPart = 0 : Left Web , 1 : Right Web,  2 : Both Web
	double Get_Area_ConcreteSlab(const T_SECT_D *pSectD);

	// About Calc Stiffener Section Property
	bool Calc_Stiffener_Area(int nType, const T_STIFF_SHAPE_D& InD, double& dArea);
	bool Calc_Stiffener_Iyy(int nType, const T_STIFF_SHAPE_D& InD, double& dIyy, double& dCzp, double& dCzm);
	bool Calc_Stiffener_Izz(int nType, const T_STIFF_SHAPE_D& InD, double& dIzz, double& dCyp, double& dCym);



	// About Get Stiffener Data for Steel Section
	bool Get_StlSect_StiffenerD(const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePosFlg, bool bReservePosWeb); 
	// 0 : Left Flange,  1 : Middle Flange,  2 :  Right Flange, //     bReservePos :  false : Left -> Right,  true : Right -> Left
	bool Get_Flange_StiffenerD(bool bIsTop, int nPart, const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos = false); 
	//     bReservePos :  false : Top -> Bottom,  true : Bottom -> Top
	bool Get_Web_StiffenerD(bool bIsLeft, const T_SECT_D *pSectD, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos = false);





protected:
	// About Convert Stiffener Data  for Steel Section
	bool Convert_FlangeStiffenerD_OldT(const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD, const T_SECT_STIFF_D* pStiffD,
									double dLength, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos = false);
	bool Convert_WebStiffenerD_OldT(const CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&>* paStiffD, const T_SECT_STIFF_D* pStiffD,
									double dHw, T_DGN_STL_STIFFENER_PART& rD, bool bReservePos = false);





private:
	bool IsTaperedPosJ(const T_SECT_D* pSectD, bool bPosiI) const;
	const T_SECT_SECTBASE_D& GetCurrentSect(const T_SECT_D* pSectD, bool bPosiI) const;
	

	// Member Variables
public:


protected:


private:
	double m_dZero_Size;
	double m_dZero_Area;
	double m_dZero_I;

};

#include "HeaderPost.h"

#endif // !defined(__DGNSECTUTIL_DGN_H__)