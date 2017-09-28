#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	LONGLONG							m_lCellScore;

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wLandScore;
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_bOverTimes;
	bool							m_bUserNoMain[GAME_PLAYER];

	//�÷���Ϣ
protected:
	BYTE							m_cbScoreCardCount;					//������Ŀ
	BYTE							m_cbScoreCardData[12*MAX_PACK];		//�÷��˿�

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
	BYTE							m_cbBackCard[8];					//�����˿�
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];				//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount;					//�˿���Ŀ
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�

	//���ñ���
protected:
	bool							m_bStustee;							//�йܱ�־
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[MAX_COUNT];	//�ɷ��˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//ϵͳ�¼�
public:
	//��������
	virtual bool OnInitGameEngine();
	//���ú���
	virtual bool OnResetGameEngine();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//ȡ����Ϣ
	//virtual VOID OnCancel();

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCallCard(const void * pBuffer, WORD wDataSize);
	//���͵���
	bool OnSubSendConceal(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGamePlay(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//˦�ƽ��
	bool OnSubThrowResult(const void * pBuffer, WORD wDataSize);
	//һ�ֽ���
	bool OnSubTurnBalance(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��з�
	bool OnSubLandScore(const void * pBuffer, WORD wDataSize);

	bool OnSubGMCard(const void * pBuffer, WORD wDataSize);
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

	void PlayLandScoreSound(IClientUserItem *pUserItem,WORD wLandScore);

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
	//LRESULT OnRuquestLeave(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnLastTurnCard(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);

	//�з���Ϣ
	LRESULT OnLandScore(WPARAM wParam, LPARAM lParam);

	LRESULT OnGiveUpGame(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif