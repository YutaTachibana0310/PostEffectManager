//=====================================
//
//�e���v���[�g�w�b�_[BaseSingleton.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _BASESINGLETON_H_
#define _BASESINGLETON_H_

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
template <class T>
class BaseSingleton
{	
public:
	static T* Instance()
	{
		static T mInstance;

		return &mInstance;
	}

	//static void Destroy()
	//{
	//	delete mInstance;
	//}

protected:
	BaseSingleton() {}
	virtual ~BaseSingleton() {}

	BaseSingleton(const BaseSingleton &) = delete;
	BaseSingleton(BaseSingleton &&) = delete;

	BaseSingleton& operator=(const BaseSingleton &) = delete;
	BaseSingleton& operator=(BaseSingleton &&) = delete;
};

#endif