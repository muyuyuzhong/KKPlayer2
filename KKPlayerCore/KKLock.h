/**************************kkplayer*********************************************/
/*******************Copyright (c) Saint ******************/
/******************Author: Saint *********************/
/*******************Author e-mail: lssaint@163.com ******************/
/*******************Author qq: 784200679 ******************/
/*******************KKPlayer  WWW: http://www.70ic.com/KKplayer ********************************/
/*************************date��2015-6-25**********************************************/
#include "stdafx.h"
#ifndef KKLock_H
#define KKLock_H
/***���ֵ***/
class CKKLock
{
public:
	CKKLock();
    CKKLock(DWORD dwSpinCount);
    ~CKKLock();
    void Lock();
    void Unlock();
    BOOL TryLock();
private:
		CKKLock(const CKKLock& cs);
		CKKLock operator = (const CKKLock& cs);
 
private:
        //�ٽ�ֵ
        KKCRITICAL_SECTION m_crisec;
};
#endif