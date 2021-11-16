#include <iostream>
#include <signal.h>
#include <locale>

#include <string>
#include <mutex>
#include <condition_variable>

#include "ServiceCentreApp.hh"
static std::mutex g_mutex;
static std::condition_variable g_cond;
void sig_handler(int signo) 
{
    std::unique_lock<std::mutex> lck(g_mutex);
    g_cond.notify_all();
}


void Init(IServiceCentre& app, int argc, const char* argv[])
{
#ifdef LINUX
    if (app.GetArgument("-d"))
    {
        // 转换为 daemon
        daemon(1, 1);
    }
#endif

    /*
     * 注册Ctrl+C
     */
#ifdef _WIN32
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
#else
    memset(&sighandler, 0, sizeof(struct sigaction));
    sighandler.sa_handler = sig_handler;
    sigaction(SIGINT, &sighandler, NULL);
    sigaction(SIGTERM, &sighandler, NULL);
    signal(SIGPIPE, SIG_IGN);
#endif

    // 允许中文打印
#ifdef _WIN32
    setlocale(LC_ALL, "chs");
#else
    setlocale(LC_ALL, "zh_CN");
#endif
}

int main(int argc, const char* argv[])
{
    ServiceCentreApp SCA(argc, argv);

    Init(SCA, argc, argv);

    SCA.Start();
    printf("Func:%s  ---- Current Task ID is %ld.\n", __func__, GetCurrentThreadId());

    std::unique_lock<std::mutex> lck(g_mutex);
    g_cond.wait(lck);

    SCA.Stop();
    std::cout << "ServiceCentreApplication Terminated." << std::endl;

    return 0;
}

