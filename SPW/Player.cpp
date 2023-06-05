#include "Player.h"
#include "Camera.h"
#include "Scene.h"
#include "Collectable.h"
#include "Enemy.h"
#include "Graphics.h"

Player::Player(Scene &scene) :
    GameBody(scene, Layer::PLAYER), m_animator(),
    m_jump(false), m_facingRight(true), m_bounce(false), m_hDirection(0.0f),
    m_lifeCount(5), m_fireflyCount(0), m_heartCount(2), m_state(Player::State::IDLE)
{
    m_name = "Player";

    SetToRespawn(true);

    AssetManager &assetsManager = scene.GetAssetManager();
    RE_Atlas *atlas = assetsManager.GetAtlas(AtlasID::PLAYER);
    RE_AtlasPart *part = nullptr;

    // Animation "Idle"
    part = atlas->GetPart("Idle");
    AssertNew(part);
    RE_TexAnim *idleAnim = new RE_TexAnim(
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

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 0;
    m_debugColor.b = 0;
}

Player::~Player()
{
}

void Player::Start()
{
    // Joue l'animation par défaut
    m_animator.PlayAnimation("Idle");

    // Crée le corps
    PE_World &world = m_scene.GetWorld();
    PE_BodyDef bodyDef;
    bodyDef.type = PE_BodyType::DYNAMIC;
    bodyDef.position = GetStartPosition() + PE_Vec2(0.5f, 0.0f);
    bodyDef.name = (char *)"Player";
    bodyDef.damping.SetZero();
    PE_Body *body = world.CreateBody(bodyDef);
    SetBody(body);

    // Création du collider
    PE_ColliderDef colliderDef;

    // TODO : Donner une taille normale à la capsule
    PE_CapsuleShape capsule(PE_Vec2(0.0f, 0.35f), PE_Vec2(0.0f, 0.85f), 0.35f);
    colliderDef.friction = 1.0f;
    colliderDef.filter.categoryBits = CATEGORY_PLAYER;
    colliderDef.shape = &capsule;
    PE_Collider *collider = body->CreateCollider(colliderDef);
}

void Player::Update()
{
    ControlsInput &controls = m_scene.GetInputManager().GetControls();

    // Sauvegarde les contrôles du joueur pour modifier
    // sa physique au prochain FixedUpdate()
    
	// TODO : Mettre à jour l'état du joueur en fonction des contrôles de jump
    if (controls.jumpPressed)
        m_jump = true;

    m_hDirection = controls.hAxis;
    // printf("%d\n", m_hDirection);
}

void Player::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    // Met à jour les animations du joueur
    m_animator.Update(m_scene.GetTime());

    float scale = camera->GetWorldToViewScale();
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_FRect rect = { 0 };

    // TODO : Trouver les bonnes diemnsions de l'affichage en fonction du sprite (dimensions en tuiles)
    rect.h = 1.375f * scale; // Le sprite fait 1.375 tuile de haut
    rect.w = 1.000f * scale; // Le sprite fait 1 tuile de large
    camera->WorldToView(GetPosition(), rect.x, rect.y);

    // Dessine l'animateur du joueur
    // TODO : Trouver le bon anchor
    m_animator.RenderCopyExF(
        &rect, RE_Anchor::SOUTH , 0.0f, Vec2(0.5f, 0.5f), flip
    );
}

void Player::FixedUpdate()
{
    PE_Body *body = GetBody();
    PE_Vec2 position = body->GetPosition();
    // TODO : Récuperer la vitesse du joueur
    PE_Vec2 velocity = body->GetLocalVelocity();

    // Réveille les corps autour du joueur
    WakeUpSurroundings();
    
    // Tue le joueur s'il tombe dans un trou
    if (position.y < -2.0f)
    {
        m_scene.Respawn();
        return;
    }

    //--------------------------------------------------------------------------
    // Détection du sol

    bool m_onGround = false;
    PE_Vec2 gndNormal = PE_Vec2::up;

    // Lance deux rayons vers le bas ayant pour origines
    // les coins gauche et droit du bas du collider du joueur
    // Ces deux rayons sont dessinés en jaune dans DrawGizmos()
    PE_Vec2 originL = position + PE_Vec2(-0.35f, 0.0f);
    PE_Vec2 originR = position + PE_Vec2(+0.35f, 0.0f);

    // Les rayons ne touchent que des colliders solides (non trigger)
    // ayant la catégorie FILTER_TERRAIN
    RayHit hitL = m_scene.RayCast(originL, PE_Vec2::down, 0.1f, CATEGORY_TERRAIN, true);
    RayHit hitR = m_scene.RayCast(originR, PE_Vec2::down, 0.1f, CATEGORY_TERRAIN, true);

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
    // Etat du joueur

    // Détermine l'état du joueur et change l'animation si nécessaire

    // TODO : Ajouter la gestion des animations Idle et Falling
    // printf("POSITION %f\n", position.x);
    // printf("mem %f\n",  abs(position.x - m_positionMemory));

    if (m_onGround)
    {
        if (m_state != State::IDLE && m_hDirection == 0.0f)
        {
            m_state = State::IDLE;
            m_animator.PlayAnimation("Idle");
        }
        else if (m_state != State::RUNNING && m_hDirection == 1.0f)
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


    // TODO : Limiter la vitesse horizontale
    float maxHSpeed = 9.0f;
    velocity.x = PE_Clamp(velocity.x, -maxHSpeed, maxHSpeed);

    // TODO : Ajouter un jump avec une vitesse au choix*
    if (m_jump && m_onGround) {
        m_jump = false;
        velocity.y = 20.0f;
    }

    if (m_bounce)
    {
        m_bounce = false;
        velocity.y = 15.0f;
    }

    // TODO : Rebond sur les ennemis

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
    else if (otherCollider->CheckCategory(CATEGORY_TERRAIN))
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
    m_heartCount++;
}

void Player::Damage()
{
    // Méthode appelée par un ennemi qui touche le joueur
    Kill();
}

void Player::Kill()
{
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
