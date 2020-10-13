#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <unlib/test/unlib_test.hpp>

#include <memory>

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

#include <doctest/doctest.h>

namespace test {

inline void remove_substr(std::string& name, const std::string& substr) {
	for(std::string::size_type idx = name.find(substr); idx != std::string::npos; idx = name.find(substr))
		name.erase(idx, substr.size());
}
inline void remove_prefix(std::string& name, std::string prefix) {
	remove_substr(name, prefix + "::");
}
inline std::string filter_type_name(std::string name) {
	remove_prefix(name, "unlib");
	remove_prefix(name, "std");
	remove_substr(name, "ll");
	remove_substr(name, "ratio");
	return name;
}

#ifndef _MSC_VER
std::string demangle(const char* mangled_symbol) {
	int status = 0;
	std::shared_ptr<char> demangled(abi::__cxa_demangle(mangled_symbol, nullptr, nullptr, &status), std::free);

	return (demangled && status == 0) ? filter_type_name(demangled.get()) : mangled_symbol;
}
#else
std::string demangle(const char* symbol) {
	return filter_type_name(symbol);
}
#endif
std::string demangle(const std::type_info& type_info) {
	return demangle(type_info.name());
}

}
