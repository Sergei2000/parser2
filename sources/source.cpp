// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

using  std::map;
using  std::vector;
using  std::fstream;
using  std::cout;
using  std::any;
using  std::string;

enum jsonEnums {
    Array,
    Object,
    Int,
    Float,
    String,
    Bool,
};

class Json {
public:
    explicit Json(const std::string &s) {
        fileContent = s;
    }

private:
    Json(void) {}
    void deleteUseless() {
        bool filedFlag = false;
        bool filedBoolFlag = false;
        string s;
        for (int i = 0; i < fileContent.size(); i++) {
            char c = fileContent[i];
            if ((c == '"' || c == '{' || c == '}' ||
            c == '[' || c == ']' || c == ',' || c == ':') &&
                filedFlag == false) {
                s.push_back(c);
                if (c == '"') {
                    filedFlag = true;
                }
                continue;
            }
            if (filedFlag == true) {
                s.push_back(c);
                if (c == '"') {
                    filedFlag = false;
                }
                continue;
            }
            if (c >= '0' && c <= '9') {
                s.push_back(c);
                continue;
            }
            if (c >= '.') {
                s.push_back(c);
                continue;
            }
            if (c == 'f' || c == 't') {
                s.push_back(c);
                filedBoolFlag = true;
                continue;
            }
            if (filedBoolFlag) {
                s.push_back(c);
                if (c == 'e') {
                    filedBoolFlag = false;
                }
            }
        }
        fileContent = s;
    }

public:
    bool is_array() const {
        for (int i = 0; i < fileContent.size(); i++) {
            if (fileContent[i] == '[') {
                return true;
            }
            if (fileContent[i] == '{') {
                return false;
            }
        }
        return false;
    }
    bool is_object() const {
        for (int i = 0; i < fileContent.size(); i++) {
            char c = fileContent[i];
            if (fileContent[i] == '{') {
                return true;
            }
            if (fileContent[i] == '[') {
                return false;
            }
        }
        return false;
    }

private:
    unsigned long getPair(unsigned long number, string object, int typeFlag) {
        if (number >= object.size()) {
            return -1;
        }
        int cursPosition = number;
        if (typeFlag == Object) {
            cursPosition = getKey(number, object);
            if (cursPosition == -1) {
                return cursPosition;
            }
            cursPosition++;
        }

        cursPosition = getValue(cursPosition, object);
        return cursPosition;
    }

    int getKey(int number, string object) {
        bool flagFinishReading = false;
        bool flagStartReading = false;
        while (number < object.size()) {
            char c = object[number];
            number++;
            if (c == '{') {
                return 0;
            }
            if ((c == '}' && number == object.size())) {
                return -1;
            }
            if (c == '"' && !flagStartReading) {
                flagStartReading = true;
                continue;
            }
            if (c != '"' && flagStartReading) {
                key.push_back(c);
                continue;
            }
            if (c == '"' && flagStartReading) {
                return number;
            }
        }
        return -1;
    }

    int getValue(int number, string object) {
        switch (getType(number, object)) {
            case Object: {
                number = getObject(number, object);
                break;
            }
            case Array: {
                number = getArray(number, object);
                break;
            }
            case String: {
                number = getString(number, object);
                break;
            }
            case Int: {
                number = getInt(number, object);
                break;
            }
            case Float: {
                number = getFloat(number, object);
                break;
            }
            case Bool: {
                number = getBool(number, object);
                break;
            }
        }
        return number;
    }

    int getType(int number, string object) {
        int typeValue = -1;
        if (number >= object.size()) {
            return typeValue;
        }
        if (key == "") {
            number = getKey(number, object) + 1;
            if (number == 0) {
                return -1;
            }
        }
        if (is_object(number, object)) {
            typeValue = Object;
            return typeValue;
        }
        if (is_array(number, object)) {
            typeValue = Array;
            return typeValue;
        }
        if (is_string(number, object)) {
            typeValue = String;
            return typeValue;
        }
        if (is_number(number, object)) {
            if (int_or_float(number, object)) {
                typeValue = Int;
                return typeValue;
            }
            typeValue = Float;
            return typeValue;
        }
        if (is_bool(number, object)) {
            typeValue = Bool;
            return typeValue;
        }
        return typeValue;
    }

    bool is_object(int number, string object) {
        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '"' || c == '[' || c == ']' || c == ',' || c == 'f' || c == 't' || (c >= '0' && c <= '9')) {
                return false;
            }
            if (c == '{') {
                return true;
            }
        }
        return false;
    }

    bool is_array(int number, string object) {

        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '"' || c == '{' || c == '}' || c == ',' || c == 'f' || c == 't' || (c >= '0' && c <= '9')) {
                return false;
            }
            if (c == '[') {
                return true;
            }
        }
        return false;
    }

    bool is_string(int number, string object) {
        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '[' || c == '{' || c == '}' || c == ',' || c == 'f' || c == 't' || (c >= '0' && c <= '9')) {
                return false;
            }
            if (c == '"') {
                return true;
            }
        }
        return false;
    }

    bool is_number(int number, string object) {
        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '[' || c == '{' || c == '}' || c == ',' || c == 'f' || c == 't' || c == '"') {
                return false;
            }
            if ((c >= '0' && c <= '9')) {
                return true;
            }
        }
        return false;
    }

    bool int_or_float(int number, string object) {
        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '.') {
                return false;
            }
            if (c == ',' || c == '}') {
                return true;
            }
        }
        return true;
    }

    bool is_bool(int number, string object) {
        for (int i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '[' || c == '{' || c == '}' || c == ',' || (c >= '0' && c <= '9') || c == '"') {
                return false;
            }
            if (c == 'f' || c == 't') {
                return true;
            }
        }
        return false;
    }

    int getBool(unsigned long number, string object) {
        for (unsigned long i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == 't') {
                value = true;
                i += 5;
                long kostil = i;

                return i;
            }
            if (c == 'f') {
                i += 6;

                value = false;
                return i;
            }
        }
        return -1;
    }

    int getFloat(unsigned long number, string object) {
        string s;
        for (unsigned long i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c != ',' && c != ':' && c != ',' && c != ']' && c != '}') {
                s.push_back(c);
                continue;
            }
            if (c == ',' || c == ']' || c == '}') {
                stringstream ss;
                ss << s;
                float floatValue;
                ss >> floatValue;
                value = floatValue;
                i++;
                return i;
            }
        }
        return -1;
    }

    int getInt(unsigned long number, string object) {
        string s;
        for (unsigned long i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c != ',' && c != ':' && c != ',' && c != ']' && c != '}') {
                s.push_back(c);
                continue;
            }
            if (c == ',' || c == ']' || c == '}') {
                stringstream ss;
                ss << s;
                int intValue;
                ss >> intValue;
                value = intValue;
                i++;
                return i;
            }
        }
        return -1;
    }

    int getString(unsigned long number, string object) {
        string s;
        bool skipIt = false;
        for (unsigned long i = number; i <= object.size(); i++) {
            char c = object[i];
            if (c == '"' && !skipIt) {
                skipIt = true;
                continue;
            }
            if (c == '"' && skipIt) {
                skipIt = false;
                continue;
            }
            if (c != ',' && c != ':' && c != ',' && c != ']' && c != '}' && skipIt) {
                s.push_back(c);
                continue;
            }

            if ((c == ',' || c == ']' || c == '}') && !skipIt) {
                value = s;
                i++;
                return i;
            }
        }
    }

    int getArray(unsigned long number, string object) {
        vector <any> valueForMap;
        number++;
        while (object[number] != ']') {
            switch (getType(number, object)) {
                case Object: {
                    int numberForMap = number;
                    char z = object[numberForMap];
                    char f = object[numberForMap + 1];
                    char g = object[numberForMap + 3];
                    char k = object[numberForMap + 4];
                    number = getObject(numberForMap, object);
                    multimap <string, any> mapForVector;
                    array.push_back(getMap(1, 0, mapForVector, any_cast<string>(value)));
                    mapForVector.clear();
                    break;
                }
                case Array: {
                    number = getArray(number + 1, object);
                    array.push_back(value);
                    valueForMap.push_back(value);
                    value.reset();
                    break;
                }
                case String: {
                    number = getString(number, object);
                    cout << any_cast<string>(value);
                    array.push_back(value);
                    valueForMap.push_back(value);
                    value.reset();
                    break;
                }
                case Int: {
                    number = getInt(number, object);
                    cout << any_cast<int>(value);
                    valueForMap.push_back(value);
                    array.push_back(value);
                    value.reset();
                    break;
                }
                case Float: {
                    number = getFloat(number, object);
                    cout << any_cast<float>(value);
                    valueForMap.push_back(value);
                    array.push_back(value);
                    value.reset();
                    break;
                }
                case Bool: {
                    number = getBool(number, object);
                    cout << any_cast<bool>(value);
                    valueForMap.push_back(value);
                    array.push_back(value);
                    value.reset();
                    break;
                }
                case -1: {
                    value = valueForMap;
                    number++;
                    return number;
                }
            }
        }
        value = valueForMap;
        number++;
        return number;
    }

    unsigned long getObject(unsigned long number, string object) {
        string s;
        for (unsigned long i = number; i < object.size(); i++) {
            char c = object[i];
            if (c != '}') {
                s.push_back(c);
            }
            if (c == '}') {
                s.push_back(c);
                value = s;
                return i + 1;
            }
        }
        return -1;
    }

    multimap <string, any> *getMap(unsigned long number, int rang, multimap <string, any> currentMap, string object) {
        key.clear();
        value.reset();
        if (getType(number, object) == Object) {
            key.clear();
            value.reset();
            number = getPair(number, object, Object);
            multimap <string, any> newMap;
            for (int i = 0; i < rang; i++) {
                cout << "     ";
            }
            cout << key << "{" << endl;
            rang++;
            string kek = any_cast<string>(value);
            currentMap.insert(pair<string, any>(key, getMap(1, rang, newMap, any_cast<string>(value))));
            rang--;
            for (int i = 0; i < rang; i++) {
                cout << "     ";
            }
            cout << "}" << endl;
            number++;
            char c = object[number];
            char f = object[number + 1];
            char g = object[number + 2];
            char h = object[number + 3];
            key.clear();
            value.reset();
            getMap(number, rang, currentMap, object);
        } else {
            key.clear();
            value.reset();
            unsigned long numberForPrint = number;
            number = getPair(number, object, Object);
            if (number == -1) {
                return &currentMap;
            }
            currentMap.insert(pair<string, any>(key, value));
            printMe(numberForPrint, object, rang);
            getMap(number, rang, currentMap, object);
        }
    }
    void printMe(int filedPosition, string object, int rang) {
        filedPosition = moveCursToField(filedPosition, object);
        for (int i = 0; i < rang; i++) {
            cout << "     ";
        }
        cout << key << " : ";
        switch (getType(filedPosition, object)) {
            case Array: {
                cout << "(array)";
                const char *type = any_cast < vector < any >> (value)[0].type().name();
                for (unsigned long j = 0; j < any_cast < vector < any >> (value).size(); j++) {
                    if (!strcmp(type, "i")) {
                        cout << any_cast<int>(any_cast < vector < any >> (value)[j]);
                        if (j < any_cast < vector < any >> (value).size() - 1) {
                            cout << ", ";
                        }
                        continue;
                    }
                    if (!strcmp(type, "f")) {
                        cout << any_cast<float>(any_cast < vector < any >> (value)[j]);
                        if (j < any_cast < vector < any >> (value).size() - 1) {
                            cout << ", ";
                        }
                        continue;
                    } else {
                        cout << any_cast<string>(any_cast < vector < any >> (value)[j]);
                        if (j < any_cast < vector < any >> (value).size() - 1) {
                            cout << ", ";
                        }
                        continue;
                    }
                }
                break;
            }
            case String: {
                cout << "(string)" << any_cast<string>(value);
                break;
            }
            case Int: {
                cout << "(int)" << any_cast<int>(value);
                break;
            }
            case Float: {
                cout << "(float)" << any_cast<float>(value);
                break;
            }
            case Bool: {
                cout << "(bool)" << any_cast<bool>(value);
                break;
            }
        }
        cout << endl;
    }

    int moveCursToField(int number, string object) {
        char c = object[number];
        while (c != ':') {
            number++;
            c = object[number];
        }
        return number;
    }


public:
    any &operator[](const std::string &key) {
        if (is_object()) {
            any kostil = getValue(key);
            return kostil;
        }
    }

private:
    any getValue(const string key) {
        for (auto itr = parsedObject.begin(); itr != parsedObject.end(); ++itr) {
            if (itr->first == key) {
                return itr->second;
            }
        }
        return nullptr;
    }
public:
    any &operator[](int index) {
        if (is_array()) {
            return reinterpret_cast<any &>(parsedArray[index]);
        }
    }
    Json parse(const std::string &s) {
        deleteUseless();
        cout << fileContent << endl;
        if (is_array()) {
            cout << "array" << endl;
            getArray(0, fileContent);
            parsedArray = array;
        }
        if (is_object()) {
            cout << "object" << endl;
            multimap <string, any> kek;
            parsedObject = *getMap(1, 0, kek, fileContent);
        }
        return Json(fileContent);
    }
    Json parseFile(const std::string &path_to_file) {
        ifstream jsonFile("/Users/dellvin/Desktop/test.txt");
        if (!jsonFile.is_open()) {
            cout << "Opening file error!" << endl;
            return Json();
        }
        string kostil;
        while (getline(jsonFile, kostil)) {
            fileContent.append(kostil);
        }
        deleteUseless();
        cout << fileContent << endl;
        if (is_array()) {
            cout << "array" << endl;
            getArray(0, fileContent);
            parsedArray = array;
        }
        if (is_object()) {
            cout << "object" << endl;
            multimap <string, any> kek;
            parsedObject = *getMap(1, 0, kek, fileContent);
        }
        return Json(fileContent);
    }

private:
    string fileContent;
    string key;
    any value;
    vector <any> array;
public:
    multimap <string, any> parsedObject;
    vector <any> parsedArray;
};

int main() {
    return 0;
}