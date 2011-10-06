/**
  @file App.h

  The G3D 9.0 default starter app is configured for OpenGL 3.0 and relatively recent
  GPUs.  To support older GPUs you may want to disable the framebuffer and film
  classes and use G3D::Sky to handle the skybox.
 */
#ifndef App_h
#define App_h

#include <G3D/G3DAll.h>
#include <algorithm>

#include "Player.h"

class App : public GApp {
    Texture::Ref        m_automata;
    Texture::Ref        m_automata_new;
    Shader::Ref         m_cellularSimulate;
    Shader::Ref         m_cellularRenderer;
    GuiTextureBox*	    m_automataTextureBox;
    Framebuffer::Ref    m_cellularFB;
    bool                m_spawn;
    Vector2             m_mouseXY;
    int                 m_brushSize;
    GuiWindow::Ref      m_HUD;
    int                 m_matSelect;
    Texture::Ref        m_noise;
    Texture::Ref        m_debugLabels;
    SimTime             lastSimTime;
    Player*             m_player;
    bool                followPlayer;
    int                 m_simSlice;
    Texture::Ref        background;
    float               scale;
    Vector2             m_cameraPos;
    Vector2             m_cameraVel;
    Texture::Ref        fireMask;

    int                 m_playerMat;
    Texture::Ref        sandActive;
    Texture::Ref        sandOff;
    Texture::Ref        waterActive;
    Texture::Ref        waterOff;
    Texture::Ref        woodActive;
    Texture::Ref        woodOff;
    Texture::Ref        fireActive;
    Texture::Ref        fireOff;

    /** Loads direct.vrt and direct.pix, a vertex and pixel shader, if they exist.
     Otherwise, it loads nothing.*/
    void loadShader();

    bool insideWindow(const Vector2& loc) const;

    void setMatToSand();
    
    void setMatToWater();
    
    void setMatToWood();
    
    void setMatToFire();

    const Color3 determineDebugColor(int x, int y, uint8 currentMat, 
                                     Image4uint8::Ref img) const;

public:
    
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit();
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
    virtual void onPose(Array<Surface::Ref>& posed3D, Array<Surface2D::Ref>& posed2D);

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<Surface::Ref>& surface, Array<Surface2D::Ref>& surface2D);

    virtual void onGraphics3D(RenderDevice* rd, Array<Surface::Ref>& surface);
    virtual void onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& surface2D);

    virtual bool onEvent(const GEvent& e);
    virtual void onUserInput(UserInput* ui);
    virtual void onCleanup();

    /** Sets m_endProgram to true. */
    virtual void endProgram();
};

#endif
