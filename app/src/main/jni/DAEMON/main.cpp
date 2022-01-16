#include "main.h"
#include "Structs.h"
#include "Utilities.h"
#include "socket.h"
#include "Algorithms.h"

void UpdateCoordinator(Response &response);

bool isLessRecoil = false;
bool isInstantHit = false;
bool isFastParachute = false;
bool isFastShootInterval = false;

int main(int argc, char*argv[]){
    if (!Create()){
        LOGE("[Server] 套接字无法创建");
        return -1;
    }
    if (!Connect()){
        LOGE("[Server] 套接字无法连接");
        return -1;
    }
    LOGI("[Server] 插座已连接");
    Request request{};
    receive((void*)&request);
    if (request.Mode == InitMode) {
        Width = request.ScreenWidth;
        Height = request.ScreenHeight;
        if (request.VersionCode == 1) {
            target_pid = find_pid("com.tencent.ig");
            //读取jizhi
            Offsets::globalOffsets();
        } else if (request.VersionCode == 2) {
            target_pid = find_pid("com.vng.pubgmobile");
            Offsets::vietnamOffsets();
        } else if (request.VersionCode == 3) {
            target_pid = find_pid("com.pubg.krmobile");
            Offsets::koreaOffsets();
        }
        if (Height > Width)
        {
            int t = Width;
            Width = Height;
            Height = t;
        }
        LOGI("Device screen:");
        LOGI("Width : %d", (int) Width);
        LOGI("Height : %d", (int) Height);
        if (target_pid < 0){
            LOGE("[Server] 无法获取游戏 pid");
            exit(1);
        }
        libbase = get_module_base("libUE4.so");
        if (libbase < 0){
            LOGE("[Server] 无法获得游戏基地");
            exit(1);
        }
    }
    while ((receive((void*)&request) > 0)){
        Response response{};
        response.Success = true;
        response.PlayerCount = 0;
        response.ItemsCount = 0;
        response.VehicleCount = 0;
        response.GrenadeCount = 0;
        isLessRecoil = request.otherFeature.LessRecoil; //午后
        isInstantHit = request.otherFeature.InstantHit;//自瞄
        isFastParachute = request.otherFeature.FastParachute;//快速降落伞
        UpdateCoordinator(response);
        send((void*)&response, sizeof(response));
    }
    Close();
    return 0;
}

int myTeamID = 101;

void UpdateCoordinator(Response &response) {
    uintptr_t uWorld = getPtr(getRealOffset(Offsets::GWorld));
    //数量
    uintptr_t uLevel = getPtr(getPtr(uWorld + 0x0) + 0x20);
    //自我数组
    uintptr_t uMyObject = getPtr(getPtr(getPtr(getPtr(uWorld + 36) + 96) + 0x20) + 6680);
    /*
    if(uMyObject != 0){
        //什么玩应
        ShootWeaponBase shootWeaponBase(uMyObject);
        if (shootWeaponBase.isValid()){
            if (shootWeaponBase.isFiring()){
                if (isLessRecoil){
                    shootWeaponBase.setLessRecoil();
                }
                if (isInstantHit){
                    shootWeaponBase.setInstantHit();
                }
                if (isFastShootInterval){
                    shootWeaponBase.setFastShootInterval();
                }
            }
        }
        //写的什么啊
        CharacterParachuteComponent characterParachuteComponent(uMyObject);
        if (characterParachuteComponent.isValid()){
            if (isFastParachute){
                characterParachuteComponent.setFastLanding();
            }
        }
    }
     */
    uintptr_t ULevelToAActors = getPtr(uLevel + Offsets::ULevelToAActors);
    int ULevelToAActorsCount = Read<int>(uLevel + Offsets::ULevelToAActorsCount);
    if (ULevelToAActorsCount < 0) {
        ULevelToAActorsCount = 0;
    } else if (ULevelToAActorsCount > 1024) {
        ULevelToAActorsCount = 1024;
    }
    //矩阵
    uintptr_t viewMatrixAddr = Read<int>(Read<int>(getRealOffset(Offsets::ViewWorld)) + 124) + 1296;
    D3DMatrix viewMatrix = Algorithms::ReadViewMatrix(viewMatrixAddr);
    for (int i = 0; i < ULevelToAActorsCount; ++i) {
        uintptr_t actor = getPtr(ULevelToAActors + i * 4);
        if (getPtr(actor + 0x4) != 8) continue;
        //actor  数组变量
    //    uintptr_t entityStruct = getPtr(actor + 16);
        //物资
     //   string resName = getResName(entityStruct);
       // if (isContain(resName, "BP_PlayerPawn")){
            uintptr_t PlayerWorld = getPtr(actor + Offsets::RootComponent);
            //血量
            float Health = Read<float>(actor + Offsets::Health);
            float HealthMax = Read<float>(actor + Offsets::HealthMax);
            //判断是否玩家死亡
            int PlayerDeath = Read<int>(actor + Offsets::PlayerDeath);
            if (PlayerDeath == 1) continue;
            //团队id
            int TeamID = Read<int>(actor + Offsets::TeamID);
            //去除自我坐标
            if (actor == uMyObject)
            {
                myTeamID = TeamID;
                continue;
            }
            if (TeamID == myTeamID) {
                continue;
            }
            //人机
            int IsBot = Read<int>(actor + Offsets::IsBot);
            bool isBot = (IsBot != 0);
            //啥几把啊武器id数据
            /*
            int WeaponID1 = Read<int>(getPtr(getPtr(getPtr(actor + 0x12C) + 0x0) + Offsets::WeaponEntityComp) + Offsets::WeaponId);
            int WeaponID2 = Read<int>(getPtr(getPtr(getPtr(actor + 0x12C) + 0x4) + Offsets::WeaponEntityComp) + Offsets::WeaponId);
            int CurAmmo1 = Read<int>(getPtr(getPtr(actor + 0x12C) + 0x0) + Offsets::CurBulletNumInClip);
            int CurAmmo2 = Read<int>(getPtr(getPtr(actor + 0x12C) + 0x4) + Offsets::CurBulletNumInClip);
             */
            //敌人数组
            Vector3 position = Read<Vector3>(PlayerWorld + Offsets::Position);

            Vector3 Location;
            int Distance;
           WorldToScreenPlayer(position,Location, &Distance, viewMatrix);
            //骨骼
            uintptr_t USkeletalMeshComponent = getPtr(actor + Offsets::SkeletalMeshComponent);
            uintptr_t CachedBoneSpaceTransforms = USkeletalMeshComponent + 0x140;
            uintptr_t boneAddv = getPtr(USkeletalMeshComponent + Offsets::BoneArray) + 0x30;
            //开始骨骼算法
            Vector3 headPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 4 * 48);
            headPos.Z += 20;
            int BoneDistance;
            Vector2 HeadLocation;
            WorldToScreenBone(viewMatrix, headPos, HeadLocation, &BoneDistance);
            Vector2 neck = HeadLocation;
            Vector3 chestPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 4 * 48);
            Vector2 chest;
            WorldToScreenBone(viewMatrix, chestPos,  chest, &BoneDistance);
            Vector3 pelvisPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 1 * 48);
            Vector2 pelvis;
            WorldToScreenBone(viewMatrix, pelvisPos,  pelvis,  &BoneDistance);
            Vector3 lSholderPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 11 * 48);
            Vector2 lSholder;
            WorldToScreenBone(viewMatrix, lSholderPos,  lSholder,  &BoneDistance);
            Vector3 rSholderPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 32 * 48);
            Vector2 rSholder;
            WorldToScreenBone(viewMatrix, rSholderPos,  rSholder,  &BoneDistance);
            Vector3 lElbowPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 12 * 48);
            Vector2 lElbow;
            WorldToScreenBone(viewMatrix, lElbowPos,  lElbow,  &BoneDistance);
            Vector3 rElbowPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 33 * 48);
            Vector2 rElbow;
            WorldToScreenBone(viewMatrix, rElbowPos,  rElbow,  &BoneDistance);
            Vector3 lWristPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 63 * 48);
            Vector2 lWrist;
            WorldToScreenBone(viewMatrix, lWristPos,  lWrist, &BoneDistance);
            Vector3 rWristPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 62 * 48);
            Vector2 rWrist;
            WorldToScreenBone(viewMatrix, rWristPos,  rWrist,  &BoneDistance);
            Vector3 lThighPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 52 * 48);
            Vector2 lThigh;
            WorldToScreenBone(viewMatrix, lThighPos,  lThigh, &BoneDistance);
            Vector3 rThighPos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 56 * 48);
            Vector2 rThigh;
            WorldToScreenBone(viewMatrix, rThighPos,  rThigh, &BoneDistance);
            Vector3 lKneePos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 53 * 48);
            Vector2 lKnee;
            WorldToScreenBone(viewMatrix, lKneePos,  lKnee, &BoneDistance);
            Vector3 rKneePos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 57 * 48);
            Vector2 rKnee;
            WorldToScreenBone(viewMatrix, rKneePos,  rKnee,  &BoneDistance);
            Vector3 lAnklePos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 54 * 48);
            Vector2 lAnkle;
            WorldToScreenBone(viewMatrix, lAnklePos,  lAnkle,  &BoneDistance);
            Vector3 rAnklePos = GetBoneWorldPosition(CachedBoneSpaceTransforms, boneAddv + 58 * 48);
            Vector2 rAnkle;
            WorldToScreenBone(viewMatrix, rAnklePos,  rAnkle,  &BoneDistance);
        //  PlayerWeapon playerWeapon1{WeaponID1, CurAmmo1};
        //   PlayerWeapon playerWeapon2{WeaponID2, CurAmmo2};
            PlayerBone playerBone{neck,chest,pelvis,lSholder,rSholder,lElbow,rElbow,lWrist,rWrist,lThigh,rThigh,lKnee,rKnee,lAnkle,rAnkle};
            PlayerData* data = &response.Players[response.PlayerCount];
            getCharacterName(Read<int>(actor + Offsets::PlayerName), data->Name);
            data->TeamID = TeamID;
            data->Health = Health;
            data->HealthMax = HealthMax;
            data->Distance = Distance;
            data->IsRobot = isBot;
            //data->Weapon1 = playerWeapon1;
           // data->Weapon2 = playerWeapon2;
            data->Location = Location;
            data->HeadLocation = HeadLocation;
            data->Bone = playerBone;
            ++response.PlayerCount;
            if(response.PlayerCount == maxplayerCount) continue;
         //   response.GrenadeCount++;
        }
      /*  int grenadeAlert = GetGrenadeType(resName);
        if (grenadeAlert != 0){
            Vector3 Location;
            int Distance;
            Vector3 position = Read<Vector3>(getPtr(actor + Offsets::RootComponent) + Offsets::Position);
            WorldToScreen(position, Location, &Distance, viewMatrix);
            GrenadeData* grenadeData = &response.Grenade[response.GrenadeCount];
            grenadeData->Type = grenadeAlert;
            grenadeData->Location = Location;
            grenadeData->Distance = Distance;
            response.GrenadeCount++;
            */
           // if (response.GrenadeCount == maxgrenadeCount) continue;
        }