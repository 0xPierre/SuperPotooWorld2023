#pragma once

class GameBody;

class PE_BodyUserData
{
public:
    PE_BodyUserData() : gameBody(nullptr) {}
    GameBody *gameBody;

    void SetDefault() { gameBody = nullptr; }
};

class PE_ColliderUserData
{
public:
    PE_ColliderUserData() : id(0) {}
    int id;

    void SetDefault() { id = 0; }
};