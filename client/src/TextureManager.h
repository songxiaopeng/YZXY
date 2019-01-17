#pragma once
#include "defines.h"
#include "defineTypes.h"
#include "Singleton.h"
#include "core/Texture.h"
#include <unordered_map>
class TextureManager final : public Singleton<TextureManager>
{
public:

using Map=std::unordered_map<String,Texture*>;

    TextureManager();
    ~TextureManager();
    void LoadTexture(String path);
    Texture* LoadTexture(String path,int w,int h,bool alpha,uint8* imageData);
    void UnloadTexture(String path);
    Texture* GetTexture(String path) ;
    void ClearAll();
private:
    
    Map m_Textures;

};
#define TEXTURE_MANAGER_INSTANCE TextureManager::GetInstance()