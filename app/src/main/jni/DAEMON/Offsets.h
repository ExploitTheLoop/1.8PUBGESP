//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_OFFSETS_H
#define RCHEATS_OFFSETS_H

namespace Offsets {
    uintptr_t GNames;
    uintptr_t GWorld;
    uintptr_t ViewWorld;
  //功能
    uintptr_t bIsWeaponFiring = 0xD44;
    uintptr_t ShootWeaponEntity = 0xC6C;
    uintptr_t ShootInterval = 0x3E8;
    uintptr_t BulletFireSpeed = 0x3C4;
    uintptr_t AccessoriesVRecoilFactor = 0x72C;
    uintptr_t AccessoriesHRecoilFactor = 0x730;
    uintptr_t AccessoriesRecoveryFactor = 0x734;
    uintptr_t ParachuteComponent = 0xD38;
    uintptr_t CurrentFallSpeed = 0x1d0;
   //绘制
    uintptr_t PersistentLevel = 0x20;
    uintptr_t ULevelToAActors = 0x70;
    uintptr_t ULevelToAActorsCount = 0x74;
    uintptr_t RootComponent = 0x144;
    uintptr_t Position = 0x150;
    uintptr_t SkeletalMeshComponent = 0x318;
    uintptr_t BoneArray = 0x5E4;
    uintptr_t Health = 0x93C;
    uintptr_t HealthMax = 0x940;
    uintptr_t PlayerDeath = 0x1024;
    uintptr_t PlayerName = 0x648;
    uintptr_t TeamID = 0x670;
    uintptr_t IsBot = 0x6E8;
    uintptr_t WeaponEntityComp = 0x4f4;
    uintptr_t WeaponId = 0xcc;
    uintptr_t CurBulletNumInClip = 0x90c;
    //国际服
    void globalOffsets(){
      //  GNames = 0x6EADCE0;
        GWorld = 0x785EDFC;
        ViewWorld = 0x77E5710;
    }
    void vietnamOffsets(){
        GNames = 0x6EADC60;
        GWorld = 0x6CB087C;
        ViewWorld = 0x7111E80;
    }
    void koreaOffsets(){
        GNames = 0x6EADC60;
        GWorld = 0x6CB087C;
        ViewWorld = 0x7111E80;
    }
    void taiwanOffsets(){
        GNames = 0x6EADC60;
        GWorld = 0x6CB087C;
        ViewWorld = 0x7111E80;
    }

}

#endif //RCHEATS_OFFSETS_H
