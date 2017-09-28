#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//�뿪����
protected:
	BYTE							m_cbResponses[GAME_PLAYER];			//��Ӧ״̬
	WORD							m_wRequestTimes[GAME_PLAYER];		//�����뿪

	//������Ϣ
protected:
	BYTE							m_cbValueOrder[2];					//��ֵ�ȼ�

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	bool							m_bUserStrustee[GAME_PLAYER];		//�û��й�

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	bool							m_bCallCard[4];						//���Ʊ�־
	BYTE							m_cbScoreCardCount;					//�˿���Ŀ
	BYTE							m_cbScoreCardData[12*PACK_COUNT];	//�÷��˿�

	//���Ʊ���
protected:
	WORD							m_wTurnWinner;						//ʤ�����
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4];				//������Ŀ
	BYTE							m_cbOutCardData[4][MAX_COUNT];		//�����б�
	BYTE							m_cbLastTurnCard[GAME_PLAYER][MAX_COUNT];		//�ϴγ���
	BYTE							m_cbLastTurnCount[GAME_PLAYER];					//�ϴγ�����Ŀ

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount[4];				//�˿���Ŀ
	BYTE							m_cbHandCardData[4][MAX_COUNT];		//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����


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
	
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();

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

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);


	//�����¼�
public:
	//�û�ͬ��
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem,VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount);
	//�������
	bool OnUserCallFinish(WORD wChairID);
	//�����˿�
	bool OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
	//�û��й�
	bool OnUserStrustee(WORD wChairID,bool bStrustee);
	//�����뿪
	bool OnUserRequestLeave(WORD wChairID, LPCTSTR pszLeaveReason);
	//��Ӧ�뿪
	bool OnUserResponsesLeave(WORD wChairID, WORD wTargerChairID, BYTE cbAgreeLeave);

	//��������
protected:
	//�ɷ��˿�
	bool DispatchUserCard();
};

//////////////////////////////////////////////////////////////////////////

#endif
