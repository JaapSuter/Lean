// Copyright 2005, Jaap Suter - MIT License

#ifndef FILE_LEAN_EMPTY_HPP_INCLUDED
#define FILE_LEAN_EMPTY_HPP_INCLUDED

namespace lean {

struct empty {};

inline bool operator == (const empty&, const empty&) { return true; }
inline bool operator != (const empty&, const empty&) { return false; }

}

#endif
