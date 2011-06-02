// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_LAMBDA_HPP_INCLUDED
#define FILE_LEAN_LAMBDA_HPP_INCLUDED

namespace lean {

struct lambda {};

inline bool operator == (const lambda&, const lambda&) { return true; }
inline bool operator != (const lambda&, const lambda&) { return false; }

}

#endif
