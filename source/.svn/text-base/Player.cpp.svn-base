#include "Player.h"

Player::Player (const Array<Texture::Ref> textures, const Rect2D& bounds) {
    poses = textures;
    m_display = poses[STILL + FACE_RIGHT];
    m_bounds = bounds;
    falling = false;
    velocity = Vector2int16(0, 0);
    spawnPlayer = false;
    facing = FACE_RIGHT;
    spawnOffset = Vector2(bounds.width() * MIN_PLACEMENT_FACTOR, bounds.height() / 2);
    speedFactor = Vector2(1.0f, 1.0f);
    range = 1.0f;
}

void Player::onSimulation(GameTime absoluteTime, GameTime deltaTime, 
                          const Texture::Ref& world,
                          Framebuffer::Ref buffer, RenderDevice* rd) {

    if (m_bounds.x0() < 0 || m_bounds.y0() < 0) {
        m_bounds = Rect2D::xywh(2 * m_bounds.width(),
                    world->height() - 2 * m_bounds.height(),
                    m_bounds.width(), m_bounds.height());
    }

    Vector2 delta = Vector2(velocity) * deltaTime;
    delta = delta * speedFactor;
    Rect2D nextXBounds = m_bounds + Vector2(delta.x, 0);
    Rect2D nextYBounds = m_bounds + Vector2(0, delta.y);

    const int width = m_bounds.width();
    const int height = m_bounds.height();
    const float x0 = nextXBounds.x0();
    const float y0 = nextYBounds.y0();
    Color4uint8 imgX[width * height];
    Color4uint8 imgY[width * (height + 1)];

    // Here we read back from the texture twice to make the collision
    // detection easier. The read into imgX is from where the player 
    // would be if he just moved in the x-direction. Similarly, the read 
    // into imgY is from where the player would be if he just moved in
    // the y-direction. Checking these two separately means we don't have
    // to worry about which axis of movement caused the collision.
    buffer->set(Framebuffer::COLOR0, world);
    rd->pushState(buffer);
    glReadPixels(x0, m_bounds.y0(), width, height,
                 GL_RGBA, GL_UNSIGNED_BYTE, imgX);
    glReadPixels(m_bounds.x0(), y0, width, height + 1,
                 GL_RGBA, GL_UNSIGNED_BYTE, imgY);
    rd->popState();

    /* Animation */
    if (velocity.x < 0) {
        facing = FACE_LEFT;
        spawnOffset.x = -m_bounds.width() * (MIN_PLACEMENT_FACTOR - 1.0);
    } else if (velocity.x > 0) {
        facing = FACE_RIGHT;
        spawnOffset.x = m_bounds.width() * MIN_PLACEMENT_FACTOR;
        
    }
    if (velocity.y != 0) {
        m_display = poses[JUMP + facing];
    } else {
        m_display = poses[STILL + facing];
    }

    /* Vertical movement */
    if (velocity.y != 0) {
        speedFactor.y = 1.0f;
        for (int y = 0; y < height && falling; ++y) { 
            for (int x = 0; x < width; ++x) {
                uint8 pixel = imgY[x + y * width].r;
                if (pixel == R_WOOD || pixel == R_SAND || 
                    pixel == R_BORDER) {
                    if (velocity.y <= 0) {
                        velocity.y = 0;
                        delta.y = min(0.0f, delta.y + 1);
                    } else {
                        velocity.y = 0;
                        delta.y += (y - (height));
                        falling = false;
                        break;
                    }
                } else if (pixel == R_WATER) {
                    speedFactor.y = 0.5f;
                }
            }
        }
    } else {
        falling = true;
        for (int x = 0; x < width; ++x) {
            uint8 pixel = imgY[x + height * width].r;
            if (pixel == R_WOOD || pixel == R_SAND ||
                pixel == R_BORDER) {
                falling = false;
                break;
            }
        }
    }
        
    /* Horizontal movement */
    if (velocity.x != 0) {
        speedFactor.x = 1.0f;
        for (int y = 1; y < height; ++y) { 
            for (int x = 0; x < width; ++x) {
                uint8 pixel = imgX[x + y * width].r;
                if (pixel == R_WOOD || pixel == R_SAND ||
                    pixel == R_BORDER) {                    
                    if (velocity.x < 0) {
                        delta.x = min(0.0f, delta.x + 1);
                    } else {
                        delta.x = max(0.0f, delta.x + (x - width));
                    }                    
                } else if (pixel == R_WATER) {
                    speedFactor.x = 0.5f;
                }
            }
        }
    }

    if (falling) {
        velocity.y += GRAVITY;
    }
        
    m_bounds = m_bounds + delta;
}
