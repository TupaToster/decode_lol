#include "lib.hpp"
#include "cpu.hpp"

int main (int argc, char* argv[]) {

    // fixes: вынести все в конструктор цпу; 

    Cpu_t cpu (1<<12);
    std::vector<Word_t> fibonacchi_prikols = {};
    std::vector<Word_t> initMem = {0b00000000001000001000000110110011};
    std::vector<Word_t> initReg = {1, 2};

    cpu.Init (initMem, initReg);
    cpu.run_stuff ();
}