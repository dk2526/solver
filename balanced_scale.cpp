
// happy to supply unit tests if required
// can be more efficient if required

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> splitStr(const std::string &s, char delim) {
    std::vector<std::string> v;
    v.reserve(3);

    size_t start = 0;
    size_t end = 0;
    while (end != std::string::npos) {
        end = s.find(delim, start);
        v.push_back(s.substr(start, end - start)); // works even if end == npos
        start = end + 1;
    }

    return v;
}

int getScaleMasses(const std::string &scaleName);

class Scale {
public:
    Scale(const std::string &name, const std::string &left, const std::string &right):
        name_(name),
        left_(left),
        right_(right) {
    }

    std::pair<int, int> calcExtraMasses() const {
        int left = getMasses(left_);
        int right = getMasses(right_);
        int target = std::max(left, right);

        return {target - left, target - right};
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
    int getMasses(const std::string &massOrScale) const {
        if (std::isdigit(massOrScale.front()))
            return std::stoi(massOrScale);

        return getScaleMasses(massOrScale); // it's a scale
    }

private:
    const std::string name_;
    const std::string left_;
    const std::string right_;
    mutable int totalMasses_ = 0;
};

std::unordered_map<std::string, Scale> scalesMap;
int getScaleMasses(const std::string &scaleName) {
    return scalesMap.at(scaleName).getTotalMasses();
}

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
        if (line.empty() || line.front() == '#')
            continue;

        auto parts = splitStr(line, ',');
        if (parts.size() != 3)
            throw std::runtime_error("Expecting 3 fields in " + line);

        const auto &[scaleName, left, right] = tie(parts[0], parts[1], parts[2]);
        scalesMap.emplace(scaleName, Scale(scaleName, left, right));
        scaleNames.push_back(scaleName);
    }

    for (const auto &name: scaleNames) {
        const auto &scale = scalesMap.at(name);
        auto [leftExtra, rightExtra] = scale.calcExtraMasses();
        std::cout << name << ',' << leftExtra << ',' << rightExtra << '\n';
    }
}
