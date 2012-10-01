#ifndef NODEBIND_META_HPP
#define NODEBIND_META_HPP
#pragma once
#include <v8.h>
#include <node.h>
#include <uv.h>

#include <vector>
#include <string>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/optional.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/cstdint.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/back_inserter.hpp>

#endif
