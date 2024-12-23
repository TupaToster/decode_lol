#include "lib.hpp"
#include "cpu.hpp"
#include "../elfio/elfio/elfio.hpp"

using namespace ELFIO;

int main (int argc, char* argv[]) {

    // fixes: вынести все в конструктор цпу;

    elfio reader;
    std::vector< std::pair <Word_t, std::vector<Byte_t>>> bytecode;


    if (argc != 2 or !reader.load (argv[1])) {

        std::cout << "Give file elf now\n";
        return 0;
    }

    if (reader.get_class () == ELFCLASS32) std::cout << "32\n";
    if (reader.get_encoding () == ELFDATA2LSB) std::cout << "LSB\n";
    std::cout << std::hex << reader.get_entry () << '\n';

    for (int i = 0; i < reader.segments.size ();i++) {

        const segment* pseg = reader.segments[i];
        std::cout << i << " : \n";
        std::cout << "type : " << std::bitset<32> (pseg->get_type ()) << "\n";
        std::cout << "flags : " <<std::bitset<32> (pseg->get_flags ()) << '\n';
        std::cout << "offset : " << std::hex << pseg->get_offset () << "\n";
        std::cout << "virt addr : " << std::hex << pseg->get_virtual_address () << '\n';
        std::cout << "file size : " << pseg->get_file_size () << '\n';
        std::cout << "memory size : " << pseg->get_memory_size () << '\n';

        const char* p = reader.segments[i]->get_data();
        for (int j = 0; j < pseg->get_file_size (); j+=4) {

            std::cout << std::hex << j + pseg->get_virtual_address () << std::dec << " : " << std::bitset<32> (*(int*)(p + j)) << "\n";
        }

        if (pseg->get_type () & 1) {

            std::cout << "Loading segment " << i << "\n";
            bytecode.emplace_back (pseg->get_virtual_address (), std::vector<Byte_t> (pseg->get_file_size ()));
            // std::copy (p, p + pseg->get_file_size(), bytecode.back ().second);
            for (int j = 0; j < pseg->get_file_size (); j++) {

                bytecode.back ().second[j] = p[j];
            }
        }
    }

    std::vector<Word_t> initReg = {};
    Cpu_t cpu ((Word_t) reader.get_entry (), bytecode, initReg);
    cpu.run_stuff ();
}