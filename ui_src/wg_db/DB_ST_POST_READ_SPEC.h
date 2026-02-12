#ifndef __DB_ST_POST_READ_SPEC_H__
#define __DB_ST_POST_READ_SPEC_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define _REAC_RESP_BN _REAC_STLD_BN
#define _LRAC_RESP_BN _LRAC_STLD_BN
#define _DISP_RESP_BN _DISP_STLD_BN 
#define _STRT_RESP_BN _STRT_STLD_BN
#define _STRB_RESP_BN _STRB_STLD_BN
#define _SIMB_RESP_BN _SIMB_STLD_BN
#define _FSIM_RESP_BN _FSIM_STLD_BN
#define _SRIM_RESP_BN _SRIM_STLD_BN
#define _STRM_RESP_BN _STRM_STLD_BN
#define _STRP_RESP_BN _STRP_STLD_BN
#define _STRS_RESP_BN _STRS_STLD_BN
#define _STRW_RESP_BN _STRW_STLD_BN
#define _STPN_RESP_BN _STPN_STLD_BN
#define _ELNK_RESP_BN _ELNK_STLD_BN
#define _NLNK_RESP_BN _NLNK_STLD_BN
#define _STCM_RESP_BN _STCM_STLD_BN
#define _STCM_RESP_BN_WP _STCM_STLD_BN_WP
#define _PTSB_RESP_BN _SIMB_STLD_BN
#define _PTPF_RESP_BN _SIMB_STLD_BN

// story shear
struct _STSR_RESP_BN
{
// dStoryShear
//                component : InertiaForce_X, InertiaForce_Y, Shear_X, Shear_Y, 
//                            Shear+Spring_X, Shear+Spring_Y, SpringForce_X, SpringForce_Y
	double dStoryShear[19];
	void ConvertOut(T_STSR_D& Data)
	{
		Data.Initialize();
		Data.dInertiaForce[0]					=	dStoryShear[1];
		Data.dInertiaForce[1]					=	dStoryShear[2];
		Data.dStoryShear[0]						=	dStoryShear[3];
		Data.dStoryShear[1]						=	dStoryShear[4];
		Data.dStoryShearWithSpring[0]	=	dStoryShear[5];
		Data.dStoryShearWithSpring[1]	=	dStoryShear[6];
		Data.dSpringForce[0]					=	dStoryShear[7];
		Data.dSpringForce[1]					=	dStoryShear[8];
	}	
};

// story shear // Dummy Story 정보 (다탑의 경계층 추가 결과)
struct _STSD_RESP_BN
{
	// dStoryShear
	double dStorSerial_cur;
	double dStorSerial_up;
	double dStoryShear[2];
	void ConvertOut(T_STSD_D& Data)
	{
		Data.Initialize();
		Data.dStoryShear[0]						=	dStoryShear[0];
		Data.dStoryShear[1]						=	dStoryShear[1];
	}	
};

#define _SSRT_RESP_BN _SSRT_STLD_BN

// KYE-HONG-20060321
// Nodal Acceleration
struct _NDAC_RESP_BN 
{
	double dblAccl[6]; // DX,DY,DZ,RX,RY,RZ
};

// KYE-HONG-20060321
// Nodal Inertia Force
struct _NDIF_RESP_BN
{
	double dblForce[6]; // FX,FY,FZ,MX,MY,MZ
};



#pragma pack()

#endif