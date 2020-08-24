/* ================================================================================ *
 * Comm.h                                                                           *
 *																					*
 * Serial Port Controll Class                                                       *
 * Serial Port�� ���� RS232 / RS422 / RS485 / Modem�� controll�� �� �ֵ��� �Ѵ�.    *
 * ================================================================================ */
#ifndef _COMM_H_
#define _COMM_H_

#define MAXCOMM                         10

/* ================================================================================ *
 * Define Window Message                                                            *
 * ================================================================================ */
#define WM_COMM                         WM_USER + 4

#define CM_RXCHAR                       0
#define CM_TXCHAR                       1
#define CM_TXEMPTY                      2
#define CM_CTSHIGH                      3
#define CM_CTSLOW                       4
#define CM_CTSCHANGE                    5
#define CM_RTSOUT                       6
#define CM_RING                         7
#define CM_QUEUEOVERFLOW                100

#define QUEUESIZE           1024*1024   // Queue Buffer size

#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////
//  CLASS : CQueue

class CQueue
{
protected:
    LPBYTE  m_pBuffer;
    int     m_iHead, m_iTail;           // Queue Top & Buttom Index
    int     m_nSize;                    // Queue�� ��� �ִ� Data����

public:
    // Contructor & Destructor
    CQueue();                           // Constructor
    virtual ~CQueue();                  // Destructor

    // Member function
    // Buffer�� ��� �ִ����� �˻��Ѵ�.
    virtual BOOL  isEmpty() { return ((m_nSize==0) ? TRUE: FALSE); }
    virtual void  Clear(void);          // Buffer�� �ִ� ������ ��� ����� ������ 
                                        // 0���� �ʱ�ȭ �Ѵ�.
    // Data ó�� �Լ�
    virtual int   GetSize(void) { return m_nSize; } // Data ���� ��´�.
    virtual BOOL  PutByte(BYTE  ch);    // Queue�� 1 byte�� �ִ´�. m_nSize, m_iTail ����
    virtual BOOL  GetByte(BYTE *pch);   // Queue�� 1 Byte�� ��´�. m_nSize, m_iHead ����
    virtual BOOL  PutData( int nCount, LPBYTE lpDat );
    virtual int   GetData( int nCount, LPBYTE lpDat );

//    virtual int   SaveFile( CFile  cfFile );
};


///////////////////////////////////////////////////////////////////////////////////////
//  CLASS : CComm

class CComm
{
protected:
    HANDLE	m_hPort;                    // Port Handler
    int     m_nPort;                    // Port Number
    int     m_nBaudrate;                // Baud Rate
    int     m_nStopbit;                 // Stop Bit
    int     m_nParity;                  // Parity
    int     m_nThreadInx;               // Thread Port Index
    BOOL    m_fOpen;                    // Port�� Open�Ǿ������� ��Ÿ��.
    HWND    m_hWnd;                     // Message ���� Handle
    HANDLE  m_hThreadProc;              // ThreadProc Handle
    DWORD   m_dwThreadID;               // ThreadID
    BOOL    m_bActThread;               // Active Thread

    OVERLAPPED  m_olRead;               // OVERLAPED Structure for Read
    OVERLAPPED  m_olWrite;              // OVERLAPED Structure for Write

public:
    CQueue  m_Queue;                    // Input Buffer
    BOOL    m_bThread;                  // Thread ����  TRUE : Thread ������, FALSE : Thread ����
    
    // Contructor & Destructor
    CComm();                            // Default Contructor
    virtual ~CComm();                   // Destructor

    // Open/Close Port
    virtual BOOL OpenPort(HWND hWnd, int nPort, int nBaudrate, int nStopbit, int nParity);
    virtual void ClosePort(void);
	virtual void SetHwnd(HWND hWnd); //csjang

    // Port���� �Լ�
    virtual DWORD  SendData(const void *pBuf, UINT nCount);
    virtual DWORD  RecvData( void );
	
    //virtual int	   IsRcvEmpty(void);
    virtual int    GetPortNum(void) { return m_nPort; }
    virtual int    GetParity(void) { return m_nParity; }
    virtual int    GetBaudrate(void) { return m_nStopbit; }
    virtual BOOL   isOpen(void) { return m_fOpen;  }
    virtual HANDLE GetHandle(void) { return m_hPort; }

    virtual void   SetRTS( BOOL fStat );
    
    // Thread ���� �Լ�
    virtual BOOL  SetThread(UINT (*pThread)(CComm *pComm));
    virtual BOOL  isActiveThread(void) { return ((m_bActThread==TRUE) ? TRUE: FALSE); }

    // Message���� �Լ�
    virtual BOOL SendMsg( UINT Msg, UINT MsgState );
};

#endif
