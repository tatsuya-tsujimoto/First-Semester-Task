
// ==============================
// 2020年 前期 
// ゲームプログラミング１
// 制作課題 その２
// ==============================

/**
■説明
	以下のプログラムは [ 〇×(三目並べ) ] というゲームをコーディングし、
	実行を確認したうえでその一部分を削除したものです。

	削除した部分は　※※　と記載し、
	本来入るべき処理の内容をコメントしてあります。

	コメント内容や他のコードを読み解き、ゲームを完成させなさい。

	※Hit And Blowに比べてコメントのヒントが少ないので
	　どのような処理を書けばよいか良く考えましょう
 */

/**
■制作環境
	このプログラムは、[ ＤＸライブラリ ] というライブラリを用いて作られています。
		ホームページ：https://dxlib.xsrv.jp/
		リファレンス：https://dxlib.xsrv.jp/dxfunc.html

	ＤＸライブラリについて興味のある人は、
	ホームページ、リファレンスページ（用意されている関数の説明）を見てみてください。
*/

/**
■〇×(三目並べ)のルール
	ルールについては、自分で調べて把握するようにしてください
*/

#include "WinMain.h"
#include "InputManager.h"
#include "DrawManager.h"
#include <stdio.h>
#include <time.h>

// ==============================
// グローバル変数
// ==============================
static int map[STAGE_HEIGHT][STAGE_WIDTH];//※※		// 横:STAGE_WIDTH、縦:STAGE_HEIGHTのint型２次元配列 map を宣言

static int nowturn = 0;			//現在何ターン目か
const int linemax = 3;			//石が一列埋まる状態
const int lastturn = 9;
// ==============================
// 関数プロトタイプ宣言
// ==============================
// 盤面の x, y の位置に石が置けるかどうか
bool IsPutStone(int x, int y);// ※※ // bool型の戻り値、int型の引数x,yを持つIsPutStone関数を宣言
// 勝者が居るかを調べる
int CheckWinner();//※※ // int型の戻り値を持つCheckWinner関数を宣言

int RowCheck();

int ColumCheck();

int DiagonalCheck();
// ==============================
// Main関数
// ==============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリを使う上での初期化処理
	// ----------------------------------------------------
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetBackgroundColor(125, 125, 255);
	SetMainWindowText("マルバツ");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	// ローカル変数宣言
	// ----------------------------------------------------
	int pos_x = 0;//※※　整数型の変数pos_xを宣言し、0で初期化		// X座標選択位置
	int pos_y = 0;//※※　整数型の変数pos_yを宣言し、0で初期化		// Y座標選択位置
	int turn = STONE_WHITE;//※※　整数型の変数turnを宣言し、STONE_WHITEで初期化	// 現在の手番
	int winner = WINNER_NON;//※※　整数型の変数winnerを宣言し、WINNER_NONで初期化	// 勝利者
	
	const int onemove  = 1;			//上下左右押下での移動量	
	const int UpMax    = 0;
	const int DownMax  = 2;
	const int RightMax = 2;
	const int LeftMax  = 0;


	// 各種初期化処理
	// ----------------------------------------------------
	InputInit();			// 入力処理初期化関数の呼び出し
	DrawInit();				// 描画処理初期化関数の呼び出し

	// mapの初期化
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			map[i][j] = STONE_MAX;// ※※　二次元配列mapの全要素を STONE_MAX で初期化する
		}
	}
	// ゲームのメインループ
	// 画面を１回表示する毎にwhile分を１回処理する
	// ----------------------------------------------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ＤＸライブラリを使う上で、１フレーム分の処理を始めるためのお約束
		// ----------------------------------------------------
		ClearDrawScreen();
		clsDx();

		// 以下、毎フレーム更新する処理
		// ----------------------------------------------------
		InputUpdate();			// 入力処理更新関数の呼び出し
		winner = CheckWinner();// ※※ winner に勝利者情報を代入	// 勝利者のチェック

		// --- 入力状況をチェックして、適切な処理を行う
		// 決着がついてない時だけ入力を受け付けるように if文 でチェックする
		if(winner == WINNER_NON)
		{
			// 上下左右の入力があった時の処理
			if(IsPushKey(MY_INPUT_DOWN))
			{
				pos_y += onemove;//※※ pos_yの値を 1 減らす
				if (pos_y > DownMax)
					pos_y = DownMax;
			}
			else if(IsPushKey(MY_INPUT_UP))
			{
				pos_y -= onemove;//※※ pos_yの値を 1 増やす
				if (pos_y < UpMax)
					pos_y = UpMax;
			}
			else if(IsPushKey(MY_INPUT_LEFT))
			{
				pos_x -= onemove;//※※ pos_xの値を 1 減らす
				if (pos_x < LeftMax)
					pos_x = LeftMax;
			}
			else if(IsPushKey(MY_INPUT_RIGHT))
			{
				pos_x += onemove;//※※ pos_xの値を 1 増やす
				if (pos_x > RightMax)
					pos_x = RightMax;
			}
			// 決定(=エンターキー)が押された時の処理
			else if(IsPushKey(MY_INPUT_ENTER))
			{
				// 現在の座標が有効か if文 でチェックし、
				// 結果が true の時、以下の処理を行う
				if ( IsPutStone(pos_y,pos_x)==true )
				{
					map[pos_y][pos_x] = turn;//※※
					switch (turn)
					{
					case STONE_WHITE:turn = STONE_BLACK; break;
					case STONE_BLACK:turn = STONE_WHITE; break;
					}
					nowturn++;
					// 以下の処理を実装する
					// 選択されている座標と対応するmap配列の要素へturnの値を代入
					// 次のターンに回すため、turnの値を変更する
				}
			}
		}

		// 以下、描画処理
		// ----------------------------------------------------
		DrawInformation(turn);			//※※　// 情報文章を描画
		DrawGameClear(winner);			//※※　// ゲームクリアの文字を描画
		DrawBgLine();					//※※　// 枠線を描画
		for (int i = 0; i < STAGE_HEIGHT; i++)
		{
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				DrawStone(j, i, map[i][j]);	//※※　２重for文を使って盤面の石を描画する
			}
		}
		DrawCursor(pos_x, pos_y);		//※※　カーソルを描画

		// ＤＸライブラリを使う上で、モニターへゲーム画面を表示するためのお約束
		// 必ずループの最後で呼び出す
		// ----------------------------------------------------
		ScreenFlip();
	}

	// 後始末
	// ----------------------------------------------------
	DrawEnd();		// 描画処理終了

	// ＤＸライブラリを使う上での終了処理
	// ----------------------------------------------------
	DxLib_End();
	return 0;
}

// ==============================
// 盤面の x, y の位置に石が置けるかどうか
// true = 置ける
// false = 置けない
// ==============================
bool IsPutStone( int y, int x )
{
	if (map[y][x] == STONE_MAX)//※※　盤面の x, y の位置に石が置けるならtrue,置けないならfalseを返す処理
	{
		return true;
	}
	else
		return false;
}

// ==============================
// 勝者が居るかを調べる
// ==============================
int CheckWinner()
{
	
	//※※　以下の処理を実装する
	
	
	if (nowturn > 4)
	{
		if (RowCheck() != WINNER_NON)
			return RowCheck();
		if (ColumCheck() != WINNER_NON)
			return ColumCheck();
		if (DiagonalCheck() != WINNER_NON)
			return DiagonalCheck();
		if (nowturn == lastturn)
		{
			return WINNER_DRAW;
		}
	}
	return WINNER_NON;
	// 縦、横、斜めが同じ石かどうかを調べる
	// STONE_WHITE, STONE_BLACK, STONE_MAXを上手く使いましょう

	// もし、まだ揃っていなかったら、盤面に置かれている石の数を調べる
	// 全てのマスに石が置かれていたら引き分け
	
	// 上記のいずれかでも無かったらWINNER_NONを返す
}


int RowCheck()
{

	int row_count_w = 0;
	int row_count_b = 0;
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (map[i][j] == STONE_WHITE)
				row_count_w++;
			else if (map[i][j] == STONE_BLACK)
				row_count_b++;
			if (row_count_w == linemax || row_count_b == linemax)
				switch (map[i][j])
				{
				case STONE_WHITE:return WINNER_WHITE; break;
				case STONE_BLACK:return WINNER_BLACK; break;
				}
		}
		row_count_w = 0;
		row_count_b = 0;
	}
	return WINNER_NON;
}

int ColumCheck()
{

	int col_count_w = 0;
	int col_count_b = 0;
	for (int j = 0; j < STAGE_HEIGHT; j++)
	{
		for (int i = 0; i < STAGE_WIDTH; i++)
		{
			if (map[i][j] == STONE_WHITE) {
				col_count_w++;
			}
			else if (map[i][j] == STONE_BLACK) {
				col_count_b++;
			}
			if (col_count_w == linemax || col_count_b == linemax)
				switch (map[i][j])
				{
				case STONE_WHITE:return WINNER_WHITE; break;
				case STONE_BLACK:return WINNER_BLACK; break;
				}
		}
		col_count_w = 0;
		col_count_b = 0;
	}
	return WINNER_NON;
}


int DiagonalCheck()
{

	int dia_count_w = 0;
	int dia_count_b = 0;
	int Rdia_count_w = 0;
	int Rdia_count_b = 0;
	int R_dia = 2;
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		if (map[i][i] == STONE_WHITE) {
			dia_count_w++;
		}
		else if (map[i][i] == STONE_BLACK) {
			dia_count_b++;
		}
		if (dia_count_w == linemax || dia_count_b == linemax)
			switch (map[i][i])
			{
			case STONE_WHITE:return WINNER_WHITE; break;
			case STONE_BLACK:return WINNER_BLACK; break;
			}

		if (map[i][R_dia] == STONE_WHITE) {
			Rdia_count_w++;
		}
		else if (map[i][R_dia] == STONE_BLACK) {
			Rdia_count_b++;
		}
		if (Rdia_count_w == linemax || Rdia_count_b == linemax)
			switch (map[i][R_dia])
			{
			case STONE_WHITE:return WINNER_WHITE; break;
			case STONE_BLACK:return WINNER_BLACK; break;
			}
		R_dia--;
	}
	return WINNER_NON;
}



