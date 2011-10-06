/** \file App.cpp */
#include "App.h"
#include "../data-files/simSettings.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    (void)argc; (void)argv;
    GApp::Settings settings(argc, argv);
    
    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.width       = 1024; 
    settings.window.height      = 768;

#   ifdef G3D_WIN32
	if (FileSystem::exists("data-files", false)) {
            // Running on Windows, building inside the starter directory
            chdir("data-files");
        } else if (FileSystem::exists("../samples/starter/data-files", false)) {
            // Running on Windows, building from the G3D.sln project (TODO: remove this from your program!)
            chdir("../samples/starter/data-files");
        }
#   endif

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings), m_spawn(false), m_brushSize(3),
                                           m_matSelect(0), followPlayer(false)
{
#   ifdef G3D_DEBUG
        // Let the debugger catch unhandled exceptions
        catchCommonExceptions = true;
#   endif
}


void App::onInit() {
    // Called before the application loop beings.  Load data here and
    // not in the constructor so that common exceptions will be
    // automatically caught.

    setCameraManipulator(NULL);
    m_widgetManager->remove(defaultController);
    defaultController = NULL;

    // Turn on the developer HUD    

    setDesiredFrameRate(60);

    debugWindow->setVisible(true);
    developerWindow->cameraControlWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);
    showRenderingStats = true;

    /////////////////////////////////////////////////////////////
    // Debugging controls
    debugPane->addButton("Exit", this, &App::endProgram);
    
    followPlayer = true;
    debugPane->addCheckBox("Camera follows player", &followPlayer);
    
    debugPane->addButton(GuiText("Shader"), this, &App::loadShader, GuiTheme::TOOL_BUTTON_STYLE);

    // Start wherever the developer HUD last marked as "Home"
    defaultCamera.setCoordinateFrame(bookmark("Home"));
    defaultCamera.setFarPlaneZ(-finf());
    
    m_automata = Texture::fromFile(START_FILE, ImageFormat::RGBA8(), 
                                   Texture::DIM_2D_NPOT, 
                                   Texture::Settings::buffer());
    m_automata_new = Texture::fromFile(START_FILE, ImageFormat::RGBA8(), 
                                       Texture::DIM_2D_NPOT, 
                                       Texture::Settings::buffer());  
    //m_automataTextureBox = debugPane->addTextureBox(format("currnet: %p",m_automata.pointer()),m_automata);
    //debugPane->addTextureBox(format("new: %p",m_automata_new.pointer()),m_automata_new);

    m_debugLabels = Texture::createEmpty("labels", m_automata->width(), m_automata->height(),
                                         ImageFormat::RGBA8(), Texture::DIM_2D_NPOT, 
                                         Texture::Settings::buffer());  

    background = Texture::fromFile("fire-lines.jpg", ImageFormat::RGBA8(), 
                                   Texture::DIM_2D_NPOT);
    // TODO non-default texture settings, wrap mode should be clamp

    // Generate noise image
    Random n(System::time());

    GImage noise = GImage(m_automata->width(), m_automata->height(), 1);
    for (int y = 0; y < noise.height(); y++) {
        for (int x = 0; x < noise.width(); x++) {
            int tmp = n.bits();
            noise.pixel1(x, y) = Color1uint8(tmp & 0xFF);
            //noise.pixel1(x, y) = Color1uint8(n.sampleUint8(x << 12, y <<12, 0));
        }
    }
    std::string noise_name;
    m_noise = Texture::fromGImage(noise_name, noise, ImageFormat::R8(),
                                  Texture::DIM_2D_NPOT, Texture::Settings::buffer());
    //noise.save("noise.png", GImage::PNG);
    
    m_cellularFB = Framebuffer::create("simulation");    
    m_cellularFB->set(Framebuffer::COLOR0, m_automata_new);
    #ifdef SIM_TEST
    m_cellularFB->set(Framebuffer::COLOR1, m_debugLabels);
    #endif

    scale = 1.0f;
    //debugPane->addSlider("scale", &scale, 0.1f, 10.0f); 
    debugPane->addNumberBox("", &scale, "", GuiTheme::LINEAR_SLIDER, 0.1f, 10.0f);
    
    debugWindow->pack();
    debugWindow->moveTo(Vector2(0, window()->height() - debugWindow->rect().height()));

    m_HUD = GuiWindow::create("material_selector");
    GuiPane* pane = m_HUD->pane();
    pane->addButton(GuiText("Sand"), this, &App::setMatToSand);
    pane->addButton(GuiText("Water"), this, &App::setMatToWater);
    pane->addButton(GuiText("Wood"), this, &App::setMatToWood);
    pane->addButton(GuiText("Fire"), this, &App::setMatToFire);	
    pane->addNumberBox(GuiText("Brush size"), &m_brushSize, "", GuiTheme::LINEAR_SLIDER, 1, 10, 1);
    
    addWidget(m_HUD);
    
    Array<Texture::Ref> poses;
    poses.resize(10);
    poses[STILL+FACE_RIGHT] = Texture::fromFile("Character/StillRight.png");
    poses[STILL+FACE_LEFT] = Texture::fromFile("Character/StillLeft.png");
    poses[JUMP+FACE_RIGHT] = Texture::fromFile("Character/JumpRight.png");
    poses[JUMP+FACE_LEFT] = Texture::fromFile("Character/JumpLeft.png");

    m_player = new Player(poses,Rect2D::xywh(300,150,poses[0]->width(),poses[0]->height()));
    
    loadShader();

    lastSimTime = 0.0;
    m_simSlice = 0;

    sandActive = Texture::fromFile("Icons/sandIconActive.png");
    sandOff = Texture::fromFile("Icons/sandIconOff.png");
    waterActive = Texture::fromFile("Icons/waterIconActive.png");
    waterOff = Texture::fromFile("Icons/waterIconOff.png");
    woodActive = Texture::fromFile("Icons/woodIconActive.png");
    woodOff = Texture::fromFile("Icons/woodIconOff.png");
    fireActive = Texture::fromFile("Icons/fireIconActive.png");
    fireOff = Texture::fromFile("Icons/fireIconOff.png");
    m_playerMat = R_SAND;

    m_cameraPos = Vector2::zero();
    m_cameraVel = Vector2::zero();

    fireMask = Texture::fromFile("fireMask.png");
}


void App::setMatToSand() {
	m_matSelect = R_SAND;
}

void App::setMatToWater() {
	m_matSelect = R_WATER;
}

void App::setMatToWood() {
	m_matSelect = R_WOOD;
}

void App::setMatToFire() {
	m_matSelect = R_FIRE;
}


void App::loadShader() {
    try {
        Shader::Ref s;
        //load the vertex and pixel shader programs in these files
        #ifdef SIM_TEST
        s = Shader::fromFiles("old_shaders/render.vrt", "old_shaders/render.pix");
        #endif
        #ifdef RENDER
        s = Shader::fromFiles("render.vrt", "render.pix");
        #endif
        m_cellularRenderer = s;
    } catch (const std::string& e) {
        //Report any error and do not set the shader
        drawMessage(e);
        debugPrintf("%s", e.c_str());
        System::sleep(5);
    }
    try {
        Shader::Ref s;
        //load the vertex and pixel shader programs in these files
        #ifdef SIM_TEST
        s = Shader::fromFiles("old_shaders/cellular.vrt", "old_shaders/cellular.pix");
        #endif
        #ifdef RENDER
        s = Shader::fromFiles("cellular.vrt", "cellular.pix");
        #endif
        m_cellularSimulate = s;
    } catch (const std::string& e) {
        //Report any error and do not set the shader
        drawMessage(e);
        debugPrintf("%s", e.c_str());
        System::sleep(5);
    }
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    (void)sdt; (void)rdt;
    // Add physical simulation here.  You can make your time
    // advancement based on any of the three arguments
    if (m_player->spawnPlayer) {
        debugPrintf("player spawning %i at %s\n", m_playerMat,
                    ((m_player->spawnOffset + m_player->m_bounds.x0y0()) / scale ).toString().c_str());
    }

    #ifdef SIM_AUTO
    lastSimTime += idt;

    m_cameraPos += (float)idt * m_cameraVel;

    if (m_simSlice == SIM_SLICES) {
        //debugPrintf("Swapping\n");
        Texture::Ref tmp = m_automata_new;
        m_automata_new = m_automata;
        m_automata = tmp;
        m_cellularFB->set(Framebuffer::COLOR0, m_automata_new);           
        m_cellularFB->bind();
        m_simSlice = 0;
        //renderDevice->push2D(m_cellularFB);
        renderDevice->setColorClearValue(Color4(R_AIR, G_STILL, 0, 1));
        renderDevice->clear();
        //renderDevice->pop2D();
    }

    renderDevice->push2D(m_cellularFB);
    {
        //renderDevice->setFramebuffer(m_cellularFB);
        const Rect2D& bounds = Rect2D::xywh(m_automata->width() * m_simSlice / SIM_SLICES,
                                            0, 
                                            m_automata->width() / SIM_SLICES,
                                            m_automata->height());        
        //debugPrintf("bounds: x = %f y = %f w = %f h = %f\n",bounds.x0(),bounds.y0(),
        //bounds.width(), bounds.height());
        //renderDevice->setViewport(bounds);
        renderDevice->setShader(m_cellularSimulate);
        m_cellularSimulate->args.set("wh", m_automata->rect2DBounds().wh());
        m_cellularSimulate->args.set("currentState", m_automata);
        m_cellularSimulate->args.set("spawn", m_spawn);
        m_cellularSimulate->args.set("mouseXY", m_mouseXY);
        m_cellularSimulate->args.set("brushSize", m_brushSize);
        m_cellularSimulate->args.set("matType", m_matSelect);
        m_cellularSimulate->args.set("noiseTexture", m_noise);
        m_cellularSimulate->args.set("offset", Vector2((float)rand()/(float)RAND_MAX,
                                                       (float)rand()/(float)RAND_MAX));
        m_cellularSimulate->args.set("spawnPlayer", m_player->spawnPlayer);
        m_cellularSimulate->args.set("playerMat", m_playerMat);
        m_cellularSimulate->args.set("playerLoc", (m_player->spawnOffset + 
                                                   m_player->m_bounds.x0y0()) / scale);
        m_cellularSimulate->args.set("cameraLoc", m_cameraPos);
        m_cellularSimulate->args.set("playerRange", m_player->range);
        m_cellularSimulate->args.set("playerFacing", m_player->facing);
        Draw::fastRect2D(bounds, renderDevice, Color3::white());       
    }
    renderDevice->pop2D();
    lastSimTime = 0.0;
    m_simSlice++;

    //debugPrintf("%i out of %i slices\n",m_simSlice, SIM_SLICES);

    #endif

    m_player->onSimulation(idt, idt, m_automata,
                           m_cellularFB, renderDevice);
    m_cellularFB->set(Framebuffer::COLOR0, m_automata_new);
    //debugPrintf("player at (%f, %f)\n",m_player->m_bounds.x0(), m_player->m_bounds.y0());
}


bool App::onEvent(const GEvent& e) {
    if (GApp::onEvent(e)) {
        return true;
    }
    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((e.type == GEventType::GUI_ACTION) && (e.gui.control == m_button)) { ... return true;}
    // if ((e.type == GEventType::KEY_DOWN) && (e.key.keysym.sym == GKey::TAB)) { ... return true; }

    #ifdef SIM_TEST
    if ((e.type == GEventType::MOUSE_BUTTON_DOWN) &&
        insideWindow(Vector2(e.button.x, e.button.y))) {
        m_mouseXY = Vector2int16(e.button.x, e.button.y);
        m_spawn = true;
        return true;
    }

    if (m_spawn && e.type == GEventType::MOUSE_MOTION) {
        m_mouseXY = Vector2int16(e.button.x, e.button.y);
        return true;
    }

    if (e.type == GEventType::MOUSE_BUTTON_UP) {
        m_spawn = false;
        return true;
    }
    #endif

    return false;
}


void App::onUserInput(UserInput* ui) {

    #ifdef RENDER
    m_mouseXY = ui->mouseXY();        
    GKey leftMouse = GKey(GKey::LEFT_MOUSE);
    if (ui->keyDown(leftMouse)) {
        Vector2 offset = (m_automata->rect2DBounds().x0y0() - m_cameraPos) * scale + 
            renderDevice->viewport().center();
        m_mouseXY = (ui->mouseXY() - offset) / scale;//(m_mouseXY - m_cameraPos//renderDevice->viewport().center()) / scale;
        if (!followPlayer) {
            //if (m_automata->rect2DBounds().contains(m_mouseXY)) {
            //debugPrintf("mouse click at (%f, %f), offset = (%f, %f)\nmouse - offset = (%f, %f)\n",ui->mouseXY().x, ui->mouseXY().y,
            //offset.x, offset.y, m_mouseXY.x, m_mouseXY.y);
                //} else {
                //debugPrintf("clicked outside texture\n");
                //}
        }
        m_spawn = true;
    }
    
    else if (ui->keyReleased(leftMouse)) {
        m_spawn = false;
        //debugPrintf("mouse released\n");
    }
    #endif

    #ifdef SIM_ON_USER_INPUT
    GKey space = GKey(GKey::SPACE);
    if (ui->keyDown(space)) {
        if (simTime() - lastSimTime > SIM_TIME) {
            //debugPrintf("Simulating\n");
            Texture::Ref tmp = m_automata_new;
            m_automata_new = m_automata;
            m_automata = tmp;
            m_cellularFB->set(Framebuffer::COLOR0, m_automata_new);           
            m_cellularFB->bind();
            //debugPrintf("storage: %p to shader: %p bound: %p\n",m_automata.pointer(), 
            //m_automata_old.pointer(), m_cellularFB->get(Framebuffer::COLOR0)->texture().pointer());
            renderDevice->push2D(m_cellularFB);
            {
                renderDevice->setFramebuffer(m_cellularFB);
                renderDevice->setViewport(Rect2D::xywh(0,0,m_automata->width(),m_automata->height()));
                renderDevice->setShader(m_cellularSimulate);

                m_cellularSimulate->args.set("wh", m_automata->rect2DBounds().wh());
                m_cellularSimulate->args.set("currentState", m_automata);
                m_cellularSimulate->args.set("appDim", window()->dimensions().wh());
                m_cellularSimulate->args.set("spawn", m_spawn);
                m_cellularSimulate->args.set("mouseXY", m_mouseXY);
                m_cellularSimulate->args.set("brushSize", m_brushSize);
                m_cellularSimulate->args.set("matType", m_matSelect);
                m_cellularSimulate->args.set("noiseTexture", m_noise);
                m_cellularSimulate->args.set("offset", Vector2((float)rand()/(float)RAND_MAX,
                                                               (float)rand()/(float)RAND_MAX));
                Draw::fastRect2D(m_automata->rect2DBounds(), renderDevice, Color3::white());            
            }
            renderDevice->pop2D();
            lastSimTime = simTime();
            m_player->onSimulation(SIM_TIME, SIM_TIME, m_automata,
                                   m_cellularFB, renderDevice);            
        }
    }
    #endif
    
    GKey up    = GKey(GKey::UP);
    GKey down  = GKey(GKey::DOWN);
    GKey left  = GKey(GKey::LEFT);
    GKey right = GKey(GKey::RIGHT);
    GKey one   = GKey('1');
    GKey two   = GKey('2');
    GKey three = GKey('3');
    GKey four  = GKey('4');
    GKey g     = GKey('g');


    if (followPlayer) {
        if (ui->keyPressed(up) && !m_player->falling) {
            m_player->velocity.y = -PLAYER_VERT_VEL;
            m_player->falling = true;
        }
        
        if (ui->keyDown(left)) {
            m_player->velocity.x = -PLAYER_HOR_VEL;
        } else if (ui->keyDown(right)) {
            m_player->velocity.x = PLAYER_HOR_VEL;
        } else {
            m_player->velocity.x = 0;
        }
    } else {
        m_cameraVel = Vector2::zero();
        if (ui->keyDown(up)) {
            m_cameraVel.y = -CAMERA_VERT_VEL;
        } else if (ui->keyDown(down)) {
            m_cameraVel.y = CAMERA_VERT_VEL;
        }
        
        if (ui->keyDown(left)) {
            m_cameraVel.x = -CAMERA_HOR_VEL;
        } else if (ui->keyDown(right)) {
            m_cameraVel.x = CAMERA_HOR_VEL;
        }
    }
    
    if (ui->keyPressed(one)) {
        m_playerMat = R_SAND;
    }
    if (ui->keyPressed(two)) {
        m_playerMat = R_WATER;
    }
    if (ui->keyPressed(three)) {
        m_playerMat = R_WOOD;
    }
    if (ui->keyPressed(four)) {
        m_playerMat = R_FIRE;
    }
    if (ui->keyPressed(g)) {
        m_player->spawnPlayer = true;
    } else if (ui->keyReleased(g)) {
        m_player->spawnPlayer = false;
    }
    if (ui->keyPressed(GKey('k'))) {
        RenderDevice* rd = renderDevice;
        rd->push2D(m_cellularFB);
        rd->setAlphaTest(RenderDevice::ALPHA_GREATER,0);
        rd->setTexture(0, fireMask);
        Draw::fastRect2D(fireMask->rect2DBounds() + (m_player->m_bounds.center() - 
                                                     fireMask->rect2DBounds().center()), rd);
        rd->pop2D();
    }
    
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}

bool App::insideWindow(const Vector2& loc) const {
    return (loc.x > 0 && loc.x < window()->width() &&
            loc.y > 0 && loc.y < window()->height());        
}


void App::onPose(Array<Surface::Ref>& surfaceArray, Array<Surface2D::Ref>& surface2D) {
    // Append any models to the arrays that you want to later be rendered by onGraphics()
    (void)surface2D;
}

void App::onGraphics3D(RenderDevice* rd, Array<Surface::Ref>& surface3D) {}

const Color3 App::determineDebugColor(int x, int y, uint8 currentMat, 
                                      Image4uint8::Ref img) const {
    Vector3 v;
    switch(currentMat) {
    case R_AIR:
        v = Vector3(255.0, 255.0, 255.0);
        break;
    case R_SAND:
        switch(img->fastGet(x, y).g) {
        case G_STILL:
            v = Vector3(237.0, 201.0, 175.0);
            break;
        case G_FALLING:
            v = Vector3(237.0, 255.0, 175.0);
            break;
        default:
            v = Vector3(255.0, 0.0, 255.0);
            break;
        }
        break;
    case R_WATER: 
        switch(img->fastGet(x, y).g) {
        case G_STILL:
            v = Vector3(110.0, 142.0, 225.0);
            break;
        case G_FALLING:
            v = Vector3(110.0, 192.0, 225.0);
            break;
        case G_FLOW_LEFT:
            v = Vector3(160.0, 142.0, 225.0);
            break;
        case G_FLOW_RIGHT:
            v = Vector3(60.0, 142.0, 225.0);
            break;
        default:
            v = Vector3(255.0, 0.0, 255.0);
            break;
        }
        break;
    case R_WOOD:
        v = Vector3(101.0, 67.0, 33.0);
        break;
    case R_FIRE:
        switch(img->fastGet(x, y).g) {
        case G_STILL:
            v = Vector3(204.0, 85.0, 0.0);
            break;
        case G_FALLING:
            v = Vector3(255.0, 159.0, 0.0);
            break;
        }
        break;
    case R_BORDER:
        v = Vector3(255.0, 0.0, 0.0);
        break;
    default:
        v = Vector3(255.0, 0.0, 255.0);
        break;
    }
    return Color3(v/255.0);
} 

void App::onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& posed2D) {
    
    //m_profile.beginCPU("Graphics2D");
    static Vector2 rand_offset = Vector2(0, 0);
    
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction
    
#ifdef SIM_TEST
    rd->push2D();
    std::string s = " ";
    float scale_x = window()->width() / m_automata->width();
    float scale_y = window()->height() / m_automata->height();
    Image4uint8::Ref labelImg = m_debugLabels->toImage4uint8();
    Image4uint8::Ref currentMatImg = m_automata->toImage4uint8();
    for (int y = 0; y < m_debugLabels->height(); ++y) {
        for (int x = 0; x < m_debugLabels->width(); ++x) {
            uint8 currentMat = currentMatImg->fastGet(x, y).r;
            Rect2D rect = Rect2D::xywh(x * scale_x, y * scale_y, 
                                       scale_x - 1, scale_y - 1);            
            if (currentMat != R_AIR) {
                const Color3& boxColor = determineDebugColor(x, y, currentMat, currentMatImg);
                Draw::fastRect2D(rect, rd, boxColor);

                uint8 label = labelImg->fastGet(x, y).r;
                uint8 density = currentMatImg->fastGet(x, y).b;
                float scaleFactor = 3.0;

                // Label the block
                if (label == 0) {
                    if (density != 0) {
                        s = format("(%d)", density);
                    } else {
                        s = " ";
                    }
                } else if (density != 0) {
                    s = format("%d\n(%d)", label, density);
                    scaleFactor = 5.0;
                } else {
                    s = format("%d", label);
                }

                debugFont->draw2D(rd, s, rect.center(), scale_x / scaleFactor,
                                  (boxColor.average() < 0.5f) ?
                                  Color3::white() : Color3::black(),
                                  Color4::clear(),
                                  GFont::XALIGN_CENTER, GFont::YALIGN_CENTER);
            } else {
                Draw::fastRect2D(rect, rd, Color3(1.0, 1.0, 1.0));
            }
        }
    }
    
    rd->pop2D();
#endif

#ifdef RENDER
    
    double currTime = System::time();
    float timeMod = currTime - floor(currTime / (2*pif())) * 2 * pif();
    
    rd->clear();    
    rd->push2D();
    {            
        //m_profile.beginGFX("RENDER");
        rd->setShader(m_cellularRenderer);        
        rd->setTexture(0, m_automata);
        rd->setTexture(1, background);
        m_cellularRenderer->args.set("automata", m_automata);
        m_cellularRenderer->args.set("width", m_automata->width());
        m_cellularRenderer->args.set("height", m_automata->height());
        m_cellularRenderer->args.set("x_offset", rand_offset.x);
        m_cellularRenderer->args.set("y_offset", rand_offset.y);
        m_cellularRenderer->args.set("followPlayer", followPlayer);
        m_cellularRenderer->args.set("background", background);
        m_cellularRenderer->args.set("timeVar", timeMod);

        // IDEA: pass the precomputed reciprocal to avoid division in the shader            
        m_cellularRenderer->args.set("noise", m_noise);
        
        if (followPlayer) {
            Draw::fastRect2D((m_automata->rect2DBounds() - m_player->m_bounds.x0y0())
                             * scale + rd->viewport().center(), rd);
        } else {
            Draw::fastRect2D((m_automata->rect2DBounds() - m_cameraPos)
                             * scale + rd->viewport().center(), rd);
        }
    }
    rd->pop2D();
    rd->setTexture(0, NULL);
    rd->setTexture(1, NULL);
    rd->setShader(NULL);
#endif

#ifdef SHOW_PLAYER
    if (followPlayer) {
        rd->setAlphaTest(RenderDevice::ALPHA_GREATER, 0);
        rd->setTexture(0, m_player->m_display);
        Rect2D playerBounds = m_player->m_bounds * scale;
        playerBounds = playerBounds + (rd->viewport().center() - playerBounds.x0y0());
        Draw::rect2D(playerBounds, rd);
        rd->setTexture(0, NULL);
        rd->setAlphaTest(RenderDevice::ALPHA_ALWAYS_PASS, 0);
    }
#endif

    Surface2D::sortAndRender(rd, posed2D);
    
    // Update the random offset vector
    static int update_counter = 0;
    static Random n(System::time());
    if (update_counter > 10) {
        update_counter = 0;
        rand_offset.x = n.uniform();
        rand_offset.y = n.uniform();
    }
    else {
        ++update_counter;
    }

    /* Draw HUD */
#ifdef SHOW_HUD
    const Vector2& HUDLoc     = Vector2(5, 5);    
    const Vector2& HUDSpacing = Vector2(ICON_SIZE, 0);
    const Vector2& IconDim    = Vector2(ICON_SIZE, ICON_SIZE);
    Draw::fastRect2D(Rect2D::xywh(0,0,HUDLoc.x * 2 + HUDSpacing.x * 4, 
                                  HUDLoc.y * 2 + IconDim.y), 
                     rd, Color3(0.5, 0.5,0.5));
    if (m_playerMat == R_SAND) {
        rd->setTexture(0,sandActive);
        m_player->range = RANGE_SAND;
    } else {
        rd->setTexture(0,sandOff);
    }
    Draw::fastRect2D(Rect2D::xywh (HUDLoc,IconDim), rd);
    if (m_playerMat == R_WATER) {
        rd->setTexture(0,waterActive);
        m_player->range = RANGE_WATER;
    } else {
        rd->setTexture(0,waterOff);
    }
    Draw::fastRect2D(Rect2D::xywh(HUDLoc + HUDSpacing, IconDim), rd);
    if (m_playerMat == R_WOOD) {
        rd->setTexture(0,woodActive);
        m_player->range = RANGE_WOOD;
    } else {
        rd->setTexture(0,woodOff);
    }
    Draw::fastRect2D(Rect2D::xywh(HUDLoc + HUDSpacing * 2, IconDim), rd);
    if (m_playerMat == R_FIRE) {
        rd->setTexture(0,fireActive);
        m_player->range = RANGE_FIRE;
    } else {
        rd->setTexture(0,fireOff);
    }
    Draw::fastRect2D(Rect2D::xywh(HUDLoc + HUDSpacing * 3, IconDim), rd);
    rd->setTexture(0, NULL);
#endif
}

void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught
    delete m_player;
}


void App::endProgram() {
    m_endProgram = true;
}
