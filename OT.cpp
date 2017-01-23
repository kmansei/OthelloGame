#define FrameNum 8	//マスの数。4以上の偶数のみ設定可
#include<stdio.h>
#include <stdlib.h>
void SetStone(int);
void Display();
void GameInit();
void player_in(int);
int FlipTest(int,int,int,int);	//ひっくり返せるか
int FlipFlop(int,int,int,int);	//ひっくり返す関数
int FlipConfirm(int);	//引数1:player プレイヤーが次に置ける場所が有るかどうか
void StoneCount();	//石の数を数える
void MatchResult();	//試合結果の表示
void P1_first();
void P2_first();
int StoneMap[FrameNum][FrameNum];
int dir_x[8] = {0,1,1,1,0,-1,-1,-1};	//全方向探索用の方向指定配列
int dir_y[8] = {-1,-1,0,1,1,1,0,-1};	//同上
int StoneNum[2] = {0,0};	//最終的に何個の石を取ったか

int main()
{
	int player1 = 0;
	int player2 = 0;
	int first;
	GameInit();
	Display();
	printf("先攻を入力してください\n○ : player1\n● : player2\n1 or 2\n");
	scanf("%d",&first);
	if(first == 1)
	{
		P1_first();
	}
	else
	{
		P2_first();
	}
	printf("試合終了\n");
	StoneCount();
	MatchResult();
	getchar();
	getchar();
}
//player2先攻
void P2_first()
{
	int EndNum = 0;
	while(1)
	{
		if(FlipConfirm(2) == 0)	//player2が置く場所が無かったら
		{
			EndNum++;
			printf("置ける場所が無いのでパスします\n");
		} else {
			player_in(2);
			EndNum = 0;
		}
		Display();
		if(EndNum >= 2)	//2人とも石を置ける場所が無かった場合
		{
			break;
		}
		if(FlipConfirm(1) == 0)	//player1が置く場所が無かったら
		{
			EndNum++;
			printf("置ける場所が無いのでパスします\n");
		}
		else
		{
			player_in(1);
			EndNum = 0;
		}
		Display();
		if(EndNum >= 2)	//2人とも石を置ける場所が無かった場合
		{
			break;
		}
	}
}
//player1先攻
void P1_first()
{
	int EndNum = 0;
	while(1)
	{
		if(FlipConfirm(1) == 0)	//player1が置く場所が無かったら
		{
			EndNum++;
			printf("置ける場所が無いのでパスします\n");
		} else {
			player_in(1);
			EndNum = 0;
		}
		Display();
		if(EndNum >= 2)	//2人とも石を置ける場所が無かった場合
		{
			break;
		}
		if(FlipConfirm(2) == 0)	//player2が置く場所が無かったら
		{
			EndNum++;
			printf("置ける場所が無いのでパスします\n");
		} else {
			player_in(2);
			EndNum = 0;
		}
		Display();
		if(EndNum >= 2)	//2人とも石を置ける場所が無かった場合
		{
			break;
		}
	}
}
//試合結果の表示
void MatchResult()
{
	if(StoneNum[0] > StoneNum[1])
	{
		printf("〇プレイヤーの勝利\n");
	}
	else if(StoneNum[0] < StoneNum[1])
	{
		printf("●プレイヤーの勝利\n");
	}
	else
	{
		printf("引き分け\n");
	}
}
//石を何個取ったか数える
void StoneCount()
{
	for(int x=0 ; x<=FrameNum-1 ;x++)
	{
		for(int y=0 ; y<=FrameNum-1 ; y++)
		{
			if(StoneMap[x][y] == 1)
			{
				StoneNum[0]++;
			}
			else if(StoneMap[x][y] == 2)
			{
				StoneNum[1]++;
			}
		}
	}
}
//playerが石を置ける場所があるか調べる
int FlipConfirm(int player)
{
	int Confirm = 0;
	for(int x=0; x<=FrameNum-1 ; x++)
	{
		for(int y=0 ; y<=FrameNum-1 ; y++)
		{
			for(int dir=0 ; dir<=7 ; dir++)
			{
				if(FlipTest(dir,player,x,y))	//全部のマスを探索して挟める石があるか調べる
					Confirm++;
			}
		}
	}
	//test
	if(Confirm)
		printf("試合続行\n");
	return Confirm;
}
//実際にひっくり返す関数
int FlipFlop(int dir,int player,int x,int y)
{
	int EnemyStone = 0;	//
	int Enemy;
	int Enemy_x[FrameNum];	//挟める敵の石位置を保存する配列
	int Enemy_y[FrameNum];
	for(int i=0; i<=FrameNum-1 ;i++)	//初期化
	{
		Enemy_x[i] = 0;
		Enemy_y[i] = 0;
	}
	if(player == 1)
	{
		Enemy = 2;
	}
	else
	{
		Enemy = 1;
	}
	while(1)
	{
		x = x+dir_x[dir];
		y = y+dir_y[dir];
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0)	//探索範囲が枠の外になってしまったら
		{
			//printf("x=%d,y=%d枠外です\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player)	//もし探索方向に自分の石が有ったら
		{
			if(EnemyStone != 0)	//挟める石が有ったら
			{
				EnemyStone--;
				while(EnemyStone >= 0)
				{
					StoneMap[Enemy_x[EnemyStone]][Enemy_y[EnemyStone]] = player;
					//printf("x=%d y=%dをひっくり返します\n",Enemy_x[EnemyStone],Enemy_y[EnemyStone]);
					EnemyStone--;
				}
				return 1;
			}
			else
			{
				//printf("x=%d,y=%d挟める石がありません\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy)	//もし探索方向に敵の石が有ったら
		{
			//printf("x=%d,y=%d敵の石有り\n",x,y);
			Enemy_x[EnemyStone] = x;
			Enemy_y[EnemyStone] = y;
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0)	//もし探索方向が開きだったら
		{
			//printf("x=%d,y=%d探索方向が開きです\n",x,y);
			return 0;
		}
	}
}
//ひっくり返せるかテストする関数
int FlipTest(int dir,int player,int x,int y)	//dir:探索方向の指定, player:どのプレイヤーの操作か x,y:自分の今の位置
{
	int EnemyStone = 0;	//
	int Enemy;
	if(player == 1)
	{
		Enemy = 2;
	} else {
		Enemy = 1;
	}
	//printf("初期位置x=%d,y=%d\n",x,y);
	if(StoneMap[x][y] != 0)	//既になんか石が有って置けない場合
	{
		return 0;
	}
	while(1)
	{
		x = x+dir_x[dir];
		y = y+dir_y[dir];
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0)	//探索範囲が枠の外になってしまったら
		{
			//printf("x=%d,y=%d枠外です\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player)	//もし探索方向に自分の石が有ったら
		{
			if(EnemyStone != 0)	//挟める石が有ったら
			{
				//printf("挟める石有り\n");
				return 1;
			}
			else
			{
				//printf("x=%d,y=%d挟める石がありません\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy)	//もし探索方向に敵の石が有ったら
		{
			//printf("x=%d,y=%d敵の石有り\n",x,y);
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0)	//もし探索方向が開きだったら
		{
			//printf("x=%d,y=%d探索方向が開きです\n",x,y);
			return 0;
		}
	}
}
//playerからの入力を受け付ける
void player_in(int player)
{
	int x,y,q,r;
	int return_flag = 1;
	//場所ごとに優先度を決める。数字が小さいほど優先度が高い。
	//角，中心優先。角の周りを徹底的に避ける。
	int pri[8][8] = { 
		{0, 7, 2, 1, 1, 2, 7, 0},
    	{7, 6, 5, 4, 4, 5, 7, 7},
    	{2, 5, 2, 3, 3, 2, 5, 2},
    	{1, 4, 3, 3, 3, 3, 4, 1},
    	{1, 4, 3, 3, 3, 3, 4, 1},
    	{2, 5, 2, 3, 3, 2, 5, 2},
    	{7, 6, 5, 4, 4, 5, 7, 7},
    	{0, 7, 2, 1, 1, 2, 7, 0},
	};

	if(player == 1)
		printf("〇プレイヤー入力\n");
	else
		printf("●プレイヤー入力\n");

	do
	{	
		int FlipResult = 0;
		if(player == 1){			
			printf("x=");
			scanf("%d",&x);
			printf("y=");
			scanf("%d",&y);
			for(int k=0; k<=7 ; k++)	//全方向探索で置けるかどうか判定
			{
				if(FlipTest(k,player,x,y))
				{
					FlipResult++;
				}
			}
		if(StoneMap[x][y] == 0 && FlipResult != 0)	//その場所に石が無い、且つ相手をひっくり返せるなら
		return_flag = 0;
		} 
		else 
		{
		for (int p = 0; p <= 7; p++){ //0→7 数字が少ないから
			for (int q = 0; q < 8; q++) 
			{
				for (int r = 0; r < 8; r++) 
				{
					if (pri[q][r] == p) 
					{
						for(int i=0; i<=7; i++)
						{
							if(FlipTest(i,player,q,r)) 
							{
								x = q, y = r;
								printf("%d%d\n", x,y);
								FlipResult++;
								goto next;
							}
						}
					}
				}
			}
		}
			next:
			if(StoneMap[x][y] == 0 && FlipResult != 0)	//その場所に石が無い、且つ相手をひっくり返せるなら
			{
				return_flag = 0;
			}

		}
	}while(return_flag != 0);
	StoneMap[x][y] = player;
	for(int dir=0; dir<=7 ; dir++)
	{
	 	FlipFlop(dir,StoneMap[x][y],x,y);	//全方位探索をして実際にひっくり返す関数
	}

}
//ゲーム初期化関数
void GameInit()
{
	for(int i=0;i<=FrameNum-1;i++)
	{
		for(int j=0;j<=FrameNum-1;j++)
		{
			StoneMap[i][j] = 0;
		}
	}
	StoneMap[(FrameNum/2)-1][(FrameNum/2)-1] = 1;	//初期石の配置
	StoneMap[(FrameNum/2)][(FrameNum/2)] = 1;
	StoneMap[(FrameNum/2)][(FrameNum/2)-1] = 2;
	StoneMap[(FrameNum/2)-1][(FrameNum/2)] = 2;
}

//画面表示関数
void Display()
{
	//1行目枠
	for(int j=0;j<=FrameNum-1;j++)
	{
		for(int i=0;i<=FrameNum-1;i++)
		{
			printf("|");
			printf("￣￣￣");
		}
		printf("|\n");
		//枠内
		for(int k=0;k<=1;k++)	//枠内の行の数
		{
			for(int i=0;i<=FrameNum-1;i++)//1行の中に何個も枠を作るループ
			{
				if(k == 0)	//枠内の石を置く行に来たら
				{
					if(StoneMap[i][j] != 0)
					{
						SetStone(StoneMap[i][j]);
					}
					else
					{
						printf("|      ");
					}
				}
				else
				{
					printf("|  %d%d  ",i,j);
				}
			}
			printf("|\n");
		}
	}
	for(int i=0;i<=FrameNum-1;i++)
	{
		printf(" ￣￣￣");
	}
	printf("\n");
}
//石を置く関数
void SetStone(int choice)
{
	if(choice == 1)
	{
		printf("|　〇　");
	}
	else
	{
		printf("|　●　");
	}
}

