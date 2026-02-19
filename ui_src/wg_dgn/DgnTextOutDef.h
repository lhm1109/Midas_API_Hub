#if !defined(DGNTEXTOUT_DEF_H__)
#define DGNTEXTOUT_DEF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_VALUE_LEN 12
#define DEFAULT_VALUE_DEC  4

#define DEFAULT_STRLEN 12

#define STR_OK    _T("---> OK")
#define STR_NG    _T("---> NG")
#define STR_SMALL _T("<")
#define STR_BIGGR _T(">")

enum class EN_DIV_TYPE
{
    DBDOT = 0,
    EQUAL,
    BLANK,
    COMMA,
    COLON,
    EMPTY
};

enum class EN_ALIGN_TYPE
{
    RIGHT = 0,
    LEFT,
    CENTER
};


#endif // !defined(DGNTEXTOUT_DEF_H__)