#pragma once
#include "lib.hpp"

using Register_t = u_int32_t;
using MemAddr_t = u_int32_t;
using RegAddr_t = u_int8_t;


class Memory_t {

    private:

        static const unsigned int DEF_SIZE = (1<<31);

        std::vector<int32_t> data_;

    public:

        Memory_t (const unsigned int size = DEF_SIZE) : data_ (size) {}

        u_int32_t get (const unsigned int pos) {return data_[pos];}

        void set (const unsigned int pos, const int32_t val) {data_[pos] = val;}
};

enum class OpNum {

    kUNKNOWN = 0,
    kEBREAK,
    kADD ,
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

struct Instruction_t {

    enum class Masks : u_int32_t {

        kOPCODE         = static_cast<u_int32_t> (((1<<7) - 1) | ((1<<15) - (1<<12)) | ((1<<32) - (1<<25))),
        kRD             = static_cast<u_int32_t> ((1<<12) - (1<<7)),
        kRS1            = static_cast<u_int32_t> ((1<<20) - (1<<15)),
        kRS2            = static_cast<u_int32_t> ((1<<25) - (1<<20)),
        kIMM_I          = static_cast<u_int32_t> ((1<<32) - (1<<20)),
        kIMM1_S         = static_cast<u_int32_t> ((1<<12) - (1<<7)),
        kIMM2_S         = static_cast<u_int32_t> ((1<<32) - (1<<25)),
        kIMM_U          = static_cast<u_int32_t> ((1<<32) - (1<<12)),
    };

    enum class OpCodes : u_int32_t {
        kJAL                 = 0b1101111,
        kJALR                = 0b1100111,
        kBNE                 = 0b1100011 | (0b001 << 12),
        kLB                  = 0b0000011,
        kLH                  = 0b0000011 | (0b001 << 12),
        kLW                  = 0b0000011 | (0b010 << 12),
        kSB                  = 0b0100011,
        kSH                  = 0b0100011 | (0b001 << 12),
        kSW                  = 0b0100011 | (0b010 << 12),
        kADDI                = 0b0010011,
        kADD                 = 0b0110011,
        kSUBI                = 0b0010011 | (0b010000 << 25),
        kSUB                 = 0b0110011 | (0b010000 << 25),
        kEBREAK              = 0b1110011 | (0b1 << 20),
        kLUI                 = 0b0110111,
        kAUIPC               = 0b0010111,
    };

    OpNum op_;
    RegAddr_t src1_, src2_, dst_;

    Instruction_t (const u_int32_t instr) {

        if ((instr & static_cast<u_int32_t>(Masks::kOPCODE))  == static_cast<u_int32_t>(OpCodes::kEBREAK)) {

            op_ = OpNum::kEBREAK;
            src1_ = 0;
            dst_ = 0;
        }

        switch ((instr & static_cast<u_int32_t> (Masks::kOPCODE))) {

            case static_cast<u_int32_t> (OpCodes::kJAL):

                op_ = OpNum::kJAL;
                dst_ = instr & static_cast<u_int32_t> (Masks::kRD);
        }
    }
};

class Cpu_t {

    private:

        static const unsigned int DEF_START_POINT   = (1<<10) + 1;

        Memory_t mem_;
        std::vector<int32_t> regs_;
        u_int32_t pc_ = 0;
        class GenericException : std::exception {

            public:

                const char* what (const char* what_is_wrong = "kys lol") const noexcept {

                    return what_is_wrong;
                }
        };

    public:

        Cpu_t (const unsigned int mem_size) : mem_ (mem_size), regs_ (32, 0) {}

        unsigned int writeCodeToMem (const std::vector<int32_t>& code_input, const unsigned int start_point = DEF_START_POINT) {

            for (int i = 0; i < code_input.size (); i++) mem_.set (start_point + i, code_input[i]);
        }

        unsigned int run (const unsigned int start_point = DEF_START_POINT) {

            for (pc_ = start_point; ; pc_++) {

                u_int32_t instr = mem_.get(pc_);

                switch (ComEnum_t (instr & MASK_OPCODE)) {

                    case ComEnum_t::kEBREAK: return 0;

                    case ComEnum_t::kADD:

                        RegNum_t dst = (instr & MASK_RD);
                        RegNum_t src1 = (instr & MASK_RS1);
                        RegNum_t src2 = (instr & MASK_RS2);


                }
            }
        }
};