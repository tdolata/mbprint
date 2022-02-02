#include "poll.hpp"
#include <iostream>
#include <regex>

using namespace std;

//std::regex Poll::R("\\{([xsufb])([a-f])(\\d+)\\}");

std::regex Poll::R("\\{(\\%[^\\:]+)\\:([a-f])(\\d+)\\}");

string &Poll::Replace(string &str, const string &find, const string &rep) {
    size_t pos = str.find(find);
    return str.replace(pos, find.length(), rep);
    //return str.erase(pos, find.length()).insert(pos, rep);
}

string Poll::GetValue(const string &en, Connection &conn) {
    cmatch m;

    regex_search(en.c_str(), m, R);

    string format = m.str(1);
    char order = m.str(2).front();
    int index = stoi(m.str(3));//  strtol(m.str(3), NULL, 10);

    //cout << "format:" << format << " order:" << order << " index:" << index << endl;
    return conn.Read(index, format, order);
}

void Poll::Loop(Connection &conn) {
    string s(Pattern);
    regex_token_iterator<string::iterator> rend;

    map<string,string> toreplace;

    for (regex_token_iterator<string::iterator> a(s.begin(), s.end(), R); a != rend; a++) {
        string val = GetValue(*a, conn);
        toreplace[*a] = val;
    }

    for(auto &m : toreplace) {
        Replace(s, m.first, m.second);
    }

    cout << s << endl;
}
