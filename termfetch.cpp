#ifndef _WIN32
# error "termfetch only supports windows."
#endif

#include "lib.h"

void check(const int option)
{
    switch (option) {
        case KEY_ESCAPE:
        case KEY_CTRL_C:
            exit(0);
        
        case KEY_1:
            show_int_ip();
            pause();
            break;
        
        case KEY_2:
            show_ext_ip();
            pause();
            break;

        case KEY_3:
            show_hardware_info();
            pause();
            break;

        case KEY_4:
            Hardware::show_disk_info();
            pause();
            break;

        case KEY_5:
            Speedtest::perform_speedtest(true);
            break;

        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    system("cls");
    system("title termfetch (by Kian Heitkamp)");

    int choice;

    while (true)
    {
        system("cls");

        Util::reset_colors(true);

        cout << COLOR_GREEN << "  esc" << COLOR_CYAN << " -->" << COLOR_YELLOW << " exit" << '\n';
        cout << COLOR_MAGENTA << "  =============================" << '\n';
        cout << COLOR_GREEN << "  1" << COLOR_CYAN << "   -->" << COLOR_WHITE << " show internal ip4/ip6" << '\n';
        cout << COLOR_GREEN << "  2" << COLOR_CYAN << "   -->" << COLOR_WHITE << " show external ip4/ip6" << '\n';
        cout << COLOR_GREEN << "  3" << COLOR_CYAN << "   -->" << COLOR_WHITE << " show hardware info" << '\n';
        cout << COLOR_GREEN << "  4" << COLOR_CYAN << "   -->" << COLOR_WHITE << " show disk info" << '\n';
        cout << COLOR_GREEN << "  5" << COLOR_CYAN << "   -->" << COLOR_WHITE << " run speedtest" << '\n';

        choice = getch();

        check(choice);
    }

    return 0;
}
