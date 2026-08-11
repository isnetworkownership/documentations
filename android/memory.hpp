#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <cinttypes>
#include <unistd.h>

namespace memory {
    inline uintptr_t getbase(const char* module_name) {
        std::ifstream maps("/proc/self/maps");
        std::string line;
        while (std::getline(maps, line)) {
            if (line.find(module_name) != std::string::npos) {
                uintptr_t base = 0;
                sscanf(line.c_str(), "%" SCNxPTR "-", &base);
                return base;
            }
        }
        return 0;
    }

    inline uintptr_t getlib() {
        static uintptr_t cached = 0;
        if (cached == 0) {
            cached = getbase("libroblox.so");
        }
        return cached;
    }

    inline uintptr_t getaddr(uint64_t rva) {
        return getlib() + rva;
    }

    template<typename t>
    inline t read(uintptr_t address) {
        return *reinterpret_cast<t*>(address);
    }

    template<typename t>
    inline void write(uintptr_t address, t value) {
        *reinterpret_cast<t*>(address) = value;
    }

    inline bool patch(uintptr_t address, const void* data, size_t size) {
        if (!address || !data || size == 0) return false;
        long pagesize = sysconf(_SC_PAGESIZE);
        if (pagesize <= 0) return false;
        uintptr_t start = address & ~(pagesize - 1);
        size_t totalsize = ((address - start) + size + pagesize - 1) & ~(pagesize - 1);
        if (mprotect(reinterpret_cast<void*>(start), totalsize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
            return false;
        memcpy(reinterpret_cast<void*>(address), data, size);
        mprotect(reinterpret_cast<void*>(start), totalsize, PROT_READ | PROT_EXEC);
        return true;
    }

    inline std::string readstring(uintptr_t address, size_t max = 512) {
        if (!address) return "";
        const char* str = reinterpret_cast<const char*>(address);
        size_t length = strnlen(str, max);
        return std::string(str, length);
    }
}
