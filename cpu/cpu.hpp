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
public:
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

        enum class OpOffset : u_int32_t {

            kRS1        = 15,
            kRS2        = 20,
            kRD         = 7
        };

        enum class OpTypeMask : Reg_t {

            kRTYPE      = (0b1111111 << 25) + (0b111 << 12) + 0b111111,
            kISBTYPE    = (1<<15) - (1<<12) + (1<<7) - 1u,
            kUJTYPE     = (1<<7) - 1u,
            kRS1        = (1<<20) - (1<<15),
            kRS2        = (1<<25) - (1<<20),
            kRD         = (1<<12) - (1<<7),

        };

        /// Some constants that have proved themselves useful
        unsigned kRDOff = 7;
        unsigned kRS1Off = 15;
        unsigned kRS2Off = 20;

        /// Some variables to be used eventually
        OpCode  opc; ///< Operation code
        Byte_t   src1; ///< First source
        Byte_t   src2; ///< Second source
        Byte_t   dst; ///< Destination
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
                    dst = (insnBytes_ & ((Reg_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Reg_t) OpTypeMask::kRS1)) >> kRS1Off;
                    src2 = (insnBytes_ & ((Reg_t) OpTypeMask::kRS2)) >> kRS2Off;
                break;

                case OpMask::kSUB:

                    opc = OpCode::kSUB;
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
    static const u_int32_t  kMemSize        = 1<<20; ///<constant for memory size
    static const u_int32_t  kRegCnt         = 32; ///< constant for amount of registers

    /// Some variables to be used fairly frequently
    Reg_t                   pcInit; ///< Basically an initial pc value
    Reg_t                   pc; ///< Stores current instrucion address in mem

    /// Some containers (aka classes) to be used somewhat regularly
    std::vector<Reg_t>      reg; ///< register array
    Memory_t<Reg_t>        mem; ///< memory class

    /**
     * @brief get value of a register
     *
     * @param regNum register number
     * @return Reg_t value in register
     */
    Reg_t getReg (Byte_t regNum) {

        return reg[regNum];
    }

    /**
     * @brief Set the Reg object
     *
     * @param regNum register number
     * @param val value to set
     */
    void setReg (Byte_t regNum, Reg_t val) {

        reg[regNum] = val;
    }

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
    Insn_t decode (Reg_t insn) {

        return Insn_t (insn, false);
    }

    /**
     * @brief Basically executes an instruction
     *
     * @param insn instruction to be executed
     * @remark later i intend to add a storage unit for the instruction (it may add some convenience and speed up the process a bit)
     */
    int exec (Insn_t insn) {
        switch (insn.opc){

            case Insn_t::OpCode::kADD:

                setReg (insn.dst, getReg (insn.src1) + getReg (insn.src2));
            break;

            case Insn_t::OpCode::kSUB:

                setReg (insn.dst, getReg (insn.src1) - getReg (insn.src2));
            break;

            case Insn_t::OpCode::kEBREAK:

                return 1;
            break;

            default:

                exit (0); ///< i will add a system of exceptions/error codes later
            break;
        }

        return 0;
    }

public:

    /**
     * @brief Construct a new Cpu_t object
     *
     */
    Cpu_t (Reg_t pcInit_) : reg (kRegCnt, 0), mem (kMemSize), pcInit (pcInit_) {}

    /**
     * @brief Initializes memory and registers with some values
     *
     * @param initialMem initial memory values (will be written from pcInit for now)
     * @param initialReg initial register state (will be written from first reg correspondin to element zero of provided vector)
     */
    void Init (std::vector<Reg_t>& initialMem, std::vector<Reg_t>& initialReg) {

        for (int i = 0; i < initialMem.size (); i++) {

            mem.set (pcInit + i, initialMem[i]);
        }

        for (int i = 0; i < initialReg.size (); i++) {

            setReg (i + 1, initialReg[i]);
        }
    }

    void dump (const char* dumpFileName = "cpu_dump.log") {

        static int callCnt = 0;
        callCnt++;
        if (callCnt == 1) system ("rm cpu_dump.log");

        std::ofstream dumpFile (dumpFileName, std::ios::app);

        dumpFile << "PC : " << pc << "\n";
        dumpFile << "Registers : \n";

        for (int i = 0; i < 32; i++){

            dumpFile << "\treg[" << i << "] = " << getReg (i) << '\n';
        }

        dumpFile << "Mem (first 50 starting with pcInit)\n";

        for (int i = pcInit; i < pcInit + 50; i++) {

            dumpFile << "\tmem[" << i << "] : " <<  std::bitset<32> (mem.get (i)) << "\n";
        }

        dumpFile << "---------------------------------------------------\n";
    }

    /**
     * @brief Just runs everything starting from pcInit
     *
     */
    void run_stuff () {


        for (;pc < 2;pc++) {

            dump ();
            Reg_t rawInsn = fetch ();
            Insn_t insn = decode (rawInsn);
            if (exec (insn) == 1) break;
        }
        dump ();
    }

};
