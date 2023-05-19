#include <Novice.h>
#include "Calc.h"
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LD2A_トリヤマ_ミヒロ";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
//ベクトルのx,y,zの結果と文字を出力する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	float theta = 0.0f;

		Vector3 v1{ 1.2f,-3.9f,2.5f };
		Vector3 v2{ 2.8f,0.4f,-1.3f };
		Vector3 cross = Cross(v1, v2);

		Vector3 cameraPositinon = {0,0,-20 };

		Vector3 rotate{};
		//Vector3 translate{ 640,360,1000 };
		Vector3 translate{ };

		Vector3 screenVertices[3] = { 0 };

		Vector3 kLoacalVertices[3] = { {0,2,0},{2,-2,0},{-2,-2,0} };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		rotate.y += 0.1f;

		if (keys[DIK_A]) {
			translate.x -= 0.1f;
		}
		if (keys[DIK_D]) {
			translate.x += 0.1f;
		}
		if (keys[DIK_W]) {
			translate.z += 0.1f;
		}
		if (keys[DIK_S]) {
			translate.z -= 0.1f;
		}

		Matrix4x4 worldMatrix = MakeAffinMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);

		Matrix4x4 cameraMatrix = MakeAffinMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPositinon);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 ViewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		
		//Vector3 kLoacalVertices[3] = { {640,260,1000},{540,460,1000},{740,460,1000} };
		for (uint32_t i = 0; i < 3; i++) {
			Vector3 ndcVertex = Transform(kLoacalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, ViewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, cross, "Cross");

		Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
