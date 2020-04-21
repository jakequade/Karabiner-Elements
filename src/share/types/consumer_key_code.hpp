#pragma once

#include "stream_utility.hpp"
#include <IOKit/hid/IOHIDUsageTables.h>
#include <cstdint>
#include <mapbox/eternal.hpp>
#include <pqrs/osx/iokit_hid_value.hpp>
#include <spdlog/fmt/fmt.h>

namespace krbn {
enum class consumer_key_code : uint32_t {
  power = kHIDUsage_Csmr_Power,
  display_brightness_increment = kHIDUsage_Csmr_DisplayBrightnessIncrement,
  display_brightness_decrement = kHIDUsage_Csmr_DisplayBrightnessDecrement,
  fastforward = kHIDUsage_Csmr_FastForward,
  rewind = kHIDUsage_Csmr_Rewind,
  scan_next_track = kHIDUsage_Csmr_ScanNextTrack,
  scan_previous_track = kHIDUsage_Csmr_ScanPreviousTrack,
  eject = kHIDUsage_Csmr_Eject,
  play_or_pause = kHIDUsage_Csmr_PlayOrPause,
  mute = kHIDUsage_Csmr_Mute,
  volume_increment = kHIDUsage_Csmr_VolumeIncrement,
  volume_decrement = kHIDUsage_Csmr_VolumeDecrement,
};

namespace impl {
constexpr std::pair<const mapbox::eternal::string, const consumer_key_code> consumer_key_code_name_value_pairs[] = {
    {"power", consumer_key_code::power},
    {"display_brightness_increment", consumer_key_code::display_brightness_increment},
    {"display_brightness_decrement", consumer_key_code::display_brightness_decrement},
    {"fastforward", consumer_key_code::fastforward},
    {"rewind", consumer_key_code::rewind},
    {"scan_next_track", consumer_key_code::scan_next_track},
    {"scan_previous_track", consumer_key_code::scan_previous_track},
    {"eject", consumer_key_code::eject},
    {"play_or_pause", consumer_key_code::play_or_pause},
    {"mute", consumer_key_code::mute},
    {"volume_increment", consumer_key_code::volume_increment},
    {"volume_decrement", consumer_key_code::volume_decrement},
};

constexpr auto consumer_key_code_name_value_map = mapbox::eternal::hash_map<mapbox::eternal::string, consumer_key_code>(consumer_key_code_name_value_pairs);
} // namespace impl

inline std::optional<consumer_key_code> make_consumer_key_code(const std::string& name) {
  auto& map = impl::consumer_key_code_name_value_map;
  auto it = map.find(name.c_str());
  if (it == map.end()) {
    return std::nullopt;
  }
  return it->second;
}

inline std::string make_consumer_key_code_name(consumer_key_code consumer_key_code) {
  for (const auto& pair : impl::consumer_key_code_name_value_pairs) {
    if (pair.second == consumer_key_code) {
      return pair.first.c_str();
    }
  }
  return fmt::format("(number:{0})", static_cast<uint32_t>(consumer_key_code));
}

inline std::optional<consumer_key_code> make_consumer_key_code(pqrs::hid::usage_page::value_t usage_page,
                                                               pqrs::hid::usage::value_t usage) {
  if (usage_page == pqrs::hid::usage_page::consumer) {
    auto u = type_safe::get(usage);

    switch (consumer_key_code(u)) {
      case consumer_key_code::power:
      case consumer_key_code::display_brightness_increment:
      case consumer_key_code::display_brightness_decrement:
      case consumer_key_code::fastforward:
      case consumer_key_code::rewind:
      case consumer_key_code::scan_next_track:
      case consumer_key_code::scan_previous_track:
      case consumer_key_code::eject:
      case consumer_key_code::play_or_pause:
      case consumer_key_code::mute:
      case consumer_key_code::volume_increment:
      case consumer_key_code::volume_decrement:
        return consumer_key_code(u);
    }
  }

  return std::nullopt;
}

inline std::optional<consumer_key_code> make_consumer_key_code(const pqrs::osx::iokit_hid_value& hid_value) {
  if (auto usage_page = hid_value.get_usage_page()) {
    if (auto usage = hid_value.get_usage()) {
      return make_consumer_key_code(*usage_page,
                                    *usage);
    }
  }
  return std::nullopt;
}

inline std::optional<pqrs::hid::usage_page::value_t> make_hid_usage_page(consumer_key_code consumer_key_code) {
  return pqrs::hid::usage_page::consumer;
}

inline std::optional<pqrs::hid::usage::value_t> make_hid_usage(consumer_key_code consumer_key_code) {
  return pqrs::hid::usage::value_t(static_cast<uint32_t>(consumer_key_code));
}

inline void from_json(const nlohmann::json& json, consumer_key_code& value) {
  if (json.is_string()) {
    if (auto v = make_consumer_key_code(json.get<std::string>())) {
      value = *v;
    } else {
      throw pqrs::json::unmarshal_error(fmt::format("unknown consumer_key_code: `{0}`", json.dump()));
    }
  } else if (json.is_number()) {
    value = consumer_key_code(json.get<uint32_t>());
  } else {
    throw pqrs::json::unmarshal_error(fmt::format("json must be string or number, but is `{0}`", json.dump()));
  }
}

inline std::ostream& operator<<(std::ostream& stream, const consumer_key_code& value) {
  return stream_utility::output_enum(stream, value);
}

template <template <class T, class A> class container>
inline std::ostream& operator<<(std::ostream& stream, const container<consumer_key_code, std::allocator<consumer_key_code>>& values) {
  return stream_utility::output_enums(stream, values);
}

template <template <class T, class H, class K, class A> class container>
inline std::ostream& operator<<(std::ostream& stream, const container<consumer_key_code, std::hash<consumer_key_code>, std::equal_to<consumer_key_code>, std::allocator<consumer_key_code>>& values) {
  return stream_utility::output_enums(stream, values);
}
} // namespace krbn
