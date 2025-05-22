// happy to supply unit tests if required

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <ranges>

std::vector<std::string> split(const std::string &line, char delim) {
    std::vector<std::string> res;
    res.reserve(3);

    auto parts = std::views::split(line, delim);
    for (auto &&part: parts)
        res.emplace_back(part.begin(), part.end());

    return res;
}

class Scale;
std::unordered_map<std::string, Scale> scalesMap;

class Scale {
public:
    Scale(const std::string &name, const std::string &left, const std::string &right):
        name_(name),
        left_(left),
        right_(right) {
    }

    const std::string &name() const { return name_; }

    std::pair<int, int> calcExtraMasses() const {
        int left = getMasses(left_);
        int right = getMasses(right_);
        int target = std::max(left, right);

        return {target - left, target - right};
    }

private:
    static int getMasses(const std::string &massOrScale) {
        if (std::isdigit(massOrScale.front()))
            return std::stoi(massOrScale);

        const auto &scale = scalesMap.at(massOrScale);
        return scale.getTotalMasses();
    }

    int getTotalMasses() const {
        if (!totalMasses_) {
            int left = getMasses(left_);
            int right = getMasses(right_);
            int target = std::max(left, right);
            totalMasses_ = 1 + target * 2; // after get balanced
        }

        return totalMasses_;
    }

private:
    std::string name_;
    std::string left_;
    std::string right_;
    mutable int totalMasses_ = 0;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        std::exit(1);
    }

    std::fstream reader(argv[1]);
    if (!reader.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        std::exit(2);
    }

    std::vector<std::string> scaleNames; // in the order appearing in file
    std::string line;
    while (std::getline(reader, line)) {
        if (line.empty() || line.starts_with('#'))
            continue;

        auto parts = split(line, ',');
        if (parts.size() != 3)
            throw std::runtime_error("Expecting 3 fields in " + line);

        const auto &[scaleName, left, right] = tie(parts[0], parts[1], parts[2]);
        scalesMap.emplace(scaleName, Scale(scaleName, left, right));
        scaleNames.emplace_back(scaleName);
    }

    for (const auto &name: scaleNames) {
        const auto &scale = scalesMap.at(name);
        auto [leftExtra, rightExtra] = scale.calcExtraMasses();
        std::cout << scale.name() << ',' << leftExtra << ',' << rightExtra << std::endl;
    }
}
