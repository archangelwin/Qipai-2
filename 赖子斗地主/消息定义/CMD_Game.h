#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						24									//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("��Ӷ�����")						//��Ϸ����

//�������
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//��Ϸ״̬
#define GS_WK_FREE					GAME_STATUS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GAME_STATUS_PLAY							//�з�״̬
#define GS_WK_PLAYING				GAME_STATUS_PLAY+1						//��Ϸ����
#define GS_WK_QIANG_LAND			GAME_STATUS_PLAY+2						//������
#define GS_WK_ADD					GAME_STATUS_PLAY+3						//�ӱ�

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
//
#define SUB_S_MING_CARD				106									//����
#define SUB_S_QIANG_LAND			107									//������
#define SUB_S_MING_OUT				108									//��������
#define SUB_S_ADD					109									//�ӱ�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lBaseScore;							//��������

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONGLONG						lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	WORD							wBombTime;							//����
	bool							bMingCardStatus[GAME_PLAYER];		//����״̬

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusQiangLand
{
	LONGLONG						lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	WORD							wBombTime;							//����
	bool							bMingCard[GAME_PLAYER];				//�����û�
	bool							bMingCardStatus[GAME_PLAYER];		//����״̬

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};
//��Ϸ״̬
struct CMD_S_StatusAdd
{
	WORD							wLandUser;							//�������
	LONGLONG						lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	WORD							wBombTime;							//����
	bool							bMingCard[GAME_PLAYER];				//�����û�
	bool							bAddScore[GAME_PLAYER];				//�ӱ��ж�
	BYTE							bLaiZiCard;							//����
	bool							bMingCardStatus[GAME_PLAYER];		//����״̬

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};
//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//����
	LONGLONG						lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[3][20];					//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bLaiZiCard;							//������
	bool							bMingCardStatus[GAME_PLAYER];		//����״̬

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	//
	BYTE							bMingCard[GAME_PLAYER];				//������Ϣ

	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][20];			//�˿��б�
	BYTE							bBackCardData[3];					//�����˿�
	BYTE							bLaiZiCard;							//������
	WORD							wBombTime;							//����			
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	bool							bAddScore[GAME_PLAYER];				//�ӱ��ж�
	bool							bIsAdd;								//�Ƿ�ӱ�
	WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	WORD							wBombTime;							//����	
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[20];						//�˿��б�
	BYTE							bChangeCard[20];					//�����
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax;							//��Ϸ˰��
	LONGLONG						lGameScore[3];						//��Ϸ����
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[54];						//�˿��б� 
	BYTE							bUserAdd[GAME_PLAYER];				//�û��ӱ�
	int								iOtherAdd[5];						//�����ӱ�
	WORD							wPlayTime[GAME_PLAYER];							//����
	LONGLONG						lGameResult[GAME_PLAYER];						//�ɼ�
};
//����
struct CMD_S_MingCard
{
	WORD							wMingUser;							//�����û�
	BYTE							bMingCardStatus;							//����״̬
	WORD							wBombTime;							//����
};
//������
struct CMD_S_QiangLand
{
	BYTE							bQiangLand;							//������
	WORD							wQiangUser;							//�������û�
	WORD							wBombTime;							//���� 
};
//�ӱ�
struct CMD_S_Add
{
	bool						bAdd;							//�ӱ�
	bool						bIsAdd;							//�ӱ��ж�
	WORD						wAddUser;						//�ӱ��û�
	WORD						wCurrentUser;					//��ǰ�û�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ
//
#define SUB_C_MINGSTATE				5									//���ƿ�ʼ
#define SUB_C_QIANGLAND				6									//������
#define SUB_C_MINGOUT				7									//��������
#define SUB_C_ADD				8										//�ӱ�

//�йܽṹ
struct CMD_C_UserTrustee {
	WORD							wUserChairID;						//�������
	bool							bTrustee;							//�йܱ�ʶ
};

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[20];						//�˿��б�
	BYTE							bChangeCard[20];					//�����
};
//���ƿ�ʼ
struct CMD_C_MingState
{
	WORD							wUserChairID;						//�������
	BYTE							bMingState;							//����״̬
};
//������
struct CMD_C_QiangLand
{
	bool							bQiangLand;						//������
};
//��������
struct CMD_C_MingOut
{
	bool							bMingOut;						//��������
};
//�ӱ�
struct CMD_C_ADD
{
	bool							bAdd;						//�ӱ�
};
//////////////////////////////////////////////////////////////////////////

#endif
