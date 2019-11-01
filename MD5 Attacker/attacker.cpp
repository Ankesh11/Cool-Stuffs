#include<iostream>
#include<ctime>
#include<vector>
#include<algorithm>
#include "md5.h"

struct Result {
	std::string str = "";
	std::string hash = "";
	double time_taken = 0;
	unsigned long long int count = 0;
};

class Attacker {
private:
	std::vector<char> character_set;

public:
	Attacker(bool flag1, bool flag2, bool flag3, bool flag4) {
		if (flag1) {
			for (char c = 'a'; c <= 'z'; c++)
				character_set.push_back(c);
		}
		if (flag2) {
			for (char c = 'A'; c <= 'Z'; c++)
				character_set.push_back(c);
		}
		if (flag3) {
			for (char c = '0'; c <= '9'; c++)
				character_set.push_back(c);
		}
		if (flag4) {
			std::vector<char> spcl_char{ '@', '$', '#', '&' , '%'};
			for (int i = 0; i < spcl_char.size(); i++)
				character_set.push_back(spcl_char[i]);
		}
		if (flag1 + flag2 + flag3 + flag4 > 2) {
			random_shuffle(character_set.begin(), character_set.end());
			random_shuffle(character_set.begin(), character_set.end());
			random_shuffle(character_set.begin(), character_set.end());
		}
	}

	bool attack_util(std::string& hash_to_match, std::string& string_generated, size_t length, unsigned long long& count) {
		if (length < 0)
			return false;
		if (length == 0) {
			count++;
			std::string string_hash = md5(string_generated);
			if (string_hash == hash_to_match)
				return true;
			string_generated = "";
			return false;
		}
		for (auto ch = character_set.begin(); ch != character_set.end(); ch++) {
			std::string temp = string_generated;
			string_generated += (*ch);
			if (attack_util(hash_to_match, string_generated, length - 1, count))
				return true;
			string_generated = temp;
		}
		return false;

	}

	Result attack(std::string hash) {
		std::string hash_to_match = hash;
		std::string string_generated = "";
		unsigned long long count = 0;
		clock_t t = clock();
		bool matched = false;
		for (int length = 1; length <= 9; length++) {
			string_generated = "";
			matched = attack_util(hash_to_match, string_generated, length, count);
			if (matched == true)
				break;
		}
		t = clock() - t;
		Result res;
		if (matched) {
			res.str = string_generated;
		}
		res.hash = hash;
		res.time_taken = (double)t / CLOCKS_PER_SEC;
		res.count = count;
		return res;
	}

};

inline int yn_input() {
	std::string in;
	std::cin >> in;
	while (in != "0" && in != "1") {
		std::cout << "Invalid Input! Try again. 0/1? ";
		std::cin >> in;
	}
	return (in=="1")?1:0;
}
int main() {
	std::string hash;
	std::cout << "Enter MD5 hash : ";
	std::cin >> hash;
	
	std::cout << "\nPlease answer the following query as 0(no) or 1(yes) only.\n";
	bool flag1, flag2, flag3, flag4;
	std::cout << "String may contain small letters?      : ";
	flag1 = (yn_input() == 1 ? true : false);

	std::cout << "String may contain capital letters?    : ";
	flag2 = (yn_input() == 1 ? true : false);

	std::cout << "String may contain numbers?            : ";
	flag3 = (yn_input() == 1 ? true : false);

	std::cout << "String may contain special characters? : ";
	flag4 = (yn_input() == 1 ? true : false);

	Attacker a(flag1, flag2, flag3, flag4);
	std::cout << "\nAttacker running...";
	Result res = a.attack(hash);

	std::cout << "\n\n------------------------RESULT-----------------------------\n";
	std::cout << "String:              " << res.str << std::endl;
	std::cout << "MD5 hash:            " << res.hash << std::endl;
	std::cout << "Time elapsed:        " << res.time_taken << "s" << std::endl;
	std::cout << "Strings checked:     " << res.count << std::endl;
	std::cout << "-----------------------------------------------------------\n\n";
	system("pause");
	return 0;
}