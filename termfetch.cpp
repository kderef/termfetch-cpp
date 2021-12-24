#include "lib.h"

void check(const int option)
{
    if (option == 27 || option == 3) // 27 = escape | 3 = CTRL+C
    {
        exit(0);
    }

    if (option == 49)
    {
        show_int_ip();
        pause();
    }

    if (option == 50)
    {
        show_ext_ip();
        pause();
    }

    if (option == 51)
    {
        show_hardware_info();
        pause();
    }

    if (option == 52)
    {
        Hardware::show_disk_info();
        pause();
    }

    if (option == 53)
    {
        Speedtest::perform_speedtest(true);
        // pause();
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
}
