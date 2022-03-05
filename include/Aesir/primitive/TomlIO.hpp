#pragma once
//std
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

//toml
#include <toml.hpp>

namespace aesir {
	class ITomlLoadable {
	public:
		void loadFromFile(const std::string& filename) {
			auto toml = toml::parse(filename);
			this->load(toml);
		}

		virtual void load(const toml::value& toml) = 0;
	};
	class ITomlSavable {
	public:
		void saveToFile(const std::string& filename) const {
			std::ofstream file(filename);
			file << this->save();
			file.close();
		}

		virtual toml::value save() const = 0;
	};
	class ITomlIO : public ITomlLoadable, public ITomlSavable {};
}