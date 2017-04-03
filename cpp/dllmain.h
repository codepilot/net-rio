#pragma once

#include <SDKDDKVer.h>
#include <windows.h>

/* Algorithms */
//<algorithm>

/*C Library Wrappers*/

//<cassert>
//<cctype>
//<cerrno>
//<cfenv>
//<cfloat>
//<cinttypes>
//<ciso646>
//<climits>
//<clocale>
//<cmath>
//<csetjmp>
//<csignal>
//<cstdarg>
//<cstdbool>
//<cstddef>
//<cstdint>
//<cstdio>
//<cstdlib>
//<cstring>
//<ctgmath>
//<ctime>
//<cwchar>
//<cwctype>

/*Containers*/
//Sequence
#include <array>
//<deque>
//<forward_list>
//<list>
#include <vector>
//using std::vector;


/*Ordered Associative*/
//<map>
//<set>

/*Unordered Associative*/
//<unordered_map>
//<unordered_set>

/*Adaptor*/
//<queue>
//<stack>

/*Error Handling*/
//<exception>
#include <stdexcept>
//<system_error>

/*Input / Output*/
//<filesystem>
//<fstream>
//<iomanip>
//<ios>
//<iosfwd>
//<iostream>
//<istream>
//<ostream>
//<sstream>
//<streambuf>
//<strstream>

/*Iterators Library*/
//<iterator>

/*Localization*/
//<codecvt>
//<cvt - wbuffer>
//<cvt - wstring>
//<locale>

/*Math and Numerics*/
//<complex>
//<limits>
//<numeric>
//<random>
//<ratio>
//<valarray>

/*Memory Management*/
//<allocators>
//<memory>
//<new>
//<scoped_allocator>

/*Multithreading*/
//<atomic>
//<condition_variable>
//<future>
//<mutex>
//<shared_mutex>
//<thread>

/*Other Utilities*/
//<bitset>
//<chrono>
//<functional>
//<initializer_list>
//<tuple>
//<type_traits>
//<typeinfo>
//<typeindex>
//<utility>

/*Strings and Character Data*/
//<regex>
#include <string>
//using std::string;
//using std::wstring;



#include <safeint.h>
using msl::utilities::SafeInt;

#include <node.h>
/*
using v8::Int32;
using v8::Uint32;
using v8::HandleScope;
using v8::Array;
using v8::Context;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::PropertyCallbackInfo;

using v8::FunctionTemplate;
using v8::Function;
using v8::Isolate;
using v8::Local;
using v8::Handle;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;
using v8::FunctionCallback;
using v8::Eternal;

using v8::Object;
using v8::Context;
using v8::Isolate;
using v8::HandleScope;
using v8::Local;
using v8::String;
using node::AtExit;
*/

#include <node_buffer.h>
#include <node_object_wrap.h>
#include <uv.h>
#include <fcntl.h>

#include <tchar.h>


#include "ClsSockets.h"
