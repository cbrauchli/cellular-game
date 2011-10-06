/** 

    @file Player.h

*/
#ifndef Player_h
#define Player_h

#include <G3D/G3DAll.h>
#include "../data-files/simSettings.h"

// poses
#define STILL 0
#define JUMP 2

class Player {
    
 protected:
    
    Vector2int16 appDim;
    
    Array<Texture::Ref> poses;

    Vector2 speedFactor;

 public:
    
    Rect2D m_bounds;

    Texture::Ref m_display;

    Player (const Array<Texture::Ref> textures, const Rect2D& bounds);

    Vector2int16 velocity;

    bool falling;

    bool spawnPlayer;

    float range;

    int facing;

    Vector2 spawnOffset;

    void onSimulation(GameTime absoluteTime, GameTime deltaTime, 
                      const Texture::Ref& world,
                      Framebuffer::Ref buffer, RenderDevice* rd);
};

#endif
