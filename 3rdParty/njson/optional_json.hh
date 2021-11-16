// Optional Json Helper

#ifndef OPTIONAL_JSON_H_
#define OPTIONAL_JSON_H_

#include <memory>

// JSON for Modern C++ (see: https://github.com/nlohmann/json)
#include "json.hpp"
#include "../../misc/static_reflection.hh"

namespace nlohmann {

template <typename T>
struct adl_serializer<std::unique_ptr<T>> {
  static void to_json(json& j, const std::unique_ptr<T>& opt) {
    j = opt ? json(*opt) : json(nullptr);
  }

  static void from_json(const json& j, std::unique_ptr<T>& opt) {
    opt = !j.is_null() ? std::make_unique<T>(j.get<T>()) : nullptr;
  }
};

template <typename T>
struct adl_serializer<T, std::enable_if_t<simple_ref::has_schema<T>>> {
    template <typename BasicJsonType>
    static void to_json(BasicJsonType& j, const T& value) {
        simple_ref::ForEachField(value, [&j](auto&& field, auto&& name) { j[name] = field; });
    }

    template <typename BasicJsonType>
    static void from_json(const BasicJsonType& j, T& value) {
        simple_ref::ForEachField(value, [&j](auto&& field, auto&& name) {
            // ignore missing field of optional
            if (simple_ref::is_optional_v<decltype(field)> && j.find(name) == j.end())
                return;

            j.at(name).get_to(field);
            });
    }
};
}  // namespace nlohmann

#endif  // OPTIONAL_JSON_H_
