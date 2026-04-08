#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        
        explicit arguments(const string& cmd) {
            vector<string> args;
            string current_arg = "";
            bool in_space = true;
            for (char c : cmd) {
                if (c == ' ') {
                    if (!in_space) {
                        args.push_back(current_arg);
                        current_arg = "";
                        in_space = true;
                    }
                } else {
                    current_arg += c;
                    in_space = false;
                }
            }
            if (!in_space) {
                args.push_back(current_arg);
            }

            _argc = args.size();
            if (_argc > 0) {
                _argv = new char*[_argc];
                for (int i = 0; i < _argc; ++i) {
                    _argv[i] = new char[args[i].length() + 1];
                    strcpy(_argv[i], args[i].c_str());
                }
            } else {
                _argv = nullptr;
            }
        }

        arguments(const arguments& other) {
            _argc = other._argc;
            if (other._argv) {
                _argv = new char*[_argc];
                for (int i = 0; i < _argc; ++i) {
                    _argv[i] = new char[strlen(other._argv[i]) + 1];
                    strcpy(_argv[i], other._argv[i]);
                }
            } else {
                _argv = nullptr;
            }
        }

        arguments(arguments&& other) noexcept {
            _argc = other._argc;
            _argv = other._argv;
            other._argc = 0;
            other._argv = nullptr;
        }

        arguments& operator=(const arguments& other) {
            if (this != &other) {
                if (_argv) {
                    for (int i = 0; i < _argc; ++i) {
                        delete[] _argv[i];
                    }
                    delete[] _argv;
                }

                _argc = other._argc;
                if (other._argv) {
                    _argv = new char*[_argc];
                    for (int i = 0; i < _argc; ++i) {
                        _argv[i] = new char[strlen(other._argv[i]) + 1];
                        strcpy(_argv[i], other._argv[i]);
                    }
                } else {
                    _argv = nullptr;
                }
            }
            return *this;
        }

        arguments& operator=(arguments&& other) noexcept {
            if (this != &other) {
                if (_argv) {
                    for (int i = 0; i < _argc; ++i) {
                        delete[] _argv[i];
                    }
                    delete[] _argv;
                }

                _argc = other._argc;
                _argv = other._argv;
                other._argc = 0;
                other._argv = nullptr;
            }
            return *this;
        }

        ~arguments() {
            if (_argv) {
                for (int i = 0; i < _argc; ++i) {
                    delete[] _argv[i];
                }
                delete[] _argv;
                _argv = nullptr;
                _argc = 0;
            }
        }
        
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
