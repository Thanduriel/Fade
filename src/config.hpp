#pragma once

#include <string_view>
#include <unordered_map>
#include <sstream>
#include <iostream>

namespace Details {
	template<typename T>
	inline T convert(const std::string& _value)
	{
		std::stringstream ss(_value);
		T val;
		ss >> val;

		return val;
	}

	template<>
	inline std::string convert(const std::string& _value)
	{
		return _value;
	}

	template<typename T>
	inline std::string serialize(const T& _value)
	{
		std::stringstream ss;
		ss << _value;

		std::string s;
		ss >> s;
		return s;
	}
}

	/* ******************************
	 * A map of key-value pairs.
	 */
	class ConfigSection
	{
		friend class Config;
	public:
		template<typename T, size_t S>
		using Initializer = std::array<std::pair<std::string, T>, S>;


		template<typename T, size_t S>
		ConfigSection(const Initializer<T,S>& _init)
		{
			for (auto&[key, value] : _init)
				m_values[key] = Details::serialize(value);
		}

		template<typename... Args>
		ConfigSection(Args&&... _args)
			: m_values(std::forward<Args>(_args)...)
		{}

		ConfigSection() = default;

		template<typename T>
		T get(const std::string& _name, const T& _default)
		{
			const auto it = m_values.find(_name);

			if (it != m_values.end())
				return Details::convert<T>(it->second);
			else
			{
				m_values[_name] = Details::serialize(_default);
				return _default;
			}
		}

		// Add contents of another section.
		ConfigSection& operator +=(const ConfigSection& _other);
	private:
		std::unordered_map<std::string, std::string> m_values;
	};

	class Config
	{
	public:
		static Config& instance();

		void save(const std::string& _fileName = "");

		const ConfigSection& getSection(const std::string& _name) const { return m_sections.at(_name); };
		ConfigSection& getSection(const std::string& _name) { return m_sections[_name]; };
	private:
		// Load the config  from a file with the given name.
		Config(const std::string& _fileName);

		// parser stuff
		static std::pair<std::string, ConfigSection> readSection(std::istream& _stream);

		std::unordered_map<std::string, ConfigSection> m_sections;
	};

#define CONFIG_SECTION(name) Config::instance().getSection(name)