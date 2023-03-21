// # DESCRIPTION # //
//
// TermFetch - a terminal application used to get various system information.
// developer: Kian Heitkamp
// email: kianheitkamp@outlook.com

/*
language: C++
C++ version: c++20
compiler: g++
platform win32 (Windows)
to compile:

> g++ termfetch.cpp -o termfetch --static --std=c++20
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>
#include <assert.h>
#include <array>
#include <fstream>
#include <thread>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::cin;
using std::cout;
using std::exception;
using std::getline;
using std::stoi;
using std::stoll;
using std::string;
using std::stringstream;
using std::strtol;
using std::thread;
using std::vector;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PATH_LIM 1024
#define pause() system("echo press any key to return to prompt... && pause > nul")

#define EXT_IP4_COMMAND "powershell.exe -command (Invoke-WebRequest -uri http://ifconfig.me/ip).Content"
#define EXT_IP6_COMMAND "powershell.exe -command (Invoke-WebRequest -uri https://api6.ipify.org).Content"

#define INT_IP4_COMMAND "powershell.exe -command (Test-Connection -ComputerName (hostname) -Count 1).IPV4Address.IPAddressToString"
#define INT_IP6_COMMAND "powershell.exe -command (Test-Connection -ComputerName (hostname) -Count 1).IPV6Address.IPAddressToString"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define COLOR_RED "\033[91m"
#define COLOR_GREEN "\033[92m"
#define COLOR_YELLOW "\033[93m"
#define COLOR_BLUE "\033[94m"
#define COLOR_MAGENTA "\033[95m"
#define COLOR_CYAN "\033[96m"
#define COLOR_WHITE "\033[0m"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool thread_finished = false;

namespace Util
{
    vector<string> split(const string &s, char delim)
    {
        vector<string> result;
        stringstream ss(s);
        string item;

        while (getline(ss, item, delim))
        {
            result.push_back(item);
        }

        return result;
    }
    string output_from(string cmd, bool mult_lines = false, const int line_index = -1)
    {
        string data;
        FILE *stream;
        const int max_buffer = 256;
        char buffer[max_buffer];
        cmd.append(" 2>&1");

        stream = popen(cmd.c_str(), "r");

        if (stream)
        {
            while (!feof(stream))
                if (fgets(buffer, max_buffer, stream) != NULL)
                    data.append(buffer);
            pclose(stream);
        }
        if (!mult_lines && line_index != -1)
        {
            if (!data.empty() && data[data.length() - 1] == '\n')
                data.erase(data.length() - 1);
        }
        else if (!mult_lines && line_index == -1)
        {
            ;
        }
        else
        {
            data = Util::split(data, '\n')[line_index];
        }
        return data;
    }
    const void reset_colors(bool shell = false)
    {
        if (shell)
            system("color");
        else
            cout << COLOR_WHITE << '\n';
    }

    bool ask_yesno(const char *prompt)
    {
        cout << prompt;
        const int response = getch();
        if (response == 121 || response == 89)
            return true;
        else
            return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hardware
{
    string cpu_name()
    {
        string _output = Util::output_from("wmic cpu get name");
        string output = Util::split(_output, '\n')[1];
        return output;
    }

    int ram()
    {
        const string output = Util::output_from("powershell.exe -command [Math]::Round((Get-WmiObject -Class Win32_ComputerSystem).TotalPhysicalMemory/1GB)");
        const int _ram = stoi(output);
        return _ram;
    }

    int cpu_cores()
    {
        const string env_output = getenv("NUMBER_OF_PROCESSORS");
        const int cores = stoi(env_output) / 2;
        return cores;
    }

    int architecture()
    {
        return sizeof(void *) * 8;
    }

    string username()
    {
        return string(getenv("USERNAME"));
    }

    string hostname()
    {
        return string(getenv("COMPUTERNAME"));
    }

    int disk_capacity()
    {
        int val;
        string size_str = Util::output_from("wmic logicaldisk get size", true, 1);
        try
        {
            val = round(stoll(size_str) / 1073741824);
        }
        catch (exception)
        {
            val = -1;
        }
        return val;
    }

    float disk_freespace()
    {
        int val;
        string space_str = Util::output_from("wmic logicaldisk get freespace", true, 1);
        space_str.erase(remove(space_str.begin(), space_str.end(), ' '), space_str.end());
        try
        {
            val = stoll(space_str) / 1073741824;
        }
        catch (exception)
        {
            val = -1;
        }
        return val;
    }

    string windows_version()
    {
        const string output = Util::output_from("powershell.exe -command (Get-WmiObject -class Win32_OperatingSystem).Caption");
        return output;
    }
    void show_disk_info()
    {
        cout << COLOR_YELLOW << "loading disk info...\n";

        int capacity, freespace, usedspace;
        Util::reset_colors();
        cout << COLOR_MAGENTA << "disk info:" << '\n';

        try
        {
            capacity = Hardware::disk_capacity();
        }
        catch (std::exception &e)
        {
            cout << COLOR_RED << "error occurred: " << e.what() << '\n';
            capacity = -1;
        }

        try
        {
            freespace = Hardware::disk_freespace();
        }
        catch (std::exception &e)
        {
            cout << COLOR_RED << "error occurred: " << e.what() << '\n';
            freespace = -1;
        }

        usedspace = capacity - freespace;

        cout << COLOR_GREEN << "  capacity\t" << COLOR_BLUE << capacity << "GB" << '\n';
        cout << COLOR_GREEN << "  free space\t" << COLOR_BLUE << freespace << "GB" << '\n';
        cout << COLOR_GREEN << "  used space\t" << COLOR_BLUE << usedspace << "GB" << '\n';

        Util::reset_colors();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Speedtest
{
    void perform_speedtest(bool ask_confirm = false)
    {
        if (ask_confirm)
        {
            bool confirmation = Util::ask_yesno("are you sure? (y/n): ");
            if (!confirmation)
                return;
        }

        system("terfmetch-speedtest.exe");

        Util::reset_colors(true);

        pause();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Network
{
    string internal_ip4()
    {
        string output = Util::output_from(INT_IP4_COMMAND);
        output.erase(output.length() - 1);
        return output;
    }

    string internal_ip6()
    {
        string output = Util::output_from(INT_IP6_COMMAND);
        if (output[output.length() - 1] == '\n')
        {
            output.erase(output.length() - 1);
        }

        return output;
    }

    string external_ip4()
    {
        string output = Util::output_from(EXT_IP4_COMMAND);
        if (output[output.length() - 1] == '\n')
        {
            output.erase(output.length() - 1);
        }
        return output;
    }

    string external_ip6()
    {
        string output = Util::output_from(EXT_IP6_COMMAND);
        if (output[output.length() - 1] == '\n')
        {
            output.erase(output.length() - 1);
        }

        return output;
    }

    string subnetmask(string ip4)
    {
        vector<string> octet_vec = Util::split(ip4, '.');
        const int octet = stoi(octet_vec[0]);

        if (octet <= 223 && octet >= 192)
            return "255.255.255.0";
        if (octet <= 192 && octet >= 128)
            return "255.255.0.0";
        if (octet <= 127)
            return "255.0.0.0";
        else
            return "subnetMask_undetectable";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void show_hardware_info()
{
    string os, cpu, _hostname, _username;
    int _ram, platform, cores;

    cout << COLOR_YELLOW << "loading hardware info...\t[0/7]\r";
    os = Hardware::windows_version();
    os.erase(os.length() - 1); // remove '\n'

    cout << COLOR_YELLOW << "loading hardware info...\t[1/7]\r";
    cpu = Hardware::cpu_name();

    cout << COLOR_YELLOW << "loading hardware info...\t[2/7]\r";
    _hostname = Hardware::hostname();

    cout << COLOR_YELLOW << "loading hardware info...\t[3/7]\r";
    _username = Hardware::username();

    cout << COLOR_YELLOW << "loading hardware info...\t[4/7]\r";
    _ram = Hardware::ram();

    cout << COLOR_YELLOW << "loading hardware info...\t[5/7]\r";
    platform = Hardware::architecture();

    cout << COLOR_YELLOW << "loading hardware info...\t[6/7]\r";
    cores = Hardware::cpu_cores();

    cout << COLOR_YELLOW << "loading hardware info...\t[7/7]\r";

    cout << '\n';

    cout << COLOR_MAGENTA << "hardware info:" << '\n';

    cout << COLOR_GREEN << "os\t\t" << COLOR_BLUE << os << '\n';
    cout << COLOR_GREEN << "cpu\t\t" << COLOR_BLUE << cpu << '\n';
    cout << COLOR_GREEN << "cpu cores\t" << COLOR_BLUE << cores << '\n';
    cout << COLOR_GREEN << "hostname\t" << COLOR_BLUE << _hostname << '\n';
    cout << COLOR_GREEN << "username\t" << COLOR_BLUE << _username << '\n';
    cout << COLOR_GREEN << "ram\t\t" << COLOR_BLUE << _ram << "GB\n";
    cout << COLOR_GREEN << "platform\t" << COLOR_BLUE << platform << "bit\n";

    Util::reset_colors();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void show_int_ip()
{
    string ip4, ip6, subnet;

    cout << COLOR_YELLOW;
    cout << "loading...\n";

    ip4 = Network::internal_ip4();
    ip6 = Network::internal_ip6();

    subnet = Network::subnetmask(ip4);

    cout << '\n';

    cout << COLOR_MAGENTA << "internal IPs:" << '\n';

    cout << COLOR_GREEN << "ip4: " << COLOR_WHITE << ip4 << COLOR_CYAN << '/' << COLOR_WHITE << subnet << '\n';
    cout << COLOR_GREEN << "ip6: " << COLOR_WHITE << ip6 << '\n';

    Util::reset_colors();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void show_ext_ip()
{
    string ip4, ip6;

    cout << COLOR_YELLOW << "loading..." << '\n';

    ip4 = Network::external_ip4();
    ip6 = Network::external_ip6();

    cout << COLOR_MAGENTA << "external IPs:" << '\n';

    cout << COLOR_GREEN << "ip4: " << COLOR_WHITE << ip4 << '\n';
    cout << COLOR_GREEN << "ip6: " << COLOR_WHITE << ip6 << '\n';

    Util::reset_colors();
}

// end //
