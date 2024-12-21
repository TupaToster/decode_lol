#include "lib.hpp"
#include "cpu.hpp"
#include "../elfio/elfio/elfio.hpp"

using namespace ELFIO;

int main (int argc, char* argv[]) {

    // fixes: вынести все в конструктор цпу;

    elfio reader;

    if (argc != 2 or !reader.load (argv[1])) {

        std::cout << "Give file elf now\n";
        return 0;
    }

    if (reader.get_class () == ELFCLASS32) std::cout << "32\n";
    if (reader.get_encoding () == ELFDATA2LSB) std::cout << "LSB\n";

    std::vector<Word_t> bytecode;

    Elf_Half secCnt = reader.sections.size ();
    for (int i = 0; i < reader.sections.size (); i++) {

        const section* psec = reader.sections[i];
        std::cout << psec->get_name () << " " << psec->get_size () << '\n';
        if (psec->get_name () == ".text") {

            bytecode.resize (psec->get_size () / sizeof (Word_t));
            const Word_t* rawBytecode = (const Word_t*)reader.sections[i]->get_data ();
            for (int j = 0; j < bytecode.size (); j++) {

                bytecode[j] = rawBytecode[j];
                std::cout << std::bitset<32> (bytecode[j]) << "\n";
            }
        }
    }

    std::vector<Word_t> initReg = {};
    Cpu_t cpu (1<<12, bytecode, initReg);
    cpu.run_stuff ();
}