//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_STRUCTS_H
#define RCHEATS_STRUCTS_H

struct Vector2
{
    float X;
    float Y;
};

struct Vector3
{
    float X;
    float Y;
    float Z;
};

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;
};

struct FTransform
{
    Vector4 Rotation;
    Vector3 Translation;
    float ddd;
    Vector3 Scale3D;
};

FTransform ReadFTransform(unsigned int address)
{
    return Read<FTransform>(address);
}

struct D3DMatrix {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

D3DMatrix ToMatrixWithScale(Vector3 translation, Vector3 scale, Vector4 rot) {
    D3DMatrix m;
    m._41 = translation.X;
    m._42 = translation.Y;
    m._43 = translation.Z;

    float x2 = rot.X + rot.X;
    float y2 = rot.Y + rot.Y;
    float z2 = rot.Z + rot.Z;

    float xx2 = rot.X * x2;
    float yy2 = rot.Y * y2;
    float zz2 = rot.Z * z2;
    m._11 = (1.0f - (yy2 + zz2)) * scale.X;
    m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
    m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

    float yz2 = rot.Y * z2;
    float wx2 = rot.W * x2;
    m._32 = (yz2 - wx2) * scale.Z;
    m._23 = (yz2 + wx2) * scale.Y;

    float xy2 = rot.X * y2;
    float wz2 = rot.W * z2;
    m._21 = (xy2 - wz2) * scale.Y;
    m._12 = (xy2 + wz2) * scale.X;

    float xz2 = rot.X * z2;
    float wy2 = rot.W * y2;
    m._31 = (xz2 + wy2) * scale.Z;
    m._13 = (xz2 - wy2) * scale.X;

    m._14 = 0.0f;
    m._24 = 0.0f;
    m._34 = 0.0f;
    m._44 = 1.0f;

    return m;
}
D3DMatrix MatrixMultiplication(D3DMatrix pM1, D3DMatrix pM2) {
    D3DMatrix pOut = {
            pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41,
            pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42,
            pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43,
            pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44,
            pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41,
            pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42,
            pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43,
            pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44,
            pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41,
            pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42,
            pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43,
            pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44,
            pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41,
            pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42,
            pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43,
            pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44
    };

    return pOut;
}

Vector3 GetBoneWorldPosition(uintptr_t actorAddv, uintptr_t boneAddv) {
    FTransform bone = ReadFTransform(boneAddv);
    FTransform actor = ReadFTransform(actorAddv);
    D3DMatrix boneMatrix = ToMatrixWithScale(bone.Translation, bone.Scale3D, bone.Rotation);
    D3DMatrix componentToWorldMatrix = ToMatrixWithScale(actor.Translation, actor.Scale3D, actor.Rotation);
    D3DMatrix newMatrix = MatrixMultiplication(boneMatrix, componentToWorldMatrix);
    Vector3 bonePos;
    bonePos.X = newMatrix._41;
    bonePos.Y = newMatrix._42;
    bonePos.Z = newMatrix._43;
    return bonePos;
}

bool WorldToScreen(Vector3 pos, Vector3& screen, int* distance, D3DMatrix viewMatrix) {
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z + viewMatrix._44);
    screen.Z = screenW;
    *distance = (screenW / 100);
    screenW = 1 / screenW;
    float sightX = (Width / 2);
    float sightY = (Height / 2);
    screen.X = sightX + (viewMatrix._11 * pos.X + viewMatrix._21 * pos.Y + viewMatrix._31 * pos.Z + viewMatrix._41) * screenW * sightX;
    screen.Y = sightY - (viewMatrix._12 * pos.X + viewMatrix._22 * pos.Y + viewMatrix._32 * pos.Z + viewMatrix._42) * screenW * sightY;
    return true;
}

bool WorldToScreenBone(D3DMatrix viewMatrix, Vector3 pos, Vector2& screen, int* distance) {
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z + viewMatrix._44);
    *distance = (screenW / 100);
    screenW = 1 / screenW;
    float sightX = (Width / 2);
    float sightY = (Height / 2);
    screen.X = sightX + (viewMatrix._11 * pos.X + viewMatrix._21 * pos.Y + viewMatrix._31 * pos.Z + viewMatrix._41) * screenW * sightX;
    screen.Y = sightY - (viewMatrix._12 * pos.X + viewMatrix._22 * pos.Y + viewMatrix._32 * pos.Z + viewMatrix._42) * screenW * sightY;
    return true;
}

bool WorldToScreenPlayer(Vector3 pos, Vector3& screen, int* distance, D3DMatrix viewMatrix) {
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z + viewMatrix._44);
    *distance = (screenW / 100);
    float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * (pos.Z + 85) + viewMatrix._42);
    float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z + viewMatrix._41);
    screen.Y = (Height / 2) - (Height / 2) * screenY / screenW;
    screen.X = (Width / 2) + (Width / 2) * screenX / screenW;
    float y1 = (Height / 2) - (viewMatrix._12 * pos.X + viewMatrix._22 * pos.Y + viewMatrix._32 * (pos.Z - 95) + viewMatrix._42) * (Height / 2) / screenW;
    screen.Z = y1 - screen.Y;
    return true;
}

#define maxplayerCount 100
#define maxvehicleCount 50
#define maxitemsCount 400
#define maxgrenadeCount 50

enum Mode {
    InitMode = 1,
};

struct OtherFeature {
    bool LessRecoil;
    bool InstantHit;
    bool FastParachute;
};

struct PlayerWeapon {
    int WeaponId;
    int CurBulletNumInClip;
};

struct PlayerBone {
    Vector2 neck;
    Vector2 chest;
    Vector2 pelvis;
    Vector2 lSholder;
    Vector2 rSholder;
    Vector2 lElbow;
    Vector2 rElbow;
    Vector2 lWrist;
    Vector2 rWrist;
    Vector2 lThigh;
    Vector2 rThigh;
    Vector2 lKnee;
    Vector2 rKnee;
    Vector2 lAnkle;
    Vector2 rAnkle;
};

struct PlayerData {
    char Name[34];
    int TeamID;
    float Health;
    float HealthMax;
    int Distance;
    bool IsRobot;
    Vector3 Location;
    Vector2 HeadLocation;
    PlayerWeapon Weapon1, Weapon2;
    PlayerBone Bone;
};

struct GrenadeData {
    int Type;
    int Distance;
    Vector3 Location;
};

struct VehicleData {
    char Name[50];
    int Distance;
    Vector3 Location;
};

struct ItemData {
    char Name[50];
    int Distance;
    Vector3 Location;
};

struct Request {
    int Mode;
    int VersionCode;
    int ScreenWidth;
    int ScreenHeight;
    OtherFeature otherFeature;
};

struct Response {
    bool Success;
    int PlayerCount;
    int VehicleCount;
    int ItemsCount;
    int GrenadeCount;
    PlayerData Players[maxplayerCount];
    GrenadeData Grenade[maxgrenadeCount];
    VehicleData Vehicles[maxvehicleCount];
    ItemData Items[maxitemsCount];
};

struct ShootWeaponBase {
    uintptr_t Base;
    uintptr_t ShootWeaponEntity;
    int bIsWeaponFiring;
    ShootWeaponBase (uintptr_t uMyObject){
        Base = getPtr(uMyObject + 0x15E4);
        ShootWeaponEntity = getPtr(Base + Offsets::ShootWeaponEntity);
        bIsWeaponFiring = Read<int>(uMyObject + Offsets::bIsWeaponFiring);
    }

    void setInstantHit(){
        Write(ShootWeaponEntity + Offsets::BulletFireSpeed, "900000", TYPE_FLOAT);
    }

    void setLessRecoil(){
        Write(ShootWeaponEntity + Offsets::AccessoriesVRecoilFactor,"0", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets::AccessoriesHRecoilFactor, "0", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets::AccessoriesRecoveryFactor, "0", TYPE_FLOAT);
    }

    void setFastShootInterval(){
        Write(ShootWeaponEntity + Offsets::ShootInterval, "0.048000", TYPE_FLOAT);
    }
    bool isFiring(){
        return (bIsWeaponFiring != 0);
    }
    bool isValid(){
        return (Base != 0);
    }
};

struct CharacterParachuteComponent{
    uintptr_t Base;
    uintptr_t CurrentFallSpeed;
    CharacterParachuteComponent(uintptr_t uMyObject){
        Base = getPtr(uMyObject + Offsets::ParachuteComponent);
        CurrentFallSpeed = Base + Offsets::CurrentFallSpeed;
    }

    void setFastLanding(){
        Write(CurrentFallSpeed, "35000", TYPE_FLOAT);
    }

    bool isValid(){
        return (Base != 0);
    }
};

#endif //RCHEATS_STRUCTS_H
