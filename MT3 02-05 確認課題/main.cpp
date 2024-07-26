#include <Novice.h>
#include <Matrix.h>

const char kWindowTitle[] = "GC2B_02_イトウ_シュンヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Matrix matrix;

	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };
	Vector3 cameraPosition = { 0.0f,0.f,-6.0f };

	Matrix4x4 cameraMatrix = Matrix::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.26f,0.0f,0.0f }, cameraPosition);
	Matrix4x4 viewMatrix = Matrix::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = Matrix::MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
	Matrix4x4 viewportMatrix = Matrix::MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
	Matrix4x4 viewProjectionMatrix = Matrix::Multiply(viewMatrix, projectionMatrix);

	Matrix::AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{},
	};

	Matrix::AABB aabb2{
		.min{0.2f,0.2f,0.2f},
		.max{1.0f,1.0f,1.0f},
	};

	uint32_t aabb1Color = 0xFFFFFFFF;
	uint32_t aabb2Color = 0xFFFFFFFF;
	
	bool iscollision = Matrix::IsCollision(aabb1, aabb2);

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

		cameraMatrix = Matrix::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPosition);
		viewMatrix = Matrix::Inverse(cameraMatrix);
		projectionMatrix = Matrix::MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
		viewportMatrix = Matrix::MakeViewportMatrix(0, 0, float(1280), float(720), 0.0f, 1.0f);
		viewProjectionMatrix = Matrix::Multiply(viewMatrix, projectionMatrix);

		iscollision = Matrix::IsCollision(aabb1, aabb2);
		if (iscollision == true) {
			aabb1Color = RED;
		}
		else { aabb1Color = WHITE; }

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraPosition.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		matrix.DrawGrid(viewProjectionMatrix, viewportMatrix);
		matrix.DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, aabb1Color);
		matrix.DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, aabb2Color);

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
