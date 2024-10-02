#pragma once
#include "lib.hpp"


class Memory_t {

    private:

        static const unsigned int DEF_SIZE = (1<<31);

        std::vector<int32_t> mem_;

    public:

        Memory_t (const unsigned int size = DEF_SIZE) : mem_ (size) {}

        u_int32_t get (const unsigned int pos) {return mem_[pos];}

        void set (const unsigned int pos, const int32_t val) {mem_[pos] = val;}
};

class Cpu_t {

    private:
        using RegNum_t = unsigned int;

        static const unsigned int DEF_START_POINT   = (1<<10) + 1;
        static const unsigned int MASK_OPCODE       = (1<<7) - 1;
        static const unsigned int MASK_RD           = (1<<12) - (1<<7);
        static const unsigned int MASK_FUNCT3       = (1<<15) - (1<<12);
        static const unsigned int MASK_RS1          = (1<<20) - (1<<15);
        static const unsigned int MASK_RS2          = (1<<25) - (1<<20);
        static const unsigned int MASK_IMM_I_TYPE   = (1<<32) - (1<<20);
        static const unsigned int MASK_IMM1_S_TYPE  = (1<<12) - (1<<7);
        static const unsigned int MASK_IMM2_S_TYPE  = (1<<32) - (1<<25);
        static const unsigned int MASK_IMM_U_TYPE   = (1<<32) - (1<<12);

        Memory_t mem_;
        std::vector<int32_t> regs_;
        u_int32_t pc_ = 0;


        enum class ComEnum_t{

            kEBREAK = 0,
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
            kAUIPC,
            kCOM_CNT
        };

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