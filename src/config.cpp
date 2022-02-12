#include "config.hpp"

#include <sstream>
#include <fstream>
#include <string>

using namespace std::string_literals;

/*namespace Game {
	const Utils::ConfigSection::Initializer<int, 2> GamplaySettings(
		{ {
			{"autoChargeJump", 0},
			{"numWinsRequired", 11}
		} });

	const Utils::ConfigSection::Initializer<int, 1> GeneralSettings(
		{ {
			{"SkipStartScreen", 0}
		} });
}*/

	ConfigSection& ConfigSection::operator+=(const ConfigSection& _other)
	{
		m_values.insert(_other.m_values.begin(), _other.m_values.end());

		return *this;
	}

	constexpr const char* CONFIG_PATH = "settings.ini";

	Config::Config(const std::string& _fileName)
	{
		using namespace std;

		std::ifstream file(_fileName.c_str());
		if (!file.fail())
		{
			while (!file.eof())
			{
				auto section = readSection(file);
				m_sections.emplace(section);
			}
		}
	}

	// ********************************************************* //
	Config& Config::instance()
	{
		static Config config(CONFIG_PATH);
		return config;
	}

	void Config::save(const std::string& _fileName)
	{
		using namespace std;

		std::ofstream file(_fileName.empty() ? std::string(CONFIG_PATH) : _fileName);

		for (auto& [name, section] : m_sections)
		{
			file << '[' << name << ']' << "\n";

			for (auto&[key, value] : section.m_values)
			{
				file << key << "=" << value << "\n";
			}
			file << "\n";
		}
	}

	std::pair<std::string, ConfigSection> Config::readSection(std::istream& _stream)
	{
		using namespace std;

		// title
		// [name]
		string line;
		getline(_stream, line);
		const auto begin = line.find('[');
		const auto end = line.find(']');
		if (begin == std::string::npos || end == std::string::npos)
			throw "expected \"[name]\" but found: "s + line;

		const std::string name = line.substr(begin + 1, end - begin-1);
		std::unordered_map<std::string, std::string> values;

		// key value pairs
		// key=value
		while (!_stream.eof() && _stream.peek() != '[')
		{
			getline(_stream, line);
			const auto pos = line.find('=');

			if (pos == string::npos) continue;

			values[line.substr(0, pos)] = line.substr(pos+1);
		}

		return { name, values };
	}
	