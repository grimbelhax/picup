#pragma once

#ifndef DEFINITIONS
#define DEFINITIONS

#include <windows.h>
#include <inttypes.h>

typedef struct _DNS_CACHE_ENTRY
{
     struct _DNS_CACHE_ENTRY *pNext; /* Pointer to next entry */
 #if defined(__midl) || defined(__WIDL__)
     [string] PWSTR pszName;         /* DNS Record Name */
 #else
     PWSTR pszName;                  /* DNS Record Name */
 #endif
     unsigned short wType1;          /* DNS Record Type 1 */
     unsigned short wType2;          /* DNS Record Type 2 */
     unsigned short wFlags;          /* DNS Record Flags */
 } DNS_CACHE_ENTRY, *PDNS_CACHE_ENTRY;

typedef enum _DnsSection
{
    DnsSectionQuestion,
    DnsSectionAnswer,
    DnsSectionAuthority,
    DnsSectionAddtional,
}
DNS_SECTION;


#define DNS_QUERY_STANDARD                  0x00000000
#define DNS_QUERY_NO_WIRE_QUERY             0x00000010
#define DNS_TYPE_A          0x0001      //  1
#define DNS_TYPE_NS         0x0002      //  2
#define DNS_TYPE_CNAME      0x0005      //  5
#define DNS_TYPE_SOA        0x0006      //  6
#define DNS_TYPE_PTR        0x000c      //  12
#define DNS_TYPE_MX         0x000f      //  15
#define DNS_TYPE_AAAA       0x001c      //  28
#define DNS_TYPE_ATMA       0x0022      //  34
#define DNS_TYPE_SRV        0x0021      //  33

#define PDNS_RECORD     PVOID

typedef _Return_type_success_(return == 0) LONG    DNS_STATUS;
typedef DNS_STATUS                                *PDNS_STATUS;

#ifdef UNICODE
#define DnsQuery DnsQuery_W
#else
#define DnsQuery DnsQuery_A
#endif

typedef enum
{
    DnsFreeFlat = 0,
    DnsFreeRecordList,
    DnsFreeParsedMessageFields
}
DNS_FREE_TYPE;


typedef DWORD   IP4_ADDRESS, *PIP4_ADDRESS;

typedef struct
{
    IP4_ADDRESS     IpAddress;
}
DNS_A_DATA, *PDNS_A_DATA;

typedef struct
{
    PWSTR           pNameHost;
}
DNS_PTR_DATAW, *PDNS_PTR_DATAW;

typedef struct
{
    PSTR            pNameHost;
}
DNS_PTR_DATAA, *PDNS_PTR_DATAA;

typedef struct
{
    PWSTR           pNamePrimaryServer;
    PWSTR           pNameAdministrator;
    DWORD           dwSerialNo;
    DWORD           dwRefresh;
    DWORD           dwRetry;
    DWORD           dwExpire;
    DWORD           dwDefaultTtl;
}
DNS_SOA_DATAW, *PDNS_SOA_DATAW;

typedef struct
{
    PSTR            pNamePrimaryServer;
    PSTR            pNameAdministrator;
    DWORD           dwSerialNo;
    DWORD           dwRefresh;
    DWORD           dwRetry;
    DWORD           dwExpire;
    DWORD           dwDefaultTtl;
}
DNS_SOA_DATAA, *PDNS_SOA_DATAA;

typedef struct
{
    PWSTR           pNameMailbox;
    PWSTR           pNameErrorsMailbox;
}
DNS_MINFO_DATAW, *PDNS_MINFO_DATAW;

typedef struct
{
    PSTR            pNameMailbox;
    PSTR            pNameErrorsMailbox;
}
DNS_MINFO_DATAA, *PDNS_MINFO_DATAA;

typedef struct
{
    PWSTR           pNameExchange;
    WORD            wPreference;
    WORD            Pad;        // keep ptrs DWORD aligned
}
DNS_MX_DATAW, *PDNS_MX_DATAW;

typedef struct
{
    PSTR            pNameExchange;
    WORD            wPreference;
    WORD            Pad;        // keep ptrs DWORD aligned
}
DNS_MX_DATAA, *PDNS_MX_DATAA;

typedef struct
{
    DWORD           dwStringCount;
#ifdef MIDL_PASS
    [size_is(dwStringCount)] PWSTR pStringArray[];
#else
    PWSTR           pStringArray[1];
#endif
}
DNS_TXT_DATAW, *PDNS_TXT_DATAW;

typedef struct
{
    DWORD           dwStringCount;
#ifdef MIDL_PASS
    [size_is(dwStringCount)] PSTR  pStringArray[];
#else
    PSTR            pStringArray[1];
#endif
}
DNS_TXT_DATAA, *PDNS_TXT_DATAA;

typedef struct
{
    DWORD           dwByteCount;
#ifdef MIDL_PASS
    [size_is(dwByteCount)] BYTE Data[];
#else
    BYTE            Data[1];
#endif
}
DNS_NULL_DATA, *PDNS_NULL_DATA;

typedef struct
{
    IP4_ADDRESS     IpAddress;
    UCHAR           chProtocol;
    BYTE            BitMask[1];
}
DNS_WKS_DATA, *PDNS_WKS_DATA;

typedef unsigned __int64 QWORD, *PQWORD;
typedef union
{
#ifdef _WIN64
    QWORD       IP6Qword[2];
#endif
    DWORD       IP6Dword[4];
    WORD        IP6Word[8];
    BYTE        IP6Byte[16];
#ifdef  IN6_ADDR
    IN6_ADDR    In6;
#endif
}
IP6_ADDRESS, *PIP6_ADDRESS;


typedef struct
{
    IP6_ADDRESS     Ip6Address;
}
DNS_AAAA_DATA, *PDNS_AAAA_DATA;

typedef struct
{
    WORD            wTypeCovered;
    BYTE            chAlgorithm;
    BYTE            chLabelCount;
    DWORD           dwOriginalTtl;
    DWORD           dwExpiration;
    DWORD           dwTimeSigned;
    WORD            wKeyTag;
    WORD            wSignatureLength;
    PWSTR           pNameSigner;
#ifdef MIDL_PASS
    [size_is(wSignatureLength)] BYTE  Signature[];
#else
    BYTE            Signature[1];
#endif
}
DNS_SIG_DATAW, *PDNS_SIG_DATAW, DNS_RRSIG_DATAW, *PDNS_RRSIG_DATAW;

typedef struct
{
    WORD            wTypeCovered;
    BYTE            chAlgorithm;
    BYTE            chLabelCount;
    DWORD           dwOriginalTtl;
    DWORD           dwExpiration;
    DWORD           dwTimeSigned;
    WORD            wKeyTag;
    WORD            wSignatureLength;
    PSTR            pNameSigner;
#ifdef MIDL_PASS
    [size_is(wSignatureLength)] BYTE  Signature[];
#else
    BYTE            Signature[1];
#endif
}
DNS_SIG_DATAA, *PDNS_SIG_DATAA, DNS_RRSIG_DATAA, *PDNS_RRSIG_DATAA;

typedef struct
{
    WORD            wFlags;
    BYTE            chProtocol;
    BYTE            chAlgorithm;
    WORD            wKeyLength;
    WORD            wPad;            // keep byte field aligned
#ifdef MIDL_PASS
    [size_is(wKeyLength)] BYTE Key[];
#else
    BYTE            Key[1];
#endif
}
DNS_KEY_DATA, *PDNS_KEY_DATA, DNS_DNSKEY_DATA, *PDNS_DNSKEY_DATA;

typedef struct
{
    DWORD           dwByteCount;
#ifdef MIDL_PASS
    [size_is(dwByteCount)] BYTE DHCID[];
#else
    BYTE            DHCID[1];
#endif
}
DNS_DHCID_DATA, *PDNS_DHCID_DATA;

typedef struct
{
    PWSTR           pNextDomainName;
    WORD            wTypeBitMapsLength;
    WORD            wPad;            // keep byte field aligned
#ifdef MIDL_PASS
    [size_is(wTypeBitMapsLength)] BYTE  TypeBitMaps[];
#else
    BYTE            TypeBitMaps[1];
#endif
}
DNS_NSEC_DATAW, *PDNS_NSEC_DATAW;

typedef struct
{
    PSTR            pNextDomainName;
    WORD            wTypeBitMapsLength;
    WORD            wPad;            // keep byte field aligned
#ifdef MIDL_PASS
    [size_is(wTypeBitMapsLength)] BYTE  TypeBitMaps[];
#else
    BYTE            TypeBitMaps[1];
#endif
}
DNS_NSEC_DATAA, *PDNS_NSEC_DATAA;

typedef struct
{
    BYTE            chAlgorithm;
    BYTE            bFlags;
    WORD            wIterations;
    BYTE            bSaltLength;
    BYTE            bHashLength;
    WORD            wTypeBitMapsLength;
#ifdef MIDL_PASS
    [size_is(bSaltLength+bHashLength+wTypeBitMapsLength)] BYTE  chData[];
#else
    BYTE            chData[1];
#endif
}
DNS_NSEC3_DATA, *PDNS_NSEC3_DATA;

typedef struct
{
    BYTE            chAlgorithm;
    BYTE            bFlags;
    WORD            wIterations;
    BYTE            bSaltLength;
    BYTE            bPad[3];        // keep salt field aligned
#ifdef MIDL_PASS
    [size_is(bSaltLength)] BYTE  pbSalt[];
#else
    BYTE            pbSalt[1];
#endif
}
DNS_NSEC3PARAM_DATA, *PDNS_NSEC3PARAM_DATA;

typedef struct
{
    BYTE            bCertUsage;
    BYTE            bSelector;
    BYTE            bMatchingType;
    WORD            bCertificateAssociationDataLength;
    BYTE            bPad[3];        // keep certificate association data field aligned
#ifdef MIDL_PASS
    [size_is(bCertificateAssociationDataLength)] BYTE  bCertificateAssociationData[];
#else
    BYTE            bCertificateAssociationData[1];
#endif
}
DNS_TLSA_DATA, *PDNS_TLSA_DATA;

typedef struct
{
    WORD            wKeyTag;
    BYTE            chAlgorithm;
    BYTE            chDigestType;
    WORD            wDigestLength;
    WORD            wPad;            // keep byte field aligned
#ifdef MIDL_PASS
    [size_is(wDigestLength)] BYTE  Digest[];
#else
    BYTE            Digest[1];
#endif
}
DNS_DS_DATA, *PDNS_DS_DATA;

typedef struct
{
    WORD            wDataLength;
    WORD            wPad;            // keep byte field aligned
#ifdef MIDL_PASS
    [size_is(wDataLength)] BYTE Data[];
#else
    BYTE            Data[1];
#endif
}
DNS_OPT_DATA, *PDNS_OPT_DATA;

typedef struct
{
    WORD            wVersion;
    WORD            wSize;
    WORD            wHorPrec;
    WORD            wVerPrec;
    DWORD           dwLatitude;
    DWORD           dwLongitude;
    DWORD           dwAltitude;
}
DNS_LOC_DATA, *PDNS_LOC_DATA;

typedef struct
{
    PWSTR           pNameNext;
    WORD            wNumTypes;
#ifdef MIDL_PASS
    [size_is(wNumTypes)] WORD wTypes[];
#else
    WORD            wTypes[1];
#endif
}
DNS_NXT_DATAW, *PDNS_NXT_DATAW;

typedef struct
{
    PSTR            pNameNext;
    WORD            wNumTypes;
#ifdef MIDL_PASS
    [size_is(wNumTypes)] WORD wTypes[];
#else
    WORD            wTypes[1];
#endif
}
DNS_NXT_DATAA, *PDNS_NXT_DATAA;

typedef struct
{
    PWSTR           pNameTarget;
    WORD            wPriority;
    WORD            wWeight;
    WORD            wPort;
    WORD            Pad;            // keep ptrs DWORD aligned
}
DNS_SRV_DATAW, *PDNS_SRV_DATAW;

typedef struct
{
    PSTR            pNameTarget;
    WORD            wPriority;
    WORD            wWeight;
    WORD            wPort;
    WORD            Pad;            // keep ptrs DWORD aligned
}
DNS_SRV_DATAA, *PDNS_SRV_DATAA;

typedef struct
{
    WORD            wOrder;
    WORD            wPreference;
    PWSTR           pFlags;
    PWSTR           pService;
    PWSTR           pRegularExpression;
    PWSTR           pReplacement;
}
DNS_NAPTR_DATAW, *PDNS_NAPTR_DATAW;

typedef struct
{
    WORD            wOrder;
    WORD            wPreference;
    PSTR            pFlags;
    PSTR            pService;
    PSTR            pRegularExpression;
    PSTR            pReplacement;
}
DNS_NAPTR_DATAA, *PDNS_NAPTR_DATAA;


#define DNS_ATMA_MAX_ADDR_LENGTH        (20)
typedef struct
{
    BYTE            AddressType;
    BYTE            Address[ DNS_ATMA_MAX_ADDR_LENGTH ];

    //  E164 -- Null terminated string of less than
    //      DNS_ATMA_MAX_ADDR_LENGTH
    //
    //  For NSAP (AESA) BCD encoding of exactly
    //      DNS_ATMA_AESA_ADDR_LENGTH
}
DNS_ATMA_DATA, *PDNS_ATMA_DATA;


typedef struct
{
    PWSTR           pNameAlgorithm;
    PBYTE           pAlgorithmPacket;
    PBYTE           pKey;
    PBYTE           pOtherData;
    DWORD           dwCreateTime;
    DWORD           dwExpireTime;
    WORD            wMode;
    WORD            wError;
    WORD            wKeyLength;
    WORD            wOtherLength;
    UCHAR           cAlgNameLength;
    BOOL            bPacketPointers;
}
DNS_TKEY_DATAW, *PDNS_TKEY_DATAW;

typedef struct
{
    PSTR            pNameAlgorithm;
    PBYTE           pAlgorithmPacket;
    PBYTE           pKey;
    PBYTE           pOtherData;
    DWORD           dwCreateTime;
    DWORD           dwExpireTime;
    WORD            wMode;
    WORD            wError;
    WORD            wKeyLength;
    WORD            wOtherLength;
    UCHAR           cAlgNameLength;
    BOOL            bPacketPointers;
}
DNS_TKEY_DATAA, *PDNS_TKEY_DATAA;

typedef struct
{
    PWSTR           pNameAlgorithm;
    PBYTE           pAlgorithmPacket;
    PBYTE           pSignature;
    PBYTE           pOtherData;
    LONGLONG        i64CreateTime;
    WORD            wFudgeTime;
    WORD            wOriginalXid;
    WORD            wError;
    WORD            wSigLength;
    WORD            wOtherLength;
    UCHAR           cAlgNameLength;
    BOOL            bPacketPointers;
}
DNS_TSIG_DATAW, *PDNS_TSIG_DATAW;

typedef struct
{
    PSTR            pNameAlgorithm;
    PBYTE           pAlgorithmPacket;
    PBYTE           pSignature;
    PBYTE           pOtherData;
    LONGLONG        i64CreateTime;
    WORD            wFudgeTime;
    WORD            wOriginalXid;
    WORD            wError;
    WORD            wSigLength;
    WORD            wOtherLength;
    UCHAR           cAlgNameLength;
    BOOL            bPacketPointers;
}
DNS_TSIG_DATAA, *PDNS_TSIG_DATAA;

typedef struct
{
    DWORD           dwByteCount;
#ifdef MIDL_PASS
    [size_is(dwByteCount)] BYTE bData[];
#else
    BYTE            bData[1];
#endif
}
DNS_UNKNOWN_DATA, *PDNS_UNKNOWN_DATA;

//
//  MS only types -- only hit the wire in MS-MS zone transfer
//

typedef struct
{
    DWORD           dwMappingFlag;
    DWORD           dwLookupTimeout;
    DWORD           dwCacheTimeout;
    DWORD           cWinsServerCount;
    IP4_ADDRESS     WinsServers[1];
}
DNS_WINS_DATA, *PDNS_WINS_DATA;

typedef struct
{
    DWORD           dwMappingFlag;
    DWORD           dwLookupTimeout;
    DWORD           dwCacheTimeout;
    PWSTR           pNameResultDomain;
}
DNS_WINSR_DATAW, *PDNS_WINSR_DATAW;

typedef struct
{
    DWORD           dwMappingFlag;
    DWORD           dwLookupTimeout;
    DWORD           dwCacheTimeout;
    PSTR            pNameResultDomain;
}
DNS_WINSR_DATAA, *PDNS_WINSR_DATAA;


typedef struct _DnsRecordFlags
{
    DWORD   Section     : 2;
    DWORD   Delete      : 1;
    DWORD   CharSet     : 2;
    DWORD   Unused      : 3;

    DWORD   Reserved    : 24;
}
DNS_RECORD_FLAGS;
typedef _Struct_size_bytes_(FIELD_OFFSET(struct _DnsRecordW, Data) + wDataLength) struct _DnsRecordW
{
    struct _DnsRecordW *    pNext;
    PWSTR                   pName;
    WORD                    wType;
    WORD                    wDataLength;    // Not referenced for DNS record types
                                            // defined above.
    union
    {
        DWORD               DW;     // flags as DWORD
        DNS_RECORD_FLAGS    S;      // flags as structure

    } Flags;

    DWORD                   dwTtl;
    DWORD                   dwReserved;

    //  Record Data

    union
    {
        DNS_A_DATA          A;
        DNS_SOA_DATAW       SOA, Soa;
        DNS_PTR_DATAW       PTR, Ptr,
                            NS, Ns,
                            CNAME, Cname,
                            DNAME, Dname,
                            MB, Mb,
                            MD, Md,
                            MF, Mf,
                            MG, Mg,
                            MR, Mr;
        DNS_MINFO_DATAW     MINFO, Minfo,
                            RP, Rp;
        DNS_MX_DATAW        MX, Mx,
                            AFSDB, Afsdb,
                            RT, Rt;
        DNS_TXT_DATAW       HINFO, Hinfo,
                            ISDN, Isdn,
                            TXT, Txt,
                            X25;
        DNS_NULL_DATA       Null;
        DNS_WKS_DATA        WKS, Wks;
        DNS_AAAA_DATA       AAAA;
        DNS_KEY_DATA        KEY, Key;
        DNS_SIG_DATAW       SIG, Sig;
        DNS_ATMA_DATA       ATMA, Atma;
        DNS_NXT_DATAW       NXT, Nxt;
        DNS_SRV_DATAW       SRV, Srv;
        DNS_NAPTR_DATAW     NAPTR, Naptr;
        DNS_OPT_DATA        OPT, Opt;
        DNS_DS_DATA         DS, Ds;
        DNS_RRSIG_DATAW     RRSIG, Rrsig;
        DNS_NSEC_DATAW      NSEC, Nsec;
        DNS_DNSKEY_DATA     DNSKEY, Dnskey;
        DNS_TKEY_DATAW      TKEY, Tkey;
        DNS_TSIG_DATAW      TSIG, Tsig;
        DNS_WINS_DATA       WINS, Wins;
        DNS_WINSR_DATAW     WINSR, WinsR, NBSTAT, Nbstat;
        DNS_DHCID_DATA      DHCID;
        DNS_NSEC3_DATA      NSEC3, Nsec3;
        DNS_NSEC3PARAM_DATA	NSEC3PARAM, Nsec3Param;
        DNS_TLSA_DATA	    TLSA, Tlsa;
        DNS_UNKNOWN_DATA    UNKNOWN, Unknown;
        PBYTE               pDataPtr;

    } Data;
}
DNS_RECORDW, *PDNS_RECORDW;


#ifndef s_addr
#pragma once

#pragma region Desktop Family or OneCore Family or Games Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES)

//
// IPv4 Internet address
// This is an 'on-wire' format structure.
//
typedef struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
#pragma endregion

#endif



typedef struct in6_addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
} IN6_ADDR, *PIN6_ADDR, FAR *LPIN6_ADDR;

#define in_addr6 in6_addr

//
// Defines to match RFC 2553.
//
#define _S6_un      u
#define _S6_u8      Byte
#define s6_addr     _S6_un._S6_u8

//
// Defines for our implementation.
//
#define s6_bytes    u.Byte
#define s6_words    u.Word

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES) */
