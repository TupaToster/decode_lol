#pragma once
#include "lib.hpp"

/**
 * @brief A basic memory class (may be expanded further)
 *
 * @tparam T stored value type
 * @warning IT IS LITTLE ENDIAN
 */
class Memory_t {

private:

    Word_t               memSize; ///< total memory size in sizeof (T)
public:
    std::vector<Byte_t>      data; ///< vector to store memory lol

public:

    /**
     * @brief Construct a new Memory_t object
     */
    Memory_t (Word_t memSize_) : memSize (memSize_), data (memSize_) {}

    /**
     * @brief get byte
     *
     * @param pos
     * @return Byte_t
     */
    Byte_t getB (Word_t pos) {

        if (pos > memSize - 1) return 0;
        return static_cast<Byte_t> (*(Byte_t*) (data.data () + pos));
        // return data[pos];
    }

    /**
     * @brief get half word
     *
     * @param pos
     * @return HWord_t
     */
    HWord_t getHW (Word_t pos) {

        if (pos > memSize - 2) return 0; ///< Should be rewritten as an exception
        return static_cast<HWord_t> (*(HWord_t*) (data.data () + pos));
        // return data[pos] + (data[pos + 1] << 8);
    }
    /**
     * @brief get word
     *
     * @param pos
     * @return Word_t
     */
    Word_t getW (Word_t pos) {

        if (pos > memSize - 4) return 0;
        // std::cout << "mem[" << pos << "] : " << std::bitset<32> (static_cast<Word_t> (*(Word_t*)(data.data () + pos))) << "\n";
        return static_cast<Word_t> (*(Word_t*)(data.data () + pos));
        // return data[pos] + (data[pos + 1] << 8) + (data[pos + 2] << 16) + (data[pos + 3] << 24);
    }

    /**
     * @brief get double word
     *
     * @param pos
     * @return DWord_t
     */
    DWord_t getDW (Word_t pos) {

        if (pos > memSize - 8) return 0;
        return static_cast<DWord_t> (*(DWord_t*) (data.data () + pos));
        // return data[pos] + (data[pos + 1] << 8) + (data[pos + 2] << 16) + (data[pos + 3] << 24) + (data[pos + 4] << 32) + (data[pos + 5] << 40) + (data[pos + 6] << 48) + (data[pos + 7] << 56);
    }

    /**
     * @brief store byte
     *
     * @param pos
     * @param val
     */
    void setB (Word_t pos, Byte_t val) {

        if (pos > memSize - 1) return;
        (*(Byte_t*) (data.data () + pos)) = val;
        // data[pos] = val & 0xFF;
    }

    /**
     * @brief store half word
     *
     * @param pos
     * @param val
     */
    void setHW (Word_t pos, HWord_t val) {

        if (pos > memSize - 2) return;
        (*(HWord_t*) (data.data () + pos)) = val;
        // data[pos] = val & 0xFF;
        // data[pos + 1] = (val >> 8) & 0xFF;
    }

    /**
     * @brief store word
     *
     * @param pos
     * @param val
     */
    void setW (Word_t pos, Word_t val) {

        if (pos > memSize - 4) return;
        (*(Word_t*) (data.data () + pos)) = val;
        // data[pos] = val & 0xFF;
        // data[pos + 1] = (val >> 8) & 0xFF;
        // data[pos + 2] = (val >> 16) & 0xFF;
        // data[pos + 3] = (val >> 24) & 0xFF;
    }

    /**
     * @brief store double word
     *
     * @param pos
     * @param val
     */
    void setDW (Word_t pos, DWord_t val) {

        if (pos > memSize - 8) return;
        (*(DWord_t*) (data.data () + pos)) = val;
        // data[pos] = val & 0xFF;
        // data[pos + 1] = (val >> 8) & 0xFF;
        // data[pos + 2] = (val >> 16) & 0xFF;
        // data[pos + 3] = (val >> 24) & 0xFF;
        // data[pos + 4] = (val >> 32) & 0xFF;
        // data[pos + 5] = (val >> 40) & 0xFF;
        // data[pos + 6] = (val >> 48) & 0xFF;
        // data[pos + 7] = (val >> 54) & 0xFF;
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
            kAUIPC,
            kBNE,
            kBEQ,
            kSB,
            kSH,
            kSW,
        };

        /**
         * @brief enum class for real values of opcode and funct fields of supported instructions
         * @remark yeah the naming is kind of inobvious but it gets the job done
         */
        enum class OpMask : Word_t {

            kJAL        = 0b1101111,
            kJALR       = 0b1100111,
            kLB         = 0b0000011,
            kLH         = 0b0000011 | (0b001 << 12),
            kLW         = 0b0000011 | (0b010 << 12),
            kADDI       = 0b0010011,
            kADD        = 0b0110011,
            kSUBI       = 0b0010011 | (0b010000 << 25),
            kSUB        = 0b0110011 | (0b010000 << 25),
            kEBREAK     = 0b1110011 | (0b1 << 20),
            kLUI        = 0b0110111,
            kAUIPC      = 0b0010111,
            kBNE        = 0b1100011 | (0b001 << 12),
            kBEQ        = 0b1100011 | (0b000 << 12),
            kSB          = 0b0100011 | (0b000 << 12),
            kSH          = 0b0100011 | (0b001 << 12),
            kSW          = 0b0100011 | (0b010 << 12),
        };

        enum class OpOffset : u_int32_t {

            kRS1        = 15,
            kRS2        = 20,
            kRD         = 7
        };

        enum class OpTypeMask : Word_t {

            kRTYPE      = static_cast<Word_t>((0b1111111 << 25) + (0b111 << 12) + 0b111111),
            kISBTYPE    = static_cast<Word_t>((1<<15) - (1<<12) + (1<<7) - 1u),
            kUJTYPE     = static_cast<Word_t>((1<<7) - 1u),
            kRS1        = static_cast<Word_t>((1<<20) - (1<<15)),
            kRS2        = static_cast<Word_t>((1<<25) - (1<<20)),
            kRD         = static_cast<Word_t>((1<<12) - (1<<7)),
            kJUIMM      = static_cast<Word_t>(-(0b111111111111)),
            kIIMM       = static_cast<Word_t>((1 << 20) - (1 << 21)),
            kBIMM1      = static_cast<Word_t>((0b11111 << 7)),
            kBIMM2      = static_cast<Word_t>((0b1111111 << 25)),
        };

        /// Some constants that have proved themselves useful
        unsigned kRDOff = 7;
        unsigned kRS1Off = 15;
        unsigned kRS2Off = 20;
        unsigned kIIMMOff = 20;
        unsigned kBIMM1Off = 7;
        unsigned kBIMM2Off = 25;

        /// Some variables to be used eventually
        Word_t  rawInsn = 0;
        OpCode  opc = OpCode::kUNKNOWN; ///< Operation code
        Byte_t  src1 = 0; ///< First source
        Byte_t  src2 = 0; ///< Second source
        Byte_t  dst = 0; ///< Destination
        Word_t  imm = 0; ///< Immidiate value


        /// Some stuff may be added later

        /**
         * @brief Construct a new Insn_t object basically
         *
         */
        Insn_t () : opc (OpCode::kUNKNOWN), src1 (0), src2 (0), dst (0) {}

        /**
         * @brief Basically a decoder put here for sole convinience :)
         *
         * @param insnBytes_ raw instruction straight from memory
         * @param onlyOpc_ a flag to fill only opcode and not fill src1, 2 and dst (true by default)
         */
        Insn_t (Word_t insnBytes_) {

            rawInsn = insnBytes_;

            std::cout << "Current insn: " << std::bitset<32> (rawInsn) << '\n';

            if (insnBytes_ == ((Word_t) OpMask::kEBREAK)) {

                opc = OpCode::kEBREAK;
                dst = 0;
                src1 = 0;
                src2 = 0;
                return;
            }

            switch ((OpMask) (((Word_t) insnBytes_) & ((Word_t) OpTypeMask::kRTYPE))) {

                case OpMask::kADD:

                    opc = OpCode::kADD; // J type
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    src2 = (insnBytes_ & ((Word_t) OpTypeMask::kRS2)) >> kRS2Off;
                return;

                case OpMask::kSUB: // U type

                    opc = OpCode::kSUB;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    src2 = (insnBytes_ & ((Word_t) OpTypeMask::kRS2)) >> kRS2Off;
                return;
            }

            switch ((OpMask) (((Word_t) insnBytes_) & ((Word_t) OpTypeMask::kUJTYPE))) {

                case OpMask::kJAL:

                    opc = OpCode::kJAL;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kJUIMM));
                    imm = ((imm & (1 << 31)) >> 11) | ((imm & (0b1111111111 << 21)) >> 20) | ((imm & (1 << 20)) >> 9) | ((imm & (0b11111111 << 12)));
                    if ((insnBytes_ & (1<<31))) imm = imm | 0xFFF00000;
                return;

                case OpMask::kLUI: // U type

                    opc = OpCode::kLUI;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kJUIMM));
                return;

                case OpMask::kAUIPC: // U type

                    opc = OpCode::kAUIPC;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kJUIMM));
                return;
            }

            switch ((OpMask) (((Word_t) insnBytes_) & ((Word_t) OpTypeMask::kISBTYPE))) {

                case OpMask::kJALR: //I type

                    opc = OpCode::kJALR;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kLB: //I type

                    opc = OpCode::kLB;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kLH: //I type

                    opc = OpCode::kLH;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kLW: // I type

                    opc = OpCode::kLW;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kADDI: // I type

                    opc = OpCode::kADDI;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kSUBI: // I type

                    opc = OpCode::kSUBI;
                    dst = (insnBytes_ & ((Word_t) OpTypeMask::kRD)) >> kRDOff;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kIIMM)) >> kIIMMOff;
                    if (insnBytes_ & (1<<31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kBEQ:

                    opc = OpCode::kBEQ;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    src2 = (insnBytes_ & ((Word_t) OpTypeMask::kRS2)) >> kRS2Off;
                    imm = (insnBytes_ & ((Word_t) OpTypeMask::kBIMM1)) | (insnBytes_ & ((Word_t) OpTypeMask::kBIMM2));
                    imm = ((imm & (1 << 7)) << 4) | ((imm & (0b11111 << 8)) >> 7) | ((imm & (0b111111 << 25)) >> 20) | ((imm & (1 << 31)) >> 19);
                    if (insnBytes_ & (1 << 31)) imm |= 0xFFFFF000;
                return;

                case OpMask::kBNE:

                    opc = OpCode::kBNE;
                    src1 = (insnBytes_ & ((Word_t) OpTypeMask::kRS1)) >> kRS1Off;
                    src2 = (insnBytes_ & ((Word_t) OpTypeMask::kRS2)) >> kRS2Off;
                    imm = ((imm & (1 << 7)) << 4) | ((imm & (0b11111 << 8)) >> 7) | ((imm & (0b111111 << 25)) >> 20) | ((imm & (1 << 31)) >> 19);
                    if (insnBytes_ & (1 << 31)) imm |= 0xFFFFF000;
                return;

            }

            opc = OpCode::kUNKNOWN;
            return;
        }

    };

    /// Some constants that maybe will be used (maybe not)
    static const u_int32_t  kMemSize        = 1<<20; ///<constant for memory size
    static const u_int32_t  kRegCnt         = 32; ///< constant for amount of registers
    static const Word_t     pcIncr          = 4;

    /// Some variables to be used fairly frequently
    Word_t                   pcInit; ///< Basically an initial pc value
    Word_t                   pc; ///< Stores current instrucion address in mem

    /// Some containers (aka classes) to be used somewhat regularly
    std::vector<Word_t>      reg; ///< register array
    Memory_t                 mem; ///< memory class

    /**
     * @brief get value of a register
     *
     * @param regNum register number
     * @return Reg_t value in register
     */
    Word_t getReg (Byte_t regNum) {

        return reg[regNum];
    }

    /**
     * @brief Set the Reg object
     *
     * @param regNum register number
     * @param val value to set
     */
    void setReg (Byte_t regNum, Word_t val) {

        reg[regNum] = val;
    }

    /**
     * @brief Fetches instruction pointed at by pc
     *
     * @return Reg_t instruction basically
     *
     * @remark Does not advance pc
     */
    Word_t fetch () {

        return mem.getW (pc);
    }

    /**
     * @brief So basically it just decodes an instruction and returns a class using a constructor for Insn_t
     *
     * @return Insn_t a decoded instruction
     *
     * @remark This is basically done for future expandability and stuff (also for an easier call)
     */
    Insn_t decode (Word_t insn) {

        return Insn_t (insn);
    }

    /**
     * @brief Basically executes an instruction
     *
     * @param insn instruction to be executed
     * @remark later i intend to add a storage unit for the instruction (it may add some convenience and speed up the process a bit)
     */
    int exec (Insn_t insn) {

        Word_t temp = 0; 
        switch (insn.opc){

            case Insn_t::OpCode::kEBREAK:

                return 1;
            break;

            case Insn_t::OpCode::kADD:

                setReg (insn.dst, getReg (insn.src1) + getReg (insn.src2));
            break;

            case Insn_t::OpCode::kADDI:

                setReg (insn.dst, (insn.src1 + insn.imm));
            break;

            case Insn_t::OpCode::kSUB:

                setReg (insn.dst, getReg (insn.src1) - getReg (insn.src2));
            break;

            case Insn_t::OpCode::kSUBI:

                setReg (insn.dst, (insn.src1 - insn.imm));
            break;

            case Insn_t::OpCode::kJAL:

                setReg (insn.dst, pc + pcIncr);
                pc += insn.imm - pcIncr;
            break;

            case Insn_t::OpCode::kJALR:

                setReg (insn.dst, pc + pcIncr);
                pc += getReg (insn.src1) + insn.imm - pcIncr;
            break;

            case Insn_t::OpCode::kLB:

                temp = mem.getB (getReg (insn.src1) + insn.imm);
                if (temp & (1 << 7)) temp |= 0xFFFFFF00;
                setReg (insn.dst, temp);
            break;

            case Insn_t::OpCode::kLH:

                temp = mem.getHW (getReg (insn.src1) + insn.imm);
                if (temp & (1 << 15)) temp |= 0xFFFF0000;
                setReg (insn.dst, temp);
            break;

            case Insn_t::OpCode::kLW:

                here;
                temp = mem.getW (getReg (insn.src1) + insn.imm);
                setReg (insn.dst, temp);
            break;

            case Insn_t::OpCode::kLUI:

                setReg (insn.src1, insn.imm);
            break;

            case Insn_t::OpCode::kAUIPC:

                pc += insn.imm;
            break;

            case Insn_t::OpCode::kBNE:

                if (getReg (insn.src1) != getReg (insn.src2)) pc += insn.imm - pcIncr;
            break;

            case Insn_t::OpCode::kBEQ:

                if (getReg (insn.src1) == getReg (insn.src2)) pc += insn.imm - pcIncr;
            break;

            case Insn_t::OpCode::kSB:

                mem.setB (getReg (insn.src1) + insn.imm, getReg (insn.src2));
            break;

            case Insn_t::OpCode::kSH:

                mem.setHW (getReg (insn.src1) + insn.imm, getReg (insn.src2));
            break;

            case Insn_t::OpCode::kSW:

                mem.setW (getReg (insn.src1) + insn.imm, getReg (insn.src2));
            break;


            default:

                std::cout << "defaulted to exit (0)\n";
                exit (0); ///< i will add a system of exceptions/error codes later
            break;
        }

        pc += pcIncr;

        return 0;
    }

public:

    /**
     * @brief Construct a new Cpu_t object
     *
     */
    Cpu_t (Word_t pcInit_, const std::vector<Word_t>& bytecode, const std::vector<Word_t>& initReg) : reg (kRegCnt, 0), mem (kMemSize), pcInit (pcInit_) {

        for (int i = 0; i < bytecode.size () * pcIncr; i+=pcIncr) {

            mem.setW (pcInit + i, bytecode[i / pcIncr]);
        }

        for (int i = 0; i < initReg.size (); i++) {

            setReg (i + 1, initReg[i]);
        }
    }

    /**
     * @brief Initializes memory and registers with some values
     *
     * @param initialMem initial memory values (will be written from pcInit for now)
     * @param initialReg initial register state (will be written from first reg correspondin to element zero of provided vector)
     */
    void Init (std::vector<Word_t>& initialMem, std::vector<Word_t>& initialReg) {

        for (int i = 0; i < initialMem.size () * pcIncr; i+=pcIncr) {

            mem.setW (pcInit + i, initialMem[i / pcIncr]);
        }

        for (int i = 0; i < initialReg.size (); i++) {

            setReg (i + 1, initialReg[i]);
        }
    }

    void dump (const char* dumpFileName = "cpu_dump.log") {

        std::ofstream dumpFile (dumpFileName, std::ios::app);

        dumpFile << "PC : " << pc << "\n";
        dumpFile << "Registers : \n";

        for (int i = 0; i < 32; i++){

            dumpFile << "\treg[" << i << "] = " << getReg (i) << '\n';
        }

        dumpFile << "Mem (+- 5 starting with from pc)\n";

        for (int i = (pc - 5 * pcIncr >= 0 ? -5 : 0); i < (pc + 5*pcIncr < kMemSize ? 6 : 0);i++) {

            dumpFile << "\tmem[pc + (" << i << ")] : " <<  std::bitset<32> (mem.getW (pc + i * pcIncr)) << "\n";
        }

        dumpFile << "---------------------------------------------------\n";
    }

    /**
     * @brief Just runs everything starting from pcInit
     *
     */
    void run_stuff () {

        pc = pcInit;
        for (;;) {

            dump ();
            if (exec (Insn_t (fetch ())) == 1) break;
        }
        dump ();
    }

};
