#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "..\��Ϸ�ͻ���\HistoryRecord.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wBombTime;						//����
	BYTE							m_bLandScore;						//��������
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_bScoreInfo[3];					//�з���Ϣ
	//
	BYTE							m_bMingCardStatus[GAME_PLAYER];		//����״̬ 1��ʼ���� 2�������� 3����
	bool							m_bQiangLand[GAME_PLAYER];			//������
	BYTE							m_bUserDouble[GAME_PLAYER];			//�û�����
	LONGLONG						m_lAddScore;						//�ӱ�����
	bool							m_bAddScore[GAME_PLAYER];			//�ӱ��ж�
	WORD							m_wBomb;							//ը������
	WORD							m_wQiangLandDouble;					//����������
	WORD							m_wPlayTime[GAME_PLAYER];			//����


	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�

	//�˿���Ϣ
protected:
	BYTE							m_bBackCard[3];						//��Ϸ����
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bOutCardCount[3];					//���ƴ���
	BYTE							m_bHandCardData[3][20];				//�����˿�
	BYTE							m_bLaiZiCard;						//������

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	CHistoryScore					m_HistoryScore;						//��ʷ�ɼ�

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const BYTE				m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual BYTE  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//�����¼�
public:
	//����ͬ��
	virtual bool  OnActionUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�������
	virtual bool  OnActionUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool  OnActionUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool  OnActionUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//��������
	virtual bool  OnActionUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }


	//��Ϸ�¼�
protected:
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount,BYTE bChangeCard[]);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
	//��������
	bool OnMingState(BYTE bMingState,WORD wUserChairID,IServerUserItem * pIServerUserItem);
	//��������
	bool OnMingOut(WORD wCurrentUser);
	//������
	bool OnQiangLand(WORD wChairID,bool bQiangLand);
	//�û��ӱ�
	bool OnAdd(WORD wCurrentUser,bool bAdd);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

};

//////////////////////////////////////////////////////////////////////////

#endif
