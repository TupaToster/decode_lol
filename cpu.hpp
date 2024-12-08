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

/**
 * @brief Basically a cpu simulator
 *
 * @remark works on prayers to the great god Perun
 */
class Cpu_t {

private:

    /// Some classes that are used only inside of the cpu
    struct Insn_t {

        /// Some classes for local (and not only) use

        /**
         * @brief enumerator class for opcodes so that they don't clutter the constants
         *
         */
        enum class OpCode {

            kUNKNOWN = 0,
            kEBREAK,
            kADD,
            kADDI,
            kSUB,
            kSUBI,
            kJAL,
            kJALR,
            kLB,
            kLH,
            kLW,
            kLUI,
            kAUIPC
        };

        /**
         * @brief enum class for real values of opcode and funct fields of supported instructions
         * @remark yeah the naming is kind of inobvious but it gets the job done
         */
        enum class OpMask : Reg_t {

            kJAL        = 0b1101111,
            kJALR       = 0b1100111,
            kBNE        = 0b1100011 | (0b001 << 12),
            kLB         = 0b0000011,
            kLH         = 0b0000011 | (0b001 << 12),
            kLW         = 0b0000011 | (0b010 << 12),
            kSB         = 0b0100011,
            kSH         = 0b0100011 | (0b001 << 12),
            kSW         = 0b0100011 | (0b010 << 12),
            kADDI       = 0b0010011,
            kADD        = 0b0110011,
            kSUBI       = 0b0010011 | (0b010000 << 25),
            kSUB        = 0b0110011 | (0b010000 << 25),
            kEBREAK     = 0b1110011 | (0b1 << 20),
            kLUI        = 0b0110111,
            kAUIPC      = 0b0010111,
        };

        enum class OpTypeMask : Reg_t {

            kRTYPE      = (1<<32) - (1<<25) + (1<<15) - (1<<12) + (1 << 7) - 1,
            kISBTYPE    = (1<<15) - (1<<12) + (1<<7) - 1,
            kUJTYPE     = (1<<7) - 1,
            kRS1        = (1<<20) - (1<<15),
            kRS2        = (1<<25) - (1<<20),
            kRD         = (1<<12) - (1<<7),

        };
        /// Some variables to be used eventually
        OpCode  opc; ///< Operation code
        Reg_t   src1; ///< First source
        Reg_t   src2; ///< Second source
        Reg_t   dst; ///< Destination
        /// Some stuff may be added later

        /**
         * @brief Construct a new Insn_t object basically
         *
         */
        Insn_t () : opc (OpCode::kUNKNOWN), src1 (0), src2 (0), dst (0) {}

        /**
         * @brief Basically a decoder put here for sole convinience :)
         *
         * @param insnBytes_ byted instruction straight from memory
         * @param opcOnly_ a flag to fill only opcode and not fill src1, 2 and dst (true by default)
         */
        Insn_t (Reg_t insnBytes_, bool opcOnly_ = true) {


            switch ((OpMask) (((Reg_t) insnBytes_) & ((Reg_t) OpTypeMask::kRTYPE))) {

                case OpMask::kADD:

                    opc = OpCode::kADD;
                    dst = insnBytes_ & ((Reg_t) OpTypeMask::kRD);
                    src1 = insnBytes_ & ((Reg_t) OpTypeMask::kRS1);
                    src2 = insnBytes_ & ((Reg_t) OpTypeMask::kRS2);
                break;

                default:

                    opc = OpCode::kUNKNOWN;
                    dst = -1;
                    src1 = -1;
                    src2 = -1;
                break;
            }
        }

    };

    /// Some constants that maybe will be used (maybe not)
    static const u_int32_t  kMemSize    = 1<<20; ///<constant for memory size
    static const u_int32_t  kRegCnt     = 32; ///< constant for amount of registers
    static const u_int32_t  kMaskOpcode = static_cast<u_int32_t> (((1<<7) - 1) | ((1<<15) - (1<<12)) | ((1<<32) - (1<<25)));

    /// Some variables to be used fairly frequently
    Reg_t                   pcInit; ///< Basically an initial pc value
    Reg_t                   pc; ///< Stores current instrucion address in mem

    /// Some containers (aka classes) to be used somewhat regularly
    std::vector<Reg_t>      reg; ///< register array
    Memory_t<Byte_t>        mem; ///< memory class

    /**
     * @brief Fetches instruction pointed at by pc
     *
     * @return Reg_t instruction basically
     *
     * @remark Does not advance pc
     */
    Reg_t fetch () {

        return mem.get (pc);
    }

    /**
     * @brief So basically it just decodes an instruction and returns a class using a constructor for Insn_t
     *
     * @return Insn_t a decoded instruction
     *
     * @remark This is basically done for future expandability and stuff (also for an easier call)
     */
    Insn_t decode () {

        return Insn_t (mem.get (pc), false);
    }

public:

    /**
     * @brief Construct a new Cpu_t object
     *
     */
    Cpu_t (Reg_t pcInit_) : reg (kRegCnt, 0), mem (kMemSize), pcInit (pcInit_) {}

    /**
     * @brief Just runs everything starting from pcInit
     *
     */
    void run_stuff () {


    }

};
