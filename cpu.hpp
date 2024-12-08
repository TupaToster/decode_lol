#pragma once
#include "lib.hpp"

/**
 * @brief A basic memory class (may be expanded further)
 *
 * @tparam T stored value type
 */
template<typename T>
class Memory_t {

private:

    Reg_t               memSize; ///< total memory size in sizeof (T)

    std::vector<T>      data; ///< vector to store memory lol

public:

    /**
     * @brief Construct a new Memory_t object
     */
    Memory_t (Reg_t memSize_) : memSize (memSize_), data (memSize_) {}

    /**
     * @brief Returns value placed at pos
     *
     * @param pos memory address
     * @return T value placed at this address
     */
    T get (Reg_t pos) {

        return data[pos];
    }

    /**
     * @brief Sets mem cell located at pos with value of val
     *
     * @param pos where to set
     * @param val value to set
     */
    void set (Reg_t pos, T val) {

        data[pos] = val;
    }
};

class Cpu_t {

private:

    /// Some constants that maybe will be used (maybe not)
    static const u_int32_t  kMemSize    = 1<<20; ///<constant for memory size
    static const u_int32_t  kRegCnt     = 32; ///< constant for amount of registers

    std::vector<Reg_t>      reg; ///< register array
    Memory_t<Byte_t>        mem; ///< memory class

public:

    /**
     * @brief Construct a new Cpu_t object
     *
     */
    Cpu_t () : reg (kRegCnt, 0), mem (kMemSize) {}


};
