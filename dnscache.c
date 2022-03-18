#include "base.h"
#include "definitions.h"

//https://stackoverflow.com/questions/31889957/memory-leak-when-using-dnsgetcachedatatable
void dnscache()
{

    struct functionPointers fnPtrs;
    loadFunctions(&fnPtrs);

    char szSeperator[] = { ' ', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\n', 0 };
    char wszEntryName[] = { ' ', '%', 'S', '\n', 0 };
    char szNewLine[] = { '\n', 0 };
    char szARecord[] = { ' ', 'A', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 's', '\n', 0 };
    char wszAAAARecord[] = { ' ', 'A', 'A', 'A', 'A', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 's', '\n', 0 };
    char wszNSRecord[] = { ' ', 'N', 'S', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 'S', '\n', 0 };
    char wszCNAMERecord[] = { ' ', 'C','N','A','M','E', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 'S', '\n', 0 };
    char wszPTRRecord[] = { ' ', 'P','T','R', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 'S', '\n', 0 };
    char wszSRVRecord[] = { ' ', 'S','R','V', ' ', 'R', 'e', 'c', 'o', 'r', 'd', ':', ' ', '%', 'S', '\n', 0 };
    char szNoRecord[] = { ' ', 'N','o', ' ', 'R', 'e', 'c', 'o', 'r', 'd', '\n', 0 };
    char szErrorRecord[] = { ' ', 'E','r','r','o','r', ' ', 'R', 'e', 'c', 'o', 'r', 'd', '\n', 0 };

    char szBuffer[MSGSIZE] = { 0 };

    PDNS_CACHE_ENTRY pEntry = (PDNS_CACHE_ENTRY)((CALLOC)fnPtrs.fnCalloc)(1, sizeof(DNS_CACHE_ENTRY));
    
    if (pEntry == NULL)
        return;

    
    PDNS_CACHE_ENTRY pThisEntry = (PDNS_CACHE_ENTRY)((CALLOC)fnPtrs.fnCalloc)(1, sizeof(DNS_CACHE_ENTRY));
    PDNS_CACHE_ENTRY pNextEntry = (PDNS_CACHE_ENTRY)((CALLOC)fnPtrs.fnCalloc)(1, sizeof(DNS_CACHE_ENTRY));
    
    IN_ADDR Addr4;
    IN6_ADDR Addr6;

    PDNS_RECORDW pQueryResults = NULL, pThisRecord, pNextRecord;
    pNextRecord = NULL;
    DNS_STATUS Status;

    ((DNSGETCACHEDATATABLE)fnPtrs.fnDnsGetCacheDataTable)(pEntry);
    
    DWORD dwBytesWritten = 0;
    HANDLE hPipe;
    hPipe = createPipe(&fnPtrs);

    pThisEntry = pEntry;
    while (pThisEntry != NULL) 
    {
        pNextEntry = pThisEntry->pNext;

        if ((pThisEntry->wType1 == 0) && (pThisEntry->wType2 == 0))
            goto next;
        

        Status = ((DNSQUERY_W)fnPtrs.fnDnsQuery_W)(pThisEntry->pszName, pThisEntry->wType1, DNS_QUERY_NO_WIRE_QUERY, NULL,(PDNS_RECORD*)&pQueryResults, NULL);

        pThisRecord = pQueryResults;
        

        printOutput(&fnPtrs, hPipe, szBuffer, wszEntryName, pThisEntry->pszName);   
        printOutput(&fnPtrs, hPipe, szBuffer, szSeperator, NULL);   


        if (Status == DNS_INFO_NO_RECORDS)
        {
            printOutput(&fnPtrs, hPipe, szBuffer, szNoRecord, NULL);   
            goto next;
        }
        

        if (Status ==  DNS_ERROR_RCODE_NAME_ERROR)
        {
            printOutput(&fnPtrs, hPipe, szBuffer, szErrorRecord, NULL);   
            goto next;
        }
        
        
        while (pThisRecord != NULL)
        {
            pNextRecord = pThisRecord->pNext;

            if (pThisEntry->wType1 == DNS_TYPE_A)
            {    
                Addr4.S_un.S_addr = pThisRecord->Data.A.IpAddress;
                printOutput(&fnPtrs, hPipe, szBuffer, szARecord, (((INETNTOA)fnPtrs.fnInetntoa)(Addr4)));   
                goto next;
            }

            if (pThisEntry->wType1 == DNS_TYPE_AAAA)
            {
                ((MEMCPY)(fnPtrs.fnMemcpy))(&Addr6,&pThisRecord->Data.AAAA.Ip6Address, sizeof(IN6_ADDR));
                ((INETNTOP)(fnPtrs.fnInetntop))(23,&Addr6, szBuffer, sizeof(szBuffer));
                printOutput(&fnPtrs, hPipe, szBuffer, wszAAAARecord, szBuffer);   
                goto next;    

            }

            if (pThisEntry->wType1 == DNS_TYPE_NS)
            {  
                printOutput(&fnPtrs, hPipe, szBuffer, wszNSRecord, pThisRecord->Data.NS.pNameHost);  
                goto next;
            }

            if (pThisEntry->wType1 == DNS_TYPE_CNAME)
            {
                printOutput(&fnPtrs, hPipe, szBuffer, wszCNAMERecord, pThisRecord->Data.CNAME.pNameHost);  
                goto next;
            }

            if (pThisEntry->wType1 == DNS_TYPE_PTR)
            {
                printOutput(&fnPtrs, hPipe, szBuffer, wszPTRRecord, pThisRecord->Data.PTR.pNameHost);  
                goto next;
            }

            if (pThisEntry->wType1 == DNS_TYPE_SRV)
            {
                printOutput(&fnPtrs, hPipe, szBuffer, wszSRVRecord, pThisRecord->Data.SRV.pNameTarget);   
                goto next; 
            }  
 
            next: 

            pThisRecord = pNextRecord;

        }
        
        pThisEntry = pNextEntry;
        ((DNSRECORDLISTFREE)fnPtrs.fnDnsRecordListFree)((PDNS_RECORD)pQueryResults, DnsFreeRecordList);
        printOutput(&fnPtrs, hPipe, szBuffer, szNewLine, NULL); 
        
    }
        
    closePipe(&fnPtrs, hPipe, &dwBytesWritten);
    return;

}


