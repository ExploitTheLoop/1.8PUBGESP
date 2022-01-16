//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_UTILITIES_H
#define RCHEATS_UTILITIES_H

#include <string>

using namespace std;

bool isContain(string str, string check) {
    size_t found = str.find(check);
    return (found != string::npos);
}

void getCharacterName(uintptr_t address, UTF8 * transcoding)
{
    int classname;
    int m = 0;
    UTF8 buf88[256] = "";
    UTF16 buf16[34] = {0};
    int hex[2] = {0};
    for (int i = 0; i < 4; i++)
    {
        classname = Read<int>(address + i * 4);
        hex[0] = (classname & 0xfffff000) >> 16;
        hex[1] = classname & 0xffff;
        buf16[m] = hex[1];
        buf16[m + 1] = hex[0];
        m += 2;
    }
    Utf16_To_Utf8(buf16, buf88, sizeof(buf88), strictConversion);
    sprintf(transcoding, "%s", buf88);
}

string getResName(int id){
    char UE4ResName[32];
    string str;
    int page = id / 16384;
    int index = id % 16384;
    uintptr_t nameaddr = getPtr(getPtr(getPtr(getRealOffset(Offsets::GNames)) + page * 4) + index * 4) + 8;
    char var[32] = {0};
    vm_readv(reinterpret_cast<void*>(nameaddr), reinterpret_cast<void*>(&var), sizeof(var));
    memcpy(UE4ResName, var, sizeof(var));
    str = UE4ResName;
    str.shrink_to_fit();
    return str;
}

int GetGrenadeType(string classname)
{
    if (classname.find("BP_Grenade_Smoke_C") != std::string::npos)
        return 1;
    if (classname.find("BP_Grenade_Burn_C") != std::string::npos)
        return 2;
    if (classname.find("BP_Grenade_Stun_C") != std::string::npos)
        return 3;
    if (classname.find("BP_Grenade_Shoulei_C") != std::string::npos)
        return 4;
    return 0;
}

#endif //RCHEATS_UTILITIES_H
