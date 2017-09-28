#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"
#include "GameClientView.h"
#include "DlgRequestLeave.h"
#include "DlgResponseLeave.h"

//////////////////////////////////////////////////////////////////////////

//�뿪��������
typedef	CWHArray<CDlgResponseLeave *> DlgResLeaveArray;

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{

	//��Ԫ����
	friend class CGameClientView;

	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	bool							m_bBatchCard;						//��������

	//�÷���Ϣ
protected:
	BYTE							m_cbScoreCardCount;					//������Ŀ
	BYTE							m_cbScoreCardData[12*PACK_COUNT];	//�÷��˿�

	//˦�Ʊ���
protected:
	bool							m_bThrowCard;						//˦�Ʊ�־
	WORD							m_wThrowCardUser;					//˦���û�
	BYTE							m_cbResultCardCount;				//��ԭ�˿�
	BYTE							m_cbResultCardData[MAX_COUNT];		//��ԭ�˿�

	//���Ʊ���
protected:
	bool							m_bLastTurn;						//���ֱ�־
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4][2];				//������Ŀ
	BYTE							m_cbOutCardData[4][2][MAX_COUNT];	//�����б�

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount;					//�˿���Ŀ
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�

	//���ñ���
protected:
	bool							m_bStustee;							//�йܱ�־
	DWORD							m_dwCardHSpace;						//�˿�����
	WORD							m_wTimeOutCount;					//��ʱ����

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[MAX_COUNT];	//�ɷ��˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CDlgRequestLeave				m_DlgRequestLeave;					//�����뿪
	DlgResLeaveArray				m_DlgResponseArray;					//�û���������
	
	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);

	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pBuffer, WORD wDataSize);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(VOID * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(VOID * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCallCard(VOID * pBuffer, WORD wDataSize);
	//���͵���
	bool OnSubSendConceal(VOID * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGamePlay(VOID * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(VOID * pBuffer, WORD wDataSize);
	//˦�ƽ��
	bool OnSubThrowResult(VOID * pBuffer, WORD wDataSize);
	//һ�ֽ���
	bool OnSubTurnBalance(VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(VOID * pBuffer, WORD wDataSize);
	//�û��й�
	bool OnSubStrustee(VOID * pBuffer, WORD wDataSize);
	//�����뿪
	bool OnSubRequestLeave(VOID * pBuffer, WORD wDataSize);
	//��������
	bool OnSubBatchCard(VOID * pBuffer, WORD wDataSize);

	//���ܺ���
protected:
	//�����ж�
	bool VerdictOutCard();
	//�Զ�����
	bool AutomatismOutCard();
	//��ǰ�˿�
	bool SwitchToCurrentCard();
	//˦�ƽ��
	bool SwitchToThrowResult();
	//���½���
	bool UpdateCallCardControl();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	bool SetHandCardControl(BYTE cbCardData[], BYTE cbCardCount);
	//ֹͣ����
	void StopDispatchCard();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnCallCard(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnSendConceal(WPARAM wParam, LPARAM lParam);
	//�鿴����
	LRESULT OnConcealCard(WPARAM wParam, LPARAM lParam);
	//�����뿪
	LRESULT OnRuquestLeave(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnLastTurnCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�����뿪��Ϣ
	LRESULT OnMessageResponseLeave(WPARAM wParam,LPARAM lParam);
	//test
	LRESULT OnTest(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
