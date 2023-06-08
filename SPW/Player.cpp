#include "Player.h"

#include <chrono>

#include "Camera.h"
#include "Scene.h"
#include "Collectable.h"
#include "Enemy.h"
#include "Graphics.h"
#include "LevelParser.h"
#include "LevelScene.h"
#include "StaticMap.h"
#include <time.h>
#include "Brick.h"
#include "Creative.h"

Player::Player(Scene &scene) :
    GameBody(scene, Layer::PLAYER), m_animator(),
    m_jump(false), m_facingRight(true), m_bounce(false), m_hDirection(0.0f),
    m_lifeCount(3), m_fireflyCount(0), m_heartCount(2), m_state(Player::State::IDLE)
{
    m_name = "Player";
    m_immunityEnabled = false;

    SetToRespawn(true);

    AssetManager& assetsManager = scene.GetAssetManager();
    RE_Atlas* atlas = assetsManager.GetAtlas(AtlasID::PLAYER);
    RE_AtlasPart* part = nullptr;

    // Animation "Idle"
    part = atlas->GetPart("Idle");
    AssertNew(part);
    RE_TexAnim* idleAnim = new RE_TexAnim(
        m_animator, "Idle", part
    );
    idleAnim->SetCycleCount(0);

    // TODO : ajouter l'animation "Falling"
    // Animation "Falling"
    part = atlas->GetPart("Falling");
    AssertNew(part);
    RE_TexAnim* fallingAnim = new RE_TexAnim(
        m_animator, "Falling", part
    );
    fallingAnim->SetCycleCount(-1);
    fallingAnim->SetCycleTime(0.2f);

    // Animation "RunningForward"
    part = atlas->GetPart("Running");
    AssertNew(part);
    RE_TexAnim* runninForwardgAnim = new RE_TexAnim(
        m_animator, "RunningForward", part
    );
    runninForwardgAnim->SetCycleCount(-1);
    runninForwardgAnim->SetCycleTime(0.2f);

    // Animation "Dying"
    part = atlas->GetPart("Dying");
    AssertNew(part);
    RE_TexAnim* dyingAnim = new RE_TexAnim(
        m_animator, "Dying", part
    );
    dyingAnim->SetCycleCount(-1);
    dyingAnim->SetCycleTime(0.2f);
    m_animator.StopAnimation("Dying");

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 0;

    m_fallTimeMemory = time(NULL);
    m_jumped = false;
}

Player::~Player()
{
}

void Player::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World& world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = (char*)"Player";
    bodyDef.damping.SetZero();
    PE_Body* body = world.CreateBody(bodyDef);
    SetBody(body);

    // Création du collider
    PE_ColliderDef colliderDef;

    // TODO : Donner une taille normale à la capsule
    PE_CapsuleShape capsule(PE_Vec2(0.0f, 0.35f), PE_Vec2(0.0f, 0.85f), 0.35f);
    colliderDef.friction = 1.0f;
    colliderDef.filter.categoryBits = CATEGORY_PLAYER;
    colliderDef.shape = &capsule;
    PE_Collider* collider = body->CreateCollider(colliderDef);
}

void Player::Update()
{
    ControlsInput& controls = m_scene.GetInputManager().GetControls();
    const clock_t currentTime = clock();
    const double milliseconds = (double)currentTime / (CLOCKS_PER_SEC / 1000);

    // Sauvegarde les contrôles du joueur pour modifier
    // sa physique au prochain FixedUpdate()

    if (m_state == State::DEAD)
    {
        return;
    }

    // TODO : Mettre à jour l'état du joueur en fonction des contrôles de jump
    m_jump = controls.jumpPressed;
    m_longJump = controls.longJump;

    m_hDirection = controls.hAxis;

    LevelScene* levelScene = (LevelScene* )(&m_scene);
    MouseInput& mouse = m_scene.GetInputManager().GetMouse();
    /*PE_Vec2 Pos;*/

    if (!levelScene->IsPaused())
    {
        if (mouse.leftReleased && levelScene->IsCreative())
        {
            levelScene->GetCreative()->AddItem(levelScene->GetSelectedTile(), levelScene->GetSelectedPartIdx(), mouse);
        }
        if (controls.shiftLPressed && levelScene->IsCreative()) {
            levelScene->GetCreative()->RemoveItem(mouse);
        }
        if (mouse.middleClick && levelScene->IsCreative())
        {
            Tile::Type type = levelScene->GetCreative()->SelectItem(mouse);

            controls.terrainSelected = (int)type;

            switch (type)
            {
            case Tile::Type::STEEP_SLOPE_L:
                controls.groundSelected = 9;
                break;
            case Tile::Type::STEEP_SLOPE_R:
                controls.groundSelected = 10;
                break;
            case Tile::Type::GENTLE_SLOPE_R1:
                controls.groundSelected = 15;
                break;
            case Tile::Type::GENTLE_SLOPE_R2:
                controls.groundSelected = 16;
                break;
            case Tile::Type::GENTLE_SLOPE_L1:
                controls.groundSelected = 13;
                break;
            case Tile::Type::GENTLE_SLOPE_L2:
                controls.groundSelected = 12;
                break;
                break;
            case Tile::Type::GROUND:
                controls.groundSelected = 4;
				break;
            default:
                controls.groundSelected = 0;
				break;
            }
        }
    }
}

void Player::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    const clock_t currentTime = clock();
    const double milliseconds = (double)currentTime / (CLOCKS_PER_SEC / 1000);

    if (((LevelScene* )&m_scene)->IsCreative() && ((LevelScene*)&m_scene)->GetCamIndex() != 0) return;

    // Met à jour les animations du joueur
    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    m_isFlipped = false;
    SDL_FRect rect = { 0 };

    if (m_hDirection == -1.0f)
    {
        flip = SDL_FLIP_HORIZONTAL;
        m_isFlipped = true;
    }

    rect.h = 1.375f * scale; // Le sprite fait 1.375 tuile de haut
    
    if (m_state == State::DEAD)
    {
        m_animator.PlayAnimation("Dying");
    }
    else
    {
        if (m_onGround && m_jumped)
        {
            m_jumpAnimating = true;
            m_jumped = false;
        }
        else if (!m_jumpAnimating)
        {
            jumpTimeAnimMemory = milliseconds;
        }

        if (m_jumpAnimating)
        {
            rect.h -= (milliseconds - jumpTimeAnimMemory) / 10;
            if ((milliseconds - jumpTimeAnimMemory) > 100)
            {
                m_jumpAnimating = false;   
            }
        }

    }
    
    if (m_state != State::DYING && m_heartCount < 5 && m_state != State::DEAD)
    {
        if (time(NULL) - m_livesTimeMemory < 2)
        {
            m_animator.MultAlpha(0.8);
            if ((milliseconds - immunityTimeMemory) > 200)
            {
                if (!m_immunityState)
                {
                    m_immunityState = true;
                }
                else
                {
                    m_immunityState = false;
                }
                immunityTimeMemory = milliseconds;
            }
            if (m_immunityState)
            {
                rect.h *= 1.3;
                rect.w *= 1.3;
            }
        }
        else
        {
            m_immunityEnabled = false;
        }
    }

    // TODO : Trouver les bonnes diemnsions de l'affichage en fonction du sprite (dimensions en tuiles)
    rect.w = 1.000f * scale; // Le sprite fait 1 tuile de large
    camera->WorldToView(GetPosition(), rect.x, rect.y);

    float angle = 0.0f;
    if (m_state == State::DEAD)
    {
        if (milliseconds - m_deadTimeMemory < 2000)
        {
            rect.y += (milliseconds - m_deadTimeMemory) / 2;
            angle += (milliseconds - m_deadTimeMemory) / 10;
        }
        else
        {
            Kill();
        }
    }
    else
    {
        m_deadTimeMemory = milliseconds;
    }

    // Dessine l'animateur du joueur
    // TODO : Trouver le bon anchor
    m_animator.RenderCopyExF(
        &rect, RE_Anchor::SOUTH , angle, Vec2(0.5f, 0.5f), flip
    );
}

void Player::FixedUpdate()
{
    PE_Body *body = GetBody();
    PE_Vec2 position = body->GetPosition();
    // TODO : Récuperer la vitesse du joueur
    PE_Vec2 velocity = body->GetLocalVelocity();
    const clock_t currentTime = clock();
    const double milliseconds = (double)currentTime / (CLOCKS_PER_SEC / 1000);

    // Réveille les corps autour du joueur

    LevelScene *levelscene = (LevelScene *)(&m_scene);
    if (levelscene->IsCreative() && levelscene->GetCamIndex() != 0) return;

    if (!levelscene->IsCreative())
        WakeUpSurroundings();

    if(m_state == State::DEAD)
    {
        return;
    }
    
    // Tue le joueur s'il tombe dans un trou
    if (position.y < -2.0f)
    {
        m_scene.Respawn();
        return;
    }

    //--------------------------------------------------------------------------
    // Détection du sol

    m_onGround = false;
    PE_Vec2 gndNormal = PE_Vec2::up;

    // Lance deux rayons vers le bas ayant pour origines
    // les coins gauche et droit du bas du collider du joueur
    // Ces deux rayons sont dessinés en jaune dans DrawGizmos()
    PE_Vec2 originL = position + PE_Vec2(-0.35f, +0.5f);
    PE_Vec2 originR = position + PE_Vec2(+0.35f, +0.5f);

    // Les rayons ne touchent que des colliders solides (non trigger)
    // ayant la catégorie FILTER_TERRAIN
    RayHit hitL = m_scene.RayCast(originL, PE_Vec2::down, 0.6f, CATEGORY_TERRAIN, true);
    RayHit hitR = m_scene.RayCast(originR, PE_Vec2::down, 0.6f, CATEGORY_TERRAIN, true);
    
    if (hitL.collider != NULL)
    {
        // Le rayon gauche à touché le sol
        m_onGround = true;
        gndNormal = hitL.normal;
    }
    if (hitR.collider != NULL)
    {
        // Le rayon droit à touché le sol
        m_onGround = true;
        gndNormal = hitR.normal;
    }
    
    //--------------------------------------------------------------------------
    // Slop detection

    bool m_onSlope = false;

    PE_Vec2 origin = position + PE_Vec2(0.0f, +0.4f);

    // Les rayons ne touchent que des colliders solides (non trigger)
    // ayant la catégorie CATEGORY_SLOPE
    RayHit hit = m_scene.RayCast(origin, PE_Vec2::down, 0.6f, CATEGORY_SLOPE, true);
    if (hit.fraction > 0) {
        m_onSlope = true;
        m_onGround = true;
    }

    //--------------------------------------------------------------------------
    // Jump check
    
    // Les rayons ne touchent que des colliders solides (non trigger)
    // ayant la catégorie FILTER_TERRAIN
    RayHit hitJump = m_scene.RayCast(origin, PE_Vec2::down, 0.7f, CATEGORY_TERRAIN, true);
    RayHit hitJumpSl = m_scene.RayCast(origin, PE_Vec2::down, 0.7f, CATEGORY_SLOPE, true);
    
    if (hitJump.collider != NULL || hitJumpSl.collider != NULL)
    {
        m_canJump = true;
    }
    else
    {
        m_canJump = false;
    }
    
    
    //--------------------------------------------------------------------------
    // Etat du joueur

    // Détermine l'état du joueur et change l'animation si nécessaire

    // TODO : Ajouter la gestion des animations Idle et Falling

    if (m_onGround || m_onSlope)
    {
        if (m_state != State::IDLE && m_hDirection == 0.0f)
        {
            m_state = State::IDLE;
            m_animator.PlayAnimation("Idle");
        }
        else if (m_state != State::RUNNING && abs(m_hDirection) == 1.0f)
        {
            if (m_facingRight)
            {
                m_state = State::RUNNING;
                m_animator.PlayAnimation("RunningForward");   
            }
        }
    }
    else
    {
        if (m_state != State::FALLING)
        {
            m_state = State::FALLING;
            m_animator.PlayAnimation("Falling");
        }
    }

    // Orientation du joueur
    // Utilisez m_hDirection qui vaut :
    // *  0.0f si le joueur n'accélère pas ;
    // * +1.0f si le joueur accélère vers la droite ;
    // * -1.0f si le joueur accélère vers la gauche.
    m_facingRight = true;

    //--------------------------------------------------------------------------
    // Modification de la vitesse et application des forces

    // Application des forces
    // Définit la force d'accélération horizontale du joueur
    PE_Vec2 direction = PE_Vec2::right;

    // TODO : Donner une valeur cohérente au vecteur force
    PE_Vec2 force = (20.0f * m_hDirection) * direction;
    body->ApplyForce(force);

    float maxSpeedCoef = (velocity.y - force.y) / (velocity.x - force.x); // Directing coefficient
    
    // TODO : Limiter la vitesse horizontale
    float maxHSpeed = 9.0f;

    if (m_onSlope) {
        if (m_hDirection == 1.0f)
        {
            maxHSpeed = maxSpeedCoef < 0 ? 2.0f : 30.0f;
        }
        else if (m_hDirection == -1.0f)
        {
            maxHSpeed = maxSpeedCoef > 0 ? 2.0f : 30.0f;
        }
    }
    
    velocity.x = PE_Clamp(velocity.x, -maxHSpeed, maxHSpeed);

    if (milliseconds - m_jumpShotTimeMemory > 200)
    {
        m_jumpShot = false;
    }

    // TODO : Ajouter un jump avec une vitesse au choix*
    if ((m_jump && m_canJump) || m_jumpShot) {
        m_jump = false;
        m_jumped = true;
        velocity.y = 20.0f;
    } else if (!m_canJump){
        m_jump = false;
    }

    if (m_jump && !m_canJump)
    {
        m_jumpShot = true;
        m_jumpShotTimeMemory = milliseconds;
    }

    if (m_longJump)
    {
        body->SetGravityScale(0.65f);
        velocity.y += 0.15f;
    }
    else {
        body->SetGravityScale(1);
    }

    // TODO : Rebond sur les ennemis
    if (m_bounce)
    {
        m_bounce = false;
        velocity.y = 15.0f;
    }

    if (m_onSlope && m_hDirection != 0.0f)
    {
        m_fallTimeMemory = time(NULL);
    }
    
    if (m_onSlope == true && m_hDirection == 0 && time(NULL) - m_fallTimeMemory > 1) {

        double timeCoef = abs(time(NULL) - (m_fallTimeMemory));

        if (timeCoef > 20 || timeCoef < 20) {
            timeCoef = 1;
        }
        
        if (maxSpeedCoef > 0) {
            velocity.x -= 0.3f * timeCoef;
        }
        else {
            velocity.x += 0.3f * timeCoef;
        }
    }

    // Remarques :
    // Le facteur de gravité peut être modifié avec l'instruction
    // -> body->SetGravityScale(0.5f);
    // pour faire des sauts de hauteurs différentes.
    // La physique peut être différente si le joueur touche ou non le sol.

    // Définit la nouvelle vitesse du corps
    // TODO : Appliquer la nouvelle velocity au player
    body->SetVelocity(velocity);
}

void Player::OnRespawn()
{
    PE_Body *body = GetBody();
    AssertNew(body);

    body->SetPosition(GetStartPosition() + PE_Vec2(0.5f, 0.0f));
    body->SetVelocity(PE_Vec2::zero);

    m_heartCount = 2;
    m_state = State::IDLE;
    m_hDirection = 0.0f;
    m_facingRight = true;
    m_bounce = false;
    m_jump = false;

    m_animator.StopAnimations();
    m_animator.PlayAnimation("Idle");

    SetToRespawn(true);
}

void Player::DrawGizmos()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Graphics graphics(renderer, *m_scene.GetActiveCamera());
    PE_Vec2 position = GetPosition();
    PE_Vec2 velocity = GetVelocity();

    // Dessine en blanc le vecteur vitesse du joueur
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    graphics.DrawVector(0.5f * velocity, position);

    // Dessine en jaune les rayons pour la détection du sol
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    PE_Vec2 originL = position + PE_Vec2(-0.35f, 0.0f);
    PE_Vec2 originR = position + PE_Vec2(+0.35f, 0.0f);
    graphics.DrawVector(0.1f * PE_Vec2::down, originL);
    graphics.DrawVector(0.1f * PE_Vec2::down, originR);
}

void Player::OnCollisionEnter(GameCollision &collision)
{
    const PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;
    
    // Collision avec un ennemi
    if (otherCollider->CheckCategory(CATEGORY_ENEMY))
    {
        Enemy *enemy = dynamic_cast<Enemy *>(collision.gameBody);
        if (enemy == nullptr)
        {
            assert(false);
            return;
        }

        // Calcule l'angle entre la normale de contact et le vecteur "UP"
        // L'angle vaut :
        // * 0 si le joueur est parfaitement au dessus de l'ennemi,
        // * 90 s'il est à gauche ou à droite
        // * 180 s'il est en dessous
        float angle = PE_AngleDeg(manifold.normal, PE_Vec2::up);
        if (angle < PLAYER_DAMAGE_ANGLE)
        {
            enemy->Damage(this);
            collision.ResolveUp();
        }
        else
        {
            collision.SetEnabled(false);
        }

        return;
    }

    // Collision avec un objet
    if (otherCollider->CheckCategory(CATEGORY_COLLECTABLE))
    {
        Collectable *collectable = dynamic_cast<Collectable *>(collision.gameBody);
        if (collectable == nullptr) return;

        // Collecte l'objet
        // C'est ensuite l'objet qui affecte un bonus au joueur,
        // en appellant AddFirefly() par exemple.
        collectable->Collect(this);
        return;
    }
}

void Player::OnCollisionStay(GameCollision &collision)
{
    const PE_Manifold &manifold = collision.manifold;
    PE_Collider *otherCollider = collision.otherCollider;

    if (otherCollider->CheckCategory(CATEGORY_COLLECTABLE))
    {
        // Désactive la collision avec un objet
        // Evite d'arrêter le joueur quand il prend un coeur
        collision.SetEnabled(false);
        return;
    }
    else if (otherCollider->CheckCategory(CATEGORY_TERRAIN) || otherCollider->CheckCategory(CATEGORY_SLOPE))
    {
        float angleUp = PE_AngleDeg(manifold.normal, PE_Vec2::up);
        if (angleUp <= 55.0f)
        {
            // Résoud la collision en déplaçant le joueur vers le haut
            // Evite de "glisser" sur les pentes si le joueur ne bouge pas
            collision.ResolveUp();
        }
    }
}

void Player::AddFirefly(int count)
{
    m_fireflyCount += count;
}

void Player::AddHeart()
{
    if (m_heartCount < 3)
        m_heartCount++;
}

void Player::Damage()
{
    // Méthode appelée par un ennemi qui touche le joueur

    // Désactive les damages en créatif
    if (((LevelScene*)&m_scene)->IsCreative())
        return;

    time_t now = time(NULL);
    if (now - m_livesTimeMemory > 2 && m_heartCount >= 1)
    {
        m_immunityEnabled = true;
        m_livesTimeMemory = now;
        m_state = State::DYING;
        m_heartCount--;
    }
    if (m_heartCount < 1)
    {
        m_heartCount = 0;
        m_state = State::DEAD;
    }
}

void Player::Kill()
{
    m_heartCount = 3;
    m_scene.Respawn();
}

class WakeUpCallback : public PE_QueryCallback
{
public:
    WakeUpCallback() {};
    virtual bool ReportCollider(PE_Collider *collider)
    {
        collider->GetBody()->SetAwake(true);
        return true;
    }
};

void Player::WakeUpSurroundings()
{
    PE_World &world = m_scene.GetWorld();
    PE_Vec2 position = GetBody()->GetPosition();
    PE_AABB aabb(
        position.x - 20.0f, position.y - 10.0f,
        position.x + 20.0f, position.y + 10.0f
    );
    WakeUpCallback callback;
    world.QueryAABB(callback, aabb);
}