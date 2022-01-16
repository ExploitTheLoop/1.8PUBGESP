//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_MEMORY_H
#define RCHEATS_MEMORY_H

#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "Process.h"

static uintptr_t libbase = 0;

#define halfShift	10
#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD

typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;

static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, static_cast<UTF8>(0xC0), static_cast<UTF8>(0xE0), static_cast<UTF8>(0xF0), static_cast<UTF8>(0xF8), static_cast<UTF8>(0xFC) };

typedef enum
{
    strictConversion = 0,
    lenientConversion
} ConversionFlags;

typedef enum
{
    conversionOK,				/* conversion successful */
    sourceExhausted,			/* partial character in source, but hit end */
    targetExhausted,			/* insuff. room in target for conversion */
    sourceIllegal,				/* source sequence is illegal/malformed */
    conversionFailed
} ConversionResult;

int Utf16_To_Utf8(const UTF16 * sourceStart, UTF8 * targetStart, size_t outLen,
                  ConversionFlags flags)
{
    int result = 0;
    const UTF16 *source = sourceStart;
    UTF8 *target = targetStart;
    UTF8 *targetEnd = targetStart + outLen;

    if ((NULL == source) || (NULL == targetStart))
    {
        printf("ERR, Utf16_To_Utf8: source=%p, targetStart=%p\n", source, targetStart);
        return conversionFailed;
    }

    while (*source)
    {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;
        const UTF16 *oldSource = source;	/* In case we have to back up
											   because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
        {
            /* If the 16 bits following the high surrogate are in the source
               buffer... */
            if (*source)
            {
                UTF32 ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                {
                    ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) +
                         halfBase;
                    ++source;
                }
                else if (flags == strictConversion)
                {				/* it's an unpaired high surrogate */
                    --source;	/* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            }
            else
            {					/* We don't have the 16 bits following the
								   high surrogate. */
                --source;		/* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        }
        else if (flags == strictConversion)
        {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
            {
                --source;		/* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /* Figure out how many bytes the result will require */
        if (ch < (UTF32) 0x80)
        {
            bytesToWrite = 1;
        }
        else if (ch < (UTF32) 0x800)
        {
            bytesToWrite = 2;
        }
        else if (ch < (UTF32) 0x10000)
        {
            bytesToWrite = 3;
        }
        else if (ch < (UTF32) 0x110000)
        {
            bytesToWrite = 4;
        }
        else
        {
            bytesToWrite = 3;
            ch = UNI_REPLACEMENT_CHAR;
        }

        target += bytesToWrite;
        if (target > targetEnd)
        {
            source = oldSource;	/* Back up source pointer! */
            target -= bytesToWrite;
            result = targetExhausted;
            break;
        }
        switch (bytesToWrite)
        {						/* note: everything falls through. */
            case 4:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 3:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 2:
                *--target = (UTF8) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 1:
                *--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    return result;
}


int find_pid(const char* process_name) {
    int id;
    pid_t pid = -1;
    DIR* dir;
    FILE* fp;
    char filename[32];
    char cmdline[256];

    struct dirent* entry;
    if (process_name == NULL) {
        return -1;
    }
    dir = opendir("/proc");
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(process_name, cmdline) == 0) {
                    pid = id;
                    break;
                }
            }
        }
    }

    closedir(dir);
    return pid;
}

uintptr_t get_module_base(const char *module_name) {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", target_pid);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t getRealOffset(uintptr_t offset) {
    if (libbase == 0) {
        LOGE("Error: Can't Find Base Addr for Real Offset");
        return 0;
    }
    return (libbase + offset);
}

template <typename T>
T Read(uintptr_t address) {
    T data;
    vm_readv(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&data), sizeof(T));
    return data;
}

char* ReadStr(uintptr_t address, int size) {
    char* data = new char[size];
    for(int i=0; i < size; i++){
        vm_readv(reinterpret_cast<void*>(address + (sizeof(char)*i)), reinterpret_cast<void*>(data + i), sizeof(char));
        if(data[i] == 0x0){
            break;
        }
    }
    return data;
}


uintptr_t getPtr(uintptr_t address) {
    return Read<uintptr_t>(address);
}

enum type {
    TYPE_DWORD,
    TYPE_FLOAT
};

void WriteDword(uintptr_t address, int value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void WriteFloat(uintptr_t address, float value){
    vm_writev(reinterpret_cast<void*>(address), reinterpret_cast<void*>(&value), 4);
}

void Write(uintptr_t address, const char *value, type TYPE){
    switch (TYPE) {
        case TYPE_DWORD:
            WriteDword(address, atoi(value));
            break;
        case TYPE_FLOAT:
            WriteFloat(address, atof(value));
            break;
    }
}


#endif //RCHEATS_MEMORY_H
