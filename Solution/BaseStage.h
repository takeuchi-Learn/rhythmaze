#pragma once
#include "GameScene.h"

#include <DirectXMath.h>

#include <memory>

#include "Sound.h"

#include "DebugText.h"

#include "Model.h"
#include "Object3d.h"

#include "Time.h"

#include "Input.h"

#include "Camera.h"

#include "ParticleManager.h"

enum class MAP_NUM : unsigned short {
	UNDEF,		// 未定義
	WALL,		// 壁
	FRONT_ROAD,	// 表迫の道
	BACK_ROAD,	// 裏拍の道
	GOAL
};

class BaseStage
	: public GameScene {

protected:

#pragma region ビュー変換行列

	//DirectX::XMMATRIX matView;
	DirectX::XMFLOAT3 eye_local;   // 視点座標
	DirectX::XMFLOAT3 target_local;   // 注視点座標
	DirectX::XMFLOAT3 up_local;       // 上方向ベクトル

#pragma endregion ビュー変換行列

#pragma region 音

	std::unique_ptr<Sound::SoundCommon> soundCommon;

	std::unique_ptr<Sound> bgm;

	std::unique_ptr<Sound> particleSE;

	float bgmBolume = 0.2f;

#pragma endregion 音

#pragma region スプライト
	SpriteCommon spriteCommon;
#pragma endregion スプライト

#pragma region デバッグテキスト

	DebugText debugText{};
	// デバッグテキスト用のテクスチャ番号を指定
	const UINT debugTextTexNumber = Sprite::spriteSRVCount - 1;
#pragma endregion デバッグテキスト

#pragma region 3Dオブジェクト

	// 3Dオブジェクト用パイプライン生成
	Object3d::PipelineSet object3dPipelineSet;
	Object3d::PipelineSet backPipelineSet;

	const UINT obj3dTexNum = 0U;
	std::unique_ptr<Model> model;
	const float obj3dScale = 10.f;

	const float mapSide = obj3dScale * 2;

	DirectX::XMFLOAT2 angle{};	// 各軸周りの回転角

	std::vector<std::vector<Object3d>> mapObj;

	std::unique_ptr<Model> playerModel;
	std::unique_ptr<Object3d> playerObj;
	float playerScale;

	DirectX::XMFLOAT2 playerMapPos;



	std::unique_ptr<Model> backModel;
	std::unique_ptr<Object3d> backObj;

#pragma endregion 3Dオブジェクト

#pragma region 移動関連情報

	uint16_t beatChangeNum = 0;

	Time::timeType beatChangeTime = 0;

	UINT combo = 0U;

	bool playerMoved = false;
	bool missFlag = false;

	bool frontBeatFlag = true;

#pragma endregion 移動関連情報

#pragma region ライト
	DirectX::XMFLOAT3 light{};
#pragma endregion ライト

#pragma region 時間

	std::unique_ptr<Time> timer;

#pragma endregion 時間

#pragma region カメラ

	std::unique_ptr<Camera> camera;

#pragma endregion カメラ

#pragma region パーティクル

	std::unique_ptr<ParticleManager> particleMgr;

	bool createParticleFlag = false;

#pragma endregion パーティクル

#pragma region シングルトンポインタ格納
	Input* input = nullptr;

	DirectXCommon* dxCom = nullptr;
#pragma endregion シングルトンポインタ格納

#pragma region マップ

	// ファイルから読み込んだ情報を格納する
	std::vector<std::vector<MAP_NUM>> mapData;
	float floorPosY;

#pragma endregion マップ

#pragma region クリア条件
	unsigned clearCount = 250u;
	unsigned clearCombo = 1u;
#pragma endregion クリア条件

#pragma region イージング

	DirectX::XMFLOAT3 easeStartPos{};
	DirectX::XMFLOAT3 easeEndPos{};

	std::unique_ptr<Time> easeTime{};

	bool playerEasing = false;

#pragma endregion イージング

#pragma region ファイルパス

	std::string mapCSVFilePath = "Resources/map/map_stage1.csv";

	enum BOX_TEXNUM
		: unsigned short {
		WALL,
		FRONT,
		BACK,
		GOAL
	};

	std::wstring boxModelPath = L"Resources/model/box/box.obj";
	std::wstring boxModelTexPath_wall = L"Resources/model/box/box.png";
	std::wstring boxModelTexPath_front = L"Resources/model/box/haimidoriBlock.png";
	std::wstring boxModelTexPath_back = L"Resources/model/box/tyrianPurpleBlock.png";
	std::wstring boxModelTexPath_goal = L"Resources/model/box/box.png";

	std::wstring playerModelPath = L"Resources/model/player/note.obj";
	std::wstring playerModelTexPath = L"Resources/model/player/white.png";

	std::wstring effectTexPath = L"Resources/effect1.png";

	std::string bgmFilePath = "Resources/Music/mmc_125_BGM2.wav";

	std::string particleSeFilePath = "Resources/SE/Sys_Set03-click.wav";

	std::wstring backModelPath = L"Resources/model/back/back.obj";
	std::wstring backModelTexPath = L"Resources/model/back/back_tex.png";

#pragma endregion ファイルパス

	float musicBpm = 125.f;

protected:
	void updateLightPosition();

	void updateCamera();

	bool goal();
	void timeOut();

	void updatePlayerPos();

	void updateTime();

	void createParticle(const DirectX::XMFLOAT3 pos,
						const UINT particleNum = 10U, const float startScale = 1.f);
	void startParticle(const DirectX::XMFLOAT3 pos);

	DirectX::XMFLOAT3 easePos(const DirectX::XMFLOAT3 startPos,
							  const DirectX::XMFLOAT3 endPos,
							  const float timeRaito,
							  const int easeTime);

	void drawObj3d();
	void drawParticle();
	void drawSprite();

	virtual void additionalDrawSprite() {};
	virtual void additionalDrawObj3d() {};
	virtual void additionalDrawParticle() {};

	virtual void additionalUpdate() {};

public:
	virtual void pathInit() {};
	virtual void additionalInit() {};

	void init() override;
	void update() override;
	void draw() override;

	// @return 読み込んだcsvの中身。失敗したらデフォルトコンストラクタで初期化された空のvector2次元配列が返る
	static std::vector<std::vector<std::string>> loadCsv(const std::string& csvFilePath);

	void loadMapFile(const std::string& csvFilePath);

};

