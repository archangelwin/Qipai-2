#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�ڿ��û�
	WORD							m_wBombTime;						//����
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bHandCardCount;					//�˿���Ŀ
	BYTE							m_bHandCardData[20];				//�����˿�
	bool							m_bAutoPlay ;						//�йܱ���
	//
	BYTE							m_bMingCard[GAME_PLAYER];			//���Ʊ�ʶ
	BYTE							m_bHandCardUser[3][20];				//��������˿�
	BYTE							m_bLaiZiData;						//����

	//���Ʊ���
protected:
	DWORD							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�
	BYTE							m_bChangeCard[20];					//�����
	BYTE							m_bChangeTurnOut[20];					//�����

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����
	bool							m_bAllowLookon;						//�����Թ�

	//��������
protected:
	WORD							m_wMostUser;						//������
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wTimeOutCount;					//��ʱ����
	BYTE							m_cbSortType;						//��������
	bool							m_bAddScore[GAME_PLAYER];						//�ӱ�
	bool							m_bIsAdd;							//�ӱ�״̬
	BYTE							m_bLaiZiFrame;						//֡��

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[3][20];			//�ɷ��˿�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CHistoryScore					m_HistoryScore;						//��ʷ����

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
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û��з�
	bool OnSubLandScore(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�ӱ�
	bool OnSubAdd(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�Զ�����
	bool AutomatismOutCard();
	//ֹͣ����
	bool DoncludeDispatchCard();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE [GAME_PLAYER][20], BYTE cbCardCount);
	//���Ӷ���ֹͣ
	bool DoncludeFrame();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
	//�з���Ϣ
	LRESULT OnLandScore(WPARAM wParam, LPARAM lParam);
	//������ʾ
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�й���Ϣ
	LRESULT OnAutoPlay(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnMessageSortCard(WPARAM wParam, LPARAM lParam);
	//���ƿ�ʼ
	LRESULT OnMingCardState(WPARAM wParam,LPARAM lParam);
	//����
	LRESULT OnMingCard(WPARAM wParam,LPARAM lParam);
	//������
	LRESULT OnQiangLand(WPARAM wParam,LPARAM lParam);
	//��������
	LRESULT OnMingOut(WPARAM wParam,LPARAM lParam);
	//�ӱ�
	LRESULT OnAdd(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
