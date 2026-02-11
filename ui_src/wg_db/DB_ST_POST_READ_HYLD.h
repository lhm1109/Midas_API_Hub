#ifndef __DB_ST_POST_READ_HYLD_H__
#define __DB_ST_POST_READ_HYLD_H__

#define MIDAS_SDK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define _DISP_HYLD_BN _DISP_STLD_BN 
#define _STRS_HYLD_BN _STRS_STLD_BN 


// Average Nodal Solid Stress
struct _AVGN_HYLD_BN
{
	// P1, P2, P3, Tresca, Vonmis
	double dAvgNodalVal[5];
};

// Solid Element Principal Stress
struct _PRST_HYLD_BN
{
	// (Center + 8), (P1, P2, P3, Tresca, Vonmis)
	double dPrincipalVal[9][5];
};

#pragma pack()

#endif