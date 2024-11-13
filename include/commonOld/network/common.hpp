/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** common.hpp
*/

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#ifdef _WIN32
	#define NOGDI             // Excludes GDI (Graphics Device Interface) definitions
	#define NOUSER            // Excludes User API definitions
	#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
	#define NOGDI               // Exclude GDI definitions
	#define NOUSER              // Exclude User definitions
	#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#undef near
#undef far
namespace asio = boost::asio;
