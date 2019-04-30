//=====================================
//
//テンプレートヘッダ[PostEffectManager.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _POSTEFFECTMANAGER_H_
#define _POSTEFFECTMANAGER_H_

#include "main.h"
#include "PostEffect\/ScreenObject.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class PostEffectManager
{
public:
	static PostEffectManager* Instance();
	static void Destroy();
	void Update();
	void Draw();

private:
	//シングルトンインスタンス
	static PostEffectManager* instance;

	//シングルトン化
	PostEffectManager();
	~PostEffectManager();
	PostEffectManager(const PostEffectManager &) = delete;
	PostEffectManager(PostEffectManager &&) = delete;
	PostEffectManager& operator=(const PostEffectManager &) = delete;
	PostEffectManager& operator=(PostEffectManager &&) = delete;
};


#endif