//+------------------------------------------------------------------+
//|                                         MetaTrader Server Plugin |
//|                 Copyright © 2001-2008, MetaQuotes Software Corp. |
//|                                         http://www.metaquotes.ru |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class CSync
  {
private:
   CRITICAL_SECTION  m_cs;

public:
                     CSync()  { ZeroMemory(&m_cs,sizeof(m_cs)); InitializeCriticalSection(&m_cs); }
                    ~CSync()  { DeleteCriticalSection(&m_cs); }
   //----
   inline void       Lock()   { EnterCriticalSection(&m_cs);  }
   inline void       Unlock() { LeaveCriticalSection(&m_cs);  }
  };
//+------------------------------------------------------------------+
