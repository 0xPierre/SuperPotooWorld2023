#include "ControlsInput.h"
#include "StaticMap.h"

ControlsInput::ControlsInput() :
    InputGroup(), hAxis(0.0f),
    jumpDown(false), jumpPressed(false), goDownDown(false),
    terrainSelected((int)Tile::Type::WOOD),
    groundSelected(4)
{
}

void ControlsInput::OnPreEventProcess()
{
    jumpPressed = false;
}

void ControlsInput::OnEventProcess(SDL_Event evt)
{
    SDL_Scancode scanCode;
    switch (evt.type)
    {
    case SDL_JOYHATMOTION:
        switch (evt.jhat.value)
        {
        case SDL_HAT_CENTERED:
        case SDL_HAT_UP:
            hAxis = 0.0f;
            goDownDown = false;
            break;
        case SDL_HAT_RIGHT:
        case SDL_HAT_RIGHTUP:
        case SDL_HAT_RIGHTDOWN:
            hAxis = +1.0f;
            goDownDown = false;
            break;
        case SDL_HAT_LEFT:
        case SDL_HAT_LEFTUP:
        case SDL_HAT_LEFTDOWN:
            hAxis = -1.0f;
            goDownDown = false;
            break;
        case SDL_HAT_DOWN:
            goDownDown = true;
            break;
        default:
            break;
        }
        break;

    case SDL_JOYAXISMOTION:
        if (evt.jaxis.axis == 0)
        {
            // Joystick gauche : Axe X
            float value = (float)(evt.jaxis.value);
            if (fabsf(value) < 8000.0f)
                hAxis = 0.0f;
            else
                hAxis = value / 32768.0f;

        }
        break;

    case SDL_JOYBUTTONDOWN:
        switch (evt.jbutton.button)
        {
        case 0: case 2:
            // Saut
            jumpDown = true;
            jumpPressed = true;
            break;
        default:
            break;
        }
        break;

    case SDL_JOYBUTTONUP:
        switch (evt.jbutton.button)
        {
        case 0: case 2:
            // Saut
            jumpDown = false;
            break;
        default:
            break;
        }
        break;

    case SDL_KEYDOWN:
        scanCode = evt.key.keysym.scancode;

        if (evt.key.repeat)
            break;
        
        switch (scanCode)
        {
        case SDL_SCANCODE_RIGHT:
            // Deplacement à droite
            hAxis = 1.f;
            break;

        case SDL_SCANCODE_LEFT:
            // Deplacement à gauche
            hAxis = -1.f;
            break;

        case SDL_SCANCODE_DOWN:
            // Descente d'une plateforme
            goDownDown = true;
            break;

        case SDL_SCANCODE_SPACE:
        case SDL_SCANCODE_UP:
            // Saut
            jumpDown = true;
            jumpPressed = true;
            break;

        case SDL_SCANCODE_1:
            terrainSelected = (int)Tile::Type::WOOD;
            break;
            
        case SDL_SCANCODE_2:
            terrainSelected = (int)Tile::Type::GROUND;
            groundSelected = 1;
            break;

        case SDL_SCANCODE_3:
            terrainSelected = (int)Tile::Type::GROUND;
            groundSelected = 4;
            break;

        case SDL_SCANCODE_4:
            terrainSelected = (int)Tile::Type::GROUND;
            groundSelected = 0;
            break;

        case SDL_SCANCODE_5:
            terrainSelected = (int)Tile::Type::GROUND;
            groundSelected = 2;
            break;

        case SDL_SCANCODE_6:
            terrainSelected = (int)Tile::Type::STEEP_SLOPE_R;
            groundSelected = 9;
            break;

        case SDL_SCANCODE_7:
            terrainSelected = (int)Tile::Type::STEEP_SLOPE_L;
            groundSelected = 10;
            break;

        case SDL_SCANCODE_8:
            terrainSelected = (int)Tile::Type::GENTLE_SLOPE_L1;
            groundSelected = 15;
            break;

        case SDL_SCANCODE_9:
            terrainSelected = (int)Tile::Type::GENTLE_SLOPE_L2;
            groundSelected = 16;
            break;        

        default:
            //terrainSelected = (int)Tile::Type::WOOD;
            break;
        }

        switch (evt.key.keysym.sym)
        {
            case SDLK_a:
                terrainSelected = (int)Tile::Type::GENTLE_SLOPE_R1;
                groundSelected = 12;
				break;
            case SDLK_z:
                terrainSelected = (int)Tile::Type::GENTLE_SLOPE_R2;
                groundSelected = 13;
                break;
            case SDLK_e:
                terrainSelected = (int)Tile::Type::GROUND;
                groundSelected = 14;
                break;
            case SDLK_r:
                terrainSelected = (int)Tile::Type::GROUND;
                groundSelected = 17;
                break;
            case SDLK_t:
                terrainSelected = (int)Tile::Type::GROUND;
                groundSelected = 8;
                break;
            case SDLK_y:
                terrainSelected = (int)Tile::Type::GROUND;
                groundSelected = 11;
                break;
            case SDLK_u:
                terrainSelected = (int)Tile::Type::SPIKE;
                break;
            case SDLK_q:
                terrainSelected = (int)Tile::Type::ONE_WAY;
                break;
            case SDLK_s:
                terrainSelected = (int)Tile::Type::BRICK;
                break;
            case SDLK_d:
                terrainSelected = (int)Tile::Type::BONUSFULL;
                break;
            case SDLK_f:
                terrainSelected = (int)Tile::Type::BONUSEMPTY;
                break;
            case SDLK_w:
                terrainSelected = (int)Tile::Type::LEVELEND;
                break;
            case SDLK_o:
                terrainSelected = (int)Tile::Type::FIREFLY;
				break;
            //case SDLK_d:
            //    // BRICK
            //    terrainSelected = (int)Tile::Type::BRICK;
            //    break;
            //case SDLK_d:
            //    // BRICK
            //    terrainSelected = (int)Tile::Type::BRICK;
            //    break;
        }

        break;

    case SDL_KEYUP:
        scanCode = evt.key.keysym.scancode;

        switch (scanCode)
        {
        case SDL_SCANCODE_RIGHT:
            // Deplacement à droite
            if (hAxis > 0.f)
                hAxis = 0.f;
            break;

        case SDL_SCANCODE_LEFT:
            // Deplacement à gauche
            if (hAxis < 0.f)
                hAxis = 0.f;
            break;

        case SDL_SCANCODE_DOWN:
            // Descente d'une plateforme
            goDownDown = false;
            break;

        case SDL_SCANCODE_SPACE:
        case SDL_SCANCODE_UP:
            // Saut
            jumpDown = false;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void ControlsInput::Reset()
{
    hAxis = 0.0f;
    jumpDown = false;
    jumpPressed = false;
    goDownDown = false;
}
