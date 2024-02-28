#include <GoonWorld/core/Game.hpp>
#include <GoonWorld/models/AppSettings.hpp>
#include <GoonWorld/gameobjects/Player.hpp>
#include <GoonWorld/components/DebugDrawComponent.hpp>
#include <GoonWorld/components/PlayerInputComponent.hpp>
#include <GoonWorld/components/RigidbodyComponent.hpp>
#include <GoonWorld/components/AnimationComponent.hpp>
#include <GoonWorld/animation/AnimationTransition.hpp>
#include <SDL2/SDL_rect.h>

using namespace GoonWorld;

Player::Player(TiledMap::TiledObject &object)
{
    _location = Point{object.X, object.Y};
    _debugDrawComponent = new DebugDrawComponent(Point{object.Width, object.Height});
    _playerInputComponent = new PlayerInputComponent(0);
    auto bodyRect = SDL_Rect{object.X, object.Y, object.Width, object.Height};
    _rigidbodyComponent = new RigidbodyComponent(&bodyRect);
    _animationComponent = new AnimationComponent("mario");
    _animationComponent->SizeMultiplier = 2;
    AddComponent({_debugDrawComponent, _playerInputComponent, _rigidbodyComponent, _animationComponent});
    CreateAnimationTransitions();

    // Initialze Player config
    auto &playerConfig = Game::Instance()->GameSettings->PlayerConfigs;
    _jumpFrameVelocity = playerConfig.FrameJumpAcceleration;
    _initialJumpVelocity = playerConfig.InitialJumpVelocity;
    _runSpeed = playerConfig.RunSpeed;
    _moveSpeed = playerConfig.MoveSpeed;
    _maxJumpTime = playerConfig.MaxJumpTime;
    _initialMoveVelocity = playerConfig.InitialMoveVelocity;
    //
}
void Player::Update()
{

    // Handle input.

    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_LEFT))
    {
        if (_rigidbodyComponent->Velocity().x == 0)
            _rigidbodyComponent->Acceleration().x -= _initialMoveVelocity;
        _rigidbodyComponent->Acceleration().x -= _moveSpeed * DeltaTime.GetTotalSeconds();
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::DPAD_RIGHT))
    {
        if (_rigidbodyComponent->Velocity().x == 0)
            _rigidbodyComponent->Acceleration().x += _initialMoveVelocity;
        _rigidbodyComponent->Acceleration().x += _moveSpeed * DeltaTime.GetTotalSeconds();
    }
    if (_playerInputComponent->IsButtonDownOrHeld(GameControllerButton::A))
    {
        Jump();
    }
    else if (_playerInputComponent->IsButtonReleased(GameControllerButton::A))
    {
        _isJumping = false;
    }

    _canJump = _rigidbodyComponent->IsOnGround();
    _animationComponent->Mirror = _rigidbodyComponent->IsOnGround() ? _rigidbodyComponent->Velocity().x >= 0 ? false : true : _animationComponent->Mirror;

    _shouldFallAnim = _isJumping || !_rigidbodyComponent->IsOnGround();
    _shouldIdleAnim = _rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x == 0;
    _shouldRunAnim = _rigidbodyComponent->IsOnGround() && _rigidbodyComponent->Velocity().x != 0;
    if (_shouldRunAnim)
    {
    }
    printf("Jumping: %d\n", _isJumping);

    GameObject::Update();
}
void Player::CreateAnimationTransitions()
{
    auto fallTransition = new AnimationTransition();
    fallTransition->Condition = &_shouldFallAnim;
    fallTransition->ConditionMatch = true;
    fallTransition->NextAnimation = "jump";
    fallTransition->CurrentAnimation = "idle";
    _animationComponent->AddTransition(fallTransition);

    auto fallToIdleTransition = new AnimationTransition();
    fallToIdleTransition->Condition = &_shouldIdleAnim;
    fallToIdleTransition->ConditionMatch = true;
    fallToIdleTransition->NextAnimation = "idle";
    fallToIdleTransition->CurrentAnimation = "jump";
    _animationComponent->AddTransition(fallToIdleTransition);

    auto idleToWalkTransition = new AnimationTransition();
    idleToWalkTransition->Condition = &_shouldRunAnim;
    idleToWalkTransition->ConditionMatch = true;
    idleToWalkTransition->NextAnimation = "walk";
    idleToWalkTransition->CurrentAnimation = "idle";
    _animationComponent->AddTransition(idleToWalkTransition);

    auto walkToIdleTransition = new AnimationTransition();
    walkToIdleTransition->Condition = &_shouldIdleAnim;
    walkToIdleTransition->ConditionMatch = true;
    walkToIdleTransition->NextAnimation = "idle";
    walkToIdleTransition->CurrentAnimation = "walk";
    _animationComponent->AddTransition(walkToIdleTransition);
}
void Player::Jump()
{
    if (_isJumping)
    {
        if (_currentJumpTime < _maxJumpTime)
        {
            _rigidbodyComponent->Acceleration().y += (_jumpFrameVelocity * DeltaTime.GetTotalSeconds());
            _currentJumpTime += (float)DeltaTime.GetTotalSeconds();
        }
        else
        {
            _isJumping = _canJump = false;
        }
    }
    else if (_canJump)
    {
        // Play jump sound
        _currentJumpTime = 0;
        _isJumping = true;
        _canJump = false;
        _rigidbodyComponent->Velocity().y += _initialJumpVelocity;
    }
}
Player::~Player()
{
}