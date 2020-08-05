// ==============================
// 2020年 前期 
// ゲームプログラミング１
// 制作課題 その３
// ==============================

/**
■説明
	以下のプログラムは [ 倉庫番 ] というゲームをコーディングし、
	実行を確認したうえでその一部分を削除したものです。

	コメント内容や他のコードを読み解き、ゲームを完成させなさい。

	必須条件
		ステージデータは、外部ファイルstage.txtから読み込むこと
		sample_stage.txtを用意するが、最終的に独自のステージを作成すること
		※※と記載されている以外にも、必要と思われる変数、関数は独自に追加すること

	推奨条件
		操作キャラクターの表示を、入力された向き合わせた画像に切り替える
		操作キャラクターの画像をアニメーションさせる

	※Hit And Blow、マルバツよりもさらにヒントが少ないです。
	　ゲームのルールに対して、どのような処理が必要かを考える事も、プログラマーの仕事です。
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
■倉庫番のルール
	自分で調べてみましょう。
*/


#include "WinMain.h"
#include "InputManager.h"
#include "DrawManager.h"
#include "GameDefinition.h"
#include <stdio.h>

// ==============================
// 関数プロトタイプ宣言
// ==============================
//※※ 必要に応じて関数を追加する
void FileInPut();





int map[STAGE_HEIGHT][STAGE_WIDTH] = {};

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
	SetBackgroundColor(125, 125, 125);
	SetMainWindowText("倉庫番だよ！！！！");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	// ローカル変数宣言
	// ----------------------------------------------------





	// 各種初期化処理
	// ----------------------------------------------------
	InputInit();			// 入力処理初期化関数の呼び出し
	DrawInit();				// 描画処理初期化関数の呼び出し

	// 正しく読み込めたかチェック
	if ( InitSuccessed() == false )
	{
		DrawEnd();
		DxLib_End();
		return 0;
	}

	// ゲームを初期化
	// ----------------------------------------------------

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
		InputUpdate();	// 入力処理更新関数の呼び出し

		※※ 必要な処理をコーディング	
		// --- 入力
		// ゲームをリセット
		if (IsPushKey(MY_INPUT_RESET))
		{
			// 詰まってしまった場合にゲームを落とさずリスタートできるようにする
			※※　// 画面をゲーム開始時と同じ状態に戻す
		}

		// 以下、描画処理
		// ----------------------------------------------------
		//※※	// ステージ描画
		FileInPut();
		for (int i = 0; i < STAGE_HEIGHT; i++)
		{
			for (int j = 0; j < STAGE_WIDTH; j++)
			{


				DrawObject(i,j, map[i][j]);//※※	// 地面、壁、プレイヤーなどの表示
			}
		
		}
		DrawGameClear();//※※	// ゲームをクリアしていたら、追加でクリア表示

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


void FileInPut()
{
	FILE* fp;
	fopen_s(&fp, "map.txt", "rb");
	if (fp == NULL)
	{
		return;
	}


	const int lineMax = 30;
	char tmpLines[lineMax];
	int count_x = 0;
	int count_y = 0;


	while (fgets(tmpLines, lineMax, fp) != NULL)
	{
		for (int i = 0; i < lineMax; i++)
		{
			if (tmpLines[i] == '\0')
			{
				count_y++;
				count_x = 0;
				break;
			}
			else if (tmpLines[i] == ',')
			{
				count_x++;
			}
			else if (tmpLines[i] >= '0' && tmpLines[i] <= '9')
			{
				map[count_y][count_x] = tmpLines[i] - '0';		//ASCIIcodeの0
			}
		}
	}
	fclose(fp);
}
